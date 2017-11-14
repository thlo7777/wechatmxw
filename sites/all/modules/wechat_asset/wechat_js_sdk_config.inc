<?php
//wechat js-sdk config page for mobile
function wechat_js_sdk_config() {

    $response;

    $code = isset($_GET['code']) ? $_GET['code'] : '';
    //watchdog('dld mobile js-sdk config', 'code = <pre>@data</pre>', array('@data' => print_r($_GET, TRUE)));
    switch ($code) {
        case 'url': //for jsapi signpackage
            $url = isset($_GET['name']) ? $_GET['name'] : '';
            $response = jsapi_getSignPackage($url);
            drupal_json_output($response);
        break;

        default:
            watchdog(__FUNCTION__, 'ajax send unknow code @code', array(
                '@code' => $code
            ) , WATCHDOG_ERROR);
        break;
    }

    return;
}

//jspai sign package
function jsapi_getSignPackage($url) {

    if ($url == '') {
        return array();
    }

    $appId = variable_get('wechat_AppID');
    $jsapiTicket = variable_get('jsapi_ticket');
    $timestamp = time();
    $nonceStr = createNonceStr();

    // 这里参数的顺序要按照 key 值 ASCII 码升序排序
    $string = "jsapi_ticket=$jsapiTicket&noncestr=$nonceStr&timestamp=$timestamp&url=$url";

    $signature = sha1($string);

    $signPackage = array(
        "appId" => $appId,
        "nonceStr" => $nonceStr,
        "timestamp" => $timestamp,
        "signature" => $signature,
    );
    return $signPackage;
}

function createNonceStr($length = 16) {
    $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    $str = "";
    for ($i = 0;$i < $length;$i++) {
        $str .= substr($chars, mt_rand(0, strlen($chars) - 1) , 1);
    }
    return $str;
}

/**
 * Convert xml object to array
 */
function shopping_xml2array($xml) {
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
 * End of wechat_py_mobile_py_shopping_callback.inc file name
 */


