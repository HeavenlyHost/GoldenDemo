var dirs = angular.module('myDirectives', ['myServices']);

dirs.directive('dirCheckBox', [ '$timeout', '$interval', 'websoc', function ($timeout, $interval, websoc) {
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
        templateUrl: 'CtrlAssets/html/SMCheckBox.html',
        controller: function($scope){
            $scope.cbid = 'item' + uniqueId++;
            $scope.status = "smcbIdle";
            $scope.rxmyvalue = false;
            $scope.myvalue = false;
            $scope.initialised = false;
            $scope.subscribed = false;
            $scope.tout = null;
            $scope.destroyTimer = null;
            $scope.$on('delayDigest', function(event, args){        
                $scope.draw = false;
                $timeout.cancel($scope.tout);
                $scope.tout = $timeout(function(){
                    $scope.draw = true;
                    $scope.forceRedraw();                
                },1000);
            });
            $scope.reqtInit = function(){
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    websoc.sendMyData(JSON.stringify(myData));
                }
            };
            $scope.forceRedraw = function(){
                if ($scope.draw)
                {
                    $scope.$digest();        
                }  
            };
            $scope.$on('InterfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    $scope.status = "smcbISRS";
                }
                else if (args.handshake == "HostInProgress") {
                    $scope.status = "smcbISHIP";
                }
                else if (args.handshake == "HostComplete") {
                    $scope.subscribed = true;
                    $scope.status = "smcbISHC";
                    $timeout.cancel($scope.tin);
                    $scope.statusLocked = true;
                    $scope.tin = $timeout(function () { $scope.releaseLock() }, 500);
                    $scope.forceRedraw();
                }
                $scope.forceRedraw();
            });
            $scope.$on('ReportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
                    $scope.initialised = true;
                    $scope.valuetype = args.valueType;
                    if ($scope.valuetype == "String") {
                        $scope.rxmyvalue = args.String;
                    }
                    else if ($scope.valuetype == "Boolean") {
                        $scope.rxmyvalue = args.Boolean;
                    }
                    else if ($scope.valuetype == "Integer") {
                        $scope.rxmyvalue = args.Integer;
                    }
                    else if ($scope.valuetype == "Double") {
                        $scope.rxmyvalue = args.Double;
                    }
                    else {
                        return;
                    }

                    if ($scope.myvalue != $scope.rxmyvalue) {
                        $scope.myvalue = $scope.rxmyvalue;
                    }

                    if ($scope.status == "smcbISHIP") {
                        if (!statusLockeded) {
                            $scope.status = "smcbRS";
                            $timeout.cancel($scope.tin);
                            $scope.statusLocked = true;
                            $scope.tin = $timeout(function () { $scope.releaseLock() }, 500);
                            $scope.forceRedraw();
                        }
                    }
                    else {
                        $scope.forceRedraw();
                    }
                }
            });
            $scope.$on('wsConnection', function(event, args){
                if (args == connectionEnum.CONNECTED) {
                    $scope.status = "robConnected";
                    $scope.myvalue = false;
                    $scope.forceRedraw();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $timeout.cancel($scope.tin);
                    $scope.status = "robDisconnected";
                    $scope.subscribed = false;
                    $scope.initialised = false;
                    $scope.myvalue = "---";
                    $scope.forceRedraw();
                }
            });
            $scope.$watch('myvalue', function()
            {
                if ($scope.rxmyvalue != $scope.myvalue)
                {
                    if ($scope.initialised == true) {
                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                        myData.title = "Request Scalar";
                        myData.interfaceTag = $scope.interfacetag;
                        myData.valueType = $scope.valuetype;
                        if ($scope.valuetype == "String") {
                            myData.String = "";
                        }
                        else if ($scope.valuetype == "Boolean") {
                            myData.Boolean = $scope.myvalue;
                        }
                        else if ($scope.valuetype == "Integer") {
                            myData.Integer = 0;
                        }
                        else if ($scope.valuetype == "Double") {
                            myData.Double = 0;
                        }
                        else {
                            return;
                        }
                        websoc.sendMyData(JSON.stringify(myData));
                    }
                }
            });
            $interval(function(){$scope.reqtInit()}, 1000);
        },
        link: function ($scope, element, attrs) {
            element.bind("$destroy", function() {
                var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                myData.title = "Scalar Unsubscribe";
                myData.interfaceTag = $scope.interfacetag;
                websoc.sendMyData(JSON.stringify(myData));
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function(){
                    $scope.$destroy();                
                },1000);                
            });
        }
    }
}]);

