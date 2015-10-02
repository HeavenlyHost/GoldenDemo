var dirs = angular.module('myDirectives', ['ngAnimate', 'ui.bootstrap', 'myServices', 'ui.grid', 'ui.grid.selection', 'ui.grid.autoResize']);

dirs.directive('dirTable', ['$rootScope', '$timeout', '$interval', 'websoc', 'uiGridConstants', function ($rootScope, $timeout, $interval, websoc, uiGridConstants) {
    return {
        restrict: 'EA',
        scope: {
            interfacetags: '@',
            actiontype: '@',
            parameter: '@',
            formattype: '@',
            unitsuffix: '@',
            period: '@',
            phase: '@'
        },
        templateUrl: 'CtrlAssets/html/SMTable.html',
        controller: function ($scope) {
            $scope.ipoll = null;
            $scope.myTableData = [];

            $scope.gridOptions = {
                data: $scope.myTableData,
                enableFullRowSelection: true,
                multiSelect: false,
                columnDefs: []
            };
            $scope.addItem = function () {
               // $scope.myTableData.push(
               //     {
               //         "firstName": "New",
               //         "lastName": "Item"
               //     }
               // );
            }
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    //Connected
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    //Reset the settings
                    $scope.settings = eval('(' + $scope.interfacetags + ')');
                }
            });
        },
        link: function ($scope, element, attrs) {
            $scope.ipolllink = null
            $scope.arrayData = {}
            element.bind("$destroy", function () {
                $interval.cancel($scope.ipoll);
            });
            $scope.settings = eval('(' + $scope.interfacetags + ')');
            $scope.newitem = {}
            for (var item in $scope.settings.tags)
            {

                $scope.gridOptions.columnDefs.push({
                    field: $scope.settings.tags[item].colname,
                    displayName: $scope.settings.tags[item].displayname
                });

                $scope.newitem[$scope.gridOptions.columnDefs[item].field] = "";

                var value = $scope.settings.tags[item].tag;

                $scope.$on('reportArray-' + value, function (event, args) {
                    console.debug(args);
                    var index = $scope.settings.tags.map(function (x) { return x.tag; }).indexOf(args.interfaceTag);
                    if (index != -1) {

                        //Check for null tag                        
                        if ($scope.settings.tags[index].tag == null)
                            return;

                        //Extract correct data type based on returned valueType
                        if (args.valueType == "Boolean")
                        {
                            $scope.settings.tags[index].data = args.BooleanValues;
                        }
                        else if (args.valueType == "Integer")
                        {
                            $scope.settings.tags[index].data = args.IntegerValues;
                        }
                        else if (args.valueType == "Double")
                        {
                            $scope.settings.tags[index].data = args.DoubleValues;
                        }
                        else if (args.valueType == "String")
                        {
                            $scope.settings.tags[index].data = args.StringValues;
                        }
                        else
                        {
                            //Type not known so return now !!!
                            return;
                        }
                            
                        //Update table data
                        for (var i = 0; i < $scope.settings.tags[index].data.length; i++)
                        {
                            if ($scope.myTableData.length <= i)
                            {
                                //Get blank item, do JSON to from as we don't want a reference
                                var blankitem = JSON.parse(JSON.stringify($scope.newitem));

                                //Add new object - if data is added to new item then push onto myTableData
                                var propupdated = false;
                                for (property in blankitem)
                                {
                                    //if property found for column then save and break loop 
                                    if (property == $scope.settings.tags[index].colname) {
                                        blankitem[property] = $scope.settings.tags[index].data[i];
                                        propupdated = true;
                                        break;
                                    }
                                }
                                    
                                //New data item populated ? Yes, then save into data store
                                if (propupdated)
                                {
                                    $scope.myTableData.push(JSON.parse(JSON.stringify(blankitem)));
                                }
                            }
                            else
                            {
                                //Search for existing property within the data table under current index
                                for (property in $scope.myTableData[i]) {
                                    //if property found for column then save and break loop 
                                    if (property == $scope.settings.tags[index].colname) {
                                            $scope.myTableData[i][property] = $scope.settings.tags[index].data[i];
                                        break;
                                    }
                                }
                            }
                        }

                        //This is here to remove extra items if the item count reduces from server, don't need old values so delete
                        while ($scope.myTableData.length > $scope.settings.tags[index].data.length) {
                            $scope.myTableData.pop(i);
                        }
                    }
                });
                $scope.$on('interfaceStatus-' + value, function (event, args) {                    
                    for (var index = 0; index < $scope.settings.tags.length; index++) {
                        var subitem = $scope.settings.tags[index];
                        var tag = subitem.tag || null;
                        if (tag !== null && tag == args.interfaceTag)
                        {
                            if (args.handshake == "requestSent") {
                                //Do nothing
                            }
                            else if (args.handshake == "HostInProgress") {
                                //Do nothing
                            }
                            else if (args.handshake == "HostComplete") {
                                $scope.settings.tags[index].subscribed = true;
                            }
                            break;
                        }
                    }
                });
            }
            $scope.subscribe = function () {
                for (var index = 0; index < $scope.settings.tags.length; index++) {
                    var item = $scope.settings.tags[index];
                    var subscribed = item.subscribed || null;
                    if ((subscribed == null || subscribed == false) &&  websoc.isConnected())
                    {
                        var myData = $.extend(true, {}, websoc.getprotocol_Array_Subscription());
                        myData.interfaceTag = item.tag;
                        $rootScope.$emit('sendMyData', myData);
                    }
                }
            };
            $scope.ipolllink = $interval(function () { $scope.subscribe() }, 2500);
            $scope.destroyTimer = null;
            element.bind("$destroy", function () {
                $interval.cancel($scope.ipolllink);
                for (var index = 0; index < $scope.settings.tags.length; index++) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Array_Unsubscription());
                    myData.title = "arrayUnsubscribe";
                    myData.interfaceTag = $scope.settings.tags[index].tag;
                    $rootScope.$emit('sendMyData', myData);
                }
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();
                }, 1000);
            });
        }
    }

}]);

