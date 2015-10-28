var smTable = angular.module('smTable', ['ngAnimate', 'ui.bootstrap', 'myServices', 'ui.grid', 'ui.grid.selection', 'ui.grid.autoResize']);

smTable.directive('dirTable', ['$rootScope', '$timeout', '$interval', 'websoc', 'uiGridConstants', function ($rootScope, $timeout, $interval, websoc, uiGridConstants) {
    return {
        restrict: 'EA',
        scope: {
            interfacetags: '@',
            actiontype: '@',
            parameter: '@',
            period: '@',
            phase: '@'
        },
        template:'<div ui-grid="gridOptions" class="myGrid" ui-grid-selection ui-grid-auto-resize></div>',
        controller: function ($scope) {
            $scope.ipoll = null;
            $scope.selectedSubscribed = false;
            $scope.selectedIndex = 0;
            $scope.myTableData = [];
            $scope.gridOptions = {
                data: $scope.myTableData,
                enableFullRowSelection: true,
                multiSelect: false,
                columnDefs: [],
                onRegisterApi: function (gridApi) {
                    $scope.gridApi = gridApi;
                    //gridApi.selection.selectRow(vm.gridOptions.data[0]);
                    $scope.gridApi.selection.on.rowSelectionChanged($scope, function (row) {
                        if (row.isSelected) {
							myindex = -1;
							while (myindex < row.grid.rows.length)
							{
								myindex += 1;
								if (row.grid.rows[myindex].isSelected)
								{
									break;
								}
							}							
                            if ($scope.selectedIndex != myindex)
                            {
								if (myindex > -1)
								{
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
            $scope.ipolllink = null
            $scope.arrayData = {}
            $scope.settings = eval('(' + $scope.interfacetags + ')');
            $scope.newitem = {}
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
                            $scope.settings.tags[index].formattype != "none")
                        {
                            $scope.settings.tags[index].data = args.formattedValues;
                        }
                        else
                        {
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

            $scope.$on('reportScalar-' + $scope.settings.selectedtag, function (event, args) {
                $scope.selectedSubscribed = true;
                $scope.selectedIndex = args.integerVal;
            });

            $scope.$on('interfaceStatus-' + $scope.settings.selectedtag, function (event, args) {
                if ($scope.settings.selectedtag == args.interfaceTag) {
                    if (args.handshake == "requestSent") {
                        //Do nothing
                    }
                    else if (args.handshake == "hostInProgress") {
                        //Do nothing
                    }
                    else if (args.handshake == "hostComplete") {
                        $scope.selectedSubscribed = true;
                    }
                }
            });

            $scope.subscribe = function () {
                //do subscription for selected index
                if ($scope.selectedSubscribed == false && websoc.isConnected()) {
                    var myData = $.extend(true, {}, websoc.getprotocol_Scalar_Subscription());
                    myData.interfaceTag = $scope.settings.selectedtag;
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