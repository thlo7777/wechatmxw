<?php

//hotel_front_page.inc
function hotel_front_page($nid) {

    global $base_url;
    $page = array();

    //check if it's a mobile browser, use touch event feature to test.
    $page['inline_js'] = MOBILE_INLINE_JS;

    $page['body_id'] = "hotel-front-page";

    //get page state
    $state = isset($_GET['state']) ? $_GET['state'] : '';

    $id_matchs = preg_split("/200:/", $state);
    if ( empty($id_matchs[1]) ) {
        $page['js_var']['page_error'] = 1;
        return $page;
    }

    //verify parent md5id
    $parent_uid = module_invoke('wechat_asset', 'check_userid_exists', "", $id_matchs[1]);
    if ( !$parent_uid ) {
        $page['js_var']['page_error'] = 1;
        return $page;
    }

    //get page code
    $code = isset($_GET['code']) ? $_GET['code'] : '';

    if($code != '') {

        $curr_url = request_uri();
        $page_path = parse_url($curr_url);
        
        //get wechat user info
        $json = module_invoke('wechat_api', 'oauth2_get_accss_token',
            $code,
            'snsapi_base',
            $state,
            $base_url . $page_path['path'], //send curr page url (no query)
            $page_path['path']);

        if ($json != null) {
            //get Openid by code
            $page['js_var']['xyz'] = md5($json->openid);

            //get code parent md5openid and hotel nid
        }
    }

    try {
        $wrapper = entity_metadata_wrapper('node', $nid);
        $page['title'] = $wrapper->title->value();

        if ( $wrapper->status->value() == "0" ) {
            $page['js_var']['hotel_offshelf'] = "1";
            return $page;
        }

        $page['js_var']['field_hotel_gps_loc'] = $wrapper->field_hotel_gps_loc->value();
        $page['js_var']['field_hotel_address'] = $wrapper->field_hotel_address->value();
        $page['js_var']['field_hotel_phone'] = $wrapper->field_hotel_phone->value();

        //hotel facility reference
        $fac_ref_wrapper = entity_metadata_wrapper('node', $wrapper->field_hotel_facility_ref->value()->nid);
        $all_fields_on_my_website = field_info_fields();

        $field_instances = field_info_instances('node', 'hotel_facilities_page');
        foreach ($field_instances as $key => $field) {
            $page['js_var']['field_hotel_facility_ref'][$key]['label'] = $field['label'];
            $page['js_var']['field_hotel_facility_ref'][$key]['fname'] = $key;
            $page['js_var']['field_hotel_facility_ref'][$key]['value'] = $fac_ref_wrapper->{$key}->value();

            if ($field['display']['default']['type'] == 'list_default') {

                $list = list_allowed_values($all_fields_on_my_website[$key]);
                foreach ($list as $id => $text) {
                    $page['js_var']['field_hotel_facility_ref'][$key]['list'][] = array(
                        'id' => $id,
                        'text' => $text
                    );
                }
            }
        }

        //footer page url
        $page['js_var']['footer'][0]['label'] = "照片墙";
        $page['js_var']['footer'][0]['icon_class'] = "zmdi zmdi-filter-frames zmdi-hc-fw";
        $page['js_var']['footer'][0]['url'] = "https://news.sina.com.cn";
        $page['js_var']['footer'][1]['label'] = "接力红包";
        $page['js_var']['footer'][1]['icon_class'] = "zmdi zmdi-balance-wallet zmdi-hc-fw";
        $page['js_var']['footer'][1]['url'] = "https://news.sohu.com";

    }
    catch (EntityMetadataWrapperException $e) {
        watchdog_exception(__FUNCTION__, $e);
        $page['title'] = "未找到酒店";
        $page['js_var']['page_error'] = 1;
    }


    return $page;
}

function deliver_hotel_front_page_html($page) {
    global $base_url;

    $css = module_invoke('wechat_asset', 'css_array');
    array_push($css,
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/css/hotel_front_page.css',
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
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_amap/hotel_amap.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_form/hotel_form.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/plugin/hotel_footer/hotel_footer.js',
        $base_url . '/' . drupal_get_path('module', 'hotel_mobile') . '/js/hotel_front_page.js'
    );

    $render['html_page'] = array(
        '#theme' => 'hotel_front_page',
        '#page' => $page,
        '#styles' => $css,
        '#scripts' => $js,
    );

    print drupal_render($render);
}

/**
 * End of hotel_front_page.inc
 */

