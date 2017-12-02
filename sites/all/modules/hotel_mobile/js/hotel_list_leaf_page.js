(function($) {

    console.log(jsObj);

    //create hotel list table, use plugin
    $('.wm-content.container').table_list({
        xyz: jsObj.xyz,
        rest_api: jsObj.rest_url,
        edit_url: jsObj.hotel_create_leaf_page_url,
        list_node_type: jsObj.list_node_type,
        table_header: "店铺列表"
    });


})(jQuery);

