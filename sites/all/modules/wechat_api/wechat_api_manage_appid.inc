<?php

/**
 * wechat api manage appid form menu callback page
 */
function menu_wechat_api_manage_appid_form($form, &$form_state) {

/*
  //for test recv message callback array
  $recv_msg_cb = variable_get('wechat_recv_msg');
  dpm($recv_msg_cb);
  if(isset($recv_msg_cb['event']['CLICK'])){
    drupal_set_message(' has abc key');
  }else{
    drupal_set_message('no abc key');
  }
*/

  $form['appid table'] = array(
    '#type' => 'fieldset', 
    '#title' => t('微信平台应用ID, Token等'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $query = db_select('wechat_api_appid', 't')
    //->fields('t', array('Token', 'AppID', 'AppSecret', 'wechat_url', 'jsapi_ticket', 'mchid', 'mchkey', 'notify_url'))
    ->fields('t')
    ->orderBy('t.id', 'ASC');
  $result = $query->execute()->fetchAssoc();
  //dpm($result);

  //set variable
  variable_set('wechat_py_Token', decrypt($result['Token']));
  variable_set('wechat_py_AppID', decrypt($result['AppID']));
  variable_set('wechat_py_AppSecret', decrypt($result['AppSecret']));
  variable_set('wechat_py_jsapi_ticket', decrypt($result['jsapi_ticket']));
  variable_set('wechat_py_mchid', decrypt($result['mchid']));
  variable_set('wechat_py_mchkey', decrypt($result['mchkey']));
  variable_set('wechat_py_notify_url', decrypt($result['notify_url']));

  //use item list to show content, cause it has only one row
  $items = array();

  $items[] = 'Token: ' . decrypt($result['Token']);
  $items[] = 'App ID: ' . decrypt($result['AppID']);
  $items[] = 'App Secret: ' . decrypt($result['AppSecret']);
  $items[] = 'server url: ' . $result['wechat_url'];
  $items[] = 'jsapi ticket: ' . decrypt($result['jsapi_ticket']);
  $items[] = 'mchID: ' . decrypt($result['mchid']);
  $items[] = 'mch Key: ' . decrypt($result['mchkey']);
  $items[] = 'notify url: ' . decrypt($result['notify_url']);
  /*
  $items[] = array(
    'data' => t('world'),
    'class' => array('li-test'),
    'id' => 'hahah',
  );
  */
  $form['appid table']['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ul',
    '#attributes' => array('style' => array('list-style: none;')),
  );

  //use a:6:{s:4:"text";s:44:"+r2CvQxSjwGcIcA61WAMe7Lq8t4ciQPQFIKy6zO5MeY=";s:6:"method";s:14:"mcrypt_rij_256";s:12:"key_provider";s:18:"drupal_private_key";s:7:"options";a:0:{}s:15:"method_settings";s:0:"";s:17:"provider_settings";s:0:"";} to encrypt appid and token
  /*
  $rows = array();
  foreach($result as $row){
    variable_set('wechat_py_Token', decrypt($row->Token));
    variable_set('wechat_py_AppID', decrypt($row->AppID));
    variable_set('wechat_py_AppSecret', decrypt($row->AppSecret));
    variable_set('wechat_py_jsapi_ticket', decrypt($row->jsapi_ticket));
    variable_set('wechat_py_mchid', decrypt($row->mchid));
    variable_set('wechat_py_mchkey', decrypt($row->mchkey));

    $rows[] = array(
      'data' => array(
      decrypt($row->Token),
      decrypt($row->AppID),
      decrypt($row->AppSecret),
      $row->wechat_url,
      decrypt($row->jsapi_ticket),
      decrypt($row->mchid),
      decrypt($row->mchkey),
    ));
  }

  $header = array(t('Token'), t('AppID'), t('AppSecret'), t('服务器地址'), t('jsapi_ticket'), t('mchid'), t('key'));
  $form['appid table']['theme table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('wechat-appid-table'))
  );
  */
  
  
  return $form;
}

/**
 * End of wechat api manage appid form
 */