dirs.directive('dirReadOut', [ '$rootScope', '$timeout', '$interval', 'websoc', function ( $rootScope, $timeout, $interval, websoc) {
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
        templateUrl: 'CtrlAssets/html/SMReadOut.html',
        controller: function ($scope) {
            $scope.myvalue = "---";
            $scope.subscribed = false;
            $scope.destroyTimer = null;
            $scope.ipoll = null;
            $scope.subscribe = function () {
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    //Do nothing
                }
                else if (args.handshake == "HostInProgress") {
                    //Do nothing
                }
                else if (args.handshake == "HostComplete") {
                    $scope.subscribed = true;
                }
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
                    $scope.valuetype = args.valueType;
                    switch ($scope.valuetype) {
                        case "String":
                            $scope.myvalue = args.String;
                            break;
                        case "Boolean":
                            $scope.myvalue = args.Boolean;
                            break;
                        case "Integer":
                            $scope.myvalue = args.Integer;
                            break;
                        case "Double":
                            $scope.myvalue = args.Double;
                            break;
                    }
                    $scope.$digest();
                }
            });
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    $scope.myvalue = "Initialising...";
                }
                else {
                    $scope.myvalue = "---";
                }
                $scope.$digest();
            });
            $scope.ipoll = $interval(function () { $scope.subscribe() }, 2500);
        },
        link: function ($scope, element, attrs) {
            element.bind("$destroy", function () {
                $interval.cancel($scope.ipoll);
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Unsubscription());
                myData.title = "scalarUnsubscribe";
                myData.interfaceTag = $scope.interfacetag;
                $rootScope.$emit('sendMyData', myData);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();
                }, 1000);
            });
        }
    }
}]);

dirs.directive('dirCheckBox', [ '$rootScope', '$timeout', '$interval', 'websoc', function ( $rootScope, $timeout, $interval, websoc) {
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
            $scope.rxmyvalue = false;
            $scope.myvalue = false;
            $scope.initialised = false;
            $scope.subscribed = false;
            $scope.tout = null;
            $scope.destroyTimer = null;
            $scope.ipoll = null;
            $scope.status = "cbDIS";
            $scope.reqtInit = function(){
                if ($scope.subscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    $scope.status = "cbRS";
                }
                else if (args.handshake == "HostInProgress") {
                    $scope.status = "cbHIP";
                }
                else if (args.handshake == "HostComplete") {
                    $scope.status = "cbIDL";
                    $scope.subscribed = true;
                }
                else
                {
                    return;
                }
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
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
                        $scope.$digest();
                    }

                    $scope.status = "cbIDL";
                }
            });
            $scope.$on('wsConnection', function(event, args){
                if (args == connectionEnum.CONNECTED) {
                    $scope.myvalue = false;
                    $scope.status = "cbIDL";
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    $timeout.cancel($scope.tin);
                    $scope.subscribed = false;
                    $scope.initialised = false;
                    $scope.myvalue = false;
                    $scope.status = "cbDIS";
                }
                else
                {
                    return;
                }
                //$scope.$digest();
            });
            $scope.$watch('myvalue', function()
            {
                if ($scope.rxmyvalue != $scope.myvalue)
                {
                    if ($scope.initialised == true) {
                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
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
                        $rootScope.$emit('sendMyData', myData);
                    }
                }
            });
            $scope.ipoll = $interval(function () { $scope.reqtInit() }, 1000);
        },
        link: function ($scope, element, attrs) {
            element.bind("$destroy", function() {
                $interval.cancel($scope.ipoll);
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Unsubscription());
                myData.title = "scalarUnsubscribe";
                myData.interfaceTag = $scope.interfacetag;
                $rootScope.$emit('sendMyData', myData);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();                
                },1000);                
            });
        }
    }
}]);

