(function($) {
    console.log(jsObj);


    //not find page error
    if ( jsObj.hasOwnProperty('page_error') ) {
        $alert = $('<div>').attr({'class': "alert alert-dismissible alert-warning" }).insertBefore(".wm-content.container");
        $('button').attr({
            'type': "button",
            'class': "close",
            'data-dismiss': "alert"
            }).html("&times;").appendTo($alert);

        $('<h4>').attr({'class': "text-center"}).html('提示!').appendTo($alert);
        $('<p>').html('\
            没有找到该酒店！\
        '
        ).appendTo($alert);
    }

    //hotel shelf off
    if ( jsObj.hasOwnProperty('hotel_offshelf') ) {
        $alert = $('<div>').attr({'class': "alert alert-dismissible alert-warning" }).insertBefore(".wm-content.container");
        $('button').attr({
            'type': "button",
            'class': "close",
            'data-dismiss': "alert"
            }).html("&times;").appendTo($alert);

        $('<h4>').attr({'class': "text-center"}).html('提示!').appendTo($alert);
        $('<p>').html('\
            该酒店已经下架！\
        '
        ).appendTo($alert);
    }

    //insert foot 
    $(".wm-content.container").hotel_footer({ buttons: jsObj.footer });

    //insert amap 


})(jQuery);

