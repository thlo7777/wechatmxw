<?php

//hotel_list_page.inc
function hotel_list_page() {

    global $base_url;
    $page = array();

    //check if it's a mobile browser, use touch event feature to test.
    $page['inline_js'] = MOBILE_INLINE_JS;

    //get page state
    $state = isset($_GET['state']) ? $_GET['state'] : '';
    switch ($state) {
        case '':
        case '100':
            // normally access from shopping home page and user id can be getted
            break;

        default:
            $page['error_info'] = '页面state: ' . $state . ' 错误';
            return $page;
            break;
    }
    //get page code
    $code = isset($_GET['code']) ? $_GET['code'] : '';

    if($code != '') {

        $curr_url = request_uri();
        $page_path = parse_url($curr_url);
        
        //get wechat user info
        $json = module_invoke('wechat_api', 'oauth2_get_accss_token',
            $code,
            'snsapi_userinfo',
            $state,
            $base_url . $page_path['path'], //send curr page url (no query)
            $page_path['path']);

        if ($json != null) {
            //get Openid by code
            $page['js_var']['xyz'] = md5($json->openid);
        }
    }

    $page['title'] = '我的店铺';
    $page['body_id'] = "hotel-list-page";
    $page['js_var']['body_id'] = '#hotel-list-page';
    $page['js_var']['rest_url'] = WECHAT_MOBILE_URL . '/hotel-mobile-rest';
    $page['js_var']['list_node_type'] = 'hotel_main_page';


    $page['js_var']['hotel_create_page_url'] = t(
        variable_get('oauth2 redirect request'),
        array(
            '@APPID' => variable_get('wechat_AppID'),
            '@URL' => $base_url . "/" . WECHAT_MOBILE_URL . '/hotel-create-page',
            '@SNSAPI' => "snsapi_userinfo",
            '@STATE' => "100",
        )
    );

    $page['js_var']['hotel_edit_page_url'] = t(
        variable_get('oauth2 redirect request'),
        array(
            '@APPID' => variable_get('wechat_AppID'),
            '@URL' => $base_url . "/" . WECHAT_MOBILE_URL . '/hotel-edit-page/%/edit',
            '@SNSAPI' => "snsapi_userinfo",
            '@STATE' => "100",
        )
    );

    return $page;
}

function deliver_hotel_list_page_html($page) {
    global $base_url;

    $css = module_invoke('wechat_asset', 'css_array');
    array_push($css,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/hotel_list_page.css'
    );

    //One final thing to check is to make sure that you are not loading any plugins before you load jQuery
    $js = module_invoke('wechat_asset', 'base_js_array');
    array_push($js,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/table_pag/jquery.bootpag.min.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/table_pag/table_pag.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/hotel_list_page.js'
    );

    $render['html_page'] = array(
        '#theme' => 'hotel_list_page',
        '#page' => $page,
        '#styles' => $css,
        '#scripts' => $js,
    );

    print drupal_render($render);
}

/**
 * End of hotel_list_page.inc
 */

