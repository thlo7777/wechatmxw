<?php
/**
 * Check signature for legal user
 * signature、timestamp、nonce、echostr
 * 1. 将token、timestamp、nonce三个参数进行字典序排序
 * 2. 将三个参数字符串拼接成一个字符串进行sha1加密
 * 3. 开发者获得加密后的字符串可与signature对比，标识该请求来源于微信
 */
function checkSignature() {
    $signature = filter_input(INPUT_GET, 'signature', FILTER_SANITIZE_SPECIAL_CHARS);
    $timestamp = filter_input(INPUT_GET, 'timestamp', FILTER_SANITIZE_SPECIAL_CHARS);
    $nonce = filter_input(INPUT_GET, 'nonce', FILTER_SANITIZE_SPECIAL_CHARS);
    //$signature = $_GET["signature"];
    //$timestamp = $_GET["timestamp"];
    //$nonce = $_GET["nonce"];
    $token = variable_get('wechat_Token');
    $tmpArr = array(
        $token,
        $timestamp,
        $nonce
    );
    sort($tmpArr, SORT_STRING);
    $tmpStr = implode($tmpArr);
    $tmpStr = sha1($tmpStr);

    if ($tmpStr == $signature) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Convert xml object to array
 */
function xml2array($xml) {
    $arr = array();

    foreach ($xml->children() as $r) {
        $t = array();
        if (count($r->children()) == 0) {
            $arr[$r->getName() ] = strval($r);
        }
        else {
            $arr[$r->getName() ][] = xml2array($r);
        }
    }
    return $arr;
}

/**
 * hook_menu page callback for wechat access url entrance
 */
function menu_wechat_access_url() {

    $echostr = filter_input(INPUT_GET, 'echostr', FILTER_SANITIZE_SPECIAL_CHARS);

    //Retrun echostr for wechat server certification
    if (isset($echostr) || !is_null($echostr) || !empty($echostr)) {
        echo $echostr;
        exit;
    }

    //check signatrue, and parse wechat message
    if (checkSignature()) {
        $xmldata = file_get_contents("php://input"); //get xml data
        $result = wechat_api_py_dispatch_message($xmldata); // handle received message
        echo $result;
    }
    else {
        watchdog('wechat signature', 'check signature error in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        echo '';
    }
    return;
}

function wechat_api_py_dispatch_message($xmldata) {
    $result = '';

    //libxml_disable_entity_loader is to prevent XML eXternal Entity Injection,
    //the best way is to check the validity of xml by yourself.
    //use libxml_disable_entity_loader is ok now.
    libxml_disable_entity_loader(true);
    $xmldata = simplexml_load_string($xmldata, 'SimpleXMLElement', LIBXML_NOCDATA);
    //watchdog('wechat recv message', 'postobj <pre>@print_r</pre>', array('@print_r' => print_r($xmldata, TRUE)));
    $postArray = xml2array($xmldata);
    //watchdog('wechat recv message', 'postArray <pre>@print_r</pre>', array('@print_r' => print_r($postArray, TRUE)));
    //$MsgType = (string)$xmldata->MsgType;
    $MsgType = $postArray['MsgType'];

    $recv_msg = variable_get('wechat_recv_msg');

    if ($MsgType == 'event') {
        //$event = (string)$xmldata->Event;
        $event = $postArray['Event'];
        $callback_func = $recv_msg[$MsgType][$event];
        //    watchdog('wechat recv message',
        //             'msg type @msgtype and event type @event, call @call_back',
        //             array('@msgtype' => $MsgType, '@event' => $event, '@call_back' => $callback_func));
        if (!isset($callback_func)) {
            watchdog('wechat recv message', 'can not find msg type @msgtype and event @event in recv msg callback array', array(
                '@msgtype' => $MsgType,
                '@event' => $event
            ));

            return $result;
        }
    }
    else {
        $callback_func = $recv_msg[$MsgType];
        //    watchdog('wechat recv message', 'msg type @msgtype, call @call_back', array('@msgtype' => $MsgType, '@call_back' => $callback_func));
        if (!isset($callback_func)) {
            watchdog('wechat recv message', 'can not find msg type @msgtype in recv msg callback array', array(
                '@msgtype' => $MsgType
            ));

            return $result;
        }
    }
    $cb = (string)$callback_func;
    //watchdog('wechat recv message', 'callback function @msgtype ', array('@msgtype' => $cb));
    //callback function invoked by message type and event
    $result = $callback_func($postArray);

    return $result;

}

/**
 * Callback function when receive subscribe event message from wechat end user. Refer to wechat_api.install
 <xml>
 <ToUserName><![CDATA[toUser]]></ToUserName>
 <FromUserName><![CDATA[FromUser]]></FromUserName>
 <CreateTime>123456789</CreateTime>
 <MsgType><![CDATA[event]]></MsgType>
 <Event><![CDATA[subscribe]]></Event>
 </xml>
 */
function wechat_recv_event_subscribe_msg_callback($postArray) {
    $resultStr = '';

    //only subscribe from official qrcode, create website user account
    //call wechat_asset module hook to add user wechat user
    $uid = module_invoke('wechat_asset', 'subscribe_user', $postArray);

    //    watchdog('wechat recv message', 'postArray <pre>@print_r</pre>', array('@print_r' => print_r($postArray, TRUE)));
    //
    //    send media-text message to user
    //    module_load_include('inc', 'dld_mobile', 'dld_mobile_send_welcome');
    //    $resultStr = dld_mobile_send_welcome_to_user($postArray);
    if (!empty($postArray['EventKey']) && $uid) { //扫码加关注 after user login from website
        
        /**
         * User subscrib by scan bar code, eventkey is qrscene_ plus scene number
         Array
         (
         [ToUserName] => gh_849bfca55031
         [FromUserName] => oOTG9t2L0J3Pw-z5e3E4dpvVnpMc
         [CreateTime] => 1428580638
         [MsgType] => event
         [Event] => subscribe
         [EventKey] => qrscene_111374
         [Ticket] => gQHO7zoAAAAAAAAAASxodHRwOi8vd2VpeGluLnFxLmNvbS9xL1FreUpsTTNsSXpwYXR2T3NrMkw5AAIElWcmVQMECAcAAA==
         )
         */
        $eventkey = $postArray['EventKey'];
        foreach (module_implements('scan_subscribe_event') as $module) {
            $function = $module . '_scan_subscribe_event';
            // will call all modules implementing hook_hook_name
            // and can pass each argument as reference determined
            // by the function declaration
            if ($function($eventkey, $postArray, $uid) == 'end') {
                //when one module want to stop hook_scan_subscribe_event, it just return end
                break;
            }
            else {
                continue;
            }
        }

    }

    //call wechat welcome message hook
    $resultStr = module_invoke('wechat_asset', 'wechat_welcome_news', $postArray);
    return $resultStr;
}

/**
 * Callback function when receive unsubscribe event message from wechat end user. Refer to wechat_api.install
 *
 * <xml>
 * <ToUserName><![CDATA[toUser]]></ToUserName>
 * <FromUserName><![CDATA[FromUser]]></FromUserName>
 * <CreateTime>123456789</CreateTime>
 * <MsgType><![CDATA[event]]></MsgType>
 * <Event><![CDATA[subscribe]]></Event>
 * </xml>
 */
function wechat_recv_event_unsubscribe_msg_callback($postArray) {

    $resultStr = '';
    module_invoke('wechat_asset', 'unsubscribe_user', $postArray);

    return $resultStr;
}

/**
 * Callback function when receive subscribe event message from wechat end user. Refer to wechat_api.install
 * message of received template:
 * <xml>
 *  <ToUserName><![CDATA[toUser]]></ToUserName>
 *  <FromUserName><![CDATA[fromUser]]></FromUserName>
 *  <CreateTime>1348831860</CreateTime>
 *  <MsgType><![CDATA[text]]></MsgType>
 *  <Content><![CDATA[this is a test]]></Content>
 *  <MsgId>1234567890123456</MsgId>
 * </xml>
 */
function wechat_recv_text_msg_callback($postArray) {
    $resultStr = '';
    $FromUserName = $postArray['FromUserName'];
    $text = $postArray['Content'];
    watchdog(__FUNCTION__, ': got a text message from user @user: @text', array('@user' => $FromUserName, '@text' => $text));
    return $resultStr;
}

/**
 * Callback function when scan bar code event message from registered user
 * [ToUserName] => gh_849bfca55031
 *   [FromUserName] => oOTG9t2L0J3Pw-z5e3E4dpvVnpMc
 *   [CreateTime] => 1428577830
 *   [MsgType] => event
 *   [Event] => SCAN
 *   [EventKey] => 1
 *   [Ticket] => gQHt7zoAAAAAAAAAASxodHRwOi8vd2VpeGluLnFxLmNvbS9xL1UwelZNdDNsY0RvSkVlSUZ6Mkw5AAIEOl0mVQMECAcAAA==
 */
function wechat_recv_event_scan_msg_callback($postArray) {
    $resultStr = '';
    /**
     * Registered user scan bar code, event key only has scene number
     Array
     (
     [ToUserName] => gh_849bfca55031
     [FromUserName] => oOTG9t2L0J3Pw-z5e3E4dpvVnpMc
     [CreateTime] => 1428581190
     [MsgType] => event
     [Event] => SCAN
     [EventKey] => 2
     [Ticket] => gQHO7zoAAAAAAAAAASxodHRwOi8vd2VpeGluLnFxLmNvbS9xL1FreUpsTTNsSXpwYXR2T3NrMkw5AAIElWcmVQMECAcAAA==
     )
     */

    if ($postArray['Event'] == 'SCAN') { //已关注后扫码事件
        $eventkey = $postArray['EventKey'];
        foreach (module_implements('scan_msg_event') as $module) {
            $function = $module . '_scan_msg_event';
            // will call all modules implementing hook_hook_name
            // and can pass each argument as reference determined
            // by the function declaration
            $function($eventkey, $postArray);
        }
    }

    return $resultStr;
}

/**
 * Callback function when user click 热门活动 menu
 * Array
 * (
 *   [ToUserName] => gh_849bfca55031
 *   [FromUserName] => oOTG9t2L0J3Pw-z5e3E4dpvVnpMc
 *   [CreateTime] => 1428935037
 *   [MsgType] => event
 *   [Event] => CLICK
 *   [EventKey] => menu14
 * )
 */
function wechat_recv_event_click_msg_callback($postArray) {
    $resultStr = '';
    //watchdog('wechat recv message', 'postArray <pre>@print_r</pre>', array('@print_r' => print_r($postArray, TRUE)));
    return $resultStr;
}

//record all even view message
function wechat_recv_event_view_msg_callback($postArray) {
    $resultStr = '';
    //watchdog('wechat recv message', 'postArray <pre>@print_r</pre>', array('@print_r' => print_r($postArray, TRUE)));
    return $resultStr;
}

/**
 * received template message response
 *
 * <xml>
 *  <ToUserName><![CDATA[gh_7f083739789a]]></ToUserName>
 *  <FromUserName><![CDATA[oia2TjuEGTNoeX76QEjQNrcURxG8]]></FromUserName>
 *  <CreateTime>1395658920</CreateTime>
 *  <MsgType><![CDATA[event]]></MsgType>
 *  <Event><![CDATA[TEMPLATESENDJOBFINISH]]></Event>
 *  <MsgID>200163836</MsgID>
 *  <Status><![CDATA[success]]></Status>
 *  ////////
 *  <Event><![CDATA[TEMPLATESENDJOBFINISH]]></Event>
 *    <MsgID>200163840</MsgID>
 *    <Status><![CDATA[failed:user block]]></Status>
 *  ////////
 *  <Event><![CDATA[TEMPLATESENDJOBFINISH]]></Event>
 *    <MsgID>200163840</MsgID>
 *    <Status><![CDATA[failed: system failed]]></Status>
 *  ////////
 * </xml>
 *
 */
function wechat_recv_event_template_send_job_callback($postArray) {
    $resultStr = '';

    $FromUserName = $postArray['FromUserName'];
    $msgid = $postArray['MsgID'];
    $status = $postArray['Status'];
    if ($status != 'success') {
        watchdog(__FUNCTION__, ': template msg response: <pre>@data</pre>', array('@data' => print_r($status, true)));
    }

    return $resultStr;

}

/**
 * receive user location gps information every 5 seconds
 * <xml>
 * <ToUserName><![CDATA[toUser]]></ToUserName>
 * <FromUserName><![CDATA[fromUser]]></FromUserName>
 * <CreateTime>123456789</CreateTime>
 * <MsgType><![CDATA[event]]></MsgType>
 * <Event><![CDATA[LOCATION]]></Event>
 * <Latitude>23.137466</Latitude>
 * <Longitude>113.352425</Longitude>
 * <Precision>119.385040</Precision>
 * </xml>
 *
 */
function wechat_recv_event_location_msg_callback($postArray) {
    $resultStr = '';

    watchdog(__FUNCTION__, ': loc information: <pre>@data</pre>', array('@data' => print_r($postArray, true)));
    $FromUserName = $postArray['FromUserName'];
    $Latitude = $postArray['Latitude'];
    $Longitude = $postArray['Longitude'];

    return $resultStr;

}

function collect_event_statistic($postArray) {
    $msgtype = $postArray['MsgType'];

    switch ($msgtype) {
        case 'event':
            $event = $postArray['Event'];
            switch ($event) {
                case 'unsubscribe': //推出关注
                    $eventkey = '';
                break;
                case 'subscribe': //加关注
                    if (empty($postArray['EventKey'])) { //服务号加关注
                        $eventkey = '';
                    }
                    else { //扫码加关注
                        $eventkey = $postArray['EventKey'];
                    }
                case 'SCAN': //已关注后扫码事件
                    $eventkey = $postArray['EventKey'];
                break;
                case 'LOCATION': //上报地理位置
                    $eventkey = '';
                break;
                case 'CLICK': //自定义菜单click事件
                    $eventkey = $postArray['EventKey'];
                break;
                case 'VIEW': //自定义菜单url跳转事件
                    $eventkey = $postArray['EventKey'];
                break;

                default:
                    // code...
                    
                break;
            }
        break;

        default:
            // code...
            
        break;
    }

    db_insert('wechat_api_event_statistic_table')->fields(array(
        'openid' => $postArray['FromUserName'],
        'msgtype' => $postArray['MsgType'],
        'event' => $postArray['Event'],
        'eventkey' => $eventkey,
        'createtime' => $postArray['CreateTime'],
    ))->execute();
}

/**
 * End of wechat access url entrance
 */


