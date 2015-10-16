var app = angular.module('demoApp', ['ngAnimate', 'ui.bootstrap', 'ngPopup', 'myServices', 'smTable', 'smReadOutButton', 'smReadOut', 'smCheckBox']);

var connectionEnum = {
    DISCONNECTED : 0,
    CONNECTING: 1,
    CONNECTED: 2
}

app.run(['$rootScope', '$templateCache', '$http', 'fileaccessor', 'configmanager', function ($rootScope, $templateCache, $http, fileaccessor, configmanager){

    configmanager.readConfig();

    var getTemplates = function(myFilesArray){

        var templateCount = myFilesArray.length;
        
        var getServerSideTemplate = function(id){
            $http.get('Partials/' + id + '.html').success(function (t) {
                $templateCache.put(id + '.html', t);
            }).error(function (data, status){
                console.debug("ERROR: " + data);
            }).finally(function(){
                templateCount -= 1;
                if (templateCount == 0)
                    $rootScope.$broadcast('initGolden', null);            
            }); 
        }
        
        myFilesArray.forEach(function(element) {
            getServerSideTemplate(element);    
        }, this);             
    };

    fileaccessor.getFileNamesOnly('Partials', getTemplates);
}]);

app.controller('videoDemoController', [ '$scope', function( $scope ) {}]);

app.controller('tableDemoController', [ '$scope', function( $scope ) {}]);

app.controller('heyController3', [ '$scope', function( $scope ) {}]);

app.controller('heyController4', [ '$scope', function( $scope ) {}]);

app.controller('checkBoxDemoController', [ '$scope', function( $scope ) {}]);

app.controller('readOutButtonDemoContoller', [ '$scope', function( $scope ) {}]);

app.controller('popupDemoController', [ '$scope', '$rootScope', function( $scope, $rootScope ) {
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
        onOpen : function(){},
        onClose  : function()
        {
            $rootScope.$broadcast('dockDialog', this);
        },
        onDragStart : function(){},
        onDragEnd : function(){},
        onResize : function(){}
    }
}]);

app.controller('heyControllerNav', [ '$compile', '$scope', '$rootScope', 'websoc', function( $compile, $scope, $rootScope, websoc ) {
    $scope.connected = false;
    $scope.commsmessage = "Disconnected";
    $scope.addNewVideoDemoTemplate = function () {
        var newItemConfig = {
            createNew: true,
            title: "Video Demo",
            moduleId: "videoDemoModule",
            templateId: "videoDemoTemplate"
        }; 
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };        
    $scope.addNewTableDemoTemplate = function(){
        var newItemConfig = {
            createNew: true,
            title: "Table Demo",
            moduleId: "tableDemoModule",
            templateId: "tableDemoTemplate"
        }; 
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };
    $scope.addNewReadOutButtonDemoTemplate = function(){        
        var newItemConfig = {
            createNew: true,
            title: "Read Out Button Demo",
            moduleId: "readOutButtonDemoModule",
            templateId: "readOutButtonDemoTemplate"
        };         
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };    
    $scope.addNewCheckBoxDemoTemplate = function(){
        var newItemConfig = {
            createNew: true,
            title: "Check Boxes Demo",
            moduleId: "checkBoxDemoModule",
            templateId: "checkBoxDemoTemplate"
        }; 
        $rootScope.$broadcast('dockDialog', newItemConfig)
    };        
    $scope.$on('wsConnection', function(event, args) {
        if (args != connectionEnum.CONNECTING){
            if (args == connectionEnum.CONNECTED)
            {
                $scope.commsmessage = "Connected";                    
                $scope.connected = true;
            }
            else
            {
                $scope.commsmessage = "Disconnected";   
                $scope.connected = false;
            }            
        }
    });    
}]);

app.controller('glDivController', [ '$document', '$scope', function( $document, $scope ) {}]);

app.controller('rootController', [ '$document', '$templateCache', '$http', '$compile', '$scope', '$rootScope', function( $document, $templateCache, $http, $compile, $scope, $rootScope ) {
	$scope.myLayout;
    var savedState = localStorage.getItem( 'savedState' );
	if( savedState !== null ) {
		$scope.myLayout = new GoldenLayout( JSON.parse( savedState ), $document.find('#gldiv') );
	} else {
		$scope.myLayout = new GoldenLayout({
			 content:[{
				 type: 'row',
				 content: [{
					 title: 'Read Out Button Demo',
					 type: 'component',
					 componentName: 'angularModule',
					 componentState: {
						 module: 'readOutButtonDemoModule',
						 templateId: 'readOutButtonDemoTemplate',
					 }
				 }]
			 }]
		}, $document.find('#gldiv'));    
	}
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
            }, $document.find('#gldiv'));             
            
            $scope.myLayout.on( 'stackCreated', function( stack ){
                $scope.stackCreated( stack );
            });
            
            $scope.myLayout.on( 'tabDrag', function( stack ){
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

		var state = JSON.stringify( $scope.myLayout.toConfig() );
		localStorage.setItem( 'savedState', state );		

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
            var templateId = item._mSubscriptions.__all[0].ctx.config.componentState.templateId
            //var html = $( '#' + templateId ).html();
            
            var html = $templateCache.get(templateId + '.html');

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

            var myPopup = angular.element('<div id=pop' + templateId + ' data-ng-controller="popupDemoController"><ng-pop-up option="ngPopupOption"></ng-pop-up></div>');
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
	$scope.myLayout.on( 'stateChanged', function(){
		var state = JSON.stringify( $scope.myLayout.toConfig() );
		localStorage.setItem( 'savedState', state );
	});
    $scope.myLayout.on( 'itemDestroyed', function(){
        var state = JSON.stringify( $scope.myLayout.toConfig() );
        localStorage.setItem( 'savedState', state );
    });
    $scope.myLayout.on( 'tabDrag', function( stack ){
    });
    $scope.AngularModuleComponent = function (container, state) {
        var id = state.templateId + '.html';
        var html = $templateCache.get(id);
        html = $compile('<div>'+html+'</div>')($rootScope);
        container.getElement().html(html);                
    };
    $scope.$on('initGolden', function(event, args){
        $scope.myLayout.registerComponent( 'angularModule', $scope.AngularModuleComponent );
        $scope.myLayout.init();    
    });
}]);
