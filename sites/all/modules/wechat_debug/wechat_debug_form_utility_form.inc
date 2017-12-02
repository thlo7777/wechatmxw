<?php
/**
 * wechat_debug_form_utility_form.inc
 **/

function wechat_debug_form_utility_form($form, &$form_state) {

//  $voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
//  $term_array = array();
//  //module_load_include('core.inc', 'taxonomy_edge');
//  //$time = microtime(TRUE);
//  $terms = taxonomy_get_tree($voc->vid, 0, 1);
//  //$terms = taxonomy_get_tree($voc->vid);
//  //$terms = taxonomy_edge_get_tree_generic($voc->vid, 0, 1);
//  //$terms = taxonomy_edge_get_tree_optimized($voc->vid, 4585);
//  //dpm($terms);
//  //$terms = taxonomy_edge_get_tree_optimized($voc->vid);
//  //dpm('time ' . (microtime(TRUE) - $time));
//
//
//  if (!count($terms)) {
//    $form['insert_province'] = array (
//      '#type' => 'submit',
//      '#value' => t('插入省分类'),
//      '#submit' => array('insert_province_code'),
//      '#attributes' => array('class' => array('btn-success')),
//    );
//
//    $form['insert_city'] = array (
//      '#type' => 'submit',
//      '#value' => t('插入城市分类'),
//      '#submit' => array('insert_city_code'),
//      '#attributes' => array('class' => array('btn-success')),
//    );
//
//    $form['insert_zhixiashi'] = array (
//      '#type' => 'submit',
//      '#value' => t('插入直辖市分类'),
//      '#submit' => array('insert_zhixiashi_code'),
//      '#attributes' => array('class' => array('btn-success')),
//    );
//
//    $form['insert_county'] = array (
//      '#type' => 'submit',
//      '#value' => t('插入县分类'),
//      '#submit' => array('insert_county_code'),
//      '#attributes' => array('class' => array('btn-success')),
//    );
//  } else {
////    $form['insert_info'] = array (
////      '#type' => 'fieldset', 
////      '#title' => t('行政划分分类'), 
////      '#weight' => 5, 
////      '#collapsible' => TRUE, 
////      '#collapsed' => FALSE,
////    );
//
//    $form['insert_info'] = array (
//      '#markup' => '<h2>已经加入行政划分taxonomy</h2>',
//      '#prefix' => '<div class="jumbotron">',
//      '#suffix' => '</div>', 
//    );
//
//  }

//  $query = db_select('taxonomy_term_hierarchy', 't')
//    ->condition('t.parent', 4586)
//    ->fields('t');
//  $query->addTag('term_parent');
//  $result = $query->execute();
//  dpm($result);
//  foreach ($result as $value) {
//    dpm($value);
//  }
//  $result = $query->execute()->fetchAssoc();
//  $query = new EntityFieldQuery;
//  $query->entityCondition('entity_type', 'taxonomy_term')
//        ->entityCondition('bundle', 'china_administrative_divisions_taxonomy')
//        ->propertyCondition('tid', 4586)
//        ->addTag('term_parent')
//        ->propertyOrderBy('weight');
//
//  $result = $query->execute();
//  dpm($result);
//  foreach ($result['taxonomy_term'] as $tid) {
//    $wrapper = entity_metadata_wrapper('taxonomy_term', $tid->tid);
//    if ($tid->tid == '5047') {
//      $parent = $wrapper->parent->value();
//      $name = $wrapper->name->value();
//      //dpm($parent);
//    }
//    //$pcode = $wrapper->field_division_code->value();
//    $pid = $tid->tid;
//  }

//  $permissions = user_permission_get_modules();
//  dpm($permissions);

    //$login_modal = module_invoke('views', 'block_view', 'bootstrap-login-modal');
//    $login_modal = block_load('masquerade', 'masquerade');
//    $block_content = _block_render_blocks(array($login_modal));
//    $build = _block_get_renderable_array($block_content);
//    
//    $block_rendered = drupal_render($build);
    //$tmp =block_custom_block_get($login_modal->bid);

  //watchdog('dld_admin_form_utility_form', 'login_modal <pre>@openid</pre>', array('@openid' => print_r($login_modal, TRUE)));

//  $block = new stdClass();
//  $block->module = 'bootstrap_login_modal';
//  $block->delta = 'bootstrap-login-modal';
//  $block->region = '-1';
//  $block->cache = DRUPAL_CACHE_PER_ROLE;
//  // Load title.
//  global $theme_key;
//  drupal_theme_initialize();
//  $block1 = db_query("SELECT title FROM {block} WHERE module = :module AND delta = :delta AND theme = :theme",
//                    array(':module' => $block->module, ':delta' => $block->delta, ':theme' => $theme_key))->fetchObject();
//  if (is_object($block1) && isset($block1->title)) {
//    $block->title = $block1->title;
//  }
//  // Render the content and subject for the block.
//  $blocks = _block_render_blocks(array($block));
//  // Get an array of blocks suitable for drupal_render().
//  $array = _block_get_renderable_array($blocks);
//    //dpm($array);
//  watchdog('dld_admin_form_utility_form', 'login_modal <pre>@openid</pre>', array('@openid' => print_r($array, TRUE)));


//  $form['my_captcha_element'] = array(
//    '#type' => 'captcha',
//    '#captcha_type' => 'captcha/Math',
//  );


//  $form['actions'] = array('#type' => 'actions');
//  $form['actions']['save'] = array(
//    '#type' => 'submit',
//    '#value' => t('Save'),
//    '#weight' => 0.001,
//    '#submit' => array('save_test'),
//  );
//
//  $form['actions']['delete'] = array(
//    '#type' => 'submit',
//    '#value' => t('Delete'),
//    '#weight' => 0.001,
//
//    '#submit' => array('delete_test'),
//  );

  $form['title'] = array(

    '#type' => 'textfield', 

    '#title' => t('Subject'), 

    '#size' => 60, 

    '#maxlength' => 128, 

    //'#required' => TRUE,
    '#disabled' => TRUE,

  );


    $scheme = variable_get('file_default_scheme', 'public') . '://';

    $ticket = module_invoke('wechat_api', 'creat_qrscene', FALSE, '198964', 3600);
    if( !$ticket ) {
        drupal_set_message(t('创建临时二维码失败.'), 'error');
        return $form;
    }
    $qrCodeImage = module_invoke('wechat_api', 'get_qrscene_image', '198964', $ticket);
    $leaflet_image = array(
        '#theme' => 'image',
        '#path' => file_create_url($qrCodeImage),
        '#width' => '300',
    );
    $form['test_qrcode']['leaflet_image'] = array(
        '#markup' => '<h3>QRCODE 198964</h3>' .
                     '<a href="' . file_create_url($qrCodeImage) . '" target="_blank class="example-img">' .
                          drupal_render($leaflet_image) . '</a>'
    );

    $menu = module_invoke('wechat_api', 'search_custom_menu');
    dpm($menu);
  
//  $serialized_text = encrypt('1233702402');
//  dpm($serialized_text);
//
//  dpm(decrypt($serialized_text));
//  dpm(variable_get('access_token'));
//  dpm(variable_get('page_cache_maximum_age', 0));
//  
//  dpm(user_roles(TRUE));
//
//  $query = new EntityFieldQuery();
//
//  $results = $query->entityCondition('entity_type', 'user')
//    ->propertyCondition('status', 1)  //1 active user
//    ->fieldCondition('field_business_entity_form_id', 'value', '35', '=')
//    ->execute();
//
//  dpm($results);
//  dpm(count($results['user']));
//  dpm($results['user']);
//  $ldll = current($results['user']);
//  dpm($ldll->uid);

//  $user = user_load(372);
//  dpm($user);
//  $wrapper = entity_metadata_wrapper('user', $user);  //use foreach for entity_load
//
//  $openid = $wrapper->field_wechat_openid->value();
//  if (empty($openid) || $openid == '' || $openid == null) {
//    dpm('null');
//  } else {
//    dpm('ok');
//  }
//
//  dpm($GLOBALS);

  //dpm($_SESSION['ckeditor']['UserFilesAbsolutePath']);

  global $user, $base_url;

  
//  dpm($user);
//  $str = "/sites/default/files/u372/e54f10fc1b08ea0db497d4811e761e30.jpg";
//  $split = 'u372';
//  $rrr = preg_split("/u\d+/", $str);
//  dpm($rrr);
//    //$scheme = file_default_scheme() . '://';
//  $str1 = file_default_scheme() . '://' . "u372/e54f10fc1b08ea0db497d4811e761e30.jpg";
//  $path = drupal_realpath(file_default_scheme() . '://');
//  dpm($path);
//
//  $arrays = preg_split("/\//", $str);
//  dpm($base_url);
//  dpm($arrays);
//
//  $path = DRUPAL_ROOT;
//  dpm(DRUPAL_ROOT);
//
//
//  dpm(node_load('22'));
//
//  dpm(format_date(time()));

  //dpm(array_keys(theme_get_registry()));

//  $query = db_select('field_data_field_business_entity_form_id', 't')
//    ->condition('t.field_business_entity_form_id_value', '34', '>')
//    ->fields('t');
//  $result = $query->execute();
//
//  while ($row = $result->fetchAssoc()) {
//    dpm($row);
//  }
//  foreach ($result as $key) {
//    dpm((array)$key);
//  }

  
//  $profiles = profile2_load_by_user('372');
//  foreach ($profiles as $profile) {
//    dpm($profile);

//    profile2_delete($profile);
//    $profile->field_wechat_subscribe['und'][0]['value'] = '0';
//    profile2_save($profile);
//    $wrapper = entity_metadata_wrapper('profile2', $profile);
//    //dpm($wrapper->field_wechat_subscribe->value());
//    $wrapper->field_wechat_subscribe->set('1');
//    $wrapper->save();
//  }

//  $new_profile = profile2_create(array('type' => 'wechat_user_info', 'uid' => $user->uid));
//  dpm($new_profile);
//  dpm(user_roles(TRUE));

//    $query = new EntityFieldQuery();
//    $query->entityCondition('entity_type', 'node')
//          ->entityCondition('bundle', 'wechat_unified_order')
//          ->fieldCondition('field_order_out_trade_no', 'value', 'DLD-445-20161227102021-pncl', '=')
//          ->propertyOrderBy('created', 'DESC');
//    $result = $query->execute();
//    dpm($result);
//    $order_wrapper = entity_metadata_wrapper('node', 7);
//    $detail = $order_wrapper->field_wechat_api_ref->value();
//    foreach ($detail as $ref_obj) {
//        $node_w = entity_metadata_wrapper('node', $ref_obj->nid);
//        dpm($node_w->title->value());
//    }


    //dpm(variable_get('wechat_recv_msg', 'empty'));

//
//    $gid = 314;
//    $element = array();
//        if (!empty($result)) {
//
//            //service area info
//            $group_wrapper = entity_metadata_wrapper('node', $gid);
//            $element['sa_user_order'][$gid]['title'] = $group_wrapper->title->value();
//            //service group member info
//            $element['sa_user_order'][$gid]['member_info'] = get_group_member_info($gid);
//
//            //service area user order
//            foreach ($result['node'] as $key => $node) {
//                $node_wrapper = entity_metadata_wrapper('node', $node->nid);
//                $user_order = $node_wrapper->field_user_order_ref->value();
//                $order_wrapper = entity_metadata_wrapper('node', $user_order->nid);
//                $order_detail = json_decode($order_wrapper->field_order_detail->value(), true);
//                $element['sa_user_order'][$gid]['order'][$key]['order_detail'] = $order_detail;
//                $element['sa_user_order'][$gid]['order'][$key]['order_no'] = $order_wrapper->field_order_out_trade_no->value();
//                $element['sa_user_order'][$gid]['order'][$key]['internal_state'] = $order_wrapper->field_order_internal_state->value();
//                $element['sa_user_order'][$gid]['order'][$key]['internal_state_label'] = $order_wrapper->field_order_internal_state->label();
//                //next state
//                /**
//                 * 2(订单支付完成)----->5(订单受理)---->6(订单配送)---->7(订单送达)结束
//                 **/
//                if ($element['sa_user_order'][$gid]['order'][$key]['internal_state'] == '2') {
//                    $next_state = 5;
//                }
//                if ($element['sa_user_order'][$gid]['order'][$key]['internal_state'] == '5') {
//                    $next_state = 6;
//                }
//                if ($element['sa_user_order'][$gid]['order'][$key]['internal_state'] == '6') {
//                    $next_state = 7;
//                }
//                $element['sa_user_order'][$gid]['order'][$key]['internal_next_state'] = $next_state;
//            }
//        }
//    dpm($element);

    //dpm(key($results['user']));

//    $uid = key($results['user']);
//    $profiles = profile2_load_by_user($uid);

//    if (array_key_exists('user_address_test1', $profiles)) {
//        dpm(array_key_exists('user_address_test1', $profiles), 'array_key_exists');
//    }

//    foreach ($profiles as $profile) {
//
//      if ($profile->type == 'user_address_test1') {
//
//        $wrapper = entity_metadata_wrapper('profile2', $profile);
//        //unset one of mulitiple item use unset
//        //unset($wrapper->field_er_user_address_data[2]);
//        $items = $wrapper->field_er_user_address_data->value();   //subscribe
//        dpm($items);
//
////        foreach ($items as $node) {
////            $node_wrapper = entity_metadata_wrapper('node', $node);
////            dpm($node_wrapper->nid->value());
////            $cities = $node_wrapper->field_user_address_city->value();
////            dpm($cities, 'cities');
////        }
//
//        //$wrapper->field_er_user_address_data[] = '222';
//        //$wrapper->save();
//
//      }
//    }
//  module_load_include('inc', 'wechat_api', 'wechat_api_send_msg_tpl');
//  $result = wechat_api_get_user_info('oOTG9tzINV6zA_1vIBgWrqRRlTl0');
//
//  if($result == ''){
//    watchdog('dld mobile',
//             'dld_mobile_create_wechat_user_info_type_profile2: can not get user info from open id = @openid',
//             array('@openid' => 'oOTG9tzINV6zA_1vIBgWrqRRlTl0'),
//             $severity = WATCHDOG_ERROR);
//  }else{
//    dpm($result);
//    if (empty($result->unionid)) {
//      dpm('null');
//    }
//  }
  
//  $timezone = date_default_timezone();
//  dpm($timezone);
//
//  $tag_list = array (
//    '334',
//    '1',
//    '2',
//  );
//
//  dpm(implode(',', $tag_list));
//
//
//  dpm(session_id());
//  dpm($_SESSION);
//  dpm($_COOKIE);
//  dpm(session_name(), 'session name');
//  dpm(variable_get('site_frontpage', 'node'));
//
//  $str = 'qrscene_22229859';
//
//  if (preg_match('/^' . QRCODE_SUBSCRIBE_PREFIX . LOGIN_QRCODE_PREFIX . '/', $str)) {
//    $tempstr = preg_split('/^' . QRCODE_SUBSCRIBE_PREFIX . LOGIN_QRCODE_PREFIX . '/', $str);
//    dpm($tempstr);
//    dpm(end($tempstr));
//  }

//  $terms = taxonomy_get_tree($voc->vid, 0, 1);
//  dpm($terms);
//
//  $tt_array = array('data' => array('count' => 0));
//  dpm($tt_array);
//
//  $data = preg_split('/,/', '109.840140,34.732279');
//  dpm($data);
//
//  dpm(AMAP_WEB_KEY);
//  dpm(AMAP_WEB_REST_API);
//
//  $tt_array = array();
//  if (empty($tt_array)) {
//      dpm('array() is empty');
//  }
//
//    $terms = taxonomy_get_term_by_name('北京市', 'china_administrative_divisions_taxonomy');
//    dpm(key($terms));

    //variable_set('wechat_get_user_next_openid', 'null');
//    dpm(variable_get('wechat_get_user_next_openid', 'null'));
//    dpm(variable_get('wechat_get_user_cron_number'));
//    $test_ob = new stdClass();
//    $test_ob->adresse = null;
//    if (empty($test_ob->adresse)) {
//        dpm('ok');
//    }
//    test_create_supermarket_info_node($user->uid);

//    $query = new EntityFieldQuery();
//
//    $query->entityCondition('entity_type', 'node')
//          ->entityCondition('bundle', 'dld_service_area_group')
//          ->propertyCondition('status', NODE_PUBLISHED)
//          ->propertyOrderBy('created', 'DESC');
//
//    $result = $query->execute();
//    dpm($result);
//    foreach ($result['node'] as $nid => $node) {
//        dpm($nid);
//    }
//
//    $element = array('data' => array('count' => 0));
//    if (isset($result['node'])) {
//        $nodes = node_load_multiple(array_keys($result['node']));
//        foreach($nodes as $id => $node) {
//            $node_wrapper = entity_metadata_wrapper('node', $node);
//
//            //add flag for new 3dys
//            if (strtotime("+3 day", $node_wrapper->created->value()) > time()) {
//                $new = 1;
//            } else {
//                $new = 0;
//            }
//
//            //dpm($node_wrapper->field_supermarket_intro->value());
//
//            $large_pic0 = $node_wrapper->field_supermarket_pictures_large[0]->value()['uri'];
//            $small_pic0 = $node_wrapper->field_supermarket_pictures_small[0]->value()['uri'];
//            $element['data'][$element['data']['count']] = array (
//                'nid' => $node->nid,
//                'new' => $new,
//                'intro' => $node_wrapper->field_supermarket_intro->value()['value'],
//                'title' => $node_wrapper->title->value(),
//                'l_pic0' => file_create_url($large_pic0),
//                's_pic0' => file_create_url($small_pic0),
//                'lng' => $node_wrapper->field_supermarket_longitude->value(),
//                'lat' => $node_wrapper->field_supermarket_latitude->value(),
//            );
//            $element['data']['count'] += 1;
//        }
//
//        $element['message'] = 'ok';
//        dpm($element);
//    }
//
//
//    $voc = taxonomy_vocabulary_machine_name_load('supe_market_category');
//
//    if (!$voc) {
//        return NULL;
//    }
//
//    $element = array();
//    $terms = taxonomy_get_tree($voc->vid, 0, 1);
//    foreach($terms as $delta => $term) {
//        $element[$term->tid] = $term->name;
//    }
//    dpm($element);


//    //$dir_path = drupal_realpath($scheme . 'tttt');
//    $dir_path = drupal_realpath(QRCODE_IMAGE_PATH);
//    //mkdir($dir_path);
//    $field_instances = field_info_instances('node', 'hotel_main_page');
//    dpm($field_instances);
//    $field_instances = field_info_instances('node', 'hotel_facilities_page');
//    dpm($field_instances);
//
//    $all_fields_on_my_website = field_info_fields();
//    $allowed_values= list_allowed_values($all_fields_on_my_website["field_hotel_pay_service"]);
//    //$list = list_allowed_values('field_hotel_hot_water');
//    dpm($allowed_values);
//
//
//    class foo
//    {
//        function do_foo()
//        {
//            return "Doing foo."; 
//        }
//    }
//
//    $bar = new foo;
//    $str = "do_foo";
//    dpm($bar->{$str}());
    //dpm(module_invoke('wechat_asset', 'base_js_array'));
//    $block = module_invoke('wechat_api', 'test_value', 'fuck', 'me');
//    $save_path = drupal_realpath($scheme . SUPERMARKET_PATH);
//
//    //SUPERMARKET_INFO_PATH
//    module_load_include('inc', 'wechat_api', 'wechat_api_send_msg_tpl');
//    $file_name = wechat_api_get_temp_media($save_path, 'cXoL3kt7PcqCIgl2V_ADfA4FCK6CF0nPDBYVm6ATKOiFPWQlZDTPKojqh8ryNXRj');
//
//    $image = new Imagick($file_name);
//
//    $width = $image->getImageWidth();
//    $height = $image->getImageHeight();
//
//    $ratio  = $height / $width;
//
//    if ($height > $width) {
//        $th = 300;
//        $tw = $th / $ratio;
//    } else {
//        $tw = 300;
//        $th = $tw * $ratio;
//    }
//
//    $image->resizeImage($tw, $th, Imagick::FILTER_CATROM, 0.8, true);
//
//    if (!$image->writeImage($file_name)) {
//        watchdog('dld mobile',
//                 'set_field_supermarket_pictures_large: can not write file path @data',
//                 array('@data' => $file_name),
//                 $severity = WATCHDOG_ERROR);
//        $image->clear();
//        return false;
//    }
//    $image->clear();
//
//    $scheme = variable_get('file_default_scheme', 'public') . '://';
//    $split_str = explode("/", $file_name);
//    $fm = $scheme . SUPERMARKET_PATH . "/small_" . end($split_str);
//
//    $file_temp = file_get_contents($file_name);
//    $file = file_save_data($file_temp, $fm, FILE_EXISTS_REPLACE);
//    dpm($file);

//    $scheme = variable_get('file_default_scheme', 'public') . '://';
//    $video_file = file_create_url($scheme . '/梦乡网后台管理_商品分类02.mp4');
//    dpm($video_file);
//
//    drupal_add_css( drupal_get_path('module', 'dld_mobile') . '/css/video-js.min.css');
//    drupal_add_js( drupal_get_path('module', 'dld_mobile') . '/js/video.min.js');
//
//    $form['video_field'] = array (
//        '#markup' => '
//                    <video id="example_video_1" class="video-js vjs-default-skin"
//                        controls preload="auto" width="300" height="264"
//                        poster="http://video-js.zencoder.com/oceans-clip.png" ' .
//                        'data-setup=' . '{"example_option":true}' . '>' .
//                        '<source src="' .$video_file . '" type="video/mp4" />
//                    </video>'
//                    ,
//    );
//    $element = array('data' => array('count' => 0));
//
//    $voc = taxonomy_vocabulary_machine_name_load('supe_market_category');
//    $terms = taxonomy_get_tree($voc->vid, 7921, 1);
//    foreach($terms as $delta => $term) {
//        $element['data'][$term->tid][0] = $term->name;
//        $element['data']['count'] ++;
//        $sub_terms = taxonomy_get_tree($voc->vid, $term->tid, 1);
//        foreach ($sub_terms as $sub_term) {
//            $element['data'][$term->tid][$sub_term->tid] = $sub_term->name;
//        }
//    }
//    dpm($element);
//
//
//    $dld_tt = array (
//        '11' => 'hello',
//        '22' => 'dkdkdk'
//    );
//
//    if (empty($dld_tt['lxw'])) {
//        dpm('nothing');
//    } else {
//        dpm('11');
//    }


//    /**
//     * create votes array
//     **/
//    $votes = array(
//        'entity_type' => 'node',
//        'entity_id' => 178,
//        'tag' => 'vote',
//        'uid' => 1,
//        'value' => 20 
//    );
//
//    votingapi_set_votes($votes);
//
//    /**
//     * set five star field 
//     **/
//    $node = node_load(178);
//    $node_wrapper = entity_metadata_wrapper('node', 178);
//    $star = $node_wrapper->field_fivestar_test->value();
//    dpm($star);
//    $node_wrapper->field_fivestar_test->set(array('user_rating' => 20));
//    $node_wrapper->save();
//
//
//    /**
//     * get votes result
//     **/
//    $criteria = array(
//        'entity_type' => 'node',
//        'entity_id' => 178,
//        'value_type' => 'percent',
//        'tag' => 'vote',
//    );
//    $results = votingapi_select_results($criteria);
//    dpm($results);
//
//    /**
//     * get votes id from vote api
//     **/
//    $criteria = array(
//      'entity_type' => 'node',
//      'entity_id' => 178,
//      'uid' => 1,
//    );
//     
//    $results = votingapi_select_votes($criteria);
//    dpm($results);
//    votingapi_delete_votes($results);
//    votingapi_recalculate_results('node', 178);

//    $voc = taxonomy_vocabulary_machine_name_load('restaurant_category');
//    $node_wrapper = entity_metadata_wrapper('node', 168);
//    $rest_term = $node_wrapper->field_restaurant_category->value();
//    $first_cat = taxonomy_get_tree($voc->vid, $rest_term->tid, 1);
//    $nids = taxonomy_select_nodes($rest_term->tid, FALSE);
//
//    dpm($first_cat);
//    
//    $page['products'] = array();
//    foreach ($nids as $nid) {
//        $node_wrapper = entity_metadata_wrapper('node', $nid);
//
//        if ($node_wrapper->status->value() == NODE_NOT_PUBLISHED) {
//            //product turn off
//            continue;
//        }
//
//        if ($node_wrapper->type->value() == 'rest_product_info_node') {
//
//            $node_cat = $node_wrapper->field_rest_product_category->value();
//            //dpm($node_cat);
//            //dpm($node_wrapper->title->value());
//            $page['products'][$node_cat[1]->tid]['name'] = $node_cat[1]->name;
//            $page['products'][$node_cat[1]->tid]['nodes'][$nid] = array(
//                'title' => $node_wrapper->title->value(),
//                'pic' => file_create_url($node_wrapper->field_product_pictures_small->value()[1]['uri']),
//                'price' => $node_wrapper->field_rest_product_price->value(),
//                'vol' => $node_wrapper->field_product_total_volume->value(),
//            );
//        }
//    }
//    dpm($page['products']);
//    $sm_term = $node_wrapper->field_supermarket_category->value();
//    $first_cat = taxonomy_get_tree($voc->vid, $sm_term->tid, 1);
//    $nids = taxonomy_select_nodes($sm_term->tid, FALSE);
//
//    $page['products'] = array();
//
//    foreach ($nids as $nid) {
//        $node_wrapper = entity_metadata_wrapper('node', $nid);
//        if ($node_wrapper->type->value() == 'sm_product_info_node') {
//
//            $node_cat = $node_wrapper->field_sm_product_category->value();
//            if (compare_supermarket_category($node_cat[1]->tid, $first_cat)) {
//                $page['products'][$node_cat[1]->tid]['name'] = $node_cat[1]->name;
//                $second_cat = taxonomy_get_tree($voc->vid, $node_cat[1]->tid, 1);
//                if (compare_supermarket_category($node_cat[2]->tid, $second_cat)) {
//                    $page['products'][$node_cat[1]->tid]['child'][$node_cat[2]->tid]['name'] = $node_cat[2]->name;
//                    $page['products'][$node_cat[1]->tid]['child'][$node_cat[2]->tid]['nodes'][$nid] = array(
//                        'title' => $node_wrapper->title->value(),
//                        'pic' => file_create_url($node_wrapper->field_product_pictures_small->value()[1]['uri']),
//                        'po' => $node_wrapper->field_product_original_price->value(),
//                        'pd' => $node_wrapper->field_product_discount_price->value(),
//                        'vol' => $node_wrapper->field_product_total_volume->value(),
//                    );
//
//                }
//            }
//        }
//    }
//    dpm($page['products']);
//
//    $query = new EntityFieldQuery();
//
//    $query->entityCondition('entity_type', 'node')
//          ->entityCondition('bundle', 'supermarket_info')
//          ->fieldCondition('field_supermarket_category', 'tid', 7916, '=')
//          ->propertyCondition('status', NODE_PUBLISHED)
//          ->propertyOrderBy('created', 'DESC');
//
//    $result = $query->execute();
//    dpm($result);
//    $node_wrapper = entity_metadata_wrapper('node', 200);
//    dpm($node_wrapper->field_product_pictures_big->value());
//    if ($node_wrapper->__isset('ddd_field_product_discount_price')) {
//        dpm('hhh');
//    } else {
//        dpm('6666');
//    }
//    $pd = $node_wrapper->ddd_field_product_discount_price->value();
//    dpm(gettype($pd));
//    dpm($pd);
//    if (empty($pd) || !isset($pd) || $pd == '') { 
//        dpm('null');
//    //if (isset($node_wrapper->field_product_discount_price)) {
//    } else {
//        dpm('ok');
//        dpm($node_wrapper->field_product_discount_price->value());
//    }


//    foreach($result['node'] as $nid => $node) {
//        $node_wrapper = entity_metadata_wrapper('node', $nid);
//        $term = $node_wrapper->field_restaurant_category->value();
//        dpm($term);
//    }
//
//    foreach ($result['node'] as $key => $node) {
//        $node_wrapper = entity_metadata_wrapper('node', $node->nid);
//        $state = $node_wrapper->field_order_internal_state->label();    //get value not key
//        dpm($node->nid);
//        dpm($state);
//
//        //unset($node_wrapper->field_sm_product_favorite[0]);
//        //$node_wrapper->field_sm_product_favorite[] = $f_node;
//        //$node_wrapper->save();
//    }


//    $libraries = libraries_load('wxpayapi');
//    dpm($libraries);
//    $input = new WxPayUnifiedOrder();
//    $jsapi = new WxPayJsApiPay();
//    dpm(variable_get('wechat_py_mchkey'));
//    $jsapi->SetMchKey(variable_get('wechat_py_mchkey'));
//    dpm('jsapi' . $jsapi->GetMchKey());


    //dpm("DLD" . date("YmdHis-") . WxPayApi::getNonceStr(14));
//    $options = array(
//        'file' => 'README.txt',
//        'pattern' => '@Version ([0-9a-zA-Z\.-]+)@',
//        'lines' => 3
//    );
//    dpm(libraries_get_version($libraries, $options));

//    $t_date = date("Y年m月d日 H:i:s");
//    dpm($t_date);
//
//    dpm(strtotime($t_date));
//    dpm(date("YmdHis", strtotime($t_date)));
//    $user = user_load(372); //user_load will change current user to uid parameter
//    dpm($user);

//    $wrapper = entity_metadata_wrapper('user', 461);
    //$gid = $wrapper->og_user_node->value();
//    if (empty($gid)) {
//        dpm('0');
//    } else {
//        dpm($gid);
//    }
//    $og_me = og_get_membership('node', 238, 'user', 461);
//    dpm($og_me);
//
//    //do not use user_load since it will change login session
//    $account = new stdClass();
//    $account->uid = 461;
//    $user_groups = og_get_groups_by_user();
//    dpm($user_groups);

//    $values = array (
//        'entity_type' => 'user',
//        'entity' => 372,
//        'state' => OG_STATE_ACTIVE,
//        'membership_type' => OG_MEMBERSHIP_TYPE_DEFAULT
//    );
//
//    $new_og = og_group('node', 238, $values);
//    dpm($new_og);
//    $og_roles = og_get_user_roles('node', 238);
//    $og_roles = og_get_user_roles('node', 238, 26, $include = TRUE, $check_active = TRUE);
//    dpm($og_roles);

//    $node_wrapper = entity_metadata_wrapper('node', 244);   //node: 商家入驻协议
//    $value = $node_wrapper->body->value();
//    $id = $node_wrapper->nid->value();
//    dpm($id);

//    $og_r  = og_get_user_roles_name();
//    dpm($og_r);
//     $og_roles = og_roles('node', 'service_group_admin', 238);
//     dpm($og_roles);
//
//     $node_og = node_load(238);
//     dpm($node_og);


//    global $language;
//    $values = array(
//        'type' => 'dld_service_area_group',
//        'uid' => 26,
//        'status' => 1,
//        'language' => $language->language
//    );
//
//    $entity = entity_create('node', $values);
//
//    // The entity is now created, but we have not yet simplified use of it.
//    // Now create an entity_metadata_wrapper around the new node entity
//    // to make getting and setting values easier
//    $wrapper = entity_metadata_wrapper('node', $entity);
//
//    $wrapper->title->set('服务组测试语言');
//
//    $wrapper->save();

//    $users = _get_group_admin_in_group(238);
//    dpm($users);

//    $form['my_field'] = array(
//      '#type' => 'link',
//      '#title' => t('Example link'),
//      '#href' => 'menu-dld-admin-url/service-center/100self',
//    );

//        $new_node = node_load(280);
//        $values = array (
//            'entity_type' => 'node',
//            'entity' => $new_node,
//            'state' => OG_STATE_PENDING
//        );
//        og_group('node', 270, $values);

//    og_ungroup('node', 238, 'user', 461);
//    og_ungroup('node', 238, 'node', 261);
    //og_membership_delete(261);
    //og_membership_delete(461);

//    $node_wrapper = entity_metadata_wrapper('node', 334);
//    $order_detail = json_decode($node_wrapper->field_order_detail->value(), true);
//    foreach ($order_detail['products'] as $nid => $item) {
//        //$text .= $item['title'] . "\n价格: ¥" . $item['price'] . ' 数量: ' . $item['vol'] . '件' . "\n";
//        $text = $item['title'] . ' ' . $item['vol'] . '件' . "\n";
//        $wrapper = entity_metadata_wrapper('node', $nid);
//        $vol = $wrapper->field_product_total_volume->value() + $item['vol'];
//        $wrapper->field_product_total_volume->set($vol);
//        $wrapper->save();
//        //set product vol to node
//    }
//
//
//    $node_wrapper = entity_metadata_wrapper('node', 705);
//    $image = $node_wrapper->field_restaurant_pictures_large->value();
//    dpm($image);
    //$node_wrapper->save();

//    $wrapper = entity_metadata_wrapper('node', 238);
//    dpm($wrapper->getPropertyInfo());
//    dpm($wrapper->group_group->value());
//
//    dpm(get_group_non_admin_member_in_group(238));
//
//    $og_state = get_group_user_state(238, 26);
//    dpm($og_state[0]->state);
//
//    dpm(check_self_uid_is_group_admin(238, 26));
//
//
//    $member_uid = get_group_non_admin_member_in_group(238);
//    dpm($member_uid);
//
//
//    dpm(check_self_in_groups(26));
//    dpm(check_self_uid_is_group_admin(270, 26));

    $wrapper = entity_metadata_wrapper('node', 88);
    dpm($wrapper->field_hotel_leaf_myqrcode->value()['uri']);



//    $values = array(
//        'type' => 'service_area_user_order',
//        'uid' => 1,
//        'status' => 1,
//    );
//
//        $entity = entity_create('node', $values);
//
//        // The entity is now created, but we have not yet simplified use of it.
//        // Now create an entity_metadata_wrapper around the new node entity
//        // to make getting and setting values easier
//        $wrapper = entity_metadata_wrapper('node', $entity);
//
//        $wrapper->title->set('订单-hello');
//        $wrapper->field_service_center_ref->set(314);
//        $wrapper->field_user_order_ref->set(355);
//
//        //find gid amdin then save
//        $admins = get_group_admin(314);
//        dpm($admins);
//
//        if (!empty($admins)) {
//            foreach($admins['admin'] as $uid => $state) {
//                $wrapper->field_service_center_operators[] = $uid . ')(' . get_openID_from_uid($uid);
//            }
//        }
//
//        $wrapper->save();
//    dpm($wrapper->field_service_express_fee_ref->value());
//
//    $express_fee = $wrapper->field_service_express_fee_ref->value();
//    if (empty($express_fee)) {
//        dpm('empty');
//    } else {
//        $ex_wrapper = entity_metadata_wrapper('node', $express_fee);
//
//        dpm($ex_wrapper->field_sa_express_distance->value());
//    }
//    dpm($wrapper->field_service_polygon_weight->value());
    //$wrapper->field_service_gps_bound[] = array('lat' => 44, 'lon' => -10);
//    $wrapper->save();
//
//
//    $og_roles = og_roles('node', 'dld_service_area_group', 268);
//    foreach ($og_roles as $key => $role) {
//        if ($role == OG_ADMINISTRATOR_ROLE) {
//            dpm('role name: ' . $role . ' rid: ' . $key);
//            $admin_rid = $key;
//            break;
//        }
//    }
//    og_role_grant('node', 238, 617, 9);

//    try {
//        $wrapper = entity_metadata_wrapper('node', 12345678);
//        dpm($wrapper->getPropertyInfo());
//        //dpm($wrapper->nid->value());
//
//        if (isset($wrapper->nid)) {
//            dpm('hhh');
//        } else {
//            dpm('ttt');
//        }
//
//    }
//    // Some properties like entity_metadata_book_get_properties() throw
//    // exceptions, so we catch them here and ignore the property (deny access).
//    catch (EntityMetadataWrapperException $e) {
//        watchdog_exception('test non exist node', $e);
//    }
//
//    
//    $my_url = $base_url . '/' . DLD_MOBILE_CLIENT_PATH . '/bs-my-page/my-group/%s/apply-group-member';
//    dpm(sprintf($my_url, 289));

//    $group = node_load(279);
//    if ($group->status == NODE_PUBLISHED) {
//        dpm('公开');
//    }
//
//    $d_roles = og_get_default_roles();
//    $og_roles = og_roles('node', 'dld_service_area_group', 279);
//    foreach ($d_roles as $d_role) {
//        foreach ($og_roles as $key => $og_role) {
//            if ($d_role == $og_role) {
//                unset($og_roles[$key]);
//            }
//        }
//    }
//    dpm($og_roles);
//    $uid_openid = "461::oOTG9tzINV6zA_1vIBgWrqRRlTl0";
//    //list($uid, $openID) = preg_split('\)\(', $uid_openid);
//    list($uid, $openID) = preg_split('/::/', $uid_openid);
//    dpm($uid);
//    dpm($openID);

      $wrapper = entity_metadata_wrapper('node', 86);
      dpm($wrapper->status->value());


    $id_matchs = array();
    $qr_code = "200:1233444444";
    //$qr_code = "200:";
    //$qr_code = "";
    $id_matchs = preg_split("/200:/", $qr_code);

    if ( !empty($id_matchs[1]) ) {
        dpm($id_matchs);
    }

    return $form;
}

