var smReadOut = angular.module('smReadOut', ['ngAnimate', 'ui.bootstrap', 'myServices']);

smReadOut.directive('dirReadOut', ['$rootScope', '$timeout', '$interval', 'websoc', function ($rootScope, $timeout, $interval, websoc) {
    return {
        restrict: 'EA',
        scope: {
            interfacetag: '@',
            actiontype: '@',
            parameter: '@',
            formattype: '@',
            formatstring: '@',
            formatunitssource: '@',
            formatunitsuffix: '@',
            period: '@',
            phase: '@'
        },
        template:'<div class="smrodiv">' +
                    '<h3 ng-class="status">{{myvalue}}</h3>' +
                 '</div>',
        controller: function ($scope) {
            $scope.myvalue = "---";
            $scope.subscribed = false;
            $scope.destroyTimer = null;
            $scope.status = "smroDisabled";
            $scope.subscribe = function () {
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    myData.action.type = $scope.actiontype;
                    myData.format.type = $scope.formattype;
                    myData.format.properties.string = $scope.formatstring;
                    myData.format.properties.unitsSource = $scope.formatunitssource;
                    myData.format.properties.unitSuffix = $scope.formatunitsuffix;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.errorState == true)
                {
                    $scope.status = "smroError";
                    $scope.myvalue = "---";
                }
                else if (args.handshake == "requestSent") {
                    //Do nothing
                }
                else if (args.handshake == "hostInProgress") {
                    //Do nothing
                }
                else if (args.handshake == "hostComplete") {
                    $scope.subscribed = true;
                    $scope.status = "smroIdle";
                }
                $scope.$digest();
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                $scope.subscribed = true;
                $scope.valuetype = args.valueType;
                if ($scope.formattype != undefined &&
                    $scope.formattype != "none")
                {
                    $scope.myvalue = args.formattedValue;
                }
                else
                {
                    switch ($scope.valuetype) {
                        case "string":
                            $scope.myvalue = args.stringVal;
                            break;
                        case "boolean":
                            $scope.myvalue = args.booleanVal;
                            break;
                        case "integer":
                            $scope.myvalue = args.integerVal;
                            break;
                        case "double":
                            $scope.myvalue = args.doubleVal;
                            break;
                    }
                }
                $scope.status = "smroIdle";
				$scope.$digest();					
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.myvalue = "Initialising";
                    $scope.subscribe();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $scope.myvalue = "---";
                    $scope.status = "smroDisabled";
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