dirs.directive('dirReadOutButton', [ '$modal', '$rootScope', '$templateCache', '$timeout', '$interval', 'fileaccessor', 'websoc', function ($modal, $rootScope, $templateCache, $timeout, $interval, fileaccessor, websoc) {
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
        templateUrl: 'CtrlAssets/html/SMReadOutButton.html',
        controller: function ($scope) {
            $scope.valuetype = "";
            $scope.status = "Disabled";
            $scope.subscribed = false;
            $scope.outValue = "---";         
            $scope.tout = null;
            $scope.tin = null;
            $scope.ipoll = null;
            $scope.destroyTimer = null;
            $scope.statusLocked = false;  
            $scope.OnClick = function(){
                if ($scope.subscribed == true) 
                {
                    var modalInstance = $modal.open({
                        animation: true,
                        templateUrl: 'CtrlAssets/html/SMModalBasic.html',
                        scope: $scope,
                        controller: function ($scope)
                        {
                            $scope.newvalue = $scope.$parent.outValue;
                            $scope.applyvalue = function(){
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
                        myData.valueType = $scope.valuetype;
                        if ($scope.valuetype == "String") {
                            myData.String = result;
                        }
                        else if ($scope.valuetype == "Boolean") {
                            myData.Boolean = result;
                        }
                        else if ($scope.valuetype == "Integer") {
                            myData.Integer = result;
                        }
                        else if ($scope.valuetype == "Double") {
                            myData.Double = result;
                        }
                        else {
                            return;
                        }
                        $rootScope.$emit('sendMyData', myData);
                    });                    
                }
            };
            $scope.reqtInit = function(){
                if ($scope.subscribed == false && websoc.isConnected())
                {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.interfacetag;
                    $rootScope.$emit('sendMyData', myData);
                }
            };
            $scope.releaseLock = function(){
                $scope.status = "Idle";
                $scope.statusLocked = false;
                //$scope.$digest();
            };
            $scope.$on('interfaceStatus-' + $scope.interfacetag, function (event, args) {
                if (args.handshake == "requestSent") {
                    $scope.status = "Orange";
                }
                else if (args.handshake == "HostInProgress") {
                    $scope.status = "Yellow";
                }
                else if (args.handshake == "HostComplete") {
                    $scope.status = "Idle";
                    $scope.subscribed = true;
                }
                else
                {
                    return;
                }
                //$scope.$digest();
            });
            $scope.$on('reportScalar-' + $scope.interfacetag, function (event, args) {
                if ($scope.subscribed == true) {
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

                    if (!$scope.statusLocked) {
                        $scope.status = "Green";
                        $timeout.cancel($scope.tin);
                        $scope.statusLocked = true;
                        $scope.tin = $timeout(function () { $scope.releaseLock() }, 1000);
                    }

                    //$scope.$digest();
                }
            });            
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED)
                {
                    $scope.status = "Idle";
                    $scope.outValue = "Intialising";         
                }
                else if (args == connectionEnum.DISCONNECTED)
                {   
                    $timeout.cancel($scope.tin);                
                    //Switch the class to disconnected
                    $scope.status = "Disabled";                   
                    //set to defaults     
                    $scope.subscribed = false;
                    $scope.outValue = "---";
                }
                else
                {
                    return;
                }
                //$scope.$digest();
            });            
            $scope.ipoll = $interval(function () { $scope.reqtInit() }, 2500);
        },
        link: function ($scope, element, attrs) {
            element.bind("click", function() {
                $scope.OnClick();
            });
            element.bind("$destroy", function() {
                $interval.cancel($scope.ipoll);
                var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Unsubscription());
                myData.title = "scalarUnsubscription";
                myData.interfaceTag = $scope.interfacetag;
                $rootScope.$emit('sendMyData', myData);
                $timeout.cancel($scope.destroyTimer);
                $scope.destroyTimer = $timeout(function () {
                    $scope.$destroy();                
                },1000); 
            });
        } //DOM manipulation
    }
}]);