<?php
/**
 * hotel_rest_api.inc
 **/

function hotel_rest_api() {

    //send response to client
    $send_data = array();

    $method = strtoupper($_SERVER['REQUEST_METHOD']);
    //watchdog('dld mobile', '_SERVER data = <pre>@data</pre>', array('@data' => print_r($_SERVER, TRUE)));

    //GET ajax and data from client
    if ($method == 'GET') {
        $data = isset($_GET['data']) ? $_GET['data'] : '';
        //watchdog('dld mobile', '_GET data = <pre>@data</pre>', array('@data' => print_r($data, TRUE)));
    }

    //POST ajax and data from client
    if ($method == 'POST') {
        $raw_data = file_get_contents("php://input"); //get xml data
        $data = drupal_json_decode($raw_data);
        //watchdog('dld mobile', 'json data = <pre>@data</pre>', array('@data' => print_r($data, TRUE)));

    }

    if ($data['type'] == 'hotel_add_form') {
        $send_data = hotel_create_node($data);
    } 
    if ($data['type'] == 'hotel_edit_form') {
        $send_data = hotel_edit_node($data);
    } 
    if ($data['type'] == 'all_list_count') {
        $send_data = get_node_list_count($data);
    } 
    if ($data['type'] == 'get_list_pager') {
        $send_data = get_node_list_pager($data);
    } 

    if ($data['type'] == 'hotel_get_leaf_tpl') {
        $send_data = get_leaf_tpl($data);
    } 

    if ($data['type'] == 'hotel_generate_leaf') {
        $send_data = create_my_leaf_page($data);
    } 

    drupal_add_http_header('Content-Type', 'application/json');
    $json_data = json_encode($send_data, JSON_UNESCAPED_UNICODE);

    echo $json_data;

}

function hotel_create_node($data) {
    
    //watchdog(__FUNCTION__, "data = <pre>@data</pre>", array("@data" => print_r($data, TRUE)));

    $element = array('data' => array('count' => 0));


    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    if (! ($node_id = create_hotel_content_info_node($uid, $data['xyz'], $data['apply_form']))) {
        return $element;    //can't create apply form node, return 0
    } else {

        $element['data']['shop_nid'] = $node_id;
        $element['data']['message'] = 'ok';
        $element['data']['count'] = 1;
    }

    return $element;
}

function create_hotel_content_info_node($uid, $md5_openid, $apply_form) {

    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property

    $ref_nid = null;
    try {

        global $language;
        $values = array(
          'type' => 'hotel_facilities_page',
          'uid' => $uid,
          'status' => 1,
          'language' => $language->language
        );
        $entity = entity_create('node', $values);
        $wrapper = entity_metadata_wrapper('node', $entity);
        $wrapper->title->set($apply_form['formField']['field_title']['value'] . '服务设施');

        $check = false;

        foreach ($apply_form['formField']['ref'] as $key => $field) {
            if ( !empty($field['value']) ) {
                $wrapper->{$key}->set($field['value']);
                $check = true;  //notify to save 
            }
        }

        if ($check) {
            $wrapper->save();
            $ref_nid = $wrapper->nid->value();
        }
    }
    // Some properties like entity_metadata_book_get_properties() throw
    // exceptions, so we catch them here and ignore the property (deny access).
    catch (EntityMetadataWrapperException $e) {
        watchdog_exception(__FUNCTION__, $e);
        return false;
    }

    try {

        global $language;
        $values = array(
          'type' => 'hotel_main_page',
          'uid' => $uid,
          'status' => 1,
          'language' => $language->language
        );

        $entity = entity_create('node', $values);

        // The entity is now created, but we have not yet simplified use of it.
        // Now create an entity_metadata_wrapper around the new node entity
        // to make getting and setting values easier
        $wrapper = entity_metadata_wrapper('node', $entity);

        $wrapper->title->set($apply_form['formField']['field_title']['value']);

        $point = array(
            'lat' => $apply_form['formField']['field_hotel_gps_loc']['latitude'],
            'lon' => $apply_form['formField']['field_hotel_gps_loc']['longitude']
        );
        $wrapper->field_hotel_gps_loc->set(geofield_compute_values($point, GEOFIELD_INPUT_LAT_LON));

        $wrapper->field_hotel_address->set($apply_form['formField']['field_hotel_address']['value']);
        $wrapper->field_hotel_phone->set($apply_form['formField']['field_hotel_phone']['value']);
        $wrapper->field_myself_md5_openid->set($md5_openid);

        if ($ref_nid) {
            $wrapper->field_hotel_facility_ref->set($ref_nid);
        }

        /**
         * create field_hotel_view_image
         **/
        if (! set_pics_field_pictures($wrapper->field_hotel_view_image,
            $apply_form['formField']['field_hotel_view_image']['new_img'], 'field_hotel_view_image'))
        {
            throw new EntityMetadataWrapperException('field_hotel_view_image error');
            return false;
        }

        $wrapper->save();
    }

    // Some properties like entity_metadata_book_get_properties() throw
    // exceptions, so we catch them here and ignore the property (deny access).
    catch (EntityMetadataWrapperException $e) {
        watchdog_exception('create_dld_shop_content_info_node', $e);
        return false;
    }

    return $wrapper->nid->value();
}

