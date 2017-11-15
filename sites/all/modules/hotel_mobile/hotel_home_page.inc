<?php

//hotel_home_page.inc
function hotel_home_page() {

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

    $page['title'] = '酒店管理页面';
    $page['body_id'] = "hotel-home-page";
    $page['js_var']['body_id'] = '#hotel-home-page';
    $page['js_var']['rest_url'] = WECHAT_MOBILE_URL . '/hotel-mobile-rest';

    $page['js_var']['hotel_list_page_url'] = t(
        variable_get('oauth2 redirect request'),
        array(
            '@APPID' => variable_get('wechat_AppID'),
            '@URL' => $base_url . "/" . WECHAT_MOBILE_URL . '/hotel-list-page',
            '@SNSAPI' => "snsapi_userinfo",
            '@STATE' => "100",
        )
    );

    $page['js_var']['create_leaf_page_url'] = t(
        variable_get('oauth2 redirect request'),
        array(
            '@APPID' => variable_get('wechat_AppID'),
            '@URL' => $base_url . "/" . WECHAT_MOBILE_URL . '/hotel-create-leaf-page',
            '@SNSAPI' => "snsapi_userinfo",
            '@STATE' => "100",
        )
    );

    return $page;
}

function deliver_hotel_home_page_html($page) {
    global $base_url;

    $css = module_invoke('wechat_asset', 'css_array');
    array_push($css,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/hotel_home_page.css'
    );

    //One final thing to check is to make sure that you are not loading any plugins before you load jQuery
    $js = module_invoke('wechat_asset', 'base_js_array');
    array_push($js,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/hotel_home_page.js'
    );

    $render['html_page'] = array(
        '#theme' => 'hotel_home_page',
        '#page' => $page,
        '#styles' => $css,
        '#scripts' => $js,
    );

    print drupal_render($render);
}

/**
 * End of hotel_home_page.inc
 */

