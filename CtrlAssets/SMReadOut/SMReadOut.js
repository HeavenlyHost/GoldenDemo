var smReadOut = angular.module('smReadOut', ['ngAnimate', 'ui.bootstrap', 'myServices']);

smReadOut.directive('dirReadOut', ['$rootScope', '$timeout', '$interval', 'websoc', function ($rootScope, $timeout, $interval, websoc) {
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
        template:'<div class="smrodiv">' +
                    '<h3 class="smroIdle">{{myvalue}}</h3>' +
                 '</div>',
        controller: function ($scope) {
            $scope.myvalue = "---";
            $scope.subscribed = false;
            $scope.destroyTimer = null;
            $scope.subscribe = function () {
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    //Do nothing
                }
                else if (args.handshake == "HostInProgress") {
                    //Do nothing
                }
                else if (args.handshake == "HostComplete") {
                    $scope.subscribed = true;
                }
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
                    $scope.valuetype = args.valueType;
                    switch ($scope.valuetype) {
                        case "String":
                            $scope.myvalue = args.stringVal;
                            break;
                        case "Boolean":
                            $scope.myvalue = args.booleanVal;
                            break;
                        case "Integer":
                            $scope.myvalue = args.integerVal;
                            break;
                        case "Double":
                            $scope.myvalue = args.doubleVal;
                            break;
                    }
					$scope.$digest();					
                }
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.myvalue = "Initialising";
                    $scope.subscribe();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $scope.myvalue = "---";
                    $scope.subscribed == false
                }
            });
            $scope.subscribe();
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
