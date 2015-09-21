var dirs = angular.module('myDirectives', ['myServices']);

dirs.directive('dirReadOutButton', [ '$rootScope', '$templateCache', '$timeout', '$interval', 'fileaccessor', 'websoc', function ($rootScope, $templateCache, $timeout, $interval, fileaccessor, websoc) {
    return {
        restrict: 'EA', //E = element, A = attribute, C = class, M = comment         
        scope: {
            //@ reads the attribute value, = provides two-way binding, & works with functions
                caption: '@',
                smid: '@'                
			},
        templateUrl: 'CtrlTemplates/ReadOutButton.html',
        controller: function($scope){
            $scope.status = "robDisconnected";
            $scope.initialised = false;
            $scope.outValue = "---";         
            $scope.draw = true;
            $scope.tout = null;
            $scope.tin = null;
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
                if ($scope.initialised === true) 
                {
                    var myData = websoc.getProtocol();
                    myData.taskId = "setData";
                    myData.smString = $scope.smid;                    
                    myData.value = "Bounce!!!";
                    $scope.status = "robUiTx";                    
                    websoc.sendMyData(JSON.stringify(myData));
                }                   
            };
            $scope.reqtInit = function(){
                if ($scope.initialised === false && websoc.isConnected()) 
                {
                    var myData = websoc.getProtocol();
                    myData.taskId = "subscribe";
                    myData.smString = $scope.smid;                    
                    $scope.status = "robUiTx";
                    websoc.sendMyData(JSON.stringify(myData));
                }
            };
            $scope.releaseLock = function(){
                $scope.status = "robGiIdle";
                $scope.outValue = "Click Me !!!"
                $scope.statusLocked = false;     
                $scope.forceRedraw();
            };
            $scope.forceRedraw = function(){
                if ($scope.draw)
                {
                    $scope.$digest();        
                }  
            };
            $scope.$on('subscribe-' + $scope.smid, function(event, args){
                if (args.status == "GiAck")
                {
                    //Do something positive, i am subscribed
                    $scope.status = "rob" + args.status;
                    $scope.initialised = true;
                } 
                else if (args.status == "GiNack")
                {
                    //Do something negative, subscription failed  
                }
                else if (args.status == "DsAck")
                {
                    console.debug("DsAck");

                    //Server recieved request
                    $scope.status = "rob" + args.status;
                }                
                $scope.forceRedraw();
            });
            $scope.$on('unsubscribe-' + $scope.smid, function(event, args){
                //Do Nothing 
            });
            $scope.$on('setData-' + $scope.smid, function(event, args){              
                console.debug(args.status);  
                if (args.status == "DsAck")
                {
                    //Server recieved request
                    //$scope.status = "rob" + args.status;                    
                }
                else if (args.status == "GiDataAck")
                {
                    $scope.status = "rob" + args.status;
                    $scope.outValue = args.value;                
                        if ($scope.draw)
                        {
                            $scope.$digest();        
                        }      
                    $timeout.cancel($scope.tin);                
                    $scope.statusLocked = true;     
                    $scope.tin = $timeout(function(){$scope.releaseLock()}, 500);    
                    $scope.forceRedraw();
                }
            });            
            $scope.$on('GiData-' + $scope.smid, function(event, args){
                $scope.initialised = true;                
                if (!$scope.statusLocked)
                {
                    $scope.status = "rob" + args.status;                
                    $scope.outValue = args.value;                
                    $scope.forceRedraw();
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
                   $scope.initialised = false;
                   $scope.outValue = "---";
                   $scope.forceRedraw();
               }          
            });            
            $interval(function(){$scope.reqtInit()}, 1000);
        },
        link: function ($scope, element, attrs) {
            element.bind("click", function() {
                $scope.OnClick();
            });
            element.bind("$destroy", function() {
                var myData = websoc.getProtocol();
                myData.taskId = "unsubscribe";
                myData.smString = $scope.smid;
                websoc.sendMyData(JSON.stringify(myData));
                $scope.$destroy();
            });
        } //DOM manipulation
    }
}]);