<?php

function menu_wechat_api_manage_encrypt_form($form, &$form_state) {

  $form['encrypt_items'] = array(
    '#type' => 'fieldset', 
    '#title' => t('使用Encryp模块加/解密字符串'), 
    '#weight' => 0, 
    '#collapsible' => FALSE, 
    '#collapsed' => FALSE,
  );

  $form['encrypt_items']['input_text'] = array(
    '#type' => 'textarea',
    '#title' => t('输入字符串'),
    '#default_value' => '', 
    '#attributes' => array('class' => array('input-text')), //Do not use id when ajax
  );

  $form['encrypt_items']['output_text'] = array(
    '#type' => 'textarea',
    '#title' => t('输出字符串'),
    '#default_value' => '', 
    '#prefix' => '<div id="encrypt-result">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('input-text')), //Do not use id when ajax
  );

  $form['encrypt_items']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('执行'),
    '#ajax' => array(
      'callback' => 'ajax_submit_encrypt_result',
      'wrapper' => 'encrypt-result',
    ),
  );


  return $form;
}

function ajax_submit_encrypt_result($form, $form_state){
  $string = $form_state['values']['input_text'];
  $serialized_text = encrypt($string);
  $split_string = preg_split('/s:[0-9]*:/', $serialized_text);
  $encrypted_text = $split_string[2];
  preg_match('/"([^"]+)"/', $encrypted_text, $matches, PREG_OFFSET_CAPTURE);
  $element = $form['encrypt_items']['output_text'];
  $element['#value'] = $matches[1][0];
  return $element;
}

/**
 * End of wechat enctypt form 
 */
