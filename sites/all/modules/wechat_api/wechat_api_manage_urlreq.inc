<?php

/**
 * wechat api manage appid form menu callback page
 */
function menu_wechat_api_manage_urlreq_form($form, &$form_state) {
  // Identify that the elements in 'example_items' are a collection, to
  // prevent Form API from flattening the array when submitted.
  // Please use single word for $form[key], if use $form['url reqs'], then $form_state['values'] is empty.
  //$form['url_reqs']['#tree'] = TRUE;

  $form['urlreq_add_submit'] = array(
    '#type' => 'submit',
    '#value' => t('添加新纪录'),
    '#submit' => array('urlreq_add_new_row_submit'),
    // No need to validate when submitting this.
    '#limit_validation_errors' => array(),
    '#validate' => array(),
    '#weight' => 0, 
  );

  if (!empty($form_state['values']['urlreq_add'])) {
    $form['url_req_insert'] = array(
      '#type' => 'fieldset', 
      '#title' => t('添加新的消息接口'), 
      '#weight' => 1, 
      '#collapsible' => FALSE, 
      '#collapsed' => FALSE,
      '#weight' => 1, 
    );

    $form['url_req_insert']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('接口名称'), 
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 255,
      '#required' => TRUE,
    );

    $form['url_req_insert']['http_method'] = array(
      '#type' => 'textfield',
      '#title' => t('http请求方式'), 
      '#default_value' => '',
      '#size' => 50,
      '#maxlength' => 255,
    );

    $form['url_req_insert']['url_req'] = array(
      '#type' => 'textarea',
      '#title' => t('URL消息接口'), 
      '#default_value' => '',
      '#cols' => 60,
      '#rows' => 3,
      '#resizable' => TRUE,
      '#required' => TRUE,
    );

    $form['url_req_insert']['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('保存'),
      '#submit' => array('urlreq_save_new_record_submit'));
  }

  $query = db_select('wechat_api_url_req', 't')
    ->fields('t', array('id', 'name', 'http_method', 'url_req'))
    ->orderBy('t.id', 'ASC');

  $result = $query->execute();
/*
  foreach ($result as $item) {
    $form['url_reqs'][$item->id] = array(
      // We'll use a form element of type '#textfield' to display the item
      // description, which will allow the value to be changed via the form.
      // We limit the input to 255 characters, which is the limit we set on
      // the database field.
      'name' => array(
        '#type' => 'textfield',
        '#default_value' => check_plain($item->name),
        '#size' => 20,
        '#maxlength' => 255,
      ),
      'http_method' => array(
        '#type' => 'textfield',
        '#default_value' => check_plain($item->http_method),
        '#size' => 20,
        '#maxlength' => 255,
      ),
      'url_req' => array(
        '#type' => 'textarea',
        '#default_value' => urldecode($item->url_req),
        '#cols' => 60,
        '#rows' => 3,
        '#resizable' => TRUE,
      ),

    );
  }
*/

  $form['url_req_table'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台消息接口列表'), 
    '#weight' => 100, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $options = array();
  foreach($result as $item){
    $name = array(
      '#type' => 'textfield',
      //Do not use default_value for theme tableselect, it will bring 
      //: Undefined index: #value in theme_textarea() 3964 line, use #value to show original value from table
      //'#default_value' => check_plain($item->name),
      '#value' => check_plain($item->name),
      '#size' => 20,
      '#maxlength' => 255,
      '#name' => 'table_select_cell_value[name&' . $item->id . ']', 
    );
    $http_method = array(
      '#type' => 'textfield',
      //'#default_value' => check_plain($item->http_method),
      '#value' => check_plain($item->http_method),
      '#size' => 20,
      '#maxlength' => 255,
      '#name' => 'table_select_cell_value[http_method&' . $item->id . ']', 
    );
    $url_req = array(
      '#type' => 'textarea',
      //'#default_value' => urldecode($item->url_req),
      '#value' => urldecode($item->url_req),
      '#cols' => 60,
      '#rows' => 3,
      '#resizable' => TRUE,
      '#name' => 'table_select_cell_value[url_req&' . $item->id . ']', 
    );

    $options[$item->id] = array(
      'name' => array('data' => $name),
      'http_method' => array('data' => $http_method),
      'url_req' => array('data' => $url_req),
    );
  }
  $header = array('name' => t('接口名称'), 'http_method' => t('http请求方式'), 'url_req' => t('URL消息接口'));
  $form['url_req_table']['table_select'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('当前没有记录'),
  );

  $form['url_req_table']['del_save_submit'] = array(
    '#type' => 'submit', 
    '#value' => t('删除／保存'),
    '#submit' => array('urlreq_del_save_row_submit'),
  );

  //Save tableselect cell text value
  $form['table_select_cell_value'] = array('#type' => 'value',);


  return $form;
}


