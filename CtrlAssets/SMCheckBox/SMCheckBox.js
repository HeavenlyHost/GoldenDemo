var smCheckBox = angular.module('smCheckBox', ['ngAnimate', 'ui.bootstrap', 'myServices']);

smCheckBox.directive('dirCheckBox', ['$rootScope', '$timeout', '$interval', 'websoc', function ($rootScope, $timeout, $interval, websoc) {
    var uniqueId = 1;
    return {
        restrict: 'EA',
        scope: {
            interfacetag: '@',
            actiontype: '@',
            parameter: '@',
            formattype: '@',
            unitsuffix: '@',
            period: '@',
            phase: '@'
        },
        template:   '<div class="smcbdiv">' +
                        '<input class="smcbinput" id={{cbid}} type="checkbox" ng-model="myvalue"/>' +
                        '<label class="smcblabel" ng-class="status" for={{cbid}}></label>' +
                    '</div>',
        controller: function ($scope) {
            $scope.cbid = 'item' + uniqueId++;
            $scope.rxmyvalue = false;
            $scope.myvalue = false;
            $scope.initialised = false;
            $scope.subscribed = false;
            $scope.tout = null;
            $scope.destroyTimer = null;
            $scope.status = "smcbdis";
            $scope.reqtInit = function () {
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    myData.action.type = $scope.actiontype;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    if ($scope.subscribed == false) {
                        $scope.status = "smcbrs";
                    }
                }
                else if (args.handshake == "hostInProgress") {
                    if ($scope.subscribed == false) {
                        $scope.status = "smcbhip";
                    }
                }
                else if (args.handshake == "hostComplete") {
                    $scope.status = "smcbidl";
                    $scope.subscribed = true;
                }
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
                    $scope.initialised = true;
                    $scope.valuetype = args.valueType;
                    if ($scope.valuetype == "string") {
                        $scope.rxmyvalue = args.stringVal;
                    }
                    else if ($scope.valuetype == "boolean") {
                        $scope.rxmyvalue = args.booleanVal;
                    }
                    else if ($scope.valuetype == "integer") {
                        $scope.rxmyvalue = args.integerVal;
                    }
                    else if ($scope.valuetype == "double") {
                        $scope.rxmyvalue = args.doubleVal;
                    }
                    else {
                        return;
                    }

                    if ($scope.myvalue != $scope.rxmyvalue) {
                        $scope.myvalue = $scope.rxmyvalue;
						$scope.$digest();
                    }
                    $scope.status = "smcbidl";
                }
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.myvalue = false;
                    $scope.status = "smcbidl";
                    $scope.reqtInit();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $timeout.cancel($scope.tin);
                    $scope.subscribed = false;
                    $scope.initialised = false;
                    $scope.myvalue = false;
                    $scope.status = "smcbdis";
                }
            });
            $scope.$watch('myvalue', function () {
                if ($scope.rxmyvalue != $scope.myvalue) {
                    if ($scope.initialised == true) {
                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                        myData.interfaceTag = $scope.interfacetag;
                        myData.parameter = $scope.myvalue;
                        $rootScope.$emit('sendMyData', myData);
                    }
                }
            });
            $scope.reqtInit();
        },
        link: function ($scope, element, attrs) {
            element.bind("$destroy", function () {
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Unsubscription());
                myData.interfaceTag = $scope.interfacetag;
                $rootScope.$emit('sendMyData', myData);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();
                }, 1000);
            });
        }
    }
}]);