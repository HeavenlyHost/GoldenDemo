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
            $scope.statusLocked = false;  
            $scope.$on('delayDigest', function(event, args){        
                $scope.draw = false;
                $timeout.cancel($scope.tout);
                $scope.tout = $timeout(function(){
                    $scope.draw = true;
                    $scope.$digest();                      
                },1000);
            });
            $scope.OnClick = function(){
                if ($scope.initialised === true) 
                {
                    var myData = websoc.getProtocol();
                    myData.taskId = "dataSet";
                    myData.smString = $scope.smid;                    
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
                $scope.statusLocked = false;
                if ($scope.status == "robSmTx")
                {
                    $scope.status == "robIdle";
                }
            };
            $scope.$on('data-' + $scope.smid, function(event, args){
                $scope.initialised = true;
                if ($scope.status === "robSrRx")
                {
                    $scope.status = "robSmTx";           
                    $scope.statusLocked = true;     
                    $timeout(function(){$scope.releaseLock()}, 500);
                }
                else
                {
                    if (!$scope.statusLocked)
                        $scope.status = "rob" + args.status;                
                }                
                $scope.outValue = args.value;                
                    if ($scope.draw)
                    {
                        $scope.$digest();        
                    }      
            });
            $scope.$on('wsConnection', function(event, args){
               if (args == connectionEnum.CONNECTED)
               {
                   $scope.status = "robIdle";
               }
               else if (args == connectionEnum.CONNECTING)
               {
                   $scope.outValue = "Intialising";         
               }
               else if (args == connectionEnum.DISCONNECTED)
               {               
                   $scope.initialised = false;
                   $scope.status = "robDisconnected";                   
                   $scope.outValue = "---";
               }          
               $scope.$digest();          
            });            
            $interval(function(){$scope.reqtInit()}, 1000);
        },
        link: function ($scope, element, attrs) {
            element.bind("click", function() {
                $scope.OnClick();
            });            
        } //DOM manipulation
    }
}]);