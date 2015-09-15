var dirs = angular.module('myDirectives', ['myServices']);

dirs.directive('dirReadOutButton', [ '$templateCache', '$timeout', 'fileaccessor', function ($templateCache, $timeout, fileaccessor) {
    return {
        restrict: 'EA', //E = element, A = attribute, C = class, M = comment         
        scope: {
            //@ reads the attribute value, = provides two-way binding, & works with functions
                caption: '@'                
			},
        templateUrl: 'CtrlTemplates/ReadOutButton.html',
        controller: function($scope){
            $scope.outValue = "initializing";         
            $scope.draw = true;
            $scope.tout = null;
            $scope.$on('configDateTime', function(event, args) {
                $scope.outValue = args.data;                
                    if ($scope.draw)
                    {
                        $scope.$digest();        
                    }                           
            });
            $scope.$on('delayDigest', function(event, args){        
                $scope.draw = false;
                $timeout.cancel($scope.tout);
                $scope.tout = $timeout(function(){
                    $scope.draw = true;
                    $scope.$digest();                      
                },1000);
            });            
        },
        link: function ($scope, element, attrs) {} //DOM manipulation
    }
}]);