/*
 * ID pictures, large picture, height 640
 */
function set_pics_field_pictures(&$field, $media_ids, $field_name) {

    //download and format pictures
    $image_file = array();
    //create large image and small image
    foreach($media_ids as $delta => $media_id) {
        download_media_id($image_file, $media_id);
    }

    if (!empty($image_file)) {

        //get caridtia
        $field_info  = field_info_field($field_name);
        $cardinality = $field_info['cardinality'];

        foreach ($image_file as $file_path) {

            $file_temp = file_get_contents($file_path);

            $split_str = explode("/", $file_path);

            $fm = HOTEL_CONTENT_PATH . end($split_str);
            $file = file_save_data($file_temp, $fm, FILE_EXISTS_REPLACE);

            if ($cardinality > 1) {
                $field[] = array('fid' => $file->fid);
            } else {
                $field->set(array('fid' => $file->fid));
            }

        }

    } else {
        return false;
    }

    return true;
}

//download media id from wechat server
function download_media_id(&$image_file, $media_id) {

    $save_path = drupal_realpath(HOTEL_CONTENT_PATH);
    if ( !file_exists($save_path) ) {
        mkdir($save_path);
    }

    if (preg_match('/;base64,/i', $media_id)) {
        //base64 code image file
        $base64_data = explode(',', $media_id);
        $matches = array();
        preg_match('/\/([^\$]+);/', $base64_data[0], $matches);
        $image_type = end($matches);
        if ($image_type == 'jgp') {
            $image_type = "jpeg";   //ios can't open jgp, replace to jpeg
        }

        //create a image file use DLD_RESTFUL_UPLOAD_IMAGE_FOLDER
        $fm = $save_path . "/ios_upload_" . mt_rand() . "." . $image_type;
        //watchdog('create_media_file_to_wechat_server', 'filename = <pre>@data</pre>', array('@data' => print_r($fm, TRUE)));
        $file_name = file_unmanaged_save_data(base64_decode($base64_data[1]), $fm, FILE_EXISTS_RENAME);
    } else {
        //wechat server id for image media
        $file_name = module_invoke('wechat_api', 'get_temp_media', $save_path, $media_id);
        $image_file[] = $file_name;
    }

    $image_file[] = $file_name;

    return null;

}

function hotel_edit_node($data) {
    //watchdog(__FUNCTION__, "data = <pre>@data</pre>", array("@data" => print_r($data, TRUE)));

    $element = array('data' => array('count' => 0));


    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    if ( !update_hotel_content_info_node($data['nid'], $data['xyz'], $data['apply_form']) ) {
        return $element;    //can't update apply form node, return 0
    } else {

        $element['data']['message'] = 'ok';
        $element['data']['count'] = 1;
    }

    return $element;
}

