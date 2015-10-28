var smReadOutButton = angular.module('smReadOutButton', ['ngAnimate', 'ui.bootstrap', 'myServices']);

//'<div class="robInnerBase" ng-class="{"robIdle": status == "Idle", "robDisabled": status == "Disabled", "robGreen": status == "Green", "robOrange": status == "Orange", "robYellow": status == "Yellow", "robGreen": status == "Green"}">' +

smReadOutButton.directive('dirReadOutButton', ['$modal', '$rootScope', '$templateCache', '$timeout', '$interval', 'fileaccessor', 'websoc', function ($modal, $rootScope, $templateCache, $timeout, $interval, fileaccessor, websoc) {
    return {
        restrict: 'EA', //E = element, A = attribute, C = class, M = comment         
        scope: {
            //@ reads the attribute value, = provides two-way binding, & works with functions
            interfacetag: '@',
            actiontype: '@',
            parameter: '@',
            formattype: '@',
            formatstring: '@',
            formatunitssource: '@',
            formatunitsuffix: '@',
            eventdriven:'@',
            period: '@',
            phase: '@',
            caption: '@'
        },
        template:'<div class="robOuter">' +
	                '<div class="robInnerBase" ng-class="status">' +
		                '<div class="robCentre">' +
            			    '<p class="robCaption">{{::caption}}</p>' +
                			'<p data-ng-if="showValue" class="robValue">{{outValue}}</p>' +
                		'</div>' +
                    '</div>' +
                '</div>',
        controller: function ($scope) {
            $scope.valuetype = "";
            $scope.status = "robDisabled";
            $scope.subscribed = false;
            $scope.outValue = "---";
            $scope.tout = null;
            $scope.tin = null;
            $scope.destroyTimer = null;
            $scope.statusLocked = false;            
            $scope.showValue = $scope.actiontype !== "toggle" && $scope.eventdriven !== "true";
            $scope.OnClick = function () {
                if ($scope.subscribed == true) {
                    if ($scope.eventdriven == "true")
                    {
                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                        myData.interfaceTag = $scope.interfacetag;
                        myData.parameter = "true";
                        $rootScope.$emit('sendMyData', myData);
                    }
                    else if ($scope.actiontype == "toggle")
                    {
                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                        myData.interfaceTag = $scope.interfacetag;
                        myData.parameter = !$scope.outValue;
                        $rootScope.$emit('sendMyData', myData);
                    }
                    else
                    {
                        var modalInstance = $modal.open({
                            animation: true,
                            templateUrl: 'CtrlAssets/html/SMModalBasic.html',
                            scope: $scope,
                            controller: function ($scope) {
                                $scope.newvalue = $scope.$parent.outValue;
                                $scope.applyvalue = function () {
                                    this.$close($scope.newvalue);
                                };
                                $scope.closemodal = function () {
                                    this.$dismiss();
                                };
                            },
                            size: 'sm'
                        });

                        modalInstance.result.then(function (result) {
                            var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                            myData.interfaceTag = $scope.interfacetag;
                            myData.parameter = result;
                            $rootScope.$emit('sendMyData', myData);
                        });
                    }
                }
            };
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
                    $scope.outValue = "---"
                    $scope.status = "robRed";
                }
                else if (args.handshake == "requestSent") {
                    $scope.status = "robYellow";
                }
                else if (args.handshake == "hostInProgress") {
                    $scope.status = "robOrange";
                }
                else if (args.handshake == "hostComplete") {
                    $scope.status = "robIdle";
                    $scope.subscribed = true;
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
                    if ($scope.valuetype == "string") {
                        $scope.outValue = args.stringVal;
                    }
                    else if ($scope.valuetype == "boolean") {
                        $scope.outValue = args.booleanVal;
                    }
                    else if ($scope.valuetype == "integer") {
                        $scope.outValue = args.integerVal;
                    }
                    else if ($scope.valuetype == "double") {
                        $scope.outValue = args.doubleVal;
                    }
                    else {
                        return;
                    }
                }

                if ($scope.actiontype == "toggle")
                {
                    if ($scope.outValue == true)
                    {
                        $scope.status = "robGreen";
                    }
                    else
                    {
                        $scope.status = "robIdle";
                    }
                }
                else
                {
                    $scope.status = "robIdle";
                }

                $scope.$digest();
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.status = "robIdle";
                    $scope.outValue = "Intialising";
                    $scope.subscribe();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $timeout.cancel($scope.tin);
                    $scope.status = "robDisabled";
                    $scope.outValue = "---";
                    $scope.subscribed = false;
                }
            });
            //initialise control
            $scope.subscribe();
        },
        link: function ($scope, element, attrs) {
            element.bind("click", function () {
                $scope.OnClick();
            });
            element.bind("$destroy", function () {
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Unsubscription());
                myData.interfaceTag = $scope.interfacetag;
                $rootScope.$emit('sendMyData', myData);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();
                }, 1000);
            });
        } //DOM manipulation
    }
}]);