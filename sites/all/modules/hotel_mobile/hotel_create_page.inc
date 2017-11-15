<?php

//hotel_create_page.inc
function hotel_create_page() {

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

    $page['title'] = '新店开张';
    $page['body_id'] = "hotel-create-page";
    $page['js_var']['body_id'] = '#hotel-create-page';
    $page['js_var']['rest_url'] = WECHAT_MOBILE_URL . '/hotel-mobile-rest';

    $page['js_var']['form']['field_title']['label'] = '客栈名称';
    $page['js_var']['form']['field_title']['fname'] = 'field_title';

    $field_instances = field_info_instances('node', 'hotel_main_page');
    $page['js_var']['form']['field_hotel_gps_loc']['label'] = $field_instances['field_hotel_gps_loc']['label'];
    $page['js_var']['form']['field_hotel_gps_loc']['fname'] = 'field_hotel_gps_loc';
    $page['js_var']['form']['field_hotel_address']['label'] = $field_instances['field_hotel_address']['label'];
    $page['js_var']['form']['field_hotel_address']['fname'] = 'field_hotel_address';
    $page['js_var']['form']['field_hotel_view_image']['fname'] = 'field_hotel_view_image';
    $page['js_var']['form']['field_hotel_view_image']['label'] = $field_instances['field_hotel_view_image']['label'];
    $page['js_var']['form']['field_hotel_phone']['fname'] = 'field_hotel_phone';
    $page['js_var']['form']['field_hotel_phone']['label'] = $field_instances['field_hotel_phone']['label'];


    $all_fields_on_my_website = field_info_fields();

    $field_instances = field_info_instances('node', 'hotel_facilities_page');
    foreach ($field_instances as $key => $field) {
        $page['js_var']['form']['ref'][$key]['label'] = $field['label'];
        $page['js_var']['form']['ref'][$key]['fname'] = $key;
        if ($field['display']['default']['type'] == 'list_default') {

            $list = list_allowed_values($all_fields_on_my_website[$key]);
            foreach ($list as $id => $text) {
                $page['js_var']['form']['ref'][$key]['list'][] = array(
                    'id' => $id,
                    'text' => $text
                );
            }
        }
    }

    return $page;
}

function deliver_hotel_create_page_html($page) {
    global $base_url;

    $css = module_invoke('wechat_asset', 'css_array');
    array_push($css,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/hotel_create_page.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/select2.min.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/select2-bootstrap.min.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/loading_icon1/loading_icon1.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/progressbar/progressBar.css',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_amap/hotel_amap.css'
    );

    //One final thing to check is to make sure that you are not loading any plugins before you load jQuery
    $js = module_invoke('wechat_asset', 'js_map_array');
    array_push($js,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/loading_icon1/loading_icon1.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/progressbar/progressBar.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/select2.min.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/i18n/zh-CN.js',
        //$base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_form/jquery.validate.min.js',
        //$base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_form/additional-methods.min.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_amap/hotel_amap.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_form/hotel_form.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/hotel_create_page.js'
    );

    $render['html_page'] = array(
        '#theme' => 'hotel_create_page',
        '#page' => $page,
        '#styles' => $css,
        '#scripts' => $js,
    );

    print drupal_render($render);
}

/**
 * End of hotel_create_page.inc
 */