function get_group_member_info($gid) {
    $element = array();

    $members = get_group_member_in_group($gid);

    foreach($members as $id => $member) {
        if ($member->state != 1) {
            continue;
        }
        $og_roles = og_get_user_roles('node', $gid, $member->etid);

        $cycle_end = true;
        foreach($og_roles as $role) {
            //admin is group manager or group member, should check service_area_admin_info node and 
            // service_area_group_member node at the same time

            if ($role == OG_ADMINISTRATOR_ROLE) {
                $element[$id]['admin']['etid'] = $member->etid;

                $query = new EntityFieldQuery();
                $query->entityCondition('entity_type', 'node')
                      ->entityCondition('bundle', 'service_area_admin_info')
                      ->propertyCondition('uid', $member->etid)
                      ->propertyCondition('status', NODE_PUBLISHED)
                      ->propertyOrderBy('created', 'DESC');
                $result = $query->execute();
                if (!empty($result)) {
                    foreach ($result['node'] as $key => $node) {
                        $node_wrapper = entity_metadata_wrapper('node', $node->nid);
                        $element[$id]['admin']['name'] = $node_wrapper->field_sa_admin_user_name->value();
                    }
                } else {
                    $query = new EntityFieldQuery();
                    $query->entityCondition('entity_type', 'node')
                          ->entityCondition('bundle', 'service_area_group_member')
                          ->propertyCondition('uid', $member->etid)
                          ->propertyCondition('status', NODE_PUBLISHED)
                          ->propertyOrderBy('created', 'DESC');
                    $result = $query->execute();
                    if (!empty($result)) {
                        foreach ($result['node'] as $key => $node) {
                            $node_wrapper = entity_metadata_wrapper('node', $node->nid);
                            $element[$id]['admin']['name'] = $node_wrapper->title->value();
                        }
                    }
                }

                $cycle_end = false;
                break;
            }
        }

        if ($cycle_end) {
            $element[$id]['member']['etid'] = $member->etid;

            $query = new EntityFieldQuery();
            $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'service_area_group_member')
                  ->propertyCondition('uid', $member->etid)
                  ->propertyCondition('status', NODE_PUBLISHED)
                  ->propertyOrderBy('created', 'DESC');
            $result = $query->execute();
            dpm($result);
            if (!empty($result)) {
                foreach ($result['node'] as $key => $node) {
                    $node_wrapper = entity_metadata_wrapper('node', $node->nid);
                    $element[$id]['member']['name'] = $node_wrapper->title->value();
                }
            }
        }
    }

    return $element;
}

