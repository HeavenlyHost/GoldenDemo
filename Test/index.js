var maxWidth  = $('#outer').width();
var maxHeight = $('#outer').height();

var windowWidth = $(window).width();
var windowHeight = $(window).height();

//$('#root').bind('resize', function () {
$(window).resize(function() {
    var $window = $(window);
    var width = $window.width();
    var height = $window.height();
    var scale;

    if (width >= windowWidth && height >= windowHeight)
    {
        scale = Math.max(width / windowWidth, height / windowHeight);
    }
    else
    {
        scale = Math.min(width / windowWidth, height / windowHeight);
    }
    
    console.debug("scale: " + scale);

    $('#outer').css({ 'transform': 'scale(' + scale + ')' });
    $('#wrap').css({ width: maxWidth * scale, height: maxHeight * scale });
});