dirs.directive('dirReadOutButton', [ '$rootScope', '$templateCache', '$timeout', '$interval', 'fileaccessor', 'websoc', function ($rootScope, $templateCache, $timeout, $interval, fileaccessor, websoc) {
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
                phase: '@'
			},
        templateUrl: 'CtrlAssets/html/ReadOutButton.html',
        controller: function ($scope) {
            $scope.valuetype = "";
            $scope.status = "robDisconnected";
            $scope.subscribed = false;
            $scope.initialised = false;
            $scope.outValue = "---";         
            $scope.draw = true;
            $scope.tout = null;
            $scope.tin = null;
            $scope.ipoll = null;
            $scope.destroyTimer = null;
            $scope.statusLocked = false;  
            $scope.$on('delayDigest', function(event, args){        
                $scope.draw = false;
                $timeout.cancel($scope.tout);
                $scope.tout = $timeout(function(){
                    $scope.draw = true;
                    $scope.forceRedraw();                
                },1000);
            });
            $scope.OnClick = function(){
                if ($scope.initialised == true) 
                {
                    var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                    myData.title = "Request Scalar";
                    myData.interfaceTag = $scope.interfacetag;
                    myData.valueType = $scope.valuetype;
                    if ($scope.valuetype == "String") {
                        myData.String = "Bounce!!!";
                    }
                    else if ($scope.valuetype == "Boolean") {
                        myData.Boolean = true;
                    }
                    else if ($scope.valuetype == "Integer") {
                        myData.Integer = 1;
                    }
                    else if ($scope.valuetype == "Double") {
                        myData.Double = 1.1;
                    }
                    else
                    {
                        return;
                    }
                    websoc.sendMyData(JSON.stringify(myData));
                }                   
            };
            $scope.reqtInit = function(){
                if ($scope.subscribed == false && websoc.isConnected())
                {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    websoc.sendMyData(JSON.stringify(myData));
                }
            };
            $scope.releaseLock = function(){
                $scope.status = "robIdle";
                $scope.statusLocked = false;     
                $scope.forceRedraw();
            };
            $scope.forceRedraw = function(){
                if ($scope.draw)
                {
                    $scope.$digest();        
                }  
            };
            $scope.$on('InterfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent")
                {                
                    $scope.status = "robOrange";
                }
                else if (args.handshake == "HostInProgress")
                {
                    $scope.status = "robYellow";
                }
                else if (args.handshake == "HostComplete")
                {
                    $scope.subscribed = true;
                    $scope.status = "robGreen";
                    $timeout.cancel($scope.tin);
                    $scope.statusLocked = true;
                    $scope.tin = $timeout(function () { $scope.releaseLock() }, 500);
                    $scope.forceRedraw();
                }
                $scope.forceRedraw();
            });
            $scope.$on('ReportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true)
                {
                    $scope.initialised = true;
                    $scope.valuetype = args.valueType;
                    if ($scope.valuetype == "String") {
                        $scope.outValue = args.String;
                    }
                    else if ($scope.valuetype == "Boolean") {
                        $scope.outValue = args.Boolean;
                    }
                    else if ($scope.valuetype == "Integer") {
                        $scope.outValue = args.Integer;
                    }
                    else if ($scope.valuetype == "Double") {
                        $scope.outValue = args.Double;
                    }
                    else {
                        return;
                    }

                    if ($scope.status == "robYellow") {
                        if (!statusLockeded) {
                            $scope.status = "robGreen";
                            $timeout.cancel($scope.tin);
                            $scope.statusLocked = true;
                            $scope.tin = $timeout(function () { $scope.releaseLock() }, 500);
                            $scope.forceRedraw();
                        }
                    }
                    else {
                        $scope.forceRedraw();
                    }
                }
            });            
            $scope.$on('wsConnection', function(event, args){
               if (args == connectionEnum.CONNECTED)
               {
                   $scope.status = "robConnected";
                   $scope.outValue = "Intialising";         
                   $scope.forceRedraw();
               }
               else if (args == connectionEnum.DISCONNECTED)
               {   
                   $timeout.cancel($scope.tin);                
                   //Switch the class to disconnected
                   $scope.status = "robDisconnected";                   
                   //set to defaults     
                   $scope.subscribed = false;
                   $scope.initialised = false;
                   $scope.outValue = "---";
                   $scope.forceRedraw();
               }          
            });            
            $scope.ipoll = $interval(function () { $scope.reqtInit() }, 2000);
        },
        link: function ($scope, element, attrs) {
            element.bind("click", function() {
                $scope.OnClick();
            });
            element.bind("$destroy", function() {
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                myData.title = "Scalar Unsubscription";
                myData.interfaceTag = $scope.interfacetag;
                websoc.sendMyData(JSON.stringify(myData));
                $interval.cancel($scope.ipoll);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function(){
                    $scope.$destroy();                
                },1000); 
            });
        } //DOM manipulation
    }
}]);