function update_hotel_content_info_node($nid, $md5_openid, $apply_form) {

    try {
        $wrapper = entity_metadata_wrapper('node', $nid);
       
        $wrapper->title->set($apply_form['formField']['field_title']['value']);

        $point = array(
            'lat' => $apply_form['formField']['field_hotel_gps_loc']['latitude'],
            'lon' => $apply_form['formField']['field_hotel_gps_loc']['longitude']
        );
        $wrapper->field_hotel_gps_loc->set(geofield_compute_values($point, GEOFIELD_INPUT_LAT_LON));

        $wrapper->field_hotel_address->set($apply_form['formField']['field_hotel_address']['value']);
        $wrapper->field_hotel_phone->set($apply_form['formField']['field_hotel_phone']['value']);

        $ref_wrapper = entity_metadata_wrapper('node', $wrapper->field_hotel_facility_ref->value()->nid);

        foreach ($apply_form['formField']['ref'] as $key => $field) {
            if ( !empty($field['value']) ) {
                $ref_wrapper->{$key}->set($field['value']);
            }
        }
        $ref_wrapper->save();

        //update image
        if ( !empty($apply_form['formField']['field_hotel_view_image']['new_img']) ) {
            set_pics_field_pictures(
                $wrapper->field_hotel_view_image,
                $apply_form['formField']['field_hotel_view_image']['new_img'],
                'field_hotel_view_image'
            );
        }

        $wrapper->save();
        return true;
    }
    catch (EntityMetadataWrapperException $e) {
        watchdog_exception(__FUNCTION__, $e);
        return false;
    }

}

function get_node_list_count($data) {
    //watchdog(__FUNCTION__, 'data = <pre>@data</pre>', array('@data' => print_r($data, TRUE)));

    $element = array('data' => array('count' => 0));

    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
          ->entityCondition('bundle', $data['list_node_type'])
          ->propertyCondition('status', NODE_PUBLISHED)
          ->fieldCondition('field_myself_md5_openid', 'value', $data['xyz'], '=')
          ->count();

    $user_count = $query->execute();

    $page_number = 7;  //7 items in every page

    $pages = ceil($user_count / $page_number);


    $element['data']['table']['page_number'] = $page_number;
    $element['data']['table']['pages'] = $pages;

    if ($user_count > 0) {
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'node')
              ->entityCondition('bundle', $data['list_node_type'])
              ->propertyCondition('status', NODE_PUBLISHED)
              ->propertyOrderBy('created', 'DESC')
              ->fieldCondition('field_myself_md5_openid', 'value', $data['xyz'], '=')
              ->range(0, $page_number);

        $result = $query->execute();
        if (!empty($result)) {
            foreach ($result['node'] as $nid => $node) {

                $node_wrapper = entity_metadata_wrapper('node', $nid);

                $element['data']['table_list'][] = array(
                    'id' => $nid,
                    'hotel_name' => $node_wrapper->title->value(),
                    'hotel_address' => $node_wrapper->field_hotel_address->value(),
                    'hotel_phone' => $node_wrapper->field_hotel_phone->value(),
                );

                $element['data']['count'] += 1;
           }
        }

    } else {
        $element['data']['table']['pages'] = 1;
    }

    $element['data']['message'] = 'ok';

    return $element;

}

/**
 * get my page list number
 **/
function get_node_list_pager($data) {

    //watchdog(__FUNCTION__, 'data = <pre>@data</pre>', array('@data' => print_r($data, TRUE)));

    $element = array('data' => array('count' => 0));

    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
          ->entityCondition('bundle', $data['list_node_type'])
          ->propertyCondition('status', NODE_PUBLISHED)
          ->propertyOrderBy('created', 'DESC')
          ->fieldCondition('field_myself_md5_openid', 'value', $data['xyz'], '=')
          ->range($data['min_num'], $data['page_num']);

    $result = $query->execute();
    if (!empty($result)) {
        foreach ($result['node'] as $nid => $node) {

            $node_wrapper = entity_metadata_wrapper('node', $nid);

            $element['data']['table_list'][] = array(
                'id' => $nid,
                'hotel_name' => $node_wrapper->title->value(),
                'hotel_address' => $node_wrapper->field_hotel_address->value(),
                'hotel_phone' => $node_wrapper->field_hotel_phone->value(),
            );

            $element['data']['count'] += 1;

        }
    }

    $element['data']['message'] = 'ok';

    return $element;

}