/**
 * get all non admin membership from group id
 **/
function get_group_admin($gid) {

    $users = get_group_member_in_group($gid);

    $member = array();
    foreach($users as $user) {

        if (check_self_uid_is_group_admin($gid, $user->etid) && $user->etid != 1) {
            $member['admin'][$user->etid] = $user->state;
        }
    }
 
    return $member;
}
function get_openID_from_uid($uid) {
    //get user id from xyz. add user role condition for different user can add diffrent content type
    $user_wrapper = entity_metadata_wrapper('user', $uid);  //use foreach for entity_load
    $openid = $user_wrapper->field_wechat_openid->value();
    if (empty($openid)){ 
        return false;
    }
    return $openid;
}

function check_self_in_groups($uid) {

    $gid_arr = array();
    $query = db_select('og_membership', 't')
        ->condition('t.entity_type', 'user', '=')
        ->condition('t.etid', $uid, '=')
        ->fields('t', array('gid'));

    $result = $query->execute()->fetchAll();
    if (empty($result)) {
        dpm('null');
    } else {
        foreach ($result as $gid) {
            $gid_arr[$gid->gid] = $gid->gid;
        }
    }

    dpm($gid_arr);
}

function get_group_user_state($gid, $uid) {
  $query = db_select('og_membership', 't')
      ->condition('t.gid', $gid, '=')
      ->condition('t.entity_type', 'user', '=')
      ->condition('t.etid', $uid, '=')
      ->fields('t', array('state'));
  $result = $query->execute()->fetchAll();

  return $result;
}

