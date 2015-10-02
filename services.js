var servs = angular.module('myServices', []);

servs.service('configmanager', ['$http', function($http){
    var ready = false;
    var wsIpAddress = "0.0.0.0";
    var wsPort = 0;
    
    var readConfig = function(){
          var req = {
              method: 'GET',
              url: 'config.xml',
              transformResponse: function(data) {
                      // convert the data to JSON and provide
                      // it to the success function below
                      var json = xmlToJSON.parseString( data );
                      return json;
                      }              
          }
            
          $http(req).success( function(data){
              wsIpAddress = data.config[0].ws[0].ip[0]._text;
              wsPort = data.config[0].ws[0].port[0]._text;
              ready = true;
          });
    }; 
    
    var getIP = function(){
        return wsIpAddress;
    }

    var getPort = function(){
        return wsPort;
    }

    var isReady = function(){
        return ready;
    }
    
    return {
        isReady: isReady,
        getIP: getIP,
        getPort: getPort,
        readConfig: readConfig
    };      
}]);

servs.service('fileaccessor', ['$http', function($http){
   
    var getFilesWithFullPathAndExt = function(src_fldr, // e.g. templates, relative path from index.html
                                        complete_func   // function to call when complete, this must accept one param for myFiles to be passed back
                                        ){
        var myFiles = [];                        
        
        $http.get(src_fldr).success(function (t) {
    
            var result = xmlToJSON.parseString(t);        
    
            result.Listing[0].Entries[0].Entry.forEach(function(element) {
                var t = element.urlPath[0]._text;
                myFiles.push(t);            
            }, this);
    
            if (complete_func != null)
            {
                complete_func(myFiles);
            }           
        }).error(function (data, status){
            console.debug("ERROR: " + data);
        }); 
    };

    var getFilesWithExt = function(src_fldr,       // e.g. templates, relative path from index.html
                                        complete_func   // function to call when complete, this must accept one param for myFiles to be passed back
                                        ){
        var myFiles = [];                        
        
        $http.get(src_fldr).success(function (t) {
    
            var result = xmlToJSON.parseString(t);        
    
            result.Listing[0].Entries[0].Entry.forEach(function(element) {
                var t = element.urlPath[0]._text;
                t = t.substr(t.lastIndexOf("/")+1, t.length-t.lastIndexOf("/")-1);
                myFiles.push(t);            
            }, this);
    
            if (complete_func != null)
            {
                complete_func(myFiles);
            }           
        }).error(function (data, status){
            console.debug("ERROR: " + data);
        }); 
    };
             
    var getFileNamesOnly = function(src_fldr,       // e.g. templates, relative path from index.html
                                    complete_func   // function to call when complete, this must accept one param for myFiles to be passed back
                                    ){
        var myFiles = [];                        
        
        $http.get(src_fldr).success(function (t) {
    
            var result = xmlToJSON.parseString(t);        
    
            result.Listing[0].Entries[0].Entry.forEach(function(element) {
                var t = element.urlPath[0]._text;
                t = t.substr(t.lastIndexOf("/")+1, t.length-t.lastIndexOf("/")-1);
                t = t.substr(0, t.lastIndexOf("."));
                myFiles.push(t);            
            }, this);
    
            if (complete_func != null)
            {
                complete_func(myFiles);
            }           
        }).error(function (data, status){
            console.debug("ERROR: " + data);
        }); 
    };
    
    return {
        getFilesWithFullPathAndExt: getFilesWithFullPathAndExt,
        getFilesWithExt: getFilesWithExt,
        getFileNamesOnly: getFileNamesOnly
    };            
}]);

servs.service('websoc', ['$timeout', '$rootScope', 'configmanager', function($timeout, $rootScope, configmanager) {        

    var connected = connectionEnum.DISCONNECTED;    
    var websock = null;    
    
    var protocol_Request_Scalar = {
        'title': 'requestScalar',
        'interfaceTag': '',
        'valueType': 'Boolean',
        'Boolean': 'false',
        'Integer': '0',
        'Double': '0',
        'String': ''
    }

    var protocol_Scalar_Subscription = {
        'title': 'scalarSubscription',
        'interfaceTag': '',
        'actionType': 'InsertConstant',
        'Parameter': '',
        'formatType': 'None',
        'FormatString': '',
        'UnitsSource': 'InterfaceDatabase',
        'UnitSuffix': '',
        'Gain': '0',
        'Offset': '0',
        'Quantity': 'Height',
        'Period': '0',
        'Phase': '0'
    }

    var protocol_Scalar_Unsubscription = {
        'title': 'scalarUnsubscription',
        'interfaceTag': '',
    }

    var protocol_Array_Subscription = {
        'title': 'arraySubscription',
        'interfaceTag': '',
        'actionType': 'InsertConstant',
        'Parameter': '',
        'formatType': 'None',
        'FormatString': '',
        'UnitsSource': 'InterfaceDatabase',
        'UnitSuffix': '',
        'Gain': '0',
        'Offset': '0',
        'Quantity': 'Height',
        'Period': '0',
        'Phase': '0'
    }

    var protocol_Array_Unsubscription = {
        'title': 'arrayUnsubscription',
        'interfaceTag': '',
    }

    var myWebSocket = function() {
        if (connected === connectionEnum.DISCONNECTED)
        {
            if (configmanager.isReady())
            {
                var ip = configmanager.getIP();
                var port = configmanager.getPort();            
                connected = connectionEnum.CONNECTING;
                websock = new WebSocket("ws://" + ip + ":" + port + "/goldenserver");    
                websock.onopen = function(evt){
                    connected = connectionEnum.CONNECTED;  
                    $rootScope.$broadcast('wsConnection', connected);
                }
                websock.onmessage = function(evt){
                    var jsonData = JSON.parse(evt.data);
                    $rootScope.$broadcast(jsonData.title + "-" + jsonData.interfaceTag, jsonData);
                }
                websock.onclose = function(evt){
                    connected = connectionEnum.DISCONNECTED;  
                    $rootScope.$broadcast('wsConnection', connected);
                }   
                websock.onerror = function(evt){
                    console.debug("error: " + evt)  
                    connected = connectionEnum.DISCONNECTED;  
                }             
            }
        }
        $timeout(myWebSocket, 1000);            
    };

    $rootScope.$on('sendMyData', function (event, args) {
        if (connected == connectionEnum.CONNECTED) {
            websock.send(JSON.stringify(args));
        }
    })
        
    myWebSocket();
    
    var getprotocol_Scalar_Subscription = function () {
        return protocol_Scalar_Subscription;
    }

    var getprotocol_Array_Subscription = function () {
        return protocol_Array_Subscription;
    }

    var getprotocol_Scalar_Unsubscription = function () {
        return protocol_Scalar_Unsubscription;
    }

    var getprotocol_Array_Unsubscription = function () {
        return protocol_Array_Unsubscription;
    }

    var getprotocol_Request_Scalar = function () {
        return protocol_Request_Scalar;
    }

    var isConnected = function(){
        return (connected == connectionEnum.CONNECTED);
    };
    
    return {
        getprotocol_Request_Scalar: getprotocol_Request_Scalar,
        getprotocol_Scalar_Subscription: getprotocol_Scalar_Subscription,
        getprotocol_Array_Subscription: getprotocol_Array_Subscription,
        getprotocol_Scalar_Unsubscription: getprotocol_Scalar_Unsubscription,
        getprotocol_Array_Unsubscription: getprotocol_Array_Unsubscription,
        isConnected: isConnected
    };    
}]);