function get_leaf_tpl($data) {
    $element = array('data' => array('count' => 0));

    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    $wrapper = entity_metadata_wrapper('node', $data['nid']);
   
    if ( !empty($wrapper->field_promotion_leaf_tpl->value()) ) {
        $element['data']['leaf_tpl'] = file_create_url($wrapper->field_promotion_leaf_tpl->value()['uri']);

        if ( !empty($leaf_node = $wrapper->field_hotel_leaf_node_ref->value()) ) {

            $leaf_wrapper = entity_metadata_wrapper('node', $leaf_node->nid);
            $element['data']['leaflet_file'] = file_create_url($leaf_wrapper->field_hotel_leaf_myqrcode->value()['uri']);
        }
        $element['data']['count'] += 1;
    }

    $element['data']['message'] = 'ok';

    return $element;
}

function create_my_leaf_page($data) {

    //watchdog(__FUNCTION__, 'data = <pre>@data</pre>', array('@data' => print_r($data, TRUE)));

    $element = array('data' => array('count' => 0));
    if (!($uid = get_user_id_from_xyz($data['xyz']))) {

        watchdog(__FUNCTION__, 'can not find uid by @xyz', array('@xyz' => $data['xyz']), $severity = WATCHDOG_ERROR);
        return $element;    //can't find any related user id, return 0
    }

    $wrapper = entity_metadata_wrapper('node', $data['nid']);
   
    if ( !empty($data['apply_form']['formField']['field_promotion_leaf_tpl']['new_img']) ) {
        //save picture
        set_pics_field_pictures(
            $wrapper->field_promotion_leaf_tpl,
            $data['apply_form']['formField']['field_promotion_leaf_tpl']['new_img'],
            'field_promotion_leaf_tpl'
        );

        $wrapper->save();
        $tpl_file = file_load($wrapper->field_promotion_leaf_tpl->value()['fid'])->uri;
    } else {
        $tpl_file = $wrapper->field_promotion_leaf_tpl->value()['uri'];
    }

    //generate my leaf picture
    //1. get wechat user head img and nick name
    $user_wrapper = entity_metadata_wrapper('user', $uid);
    $user_info = module_invoke('wechat_api', 'get_user_info', $user_wrapper->field_wechat_openid->value());
    if ( $user_info ) {
        //watchdog(__FUNCTION__, 'user info <pre>@data</pre>', array('@data' => print_r($user_info, true)));
        //2. create qrcode
        //3. create pormotion leaf
        $leaf_file = create_promotion_leaf_image($user_info, $tpl_file, $data['xyz'], $data['nid']);

        if ( empty($leaf_node = $wrapper->field_hotel_leaf_node_ref->value()) ) {
            if ( $leaf_node = create_user_leaflet_node($uid, $wrapper->title->value(), $leaf_file, $data['xyz'], $data['nid']) ) {

                $wrapper->field_hotel_leaf_node_ref->set($leaf_node['leaf_nid']);
                $wrapper->save();
                $element['data']['leaf_file'] = $leaf_node['leaf_url'];
                $element['data']['count'] += 1;
            }
        } else {
            $leaf_wrapper = entity_metadata_wrapper('node', $leaf_node->nid);
            $leaf_wrapper->field_hotel_leaf_myqrcode->set(array('fid' => $leaf_file->fid));
            $leaf_wrapper->save();

            $element['data']['leaf_file'] = file_create_url($leaf_file->uri);
            $element['data']['count'] += 1;
        }

    }

    $element['data']['message'] = 'ok';

    return $element;
}