/**
 * get all membership from group id
 **/
function get_group_non_admin_member_in_group($gid) {

    $users = get_group_member_in_group($gid);

    $member = array();
    foreach($users as $user) {

        if (!check_self_uid_is_group_admin($gid, $user->etid) && $user->etid != 1) {
            $member['member'][$user->etid] = $user->state;
        }
    }
 
    return $member;
}

function check_self_uid_is_group_admin($gid, $uid) {
    $og_roles = og_get_user_roles('node', $gid, $uid);
    foreach($og_roles as $role) {
        if ($role == OG_ADMINISTRATOR_ROLE) {
            return true;
        }
    }
    return false;
}
/**
 * Get all users of a group
 */
function get_group_member_in_group($gid) {

  $query = db_select('og_membership', 't')
      ->condition('t.gid', $gid, '=')
      ->condition('t.entity_type', 'user', '=')
      ->fields('t', array('etid', 'state'));
  $result = $query->execute()->fetchAll();
 
  return $result;
}

/**
 * Get all users of a group
 */
function _get_group_admin_in_group($gid) {
  $query = db_select('og_membership', 't')
      ->condition('t.gid', $gid, '=')
      ->condition('t.entity_type', 'user', '=')
      ->fields('t', array('etid', 'state'));
  $result = $query->execute()->fetchAll();
  //$result = $query->execute();

 
  return $result;
}

