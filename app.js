var app = angular.module('heyApp', ['ngPopup']);

app.service('ticker', ['$interval', '$rootScope', function($interval, $rootScope) {
  function beat() {
      var dt = Date.now();
      $rootScope.$broadcast('configDateTime', dt);
  }

  // start periodic checking
  $interval(beat, 20);
}]);

app.controller('heyController1', [ '$scope', 'ticker', function( $scope, ticker, container, state ) {
        console.log("heyModule1 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args;
        });
}]);

app.controller('heyController2', [ '$scope', 'ticker', function( $scope, ticker, container, state ) {
        console.log("heyModule2 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args;
        });
}]);

app.controller('heyController3', [ '$scope', 'ticker', function( $scope, ticker, container, state ) {
        console.log("heyModule3 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args;
        });
}]);

app.controller('heyController4', [ '$scope', 'ticker', function( $scope, ticker, container, state ) {
        console.log("heyModule4 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args;
        });
}]);

app.controller('heyControllerPopup', [ '$scope', '$rootScope', function( $scope, $rootScope ) {
    console.log("heyModulePopup !!!");
    
    $scope.ngPopupOption = {
        modelName: $rootScope.ngPopupOptionRoot.modelName,
        width: $rootScope.ngPopupOptionRoot.width,
        height: $rootScope.ngPopupOptionRoot.height,
        moduleId: $rootScope.ngPopupOptionRoot.moduleId,
        templateId: $rootScope.ngPopupOptionRoot.templateId,
        template: $rootScope.ngPopupOptionRoot.template,
        title: $rootScope.ngPopupOptionRoot.title,
        hasTitleBar: true,
        resizable:true,
        draggable: true,
        isShow:true,
        position: { top : 100, left : 100},
        onOpen : function()
        {
            console.log(this.title + " - Dialog Opened!!! ");
        },
        onClose  : function()
        {
            console.log(this.title + " - Dialog Closed!!! ");                 
            
            $rootScope.$broadcast('dockDialog', this);
        },
        onDragStart : function(){},
        onDragEnd : function(){},
        onResize : function(){}
    }
}]);

app.controller('heyControllerRoot', [ '$compile', '$scope', '$rootScope', function( $compile, $scope, $rootScope ) {
    $scope.myLayout = new GoldenLayout({
        content:[{
            type: 'row',
            content: [{
                title: 'Hey 1',
                type: 'component',
                componentName: 'angularModule',
                componentState: {
                    module: 'heyModule1',
                    templateId: 'heyTemplate1',
                }
            },{
                title: 'Hey 2',
                type: 'component',
                componentName: 'angularModule',
                componentState: {
                    module: 'heyModule2',
                    templateId: 'heyTemplate2',
                }
            },{
                title: 'Hey 3',
                type: 'component',
                componentName: 'angularModule',
                componentState: {
                    module: 'heyModule3',
                    templateId: 'heyTemplate3',
                }
            },{
                title: 'Hey 4',
                type: 'component',
                componentName: 'angularModule',
                componentState: {
                    module: 'heyModule4',
                    templateId: 'heyTemplate4',
                }
            }]
        }]
    });
    
    $scope.$on('dockDialog', function(event, args) {
        var newItemConfig = {
            title: args.title,
            type: 'component',
            componentName: 'angularModule',
            componentState: {
                module: args.moduleId,
                templateId: args.templateId,
            }
        }; 
        
        if ($scope.myLayout.root.contentItems.length == 0)       
        {
            //Manually remove all golden layout containers            
            $('.lm_dropTargetIndicator').remove();
            $('.lm_inner').remove();
            $('.lm_goldenlayout').remove();
            
            //Recreate golden layout            
            $scope.myLayout = new GoldenLayout({
                content:[{
                    type: 'row',
                    content: [{
                        title: args.title,
                        type: 'component',
                        componentName: 'angularModule',
                        componentState: {
                            module: args.moduleId,
                            templateId: args.templateId,
                        }
                    }]
                }]
            });             
            
            $scope.myLayout.on( 'stackCreated', function( stack ){
                $scope.stackCreated( stack );
            });
            
            $scope.myLayout.registerComponent( 'angularModule', $scope.AngularModuleComponent );
            
            $scope.myLayout.init();             
        }
        else
        {
            $scope.myLayout.root.contentItems[ 0 ].addChild( newItemConfig );            
        }
    });
    
    $scope.stackCreated = function( stack ){
        /*
        * Re-use the label
        */
        var label = stack.layoutManager.config.labels.popout;
    
        /*
        * Callback when the user clicks the popout button
        */
        var popout = function() {
            /*
            * The currently selected item - that's the one you'd want to pop out
            */
            var item = stack.header.activeContentItem;
    
            /**
            * Remove the item as a child from the stack. true indicates that we DON'T
            * want the item to be destroyed
            */
            stack.removeChild( item, true );
            
            /*************************************
            * From here on its up to you. item is an instance of contentItem (https://golden-layout.com/docs/Item.html),
            * you can access its element using item.element.
            *
            * Good luck and if you have any questions: don't hesitate to ask
            */
            console.log(item.element);  

            var templateId = item._mSubscriptions.__all[0].ctx.config.componentState.templateId
            var html = $( '#' + templateId ).html();

            $rootScope.ngPopupOptionRoot = {
                modelName: "myNgPopup",
                width: 320,
                height: 240,
                myItem: item,
                moduleId: item._mSubscriptions.__all[0].ctx.config.componentState.module,
                templateId: item._mSubscriptions.__all[0].ctx.config.componentState.templateId,
                template: html,
                title: item.config.title,
            }

            var myPopup = angular.element('<div data-ng-controller="heyControllerPopup"><ng-pop-up option="ngPopupOption"></ng-pop-up></div>');
            html = $compile(myPopup)($rootScope);
            angular.element(document.body).append(html);
        };
    
        /*
        * Re-use the native header button. This gives you the icon as well as some creation / destruction
        * logic when the header is moved / destroyed. Only trouble is that it currently appends the element,
        * so the popout icon is now last...might be worth doing something hack-ish here
        */
        new GoldenLayout.__lm.controls.HeaderButton( stack.header, label, 'lm_popout', popout );       
    }
    
    $scope.myLayout.on( 'stackCreated', function( stack ){
        $scope.stackCreated( stack );
    });

    $scope.AngularModuleComponent = function (container, state) {
        var html = $( '#' + state.templateId ).html();
        html = $compile('<div>'+html+'</div>')($rootScope);
        container.getElement().html(html);                
    };
    
    $scope.myLayout.registerComponent( 'angularModule', $scope.AngularModuleComponent );
    
    //$http.get(state.templateId, {cache:true}).success(function(html) {
    //html = $compile('<div>'+html+'</div>')($rootScope);
    //container.getElement().html(html);
    //});
    
    //myLayout.registerComponent( 'angularModule', AngularModuleComponent );
    $scope.myLayout.init();
    
}]);

//var AngularModuleComponent = function( container, state ) {
//    var html = $( '#' + state.templateId ).html(),
//        element = container.getElement();
    
//    element.html( html );

//    angular
//        .module( state.module )
//        .value( 'container', container )
//        .value( 'state', state );

//    angular.bootstrap( element[ 0 ], [ state.module ] );
//};	


