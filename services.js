var servs = angular.module('myServices', []);

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