function compare_supermarket_category($tid, $first_cat) {
    foreach ($first_cat as $term) {
        if (isset($tid) && ($term->tid == $tid)) {
        //if ($term->tid == $tid) {
            return true;
        }
    }

    return false;
}

function test_create_supermarket_info_node($uid) {

    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'supermarket_info',
      'uid' => $uid,
      'status' => 1,
    );

    $sm_info = array(
        "type" => "upload_sm_info",
        "xyz" => "c5a0db9aa6a2d58b454c69f351d21b33",
        "super_market" => Array (
            "map_location" => Array (
                "longitude" => "116.3256",
                "latitude" => "40.05536",
                "province" => "北京市",
                "city" => "",
                "district" => "昌平区",
                "township" => "回龙观街道",
                "formattedAddress" => "北京市昌平区回龙观街道龙兴园南区",
                "housing_estate" => "龙兴园南区",
            )
        ),

        "sm_region" => "北京市昌平区回龙观街道",
        "sm_name" => "西三旗建材城",
        "media_ids" => Array (
            "0" => "MwyLYi4iTUtw_Rfqp0MN2LgjhMLiZqUW4lVHxgKxHA_8Tu1uaLpQPN_khPDsLfeo",
            "1" => "Mz3fiFxhJq2CEIUCjaoxRX3FFqhmx_ESS4d4ZZYt5Qcts3N6KMLKqSysBBpCbqa0",
        )
    );

    $map_location = $sm_info['super_market']['map_location'];

    try {
        $entity = entity_create('node', $values);

        // The entity is now created, but we have not yet simplified use of it.
        // Now create an entity_metadata_wrapper around the new node entity
        // to make getting and setting values easier
        $wrapper = entity_metadata_wrapper('node', $entity);

        $wrapper->title->set($sm_info['sm_name']);

        if (!empty($sm_info['sm_intro'])) {
            $wrapper->field_supermarket_intro->set(array('value' => $sm_info['sm_intro']));
        }

        $wrapper->field_supermarket_longitude->set($map_location['longitude']);
        $wrapper->field_supermarket_latitude->set($map_location['latitude']);

        //creat supermarket category
        if (!test_set_field_supermarket_category($wrapper->field_supermarket_category, $sm_info['sm_name'])) {
            throw new EntityMetadataWrapperException('create field_supermarket_category failed');
        }

        //creat region term
        if (!test_set_field_region_term($wrapper->field_region_term, $map_location)) {
            throw new EntityMetadataWrapperException('create field_region_term failed');
        }

        //download and format pictures
        $image_file = array();
        //create large image and small image
        foreach($sm_info['media_ids'] as $delta => $media_id) {
            test_download_media_id($image_file, $media_id);
        }

        if (!empty($image_file)) {
            if (!test_set_field_supermarket_pictures_large($wrapper->field_supermarket_pictures_large, $image_file)) {
                throw new EntityMetadataWrapperException('test_set_field_supermarket_pictures_large error');
            }
            if (!test_set_field_supermarket_pictures_small($wrapper->field_supermarket_pictures_small, $image_file)) {
                throw new EntityMetadataWrapperException('test_set_field_supermarket_pictures_small error');
            }
        } else {
            throw new EntityMetadataWrapperException('can not get image file from wechat server');
        }

        $wrapper->save();
    }
    // Some properties like entity_metadata_book_get_properties() throw
    // exceptions, so we catch them here and ignore the property (deny access).
    catch (EntityMetadataWrapperException $e) {
        watchdog_exception('create_supermarket_info_node', $e);
        return NULL;
    }

    return $wrapper->nid->value();
}

