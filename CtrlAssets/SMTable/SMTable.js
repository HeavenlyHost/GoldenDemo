var smTable = angular.module('smTable', ['ngAnimate', 'ui.bootstrap', 'myServices', 'ui.grid', 'ui.grid.selection', 'ui.grid.autoResize', 'ui.grid.treeView']);

smTable.directive('dirTable', ['$rootScope', '$timeout', '$interval', 'websoc', 'uiGridConstants', 'uiGridTreeViewConstants', 'configmanager', function ($rootScope, $timeout, $interval, websoc, uiGridConstants, uiGridTreeViewConstants, configmanager) {
    return {
        restrict: 'EA',
        scope: {
            enabletreemode: '@',
            treesource: '@',
            treepath: '@',
            treequery: '@',
            interfacetags: '@',
            actiontype: '@',
            parameter: '@',
            period: '@',
            phase: '@'
        },
        template:'<div ui-grid="gridOptions" class="myGrid" ui-grid-selection ui-grid-tree-view ui-grid-auto-resize ></div>',
        controller: function ($scope) {
            $scope.ipoll = null;
            $scope.selectedSubscribed = false;
            $scope.selectedIndex = 0;
            $scope.myTableData = [];
            $scope.isTreeEnabled = $scope.enabletreemode || false;
            $scope.myTreeSource = $scope.treesource || "";
            $scope.myTreePath = $scope.treepath || "";
            $scope.myTreeQuery = $scope.treequery || "";
            if ($scope.isTreeEnabled)
            {
                $scope.gridOptions = {
                    data: $scope.myTableData,
                    enableFullRowSelection: false,
                    multiSelect: false,					
                    enableSorting: false,
                    enableFiltering: false,
                    showTreeExpandNoChildren: true,
                    columnDefs: [],
                    onRegisterApi: function (gridApi) {
                        $scope.gridApi = gridApi;
                        $scope.gridApi.treeBase.on.rowExpanded($scope, function (row) {
                            //Do something here may not need it
                        });
                        $scope.gridApi.selection.on.rowSelectionChanged($scope, function (row) {
                            if (row.isSelected) {
                                if ($scope.selectedIndex != row.entity.index)
                                {
                                    $scope.selectedIndex = row.entity.index;
                                    var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                                    myData.interfaceTag = $scope.settings.selectedtag;
                                    myData.parameter = row.entity.index.toString();
                                    $rootScope.$emit('sendMyData', myData);
                                }
                            }
                        });
                    }
                };
            }
            else
            {
                $scope.gridOptions = {
                    data: $scope.myTableData,
                    enableFullRowSelection: true,
                    multiSelect: false,
                    columnDefs: [],
                    onRegisterApi: function (gridApi) {
                        $scope.gridApi = gridApi;
                        $scope.gridApi.selection.on.rowSelectionChanged($scope, function (row) {
                            if (row.isSelected) {
                                myindex = -1;
                                while (myindex < row.grid.rows.length) {
                                    myindex += 1;
                                    if (row.grid.rows[myindex].isSelected) {
                                        break;
                                    }
                                }
                                if ($scope.selectedIndex != myindex) {
                                    if (myindex > -1) {
                                        $scope.selectedIndex = myindex;
                                        var myData = $.extend(true, {}, websoc.getprotocol_Request_Scalar());
                                        myData.interfaceTag = $scope.settings.selectedtag;
                                        myData.parameter = $scope.selectedIndex.toString();
                                        $rootScope.$emit('sendMyData', myData);
                                    }
                                }
                            }
                        });
                    }
                };
            }
            $scope.$on('wsConnection', function (event, args) {
                if (args == connectionEnum.CONNECTED) {
                    //Connected
                    $scope.subscribe();
                }
                else if (args == connectionEnum.DISCONNECTED) {
                    //Reset the settings
                    $scope.selectedSubscribed = false;
                    $scope.settings = eval('(' + $scope.interfacetags + ')');
                    $scope.arrayData = {}
                }
            });
        },
        link: function ($scope, element, attrs) {
            $scope.arrayData = {}
            $scope.settings = eval('(' + $scope.interfacetags + ')');
            $scope.newitem = {}
            
            if ($scope.isTreeEnabled)
            {
                //Do setup for tree view
                for (var item in $scope.settings.tags) {
                    $scope.gridOptions.columnDefs.push({
                        field: $scope.settings.tags[item].colname,
                        displayName: $scope.settings.tags[item].displayname,
                        visible: $scope.settings.tags[item].visible || true
                    });

                    $scope.newitem[$scope.gridOptions.columnDefs[item].field] = "";
                }
            }
            else
            {
                for (var item in $scope.settings.tags) {
                    $scope.gridOptions.columnDefs.push({
                        field: $scope.settings.tags[item].colname,
                        displayName: $scope.settings.tags[item].displayname
                    });

                    $scope.newitem[$scope.gridOptions.columnDefs[item].field] = "";

                    var value = $scope.settings.tags[item].tag;

                    $scope.$on('reportArray-' + value, function (event, args) {
                        var index = $scope.settings.tags.map(function (x) { return x.tag; }).indexOf(args.interfaceTag);
                        if (index != -1) {

                            //Check for null tag                        
                            if ($scope.settings.tags[index].tag == null)
                                return;

                            if ($scope.settings.tags[index].formattype != undefined &&
                                $scope.settings.tags[index].formattype != "none") {
                                $scope.settings.tags[index].data = args.formattedValues;
                            }
                            else {
                                //Extract correct data type based on returned valueType
                                if (args.valueType == "boolean") {
                                    $scope.settings.tags[index].data = args.booleanValues;
                                }
                                else if (args.valueType == "integer") {
                                    $scope.settings.tags[index].data = args.integerValues;
                                }
                                else if (args.valueType == "double") {
                                    $scope.settings.tags[index].data = args.doubleValues;
                                }
                                else if (args.valueType == "string") {
                                    $scope.settings.tags[index].data = args.stringValues;
                                }
                                else {
                                    //Type not known so return now !!!
                                    return;
                                }
                            }

                            //Update table data
                            for (var i = 0; i < $scope.settings.tags[index].data.length; i++) {
                                if ($scope.myTableData.length <= i) {
                                    //Get blank item, do JSON to from as we don't want a reference
                                    var blankitem = JSON.parse(JSON.stringify($scope.newitem));

                                    //Add new object - if data is added to new item then push onto myTableData
                                    var propupdated = false;
                                    for (property in blankitem) {
                                        //if property found for column then save and break loop 
                                        if (property == $scope.settings.tags[index].colname) {
                                            blankitem[property] = $scope.settings.tags[index].data[i];
                                            propupdated = true;
                                            break;
                                        }
                                    }

                                    //New data item populated ? Yes, then save into data store
                                    if (propupdated) {
                                        $scope.myTableData.push(JSON.parse(JSON.stringify(blankitem)));
                                    }
                                }
                                else {
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

                            $scope.$digest();
                        }
                    });
                    $scope.$on('interfaceStatus-' + value, function (event, args) {
                        for (var index = 0; index < $scope.settings.tags.length; index++) {
                            var subitem = $scope.settings.tags[index];
                            var tag = subitem.tag || null;
                            if (tag !== null && tag == args.interfaceTag) {
                                if (args.handshake == "requestSent") {
                                    //Do nothing
                                }
                                else if (args.handshake == "hostInProgress") {
                                    //Do nothing
                                }
                                else if (args.handshake == "hostComplete") {
                                    $scope.settings.tags[index].subscribed = true;
                                }
                                break;
                            }
                        }
                    });
                }
            }

            $scope.$on('reportScalar-' + $scope.settings.selectedtag, function (event, args) {
                $scope.selectedSubscribed = true;
                if ($scope.isTreeEnabled)
                {
                    foundindex = -1
                    for (itemindex = 0; itemindex < $scope.myTableData.length; itemindex++)
                    {
                        if ($scope.myTableData[itemindex].index == args.integerVal) {
                            foundindex = itemindex;
                            break;
                        }
                    }
                    if (foundindex > -1)
                    {
                        $scope.selectedIndex = args.integerVal;
//                        $scope.gridOptions.selectRow(foundindex, true);
						$scope.gridApi.selection.selectRow($scope.gridOptions.data[foundindex]);
                    }
                }
                else
                {
                    $scope.selectedIndex = args.integerVal;
//                    $scope.gridOptions.selectRow(args.integerVal, true);
					$scope.gridApi.selection.selectRow($scope.gridOptions.data[args.integerVal]);					
                }
            });

            $scope.$on('interfaceStatus-' + $scope.settings.selectedtag, function (event, args) {
                if (args.handshake == "requestSent") {
                    //Do nothing
                }
                else if (args.handshake == "hostInProgress") {
                    //Do nothing
                }
                else if (args.handshake == "hostComplete") {
                    $scope.selectedSubscribed = true;
                }
                
                //if this is a tree view control then update the data for each of the items
                if ($scope.isTreeEnabled)
                {
                    if (args.constraint.tree.trunk !== undefined)
                    {
                        //define recursive function for getting children
                        var getChildren = function (currentitem, treelevel) {
                            
                            var item = JSON.parse(JSON.stringify($scope.newitem));

                            //DO NOT LIKE THIS newentry definition, is there a better way ???

                            //Create new item
                            var newentry = []
                            newentry[0] = currentitem.descriptor
                            newentry[1] = currentitem.legend
                            newentry[2] = currentitem.index

                            //Add new object - if data is added to new item then push onto myTableData
                            var cIndex = 0;
                            for (property in item) {
                                //sanity check for property in blank item matching colname as defined by dev
                                if (property == $scope.settings.tags[cIndex].colname) {
                                    item[property] = newentry[cIndex];
                                }
                                else
                                {
                                    item[property] = "!ERROR"
                                }
                                //Move to next item
                                cIndex += 1
                            }

                            item.$$treeLevel = treelevel

                            foundIndex = -1
                            //Check for existing value in table
                            for (i = 0; i < $scope.myTableData.length; i += 1)
                            {
                                //DO NOT LIKE THIS colIndex hard coding, is there a better way ???
                                if ($scope.myTableData[i]["index"] == item["index"]) {
                                    foundIndex = i
                                    break;
                                }                               
                            }

                            if (foundIndex > -1)
                            {
                                //If we have found the same item then update this location with new data
                                //$scope.myTableData[foundIndex] = JSON.parse(JSON.stringify(item));
                                //Add new object - if data is added to new item then push onto myTableData
                                for (property in $scope.myTableData[foundIndex])
                                {
                                    for (itemProperty in item)
                                    {
                                        if (property == itemProperty)
                                        {
                                            if ($scope.myTableData[foundIndex][property] != item[itemProperty])
                                            {
                                                $scope.myTableData[foundIndex][property] = item[itemProperty];
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                //Not found so add as new item
                                $scope.myTableData.push(JSON.parse(JSON.stringify(item)));
                            }

                            // Need to be a bit clever here as the data will need to 
//                            $scope.myTableData.push(JSON.parse(JSON.stringify(newentry)))

                            //Do children

							if (currentitem.children !== undefined)
							{
								currentitem.children.forEach(function (child) {
									getChildren(child, treelevel + 1)
								})
							}

                        }

						if (args.constraint.tree.trunk.children !== undefined)
						{
							args.constraint.tree.trunk.children.forEach(function (child) {
								getChildren(child, 0)
							})
						}
						
//                        getChildren(args.constraint.tree.trunk, 0);
                    }

                    $scope.$digest();
                }

            });

            $scope.subscribe = function () {
                //do subscription for selected index
                if ($scope.selectedSubscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.settings.selectedtag;
                    if ($scope.isTreeEnabled)
                    {
                        myData.action.type = "treeSelection";
                        myData.action.treeSource = {};
                        myData.action.treeSource.treeType = $scope.myTreeSource;
                        if ($scope.myTreeSource == "directory")
                        {
                            myData.action.treeSource.path = $scope.myTreePath;
                        }
                        else if ($scope.myTreeSource == "database")
                        {
                            myData.action.treeSource.query = $scope.myTreeQuery;
                        }
                    }

                    $rootScope.$emit('sendMyData', myData);
                }
                //do subscription for table items
                for (var index = 0; index < $scope.settings.tags.length; index++) {
                    var item = $scope.settings.tags[index];
                    var subscribed = item.subscribed || null;
                    if ((subscribed == null || subscribed == false) && websoc.isConnected()) {
                        var myData = $.extend(true, {}, websoc.getprotocol_Array_Subscription());
                        myData.interfaceTag = item.tag;
                        myData.format.type = item.formattype;
                        myData.format.properties.string = item.formatstring;
                        myData.format.properties.unitsSource = item.formatunitssource;
                        myData.format.properties.unitSuffix = item.formatunitsuffix;
                        $rootScope.$emit('sendMyData', myData);
                    }
                }
            };
            $scope.subscribe();
            $scope.destroyTimer = null;
            element.bind("$destroy", function () {
                for (var index = 0; index < $scope.settings.tags.length; index++) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Array_Unsubscription());
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