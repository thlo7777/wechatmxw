(function($) {

    console.log(jsObj);

    $row = $('<div>').attr({'class': "row"}).appendTo('.wm-content.container');
    $col_4 = $('<div>').attr({'class': "col-xs-offset-8 col-xs-4"}).appendTo($row);
    
    $a = $('<a>').attr({
        'class': "btn btn-block btn-primary btn-sm",
        'href': jsObj.hotel_create_page_url
        }).html('\
            <i class="zmdi zmdi-plus-circle zmdi-hc-fw mdc-text-red"></i>增加店铺</a> \
            '
        ).appendTo($col_4);


    //create hotel list table, use plugin
    $('.wm-content.container').table_list({
        xyz: jsObj.xyz,
        rest_api: jsObj.rest_url,
        edit_url: jsObj.hotel_edit_page_url,
        list_node_type: jsObj.list_node_type,
        table_header: "店铺列表"
    });


})(jQuery);