function test_set_field_region_term(&$field, $map_location) {

    //find level1 tid
    $first_level_tid = 0;
    $second_level_tid = 0;

    $terms = taxonomy_get_term_by_name($map_location['province'], 'china_administrative_divisions_taxonomy');
    if (!empty($terms)) {
        $field[] = $first_level_tid = key($terms);
    } else {
        watchdog('dld mobile',
                 'set_field_region_term: can not find term id by @city',
                 array('@city' => $map_location['province']),
                 $severity = WATCHDOG_ERROR);
        return false;
    }

    if ($map_location['city'] == '') {
        //for 直辖市 level2
        $term_id = test_find_sub_level_term_id($map_location['district'], $first_level_tid);
        if ($term_id) {
            $field[] = $second_level_tid = $term_id;
        } else {
            watchdog('dld mobile',
                     'set_field_region_term: can not find term id by @city',
                     array('@city' => $map_location['district']),
                     $severity = WATCHDOG_ERROR);
            return false;
        }
    } else {
        //normal level 2 term id
        $term_id = test_find_sub_level_term_id($map_location['city'], $first_level_tid);
        if ($term_id) {
            $field[] = $second_level_tid = $term_id;
        } else {
            watchdog('dld mobile',
                     'set_field_region_term: can not find term id by @city',
                     array('@city' => $map_location['city']),
                     $severity = WATCHDOG_ERROR);
            return false;
        }

        //find level3 tid
        $term_id = test_find_sub_level_term_id($map_location['district'], $second_level_tid);
        if ($term_id) {
            $field[] = $term_id;
        } else {
            watchdog('dld mobile',
                     'set_field_region_term: can not find term id by @city',
                     array('@city' => $map_location['district']),
                     $severity = WATCHDOG_ERROR);
            return false;
        }
    }

    return true;
}

