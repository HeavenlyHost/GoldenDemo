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
            unitsuffix: '@',
            period: '@',
            phase: '@',
            caption: '@'
        },
        template:'<div class="robOuter">' +
	                '<div class="robInnerBase" ng-class="status">' +
		                '<div class="robCentre">' +
            			    '<p class="robCaption">{{::caption}}</p>' +
                			'<p class="robValue">{{outValue}}</p>' +
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
            $scope.OnClick = function () {
                if ($scope.subscribed == true) {
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
            };
            $scope.reqtInit = function () {
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.releaseLock = function () {
                $scope.status = "robIdle";
                $scope.statusLocked = false;
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    if (!$scope.subscribed) {
                        $scope.status = "robOrange";
                    }
                }
                else if (args.handshake == "HostInProgress") {
                    if (!$scope.subscribed) {
                        $scope.status = "robYellow";
                    }
                }
                else if (args.handshake == "HostComplete") {
                    $scope.status = "robIdle";
                    $scope.subscribed = true;
                }
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
                    $scope.valuetype = args.valueType;
                    if ($scope.valuetype == "String") {
                        $scope.outValue = args.stringVal;
                    }
                    else if ($scope.valuetype == "Boolean") {
                        $scope.outValue = args.booleanVal;
                    }
                    else if ($scope.valuetype == "Integer") {
                        $scope.outValue = args.integerVal;
                    }
                    else if ($scope.valuetype == "Double") {
                        $scope.outValue = args.doubleVal;
                    }
                    else {
                        return;
                    }

	                $scope.$digest();
					
                    if (!$scope.statusLocked) {
                        $scope.status = "robGreen";
                        $scope.statusLocked = true;
                        $timeout.cancel($scope.tin);
                        $scope.tin = $timeout(function () { $scope.releaseLock() }, 1000);
                    }
                }
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.status = "robIdle";
                    $scope.outValue = "Intialising";
                    $scope.reqtInit();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $timeout.cancel($scope.tin);
                    $scope.status = "robDisabled";
                    $scope.outValue = "---";
                    $scope.subscribed = false;
                }
            });
            //initialise control
            $scope.reqtInit();
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