function urlreq_add_new_row_submit($form, &$form_state){
  $form_state['values']['urlreq_add'] = 'add';
  $form_state['rebuild'] = true;
}

function urlreq_del_save_row_submit($form, &$form_state){
  //dpm($form_state);

  $del_values = $form_state['values']['table_select'];
  foreach($del_values as $id => $item){
    if($item != 0){
      db_delete('wechat_api_url_req')
        ->condition('id', $item)
        ->execute();
    }
  }

  $save_values = $form_state['values']['table_select_cell_value'];

  $name_value = '';
  $name_id = 0;
  $http_method_value = '';
  $http_method_id = 0;
  $url_req_value = '';
  $url_req_id = 0;
  foreach($save_values as $key => $item){
    $split = preg_split("/&/", $key);
    if($split[0] == 'name'){
      $name_value = $item;
      $name_id = $split[1];
    }
    if($split[0] == 'http_method'){
      $http_method_value = $item;
      $http_method_id = $split[1];
    }
    if($split[0] == 'url_req'){
      $url_req_value = $item;
      $url_req_id = $split[1];
    }
    if($name_id == $http_method_id && $name_id== $url_req_id){
      db_update('wechat_api_url_req')
      ->fields(array(
          'name' => trim($name_value),
          'http_method' => trim($http_method_value),
          'url_req' => urlencode(trim($url_req_value)),   //use urlencode convert url to utf8 format
        ))
      ->condition('id', $name_id)
      ->execute();
      //set variable for url
      variable_set(trim($name_value), trim($url_req_value));
    }
  }
}

/**
 * Submit callback for the wechat_table_hotel_ranking_form form.
 *
 */
function urlreq_save_new_record_submit($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
/*
  foreach ($form_state['values']['url_reqs'] as $id => $item) {
    db_update('wechat_api_url_req')
    ->fields(array(
        'name' => trim($item['name']),
        'http_method' => trim($item['http_method']),
        'url_req' => urlencode(trim($item['url_req'])),   //use urlencode convert url to utf8 format
      ))
    ->condition('id', $id)
    ->execute();
    //set variable for url
    variable_set(trim($item['name']), trim($item['url_req']));
  }
*/

  if(!empty($form_state['values']['name']) || 
     !empty($form_state['values']['http_method']) || 
     !empty($form_state['values']['url_req'])){
    db_insert('wechat_api_url_req')
    ->fields(array(
          'name' => $form_state['values']['name'],
          'http_method' => $form_state['values']['http_method'],
          'url_req' => $form_state['values']['url_req'],
      ))
    ->execute(); 
    //set variable for url
    $item = $form_state['values'];
    variable_set(trim($item['name']), trim($item['url_req']));
  }

}

/**
 * Theme template for menu_wechat_api_manage_urlreq_form
 */
function theme_menu_wechat_api_manage_urlreq_form($variables){
  $form = $variables['form'];
  // Initialize the variable which will store our table rows.
  $rows = array();

  foreach (element_children($form['url_reqs']) as $id) {

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $rows[] = array(
      'data' => array(
        drupal_render($form['url_reqs'][$id]['name']),
        drupal_render($form['url_reqs'][$id]['http_method']),
        drupal_render($form['url_reqs'][$id]['url_req']),
      ),
    );
  }

  $header = array(t('接口名称'), t('http请求方式'), t('URL消息接口'));

  $form['url_req_table']['theme_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('wechat-urlreq-table')),
  );

/*
  $build['url_req_table'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台消息接口列表'), 
    '#weight' => 1, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );
  $build['url_req_table']['theme_table']= array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('wechat-urlreq-table')),
  );
  $output = drupal_render($build);
*/

  $output = drupal_render_children($form);

  return $output;
}


/**
 * End of wechat api manage appid form
 */
