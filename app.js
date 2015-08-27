var app = angular.module('heyApp', ['ngPopup']);

app.controller('heyController1', [ '$scope', function( $scope, container, state ) {
        console.log("heyModule1 !!!");
}]);

app.controller('heyController2', [ '$scope', function( $scope, container, state ) {
        console.log("heyModule2 !!!");
}]);

app.controller('heyController3', [ '$scope', function( $scope, container, state ) {
        console.log("heyModule3 !!!");
}]);

app.controller('heyController4', [ '$scope', function( $scope, container, state ) {
        console.log("heyModule4 !!!");
}]);

app.controller('heyControllerPopup', [ '$scope', '$rootScope', function( $scope, $rootScope ) {
    console.log("heyModulePopup !!!");
    $scope.ngPopupOption = $rootScope.ngPopupOptionRoot;
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
    
    $scope.myLayout.on( 'stackCreated', function( stack ){
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

            $rootScope.ngPopupOptionRoot = {
                modelName: "myNgPopup",
                width: 320,
                height: 240,
                hasTitleBar:true,
                template: item.element[0].children[0].children[0].innerHTML,
                title: item.config.title,
                resizable:true,
                draggable: true,
                isShow:true,
                position: { top : 100, left : 100},
                onOpen : function()
                {
                    console.log("Dialog Opened!!!");
                },
                onClose  : function()
                {
                    
                },
                onDragStart : function(){},
                onDragEnd : function(){},
                onResize : function(){}
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