function create_user_leaflet_node($uid, $title, $leaf_file, $md5_openid, $hotel_nid) {

    global $language;
    $values = array(
      'type' => 'hotel_promote_leaf_node',
      'uid' => $uid,
      'status' => 1,
      'language' => $language->language
    );
    $entity = entity_create('node', $values);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->title->set($title . "宣传页");

    $wrapper->field_hotel_leaf_myqrcode->set(array('fid' => $leaf_file->fid));
    $wrapper->field_myself_md5_openid->set($md5_openid);
    $wrapper->field_hotel_nid->set($hotel_nid);

    $wrapper->save();

    return array(
        'leaf_nid' => $wrapper->nid->value(),
        'leaf_url' => file_create_url($leaf_file->uri)
    );
}

function create_promotion_leaf_image($user_info, $tpl_file, $md5_openid, $nid) {

    global $base_url;

   //add qr code with dld log
    $library = libraries_load('wxpayapi');


    $user_info = module_invoke('wechat_api', 'get_user_info', $user_info->openid);
    $head_image_im = module_invoke('wechat_api', 'get_user_head_image', $user_info->headimgurl, $md5_openid);

    $leaflet = imagecreatefromjpeg($tpl_file);
    $leaflet_width = imagesx($leaflet);
    $leaflet_height = imagesy($leaflet);

    $leaf_url = t(
        variable_get('oauth2 redirect request'),
        array(
            '@APPID' => variable_get('wechat_AppID'),
            '@URL' => $base_url . "/" . WECHAT_MOBILE_URL . '/hotel-front-page/' . $nid . '/nid',
            '@SNSAPI' => "snsapi_base",
            '@STATE' => "200:" . $md5_openid,
        )
    );
    $qr_file = QRCODE_IMAGE_PATH . $md5_openid . "qr_scan.png";
    QRcode::png($leaf_url, $qr_file, QR_ECLEVEL_L, 3, 2);

    $QR = imagecreatefrompng($qr_file);
    $QR_width = imagesx($QR);
    $QR_height = imagesy($QR);
    $logo_width = imagesx($head_image_im);
    $logo_height = imagesy($head_image_im);
   // Scale logo to fit in the QR Code
    $logo_qr_width = $QR_width/4;
    $scale = $logo_width/$logo_qr_width;
    $logo_qr_height = $logo_height/$scale;

    imagecopyresampled($QR, $head_image_im, $QR_width/2.65, $QR_height/2.65,
        0, 0, $logo_qr_width, $logo_qr_height, $logo_width, $logo_height);

   //leaflet templagte, top, left x = 300, y = 826 QRcode with = 190, QRcode height = 190
    imagecopyresampled($leaflet, $QR, 289, 799,
        0, 0, 190, 190, $QR_width, $QR_height);

    //add mt rand create different image file name since wechat cache for same file name
    $leaflet_local = QRCODE_IMAGE_PATH . $md5_openid . "_" . $nid . "_" . mt_rand() . "_hotel_leaflet.jpg";
    imagejpeg($leaflet, $leaflet_local, 100);
    imagedestroy($leaflet);
    imagedestroy($head_image_im);
    imagedestroy($QR);


    $file_temp = file_get_contents($leaflet_local);
    //$file = file_save_data($file_temp, $leaflet_local, FILE_EXISTS_REPLACE);
    $leaf_file = file_save_data($file_temp, $leaflet_local, FILE_EXISTS_REPLACE);
    return $leaf_file;

}

function get_user_id_from_xyz($xyz) {
    //get user id from xyz. add user role condition for different user can add diffrent content type
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'user')
        ->propertyCondition('status', 1)  //1 active user
        ->fieldCondition('field_openid_md5', 'value', $xyz, '=')
        ->execute();

    if (empty($results)) {
        return null;    //can't find any related user id, return 0
    }

    $uid = key($results['user']);
    return $uid;
}

/**
  * End of hotel_rest_api.inc
  */


