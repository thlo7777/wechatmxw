<?php

/**
 * wechat api handle receive message form
 */
function menu_wechat_api_manage_recv_msg_form($form, &$form_state){
  $form['add_submit'] = array(
    '#type' => 'submit',
    '#value' => t('添加新纪录'),
    '#submit' => array('callback_fun_add_new_row_submit'),
    // No need to validate when submitting this.
    '#limit_validation_errors' => array(),
    '#validate' => array(),
    '#weight' => 0, 
  );

  //display receive call function array
  $recv_msg = variable_get('wechat_recv_msg');
  $form['msg_recv_callback_array'] = array(
    '#type' => 'item',
    '#title' => t('显示回调函数数组'),
    '#markup' => kpr($recv_msg, TRUE),
    '#weight' => 1, 
  );

  if (!empty($form_state['values']['callback_fun_add'])) {
    unset($form['msg_recv_callback_array']);
    $form['callback_fun_insert'] = array(
      '#type' => 'fieldset', 
      '#title' => t('添加新的回调函数'), 
      '#weight' => 1, 
      '#collapsible' => FALSE, 
      '#collapsed' => FALSE,
      '#weight' => 1, 
    );

    $form['callback_fun_insert']['MsgType'] = array(
      '#type' => 'textfield',
      '#title' => t('消息类型'), 
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 64,
      '#required' => TRUE,
    );

    $form['callback_fun_insert']['Event'] = array(
      '#type' => 'textfield',
      '#title' => t('事件类型'), 
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 64,
    );

    $form['callback_fun_insert']['CallBack'] = array(
      '#type' => 'textfield',
      '#title' => t('回调函数'), 
      '#default_value' => '',
      '#size' => 100,
      '#maxlength' => 255,
      '#required' => TRUE,
    );

    $form['callback_fun_insert']['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('保存'),
      '#submit' => array('callback_fun_save_new_record_submit'));
  }
  //search all records for theme table
  $query = db_select('wechat_api_recv_event_callback', 't')
    ->fields('t', array('id', 'MsgType', 'Event', 'CallBack'))
    ->orderBy('t.id', 'ASC');

  $result = $query->execute();

  $form['callback_fun_table'] = array(
    '#type' => 'fieldset', 
    '#title' => t('回调函数列表'), 
    '#weight' => 100, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $options = array();
  foreach($result as $item){
    $MsgType = array(
      '#type' => 'textfield',
      //Do not use default_value for theme tableselect, it will bring 
      //: Undefined index: #value in theme_textarea() 3964 line, use #value to show original value from table
      '#value' => check_plain($item->MsgType),
      '#size' => 20,
      '#maxlength' => 64,
      '#name' => 'table_select_cell_value[MsgType&' . $item->id . ']', 
    );
    $Event = array(
      '#type' => 'textfield',
      '#value' => check_plain($item->Event),
      '#size' => 20,
      '#maxlength' => 64,
      '#name' => 'table_select_cell_value[Event&' . $item->id . ']', 
    );
    $CallBack = array(
      '#type' => 'textfield',
      '#value' => check_plain($item->CallBack),
      '#cols' => 60,
      '#rows' => 3,
      '#name' => 'table_select_cell_value[CallBack&' . $item->id . ']', 
    );

    $options[$item->id] = array(
      'MsgType' => array('data' => $MsgType),
      'Event' => array('data' => $Event),
      'CallBack' => array('data' => $CallBack),
    );
  }
  $header = array('MsgType' => t('消息类型'), 'Event' => t('事件类型'), 'CallBack' => t('回调函数'));
  $form['callback_fun_table']['table_select'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('当前没有记录'),
  );

  $form['callback_fun_table']['del_save_submit'] = array(
    '#type' => 'submit', 
    '#value' => t('删除／保存'),
    '#submit' => array('callback_fun_del_save_row_submit'),
  );

  //Save tableselect cell text value
  $form['table_select_cell_value'] = array('#type' => 'value',);

  return $form;
}

//for add new receive message callback function
function callback_fun_add_new_row_submit($form, &$form_state){
  $form_state['values']['callback_fun_add'] = 'add';
  $form_state['rebuild'] = true;
}

//for save new receive message callback function
function callback_fun_save_new_record_submit($form, &$form_state) {

  //clear form and show kpr for receive callback array
  $form_state['values']['callback_fun_add'] = '';

  if(!empty($form_state['values']['MsgType']) || 
     !empty($form_state['values']['Event']) || 
     !empty($form_state['values']['CallBack'])){
    db_insert('wechat_api_recv_event_callback')
    ->fields(array(
          'MsgType' => $form_state['values']['MsgType'],
          'Event' => $form_state['values']['Event'],
          'CallBack' => $form_state['values']['CallBack'],
      ))
    ->execute(); 

    //set call back function array for receive message handle function
    if (db_table_exists('wechat_api_recv_event_callback')){

      $recv_msg = array();
      $query = db_select('wechat_api_recv_event_callback', 't')
        ->fields('t', array('MsgType', 'Event', 'CallBack'))
        ->orderBy('t.id', 'ASC');
      $result = $query->execute();
      foreach ($result as $row) {
        if($row->Event === ''){
          $recv_msg[$row->MsgType] = $row->CallBack;
        }else{
          $recv_msg[$row->MsgType][$row->Event] = $row->CallBack;
        }
      }
      variable_set('wechat_recv_msg', $recv_msg);
    }
  }

}

//callback array save or delete records
function callback_fun_del_save_row_submit($form, &$form_state){
  //dpm($form_state);

  //clear form and show kpr for receive callback array
  $form_state['values']['callback_fun_add'] = '';

  $del_values = $form_state['values']['table_select'];
  foreach($del_values as $id => $item){
    if($item != 0){
      db_delete('wechat_api_recv_event_callback')
        ->condition('id', $item)
        ->execute();
    }
  }

  $save_values = $form_state['values']['table_select_cell_value'];

  $MsgType_value = '';
  $MsgType_id = 0;
  $Event_value = '';
  $Event_id = 0;
  $CallBack_value = '';
  $CallBack_id = 0;
  foreach($save_values as $key => $item){
    $split = preg_split("/&/", $key);
    if($split[0] == 'MsgType'){
      $MsgType_value = $item;
      $MsgType_id = $split[1];
    }
    if($split[0] == 'Event'){
      $Event_value = $item;
      $Event_id = $split[1];
    }
    if($split[0] == 'CallBack'){
      $CallBack_value = $item;
      $CallBack_id = $split[1];
    }
    if($MsgType_id == $Event_id && $MsgType_id== $CallBack_id){
      db_update('wechat_api_recv_event_callback')
      ->fields(array(
          'MsgType' => trim($MsgType_value),
          'Event' => trim($Event_value),
          'CallBack' => trim($CallBack_value),   //use urlencode convert url to utf8 format
        ))
      ->condition('id', $MsgType_id)
      ->execute();

      //set variable call back function array for receive message handle function
      if (db_table_exists('wechat_api_recv_event_callback')){

        $recv_msg = array();
        $query = db_select('wechat_api_recv_event_callback', 't')
          ->fields('t', array('MsgType', 'Event', 'CallBack'))
          ->orderBy('t.id', 'ASC');
        $result = $query->execute();
        foreach ($result as $row) {
          if($row->Event === ''){
            $recv_msg[$row->MsgType] = $row->CallBack;
          }else{
            $recv_msg[$row->MsgType][$row->Event] = $row->CallBack;
          }
        }
        variable_set('wechat_recv_msg', $recv_msg);
      }
    }
  }
}

/**
 * End of wechat api manage receive a message and handled by callback function form
 */

