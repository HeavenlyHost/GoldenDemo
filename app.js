var app = angular.module('heyApp', ['ngPopup']);

app.service('websoc', ['$timeout', '$rootScope', function($timeout, $rootScope) {        

    var connected = false;    
    var websock = null;

    var myWebSocket = function() {
        var tout = 5000;
        if (!connected)
        {
            websock = new WebSocket("ws://localhost:8080/goldenserver");    
            websock.onopen = function(evt){
                console.debug("Web Socket Open !!!")      
                connected = true  
                $rootScope.$broadcast('wsConnection', connected);
            }
            websock.onmessage = function(evt){
                console.debug("RX WS Data:" + evt)
                $rootScope.$broadcast('configDateTime', evt);
            }
            websock.onclose = function(evt){
                console.debug("Web Socket Close !!!")                
                connected = false 
                $rootScope.$broadcast('wsConnection', connected);
            }   
            websock.onerror = function(evt){
                console.debug("error: " + evt)  
            }             
            tout = 1000;
        }
        $timeout(myWebSocket, tout);            
    };
    
    myWebSocket();
    
    var sendMyData = function (data) {
       if (connected == true)
       {
           websock.send(data);
       }                
    }
    
    return {
        sendMyData: sendMyData 
    };
}]);

app.service('ticker', ['$interval', '$rootScope', 'websoc', function($interval, $rootScope, websoc) {
  
  function beat() {
      var dt = Date.now();
      websoc.sendMyData(dt);
  }

  // start periodic checking
  $interval(beat, 50);
}]);

app.controller('heyController1', [ '$scope', 'ticker', 'websoc', function( $scope, ticker, websoc, container, state ) {
        console.log("heyModule1 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args.data;
        });
}]);

app.controller('heyController2', [ '$scope', 'ticker', 'websoc', function( $scope, ticker, websoc, container, state ) {
        console.log("heyModule2 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args.data;
        });
}]);

app.controller('heyController3', [ '$scope', 'ticker', 'websoc', function( $scope, ticker, websoc, container, state ) {
        console.log("heyModule3 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args.data;
        });
}]);

app.controller('heyController4', [ '$scope', 'ticker', 'websoc', function( $scope, ticker, websoc, container, state ) {
        console.log("heyModule4 !!!");
        $scope.dt = new Date();
        $scope.$on('configDateTime', function(event, args) {
            $scope.dt = args.data;
        });
}]);

app.controller('heyControllerPopup', [ '$scope', '$rootScope', function( $scope, $rootScope ) {
    console.log("heyModulePopup !!!");
    
    $scope.ngPopupOption = {
        createNew: false,
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

app.controller('heyControllerNav', [ '$compile', '$scope', '$rootScope', 'websoc', function( $compile, $scope, $rootScope, websoc ) {
    $scope.addNewTemplate1 = function(){
        
        var newItemConfig = {
            createNew: true,
            title: "Hey 1",
            moduleId: "heyModule1",
            templateId: "heyTemplate1"
        }; 
        
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };        
    
    $scope.addNewTemplate2 = function(){
        
        var newItemConfig = {
            createNew: true,
            title: "Hey 2",
            moduleId: "heyModule2",
            templateId: "heyTemplate2"
        }; 
        
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };
    
    $scope.addNewTemplate3 = function(){
        
        var newItemConfig = {
            createNew: true,
            title: "Hey 3",
            moduleId: "heyModule3",
            templateId: "heyTemplate3"
        }; 
        
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };
    
    $scope.addNewTemplate4 = function(){
        
        var newItemConfig = {
            createNew: true,
            title: "Hey 4",
            moduleId: "heyModule4",
            templateId: "heyTemplate4"
        }; 
        
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };    

    $scope.connected = false;
    $scope.commsmessage = "Disconnected";
    
    $scope.$on('wsConnection', function(event, args) {
        $scope.connected = args;
        if (args == true)
        {
            $scope.commsmessage = "Connected";                    
        }
        else
        {
            $scope.commsmessage = "Disconnected";   
        }
    });
    
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
            $('.lm_transition_indicator').remove();
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
        
        if (args.createNew == false)
        {
            // Remove pop dialog from DOM as we don't need it anymore
            $("div[id=pop" + args.templateId + "]").remove();            
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
            stack.removeChild( item, false );
            
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

            var myPopup = angular.element('<div id=pop' + templateId + ' data-ng-controller="heyControllerPopup"><ng-pop-up option="ngPopupOption"></ng-pop-up></div>');
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
    $scope.myLayout.init();    
}]);
