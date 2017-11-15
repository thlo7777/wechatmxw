(function($) {

    console.log(jsObj);

    $row = $('<div>').attr({'class': "row"}).appendTo('.wm-content.container');
    $col_6 = $('<div>').attr({'class': "col-xs-6"}).appendTo($row);
    
    $a = $('<a>').attr({
        'class': "btn btn-block btn-lg btn-default",
        'href': jsObj.hotel_list_page_url
        }).html('\
            <i class="zmdi zmdi-file-plus zmdi-hc-3x mdc-text-red"></i><br> \
            <span><i class="zmdi zmdi-border-color zmdi-hc-fw"></i>我的店铺</span></a> \
            '
        ).appendTo($col_6);

    $col_6 = $('<div>').attr({'class': "col-xs-6"}).appendTo($row);
    
    $a = $('<a>').attr({
        'class': "btn btn-block btn-lg btn-default",
        'href': jsObj.create_leaf_page_url
        }).html('\
            <i class="zmdi zmdi-file-text zmdi-hc-3x mdc-text-red"></i><br> \
            <span><i class="zmdi zmdi-border-color zmdi-hc-fw"></i>宣传页</span></a> \
            '
        ).appendTo($col_6);

})(jQuery);

