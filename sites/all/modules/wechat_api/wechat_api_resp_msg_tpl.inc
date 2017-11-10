<?php

/**
 * Template to send text message to wechat user
 * send text message template:
  <xml>
  <ToUserName><![CDATA[toUser]]></ToUserName>
  <FromUserName><![CDATA[fromUser]]></FromUserName>
  <CreateTime>12345678</CreateTime>
  <MsgType><![CDATA[text]]></MsgType>
  <Content><![CDATA[你好]]></Content>
  </xml>
 */
function wechat_api_resp_text_message($postArray, $content){
  $result = '';

  $ToUserName = $postArray['FromUserName']; //user open id
  $FromUserName = $postArray['ToUserName'];//wechat server id
  $time = time();
  $MsgType = 'text';

  $xmlTpl = '<xml>
    <ToUserName><![CDATA[%s]]></ToUserName>
    <FromUserName><![CDATA[%s]]></FromUserName>
    <CreateTime>%s</CreateTime>
    <MsgType><![CDATA[%s]]></MsgType>
    <Content><![CDATA[%s]]></Content>
    </xml>';

  $result = sprintf($xmlTpl, $ToUserName, $FromUserName, $time, $MsgType, $content);
  return $result;
}

/**
 * Template to response pictures and text message to user
 *
 * <xml>
 * <ToUserName><![CDATA[toUser]]></ToUserName>
 * <FromUserName><![CDATA[fromUser]]></FromUserName>
 * <CreateTime>12345678</CreateTime>
 * <MsgType><![CDATA[news]]></MsgType>
 * <ArticleCount>2</ArticleCount>
 * <Articles>
 * <item>
 * <Title><![CDATA[title1]]></Title> 
 * <Description><![CDATA[description1]]></Description>
 * <PicUrl><![CDATA[picurl]]></PicUrl>
 * <Url><![CDATA[url]]></Url>
 * </item>
 * <item>
 * <Title><![CDATA[title]]></Title>
 * <Description><![CDATA[description]]></Description>
 * <PicUrl><![CDATA[picurl]]></PicUrl>
 * <Url><![CDATA[url]]></Url>
 * </item>
 * </Articles>
 * </xml> 
 */
function wechat_api_resp_img_text_message($postArray){
  $result = '';

  global $base_url;
  $xml_item = 
     '<item>
      <Title><![CDATA[%s]]></Title> 
      <Description><![CDATA[%s]]></Description>
      <PicUrl><![CDATA[%s]]></PicUrl>
      <Url><![CDATA[%s]]></Url>
      </item>';

  $query = db_select('wechat_py_daily_news_broadcast_table', 't')
    ->fields('t', array(
      'news_index',
      'broadcast',
      'created',
      'issue',))
    ->orderBy('t.created', 'DESC');
  $index = $query->execute();
  $news_index = 0;
  foreach($index as $key => $item){
    if($item->issue){
      $news_index = $item->news_index; 
      break;
    }
  }

  $query = db_select('wechat_py_daily_news_table', 't')
    ->fields('t', array('node_id'))
    ->condition('news_index', $news_index, '=')
    ->orderBy('t.node_id', 'ASC');
  $nids = $query->execute()->fetchAll();
  $items = '';
  foreach ($nids as $key => $node_id) {
    $node = entity_metadata_wrapper('node', node_load($node_id->node_id));

    $title = $node->title->value();
    $description = '';
    $img_field = $node->field_news_picture->value();
    $img_url = file_create_url($img_field['uri']);
    $page_url = $base_url . '/' . MENU_WECHAT_PY_MOBILE . '/news/' . $node_id->node_id . '/edit';
    //do not use redirect url, because it does not work for click title. But might be use for other. So keep it.
    //$wechat_redirect = 'http://mp.weixin.qq.com/mp/redirect?url=';
    //$items .= sprintf($xml_item, $title, $description, $img_url, $wechat_redirect . $page_url);
    $items .= sprintf($xml_item, $title, $description, $img_url, $page_url);
    if($key > 9) break; //only 10 news can be send to user
  }

  $xml_header = 
     '<ToUserName><![CDATA[%s]]></ToUserName>
     <FromUserName><![CDATA[%s]]></FromUserName>
     <CreateTime>%s</CreateTime>
     <MsgType><![CDATA[news]]></MsgType>
     <ArticleCount>%s</ArticleCount>
     <Articles>';

  $ToUserName = $postArray['FromUserName']; //user open id
  $FromUserName = $postArray['ToUserName'];//wechat server id
  $time = time();
  $header = sprintf($xml_header, $ToUserName, $FromUserName, $time, count($nids));

  $result = '<xml>' . $header . $items . '</Articles></xml>';
  
  //watchdog('wechat resp msg', '@print', array('@print' => $result));
  return $result;
}

/**
 * End of wechat api response message to wechat template interface
 */
