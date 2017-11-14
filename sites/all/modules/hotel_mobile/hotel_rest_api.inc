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
    if ($data['type'] == 'hotel_update_form') {
        $send_data = hotel_update_node($data);
    } 


    drupal_add_http_header('Content-Type', 'application/json');
    $json_data = json_encode($send_data, JSON_UNESCAPED_UNICODE);

    echo $json_data;

}

function hotel_create_node($data) {
    
    watchdog(__FUNCTION__, "data = <pre>@data</pre>", array("@data" => print_r($data, TRUE)));

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

        if ($ref_nid) {
            $wrapper->field_hotel_facility_ref->set($ref_nid);
        }

        /**
         * upload field_hotel_view_image
         **/
        if (! set_pics_field_pictures($wrapper->field_hotel_view_image,
            $apply_form['formField']['field_hotel_view_image']['value'], 'field_hotel_view_image'))
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


