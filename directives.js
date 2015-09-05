var dirs = angular.module('myDirectives', ['myServices']);

dirs.directive('dirReadOutButton', [ '$templateCache', 'fileaccessor', function ($templateCache, fileaccessor) {
    return {
        restrict: 'EA', //E = element, A = attribute, C = class, M = comment         
        scope: {
            //@ reads the attribute value, = provides two-way binding, & works with functions
                caption: '@'                
			},
        templateUrl: 'CtrlTemplates/ReadOutButton.html',
        controller: function($scope){
            $scope.outValue = "initializing";            
            $scope.$on('configDateTime', function(event, args) {
                $scope.$apply(function(){
                    $scope.outValue = args.data;                
                });
            });
        },
        link: function ($scope, element, attrs) {} //DOM manipulation
    }
}]);