function test_find_sub_level_term_id($name, $pid) {

    $voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');

    if (!$voc) {
        return NULL;
    }

    $terms = taxonomy_get_tree($voc->vid, $pid, 1);
    foreach($terms as $delta => $term) {
        if ($term->name == $name) {
            return $term->tid;
        }
    }
    return NULL;
}

function test_set_field_supermarket_category(&$field, $sm_name) {
    $voc = taxonomy_vocabulary_machine_name_load('supe_market_category');

    if (!$voc) {
        return false;
    }

    //create new term
    $term = new stdClass();
    $term->name = $sm_name;
    $term->vid = $voc->vid;
    $term->parent = 0;  //no parent
    taxonomy_term_save($term);

    $field->set($term->tid);
    return true;
}


function test_download_media_id(&$image_file, $media_id) {

    $scheme = variable_get('file_default_scheme', 'public') . '://';
    $save_path = drupal_realpath($scheme . SUPERMARKET_PATH . "/" . SUPERMARKET_INFO_PATH);

    //SUPERMARKET_INFO_PATH
    module_load_include('inc', 'wechat_api', 'wechat_api_send_msg_tpl');
    $file_name = wechat_api_get_temp_media($save_path, $media_id);

    $image_file[] = $file_name;

    return null;

}

/*
 * large picture, height 640
 */
function test_set_field_supermarket_pictures_large(&$field, $img_files) {

    foreach ($img_files as $file_path) {

        $image = new Imagick($file_path);

        $width = $image->getImageWidth();
        $height = $image->getImageHeight();
        $ratio  = $height / $width;

        $tw = 640;
        $th = $tw * $ratio;
        $image->resizeImage($tw, $th, Imagick::FILTER_CATROM, 0.8, true);

        if (!$image->writeImage($file_path)) {
            watchdog('dld mobile',
                     'set_field_supermarket_pictures_large: can not write file path @data',
                     array('@data' => $file_path),
                     $severity = WATCHDOG_ERROR);
            $image->clear();
            return false;
        }
        $image->clear();

        $scheme = variable_get('file_default_scheme', 'public') . '://';
        $split_str = explode("/", $file_path);
        $fm = $scheme . SUPERMARKET_PATH . "/" . SUPERMARKET_INFO_PATH . "/large_" . end($split_str);

        $file_temp = file_get_contents($file_path);
        $file = file_save_data($file_temp, $fm, FILE_EXISTS_REPLACE);

        $field[] = array('fid' => $file->fid);

    }


    return true;
}

/*
 *  small pictures width 300
 */ 
function test_set_field_supermarket_pictures_small(&$field, $img_files) {

    foreach ($img_files as $key => $file_path) {
        $image = new Imagick($file_path);

        $width = $image->getImageWidth();
        $height = $image->getImageHeight();
        $ratio  = $height / $width;

        $tw = 300;
        $th = $tw * $ratio;
        $image->resizeImage($tw, $th, Imagick::FILTER_CATROM, 0.8, true);
        if (!$image->writeImage($file_path)) {
            watchdog('dld mobile',
                     'set_field_supermarket_pictures_small: can not write file path @data',
                     array('@data' => $file_path),
                     $severity = WATCHDOG_ERROR);
            $image->clear();
            return false;
        }
        $image->clear();


        $scheme = variable_get('file_default_scheme', 'public') . '://';
        $split_str = explode("/", $file_path);
        $fm = $scheme . SUPERMARKET_PATH . "/" . SUPERMARKET_INFO_PATH . "/small_" . end($split_str);

        $file_temp = file_get_contents($file_path);
        $file = file_save_data($file_temp, $fm, FILE_EXISTS_REPLACE);

        $field[] = array('fid' => $file->fid);

    }


    return true;
}


function save_test($form, &$form_state){
  drupal_set_message('save');
  dpm($form);
}
function delete_test($form, &$form_state){
  drupal_set_message('delete');
}

function insert_province_code($form, &$form_state) {
  $tax_voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
  $filename = drupal_get_path('module', 'dld_admin') . '/行政区划代码';
  $data = file_get_contents($filename);
  $data_array = preg_split('/\s+/', $data);

  insert_province_term($tax_voc, $data_array);
  drupal_set_message('插入结束');
}

function insert_city_code($form, &$form_state) {
  $tax_voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
  $filename = drupal_get_path('module', 'dld_admin') . '/行政区划代码';
  $data = file_get_contents($filename);
  $data_array = preg_split('/\s+/', $data);

  insert_city_to_term($tax_voc, $data_array);
  drupal_set_message('插入结束');
}

function insert_zhixiashi_code($form, &$form_state) {
  $tax_voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
  $filename = drupal_get_path('module', 'dld_admin') . '/行政区划代码';
  $data = file_get_contents($filename);
  $data_array = preg_split('/\s+/', $data);

  insert_zhixiashi_county_to_term($tax_voc, $data_array);
  drupal_set_message('插入结束');
}

function insert_county_code($form, &$form_state) {
  $tax_voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
  $filename = drupal_get_path('module', 'dld_admin') . '/行政区划代码';
  $data = file_get_contents($filename);
  $data_array = preg_split('/\s+/', $data);

  drupal_set_time_limit(7200);
  insert_county_to_term($tax_voc, $data_array);
  drupal_set_time_limit(300);
  drupal_set_message('插入结束');
}


function insert_code() {
  $tax_voc = taxonomy_vocabulary_machine_name_load('china_administrative_divisions_taxonomy');
  $filename = drupal_get_path('module', 'wechat_py_shopping') . '/行政区划代码';
  $data = file_get_contents($filename);
  $data_array = preg_split('/\s+/', $data);

  insert_province_term($tax_voc, $data_array);
  //insert_city_to_term($tax_voc, $data_array);
  //insert_zhixiashi_county_to_term($tax_voc, $data_array);

  //drupal_set_time_limit(7200);
  //insert_county_to_term($tax_voc, $data_array);
  //drupal_set_time_limit(300);
}

