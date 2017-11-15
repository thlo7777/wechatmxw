<?php

//hotel_create_leaf_page.inc
function hotel_create_leaf_page() {

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

    $page['title'] = '宣传页推广';
    $page['body_id'] = "hotel-create-leaf-page";
    $page['js_var']['body_id'] = '#hotel-create-leaf-page';
    $page['js_var']['rest_url'] = WECHAT_MOBILE_URL . '/hotel-mobile-rest';

    return $page;
}

function deliver_hotel_create_leaf_page_html($page) {
    global $base_url;

    $css = module_invoke('wechat_asset', 'css_array');
    array_push($css,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/hotel_create_page.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/loading_icon1/loading_icon1.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/progressbar/progressBar.css',
    );

    //One final thing to check is to make sure that you are not loading any plugins before you load jQuery
    $js = module_invoke('wechat_asset', 'base_js_array');
    array_push($js,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/loading_icon1/loading_icon1.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/progressbar/progressBar.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/hotel_create_leaf_page.js'
    );

    $render['html_page'] = array(
        '#theme' => 'hotel_create_leaf_page',
        '#page' => $page,
        '#styles' => $css,
        '#scripts' => $js,
    );

    print drupal_render($render);
}

/**
 * End of hotel_create_leaf_page.inc
 */

