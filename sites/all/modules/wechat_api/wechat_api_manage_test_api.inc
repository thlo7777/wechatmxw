<?php

/**
 * wechat api manage appid form menu callback page
 */
function menu_wechat_api_manage_test_api_form($form, &$form_state) {
  $form['api test'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台消息接口测试'), 
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

  $form['api test']['url req'] = array(
    '#prefix' => t('<h3>获取access token:</h3>'),
    '#markup' => '<p>' . $req_url . '</p>', 
  );


  $form['api test']['checkbox-submit'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'wechat_api') . '/js/test_api_form.js',
  );

  $form['api test']['checkbox-submit']['checkbox'] = array(
    '#type' => 'checkbox', 
    '#title' => t('测试'),
    '#attributes' => array('class' => array('test-checkbox')),
  );

  //a perfect link button for form, and with ajax to implement a form update instead use submit
/*
  $form['api test']['checkbox-submit']['css button'] = array(
    '#type' => 'item',
    '#markup' => '<a href="javascript:void(0)" class="button">测试执行</a>',
    '#states' => array(
      // Mark this checkbox checked 
      'visible' => array(
        ':input[name="checkbox"]' => array('checked' => TRUE),
      ),
    ),
  );
*/
  $form['api test']['checkbox-submit']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('执行'),
    '#ajax' => array(
      'callback' => 'ajax_submit_test_result',
      'wrapper' => 'test-result',
      //'method' => 'replace',
    ),
    //'#executes_submit_callback' => FALSE,
    '#states' => array(
      // Mark this checkbox checked if the "more_info" textarea has something
      // in it, if it's 'filled'.
      'visible' => array(
        ':input[name="checkbox"]' => array('checked' => TRUE),
      ),
    ),
  );


  $ret_msg_values = isset($form_state['values']['return msg']) ? $form_state['values']['return msg'] : '';
  $form['api test']['return msg'] = array(
    '#type' => 'textarea',
    '#title' => t('输出结果'),
    '#default_value' => $ret_msg_values, 
    '#prefix' => '<div id="test-result">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('test-result-text')), //Do not use id when ajax
    '#states' => array(
      'visible' => array(
        ':input[name="checkbox"]' => array('checked' => TRUE),
      ),
    ),
  );

  return $form;
}

/**
 * ajax callback for wechat api test
 */
function ajax_submit_test_result($form, $form_state){
  $element = $form['api test']['return msg'];
  $element['#value'] = test_get_access_token();
  return $element;
}

function test_get_access_token(){
  module_load_include('inc', 'wechat_api', 'wechat_api_php5');
  
  $token_url = t(
    variable_get('get access token'),
    array(
      '@APPID' => variable_get('wechat_py_AppID'),
      '@APPSECRET' => variable_get('wechat_py_AppSecret'),
    )
  );

  $result = wechat_php_curl_https_get($token_url);
  if(!$result){
    return 'https get access token error';
  }

  $errcode = 'errcode';
  $pos = strpos($result, $errcode);  
  //Find errorcode, wechat return error code
  if($pos !== false){
    return 'error code: ' . $result;
  }

  $json_data = json_decode($result);
  variable_set('access_token', $json_data->access_token);
  return $json_data->access_token;
}

function menu_wechat_api_manage_test_api_page_validate($form, &$form_state) {
  $form_state['values']['return msg'] = '测试结果';

  /**
   * rebuild: Normally, after the entire form processing is completed and submit handlers have run,
   * a form is considered to be done and drupal_redirect_form() will redirect the user to a new page
   * using a GET request (so a browser refresh does not re-submit the form). However, if 'rebuild' has
   * been set to TRUE, then a new copy of the form is immediately built and sent to the browser, 
   * instead of a redirect. This is used for multi-step forms, such as wizards and confirmation forms. 
   * Normally, $form_state['rebuild'] is set by a submit handler, since it is usually logic within a submit
   * handler that determines whether a form is done or requires another step. However, a validation handler
   * may already set $form_state['rebuild'] to cause the form processing to bypass submit handlers and
   * rebuild the form instead, even if there are no validation errors.
   */
  $form_state['rebuild'] = TRUE;
  //dpm($form_state, 'validate form_state');
}
/**
 * Submit handler for menu_wechat_api_manage_test_api_page.
 */
function menu_wechat_api_manage_test_api_page_submit($form, &$form_state) {
  /**
   * The normal way to handle this is to implement a multi-step workflow. See drupal_rebuild_form()
   * for more information, but basically you set $form_state['rebuild'] = TRUE; in your submit handler,
   * and in the original form function add a check to see if the form's been submitted. If it has,
   * you have access to the submitted values and can manipulate the form as you see fit.
   */
  //$form_state['values']['return msg'] = '测试结果';
  //$form_state['rebuild'] = TRUE;
  //$form_state['redirect'] = false;
  //dpm($form_state, 'submit form_state');
}


/**
 * End of wechat api manage appid form
 */