function insert_province_term($tax_voc, $data_array) {

  $plevel_weight = 0;
  foreach ($data_array as $id => $code) {
    if(preg_match('/^[0-9]+$/', $code)) { //find 0-9 number it's area code
      if(preg_match('/0000$/', $code)) {
        $plevel1_code = $code;
        $plevel1_name = $data_array[$id + 1];

        $term = new stdClass();
        $term->name = $plevel1_name;
        $term->vid = $tax_voc->vid;
        $term->parent = 0;
        $term->weight = $plevel_weight;
        $plevel_weight++;
        $ret_code = taxonomy_term_save($term);

        if ($ret_code == SAVED_NEW) {

          $plevel1_tid = $term->tid;
          $pleve1_wrapper = entity_metadata_wrapper('taxonomy_term', $term);
          $pleve1_wrapper->field_division_code->set($plevel1_code);

          if(preg_match('/^北京市$/', $plevel1_name) || 
             preg_match('/^天津市$/', $plevel1_name) || 
             preg_match('/^上海市$/', $plevel1_name) ||
             preg_match('/^重庆市$/', $plevel1_name)) {
            $pleve1_wrapper->field_division_category->set('0'); //直辖市
          } else {
            $pleve1_wrapper->field_division_category->set('1'); //省
          }

          $pleve1_wrapper->save();
        }
      }
    }
  }
}

function insert_city_to_term($tax_voc, $code_array) {
  //$terms = taxonomy_get_tree($tax_voc->vid, 0);

  $loop_count = 0;
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'taxonomy_term')
        //->entityCondition('bundle', 'china_administrative_divisions_taxonomy')
        ->propertyCondition('vid', $tax_voc->vid)
        ->propertyOrderBy('weight');

  $result = $query->execute();
  foreach ($result['taxonomy_term'] as $tid) {
    $wrapper = entity_metadata_wrapper('taxonomy_term', $tid->tid);
    $pcode = $wrapper->field_division_code->value();
    $pid = $tid->tid;

    $h_code = substr($pcode, 0, 2);
    foreach ($code_array as $id => $code) {
      if(preg_match('/^[0-9]+$/', $code)) { //find 0-9 number it's area code
        $row_code = $code;
        $row_name = $code_array[$id + 1];

        if (preg_match('/^' . $h_code . '..00$/', $row_code)) {
          if (!preg_match('/0000$/', $row_code)) {
            if (preg_match('/^市辖区$/', $row_name) ||
                preg_match('/^县$/', $row_name)) {
              continue;
            } else {
              $term = new stdClass();
              $term->name = $row_name;
              $term->vid = $tax_voc->vid;
              $term->parent = $pid;
              $term->weight = $loop_count;
              $loop_count ++;
              $ret_code = taxonomy_term_save($term);
              if ($ret_code == SAVED_NEW) {
                $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term);
                $term_wrapper->field_division_code->set($row_code);
                if (preg_match('/0100$/', $row_code)) {
                  $term_wrapper->field_division_category->set('2'); //省会
                } else {
                  $term_wrapper->field_division_category->set('3'); //地级市
                }

                $term_wrapper->save();
              }
              unset($term);
              //dpm($row_code . ':' . $row_name);
            }

          }
        }
      }
    }


  }

 //dpm($loop_count);

}

function insert_zhixiashi_county_to_term($tax_voc, $code_array) {
  $loop_count = 0;
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'taxonomy_term')
        //->entityCondition('bundle', 'china_administrative_divisions_taxonomy')
        ->propertyCondition('vid', $tax_voc->vid)
        ->propertyOrderBy('weight');
  $result = $query->execute();
  foreach ($result['taxonomy_term'] as $tid) {
    $wrapper = entity_metadata_wrapper('taxonomy_term', $tid->tid);
    //dpm($wrapper->getPropertyInfo());
    //dpm($wrapper->parent->value());
    $count = count($wrapper->parent->value());
    if ($count == 0) {
      $name = $wrapper->name->value();
      if (($name == '北京市') ||
          ($name == '天津市') ||
          ($name == '上海市') ||
          ($name == '重庆市')) {
        $pcode = $wrapper->field_division_code->value();
        $pid = $tid->tid;
        $h_code = substr($pcode, 0, 2);

        foreach ($code_array as $id => $code) {
          if(preg_match('/^[0-9]+$/', $code)) { //find 0-9 number it's area code
            $row_code = $code;
            $row_name = $code_array[$id + 1];
            if (preg_match('/^' . $h_code . '....$/', $row_code)) {
              if (preg_match('/0000$/', $row_code) ||
                  preg_match('/0100$/', $row_code) ||
                  preg_match('/0200$/', $row_code)) {
                continue;
              } else {
                $term = new stdClass();
                $term->name = $row_name;
                $term->vid = $tax_voc->vid;
                $term->parent = $pid;
                $term->weight = $loop_count;
                $loop_count ++;
                $ret_code = taxonomy_term_save($term);
                if ($ret_code == SAVED_NEW) {
                  $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term);
                  $term_wrapper->field_division_code->set($row_code);
                  if (preg_match('/区$/', $row_name)) {
                    $term_wrapper->field_division_category->set('4'); //市区
                  } else {
                    $term_wrapper->field_division_category->set('3'); //地级市
                  }

                  $term_wrapper->save();
                }
                unset($term);
                //dpm($row_code . ':' . $row_name);
              }
            }
          }
        }

      }
    }
  }

 //dpm($loop_count);
}


function insert_county_to_term($tax_voc, $code_array) {
  $loop_count = 0;

  $terms = taxonomy_get_tree($tax_voc->vid, 0);
  foreach ($terms as $term) {
    if ($term->parents[0] == 0) {
      if (($term->name == '北京市') ||
          ($term->name == '天津市') ||
          ($term->name == '上海市') ||
          ($term->name == '重庆市')) {
        continue;
      } else {
        $child = taxonomy_get_children($term->tid, $tax_voc->vid);
        foreach ($child as $ch_term) {
          $wrapper = entity_metadata_wrapper('taxonomy_term', $ch_term->tid);
          $pcode = $wrapper->field_division_code->value();
          $pid = $ch_term->tid;

          $h_code = substr($pcode, 0, 4);

          foreach ($code_array as $id => $code) {
            if (preg_match('/^[0-9]+$/', $code)) { //find 0-9 number it's area code
              $row_code = $code;
              $row_name = $code_array[$id + 1];
              if (preg_match('/^' . $h_code . '..$/', $row_code)) {
                if (preg_match('/^' . $h_code . '00$/', $row_code)) {
                  continue;
                }

                if (preg_match('/^市辖区$/', $row_name)) {
                  continue;
                }

                $term = new stdClass();
                $term->name = $row_name;
                $term->vid = $tax_voc->vid;
                $term->parent = $pid;
                $term->weight = $loop_count;
                $loop_count ++;
                $ret_code = taxonomy_term_save($term);
                if ($ret_code == SAVED_NEW) {
                  $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term);
                  $term_wrapper->field_division_code->set($row_code);
                  if (preg_match('/区$/', $row_name)) {
                    $term_wrapper->field_division_category->set('4'); //市区
                  } elseif (preg_match('/市$/', $row_name)) {
                    $term_wrapper->field_division_category->set('5'); //县级市
                  } else {
                    $term_wrapper->field_division_category->set('6'); //县
                  }
                  $term_wrapper->save();
                }
                unset($term);
                //dpm($row_code . ':' . $row_name);
              }
            }
          }
        }
      }
    }
  }

 //dpm($loop_count);
}

/**
 * End of wechat_debug_form_utility_form.inc
 */
