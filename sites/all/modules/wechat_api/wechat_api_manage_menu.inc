<?php

/**
 * wechat api manage menu form callback page
 */
function menu_wechat_api_manage_menu_form($form, &$form_state) {

  //temporary solution for menu form
  $query = db_select('wechat_api_custom_menu_text', 't')
    ->fields('t', array('menu_text'));

  $result = $query->execute();
  $row = $result->fetchAssoc();
  //$row = $result->fetchAll();
  //for notify submit to decide insert or update content
  if(count($row) == 0){
    $form_state['store']['records'] = 0;  //use insert database
  }else{
    $form_state['store']['records'] = 1; //use update database
    $form_state['store']['menu_text'] = $row['menu_text'];
  }
/*
        {
          "type":"view",
          "name":"淘工艺",
          "url":"http://wechat.dreamland360.com/wechat-ajax/tgy"
        },
*/

  $form['menu_items']['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台自定义菜单列表'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $form['menu_items']['fieldset']['menu_text'] = array(
    '#type' => 'textarea',
    //'#type' => 'text_format',
    '#title' => t('自定义菜单'), 
    '#default_value' => $row['menu_text'],
    '#cols' => 60,
    '#rows' => 15,
    '#resizable' => TRUE,
  );

  $form['menu_items']['fieldset']['submit'] = array('#type' => 'submit', '#value' => t('保存修改'));


  /**
   * receive wechat response message after send creat custom menu
   */
  $form['menu_response']['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信返回消息'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['menu_response']['fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交微信菜单'),
    '#ajax' => array(
      'callback' => 'ajax_submit_custom_menu_result',
      'wrapper' => 'menu-response',
      'effect' => 'fade',
    ),
  );

  $form['menu_response']['fieldset']['return_value']= array(
    '#type' => 'item',
    '#title' => t("微信返回消息:"),
    '#prefix' => '<div id="menu-response">',
    '#suffix' => '</div>',
  );

  $form['get_menu']['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('查询自定义菜单'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );


  $form['get_menu']['fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('查询自定义微信菜单'),
    '#ajax' => array(
      'callback' => 'ajax_submit_get_custom_menu_result',
      'wrapper' => 'get-menu',
      'effect' => 'fade',
    ),
  );

  $form['get_menu']['fieldset']['return_value']= array(
    '#type' => 'item',
    '#title' => t("微信返回自定义菜单:"),
    '#prefix' => '<div id="get-menu">',
    '#suffix' => '</div>',
  );

  $form['conditional menu search']['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('查询微信个性菜单'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

    $form['conditional menu search']['fieldset']['user id'] = array(
        '#type' => 'textfield', 
        '#title' => t('用户openID'), 
        '#size' => 60, 
        '#maxlength' => 128, 
    );

  $form['conditional menu search']['fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('查询微信个性菜单'),
    '#ajax' => array(
      'callback' => 'ajax_submit_try_match_menu_result',
      'wrapper' => 'conditional-menu',
      'effect' => 'fade',
    ),
  );
  $form['conditional menu']['fieldset']['return_value']= array(
    '#type' => 'item',
    '#title' => t("查询微信个性菜单返回:"),
    '#prefix' => '<div id="conditional-menu">',
    '#suffix' => '</div>',
  );

    $form['conditional menu delete']['fieldset'] = array(
        '#type' => 'fieldset', 
        '#title' => t('删除微信个性菜单'), 
        '#weight' => 0, 
        '#collapsible' => FALSE, 
        '#collapsed' => FALSE,
    );

    $form['conditional menu delete']['fieldset']['menu id'] = array(
        '#type' => 'textfield', 
        '#title' => t('个性菜单ID'), 
        '#size' => 60, 
        '#maxlength' => 128, 
    );

    $form['conditional menu delete']['fieldset']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('删除微信个性菜单'),
        '#ajax' => array(
          'callback' => 'ajax_submit_remove_conditional_menu_result',
          'wrapper' => 'conditional-menu-remove',
          'effect' => 'fade',
        ),
    );
    $form['conditional menu delete']['fieldset']['return_value']= array(
        '#type' => 'item',
        '#title' => t("删除微信个性菜单返回结果:"),
        '#prefix' => '<div id="conditional-menu-remove">',
        '#suffix' => '</div>',
    );

  /**
   * get custom menu from wechat server
   */
  return $form;
}

function get_custom_menu_from_wechat(){
  module_load_include('inc', 'wechat_api', 'wechat_api_php5');
  $req_url = t(
    variable_get('search menu'),
    array('@ACCESS_TOKEN' => variable_get('access_token'))
  );
  $result = wechat_php_curl_https_get($req_url);
  if (!$result)
  {
    watchdog('wechat get custom menu', 'get custom menu error in @line line:@filename',
      array(
      '@line' => __LINE__,
      '@filename' => __FILE__,
      ),
      $severity = WATCHDOG_ERROR);
    return '';
  }
  return $result;
}


function ajax_submit_get_custom_menu_result($form, $form_state){
  $value = get_custom_menu_from_wechat();
  $element = $form['get_menu']['fieldset']['return_value'];
  $element['#markup'] = $value;
  return $element;
}

function ajax_submit_remove_conditional_menu_result($form, $form_state) {
    if (empty($form_state['input']['menu_id']) || $form_state['input']['menu_id'] == '') {
        return ;
    }
    watchdog('form_state', '<pre>@data</pre>', array('@data' => print_r($form_state, TRUE)));

    module_load_include('inc', 'wechat_api', 'wechat_api_send_msg_tpl');
    $value = wechat_conditional_menu_remove($form_state['input']['menu_id']);
    $element = $form['conditional menu delete']['fieldset']['return_value'];
    $element['#markup'] = $value;
    return $element;
}

function ajax_submit_try_match_menu_result($form, $form_state) {

    //watchdog('user id', '<pre>@data</pre>', array('@data' => print_r($form_state['input']['user_id'], TRUE)));
    //watchdog('form_state', '<pre>@data</pre>', array('@data' => print_r($form_state['input'], TRUE)));
    if (empty($form_state['input']['user_id']) || $form_state['input']['user_id'] == '') {
        return ;
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_send_msg_tpl');
    $value = wechat_conditional_menu_trymatch($form_state['input']['user_id']);
    $element = $form['conditional menu search']['fieldset']['return_value'];
    $element['#markup'] = $value;
    return $element;
}


function send_custom_menu_to_wechat($menu){
  module_load_include('inc', 'wechat_api', 'wechat_api_php5');
  $req_url = t(
    variable_get('create menu'),
    array('@ACCESS_TOKEN' => variable_get('access_token'))
  );

  $result = wechat_php_curl_https_post($req_url, $menu);
  if (!$result)
  {
    watchdog('wechat create menu', 'creat menu error in @line line:@filename',
      array(
      '@line' => __LINE__,
      '@filename' => __FILE__,
      ),
      $severity = WATCHDOG_ERROR);

    return t(
      'creat menu error in @line line:@filename',
      array('@line' => __LINE__,'@filename' => __FILE__)
    );
  }

  $json_obj = json_decode($result);
  $errcode = $json_obj->errcode;
  $errmsg = $json_obj->errmsg;
  if ($errcode != 0)
  {
    watchdog('wechat create menu', 'creat menu return error @errcode:@errmsg',
      array(
      '@errcode' => $errcode,
      '@errmsg' => $errmsg,
      ),
      $severity = WATCHDOG_ERROR);

    return t(
      'creat menu return error @errcode:@errmsg',
      array('@errcode' => $errcode, '@errmsg' => $errmsg,)
    );
  }

  return t(
    'creat menu return value is @errcode: message is @errmsg',
    array('@errcode' => $errcode, '@errmsg' => $errmsg,)
  );
}

function ajax_submit_custom_menu_result($form, $form_state){

  $result = '';
  if($form_state['store']['records'] == 1){
    $menu = $form_state['store']['menu_text'];
    $result = send_custom_menu_to_wechat($menu);
  }
  //;
  $element = $form['menu_response']['fieldset']['return_value'];
  $element['#markup'] = $result;
  return $element;
}

function menu_wechat_api_manage_menu_form_submit($form, &$form_state) {
  $value = $form_state['values']['menu_text'];
  $form_state['store']['menu_text'] = $value;   //for ajax send to server
  if($form_state['store']['records'] == 0){
    $rows = array(
      array(
        'menu_text' => $value,
      ),
    );
    foreach ($rows as $row) {
      db_insert('wechat_api_custom_menu_text')->fields($row)->execute();
    }
    $form_state['store']['records'] = 1; //use update next
  }else{
    db_update('wechat_api_custom_menu_text')
    ->fields(array(
        'menu_text' => $value,
      ))
    //->condition('id', 1)  //if only one record, then don't need condition
    ->execute();
  }
}

// for new menu form
function backup1_menu_wechat_api_manage_menu_form($form, &$form_state) {
  // Identify that the elements in 'example_items' are a collection, to
  // prevent Form API from flattening the array when submitted.
  $form['menu_items']['#tree'] = TRUE;
  $form['menu_items']['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台自定义菜单列表'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $req_url = t(
    variable_get('get access token'),
    array(
      '@APPID' => variable_get('wechat_py_AppID'),
      '@APPSECRET' => variable_get('wechat_py_AppSecret'),
    )
  );

  $users = array(
    array('uid' => 1, 'first_name' => 'Indy', 'last_name' => 'Jones'),
    array('uid' => 2, 'first_name' => 'Darth', 'last_name' => 'Vader'),
    array('uid' => 3, 'first_name' => 'Super', 'last_name' => 'Man'),
  );

  $header = array(
    'first_name' => t('First Name'),
    'last_name' => t('Last Name'),
  );
  $options = array();
  foreach ($users as $user) {
    $options[$user['uid']] =array(
      'first_name' => $user['first_name'],
      'last_name' => $user['last_name'],
    );
  }

/*
  $operations = array();
  $operations['delete'] = array(
    'title' => t('删除'),
    'href' => 'node/edit',
    'query' => $destination,
  );
*/

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
    //'#multiple' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  //$form = array();
  $rows = array();
  $row_elements = array();
 
  // Put it into a fieldset for no reason.
  $form['data_table'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data table'),
  );
 
  // Collect your data.
  $data = array(
    'some-id-1' => array(
      'enable' => TRUE,
      'default' => TRUE,
      'weight' => 1,
      'name' => 'some text from config',
      'description' => 'some description text',
    ),
    'some-id-2' => array(
      'enable' => TRUE,
      'default' => FALSE,
      'weight' => 3,
      'name' => 'some more text from config',
      'description' => 'more description text',
    ),
    'some-id-3' => array(
      'enable' => FALSE,
      'default' => TRUE,
      'weight' => 2,
      'name' => 'and even more text from config',
      'description' => 'mooore description text',
    ),
  );
 
  // Sort the rows.
  uasort($data, '_better_tabledrag_example_form_weight_arraysort');
 
  // Build the rows.
  foreach ($data as $id => $entry) {
    // Build the table rows.
    $rows[$id] = array(
      'data' => array(
        // Cell for the cross drag&drop element.
        array('class' => array('entry-cross')),
        // Weight item for the tabledrag.
        array('data' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#title_display' => 'invisible',
          '#default_value' => $entry['weight'],
          '#parents' => array('data_table', $id, 'weight'),
          '#attributes' => array(
            'class' => array('entry-order-weight'),
          ),
        )),
        // Enabled checkbox.
        array('data' => array(
          '#type' => 'checkbox',
          '#title' => t('Enable'),
          '#title_display' => 'invisible',
          '#default_value' => $entry['enable'],
          '#parents' => array('data_table', $id, 'enabled'),
        )),
        // Default checkbox.
        array('data' => array(
          '#type' => 'checkbox',
          '#title' => t('Default'),
          '#title_display' => 'invisible',
          '#default_value' => $entry['default'],
          '#parents' => array('data_table', $id, 'default'),
        )),
        // Name textfield.
        array('data' => array(
          '#type' => 'textfield',
          '#size' => 10,
          '#title' => t('Name'),
          '#title_display' => 'invisible',
          '#default_value' => $entry['name'],
          '#parents' => array('data_table', $id, 'name'),
        )),
        // Entry description.
        check_plain($entry['description']),
        // Operations.
        array('data' => array(
          '#theme' => 'link',
          '#text' => t('Edit settings'),
          '#path' => 'tabledrag/' . $id . '/edit',
          '#options' => array('attributes' => array(), 'html' => FALSE),
        )),
        array('data' => array(
          '#theme' => 'link',
          '#text' => t('Delete entry'),
          '#path' => 'tabledrag/' . $id . '/delete',
          '#options' => array('attributes' => array(), 'html' => FALSE),
        )),
      ),
      'class' => array('draggable'),
    );
    // Build rows of the form elements in the table.
    $row_elements[$id] = array(
      'weight' => &$rows[$id]['data'][1]['data'],
      'enabled' => &$rows[$id]['data'][2]['data'],
      'default' => &$rows[$id]['data'][3]['data'],
      'name' => &$rows[$id]['data'][4]['data'],
    );
  }
 
  // Add the table to the form.
  $form['data_table']['table'] = array(
    '#theme' => 'table',
    // The row form elements need to be processed and build,
    // therefore pass them as element children.
    'elements' => $row_elements,
    '#header' => array(
      // We need two empty columns for the weigth field and the cross.
      array('data' => NULL, 'colspan' => 2),
      t('Enabled'),
      t('Default'),
      t('Name'),
      t('Description'),
      array('data' => t('Operations'), 'colspan' => 2),
    ),
    '#rows' => $rows,
    '#empty' => t('There are no entries available.'),
    '#attributes' => array('id' => 'entry-order'),
  );
  drupal_add_tabledrag('entry-order', 'order', 'sibling', 'entry-order-weight');

  return $form;
}

 
/**
 * Helper function for sorting entry weights.
 */
function _better_tabledrag_example_form_weight_arraysort($a, $b) {
  if (isset($a['weight']) && isset($b['weight'])) {
    return $a['weight'] < $b['weight'] ? -1 : 1;
  }
  return 0;
}

function backup2_menu_wechat_api_manage_menu_form($form, &$form_state)
{
    $form['description'] = array(
    '#markup' => '<div>This is an example of a AJAX Form, where we can use AJAX add more concept in drupal for a set of fields.</div>',
  );

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['names_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Family member details'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = 1;
  }
  
  for ($i = 0; $i < $form_state['num_names']; $i++) {
    
    //lets add all the fields we want in the set  
    /*
    We have the prefix and suffix added here, so that we can do some sort of styling with the form, like display the fields side by side. You may remove    it, but generally we need that when we have a set of fields, hence I thought to keep it here.
    */
    
    //Parent container
    $form['names_fieldset'][$i] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
    );
    
    //other form elements
    $form['names_fieldset'][$i]['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First Name'),
      '#prefix' => '<div class="col1">',
      '#suffix' => '</div>'
    );
    
    $form['names_fieldset'][$i]['lastname'] = array(
      '#type' => 'textfield',
      '#title' => t('Last Name'),
      '#prefix' => '<div class="col2">',
      '#suffix' => '</div>'   
    );  
    
    //-- Like wise we can add more
  }
  
  $form['names_fieldset']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('modulename_add_more_add_one'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'modulename_add_more_callback',
      'wrapper' => 'names-fieldset-wrapper',
    ),
  );
  
  if ($form_state['num_names'] > 1) {
    $form['names_fieldset']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('modulename_add_more_remove_one'),
      '#ajax' => array(
        'callback' => 'modulename_add_more_callback',
        'wrapper' => 'names-fieldset-wrapper',
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function modulename_add_more_callback($form, $form_state) {
  return $form['names_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function modulename_add_more_add_one($form, &$form_state) {
  $form_state['num_names']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function modulename_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_names'] > 1) {
    $form_state['num_names']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function modulename_add_more_submit($form, &$form_state) {
  
  //Process the data of form here and use it
  
  /*
  #Example usage
  $form_state['input']['name_fieldset'][0,1,2... index number]['fieldname'];
  
  you can always use print_r($form_state); to explore the submited data.
  */
}
/**
 * End of wechat api manage menu form
 */
