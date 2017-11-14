<?php
/**
 * send news to all user
 *
 * http请求方式: POST
 * https://api.weixin.qq.com/cgi-bin/message/mass/sendall?access_token=ACCESS_TOKEN
 *
 * POST数据示例如下：
 *
 * 图文消息（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *    "filter":{
 *             "is_to_all":false
 *             "group_id":"2"
 *             },
 *    "mpnews":{
 *             "media_id":"123dsdajkasd231jhksad"
 *              },
 *    "msgtype":"mpnews"
 * }
 * 文本：
 *
 * {
 *    "filter":{
 *             "is_to_all":false
 *             "group_id":"2"
 *             },
 *     "text":{
 *            "content":"CONTENT"
 *            },
 *     "msgtype":"text"
 * }
 * 语音（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *    "filter":{
 *             "is_to_all":false
 *             "group_id":"2"
 *             },
 *     "voice":{
 *             "media_id":"123dsdajkasd231jhksad"
 *             },
 *     "msgtype":"voice"
 * }
 * 图片（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *    "filter":{
 *             "is_to_all":false
 *             "group_id":"2"
 *             },
 *     "image":{
 *             "media_id":"123dsdajkasd231jhksad"
 *             },
 *     "msgtype":"image"
 * }
 *
 * 请注意，此处视频的media_id需通过POST请求到下述接口特别地得到：
 * https://file.api.weixin.qq.com/cgi-bin/media/uploadvideo?access_token=ACCESS_TOKEN POST数据如下
 *（此处media_id需通过基础支持中的上传下载多媒体文件来得到）：
 *
 * {
 *   "media_id": "rF4UdIMfYK3efUfyoddYRMU50zMiRmmt_l0kszupYh_SzrcW5Gaheq05p_lHuOTQ",
 *   "title": "TITLE",
 *   "description": "Description"
 * }
 * 返回将为
 *
 * {
 *   "type":"video",
 *   "media_id":"IhdaAQXuvJtGzwwc0abfXnzeezfO0NgPK6AQYShD8RQYMTtfzbLdBIQkQziv2XJc",
 *   "created_at":1398848981
 * }
 * 然后，POST下述数据（将media_id改为上一步中得到的media_id），即可进行发送
 *
 * {
 *    "filter":{
 *             "is_to_all":false
 *             "group_id":"2"
 *             },
 *     "mpvideo":{
 *             "media_id":"IhdaAQXuvJtGzwwc0abfXnzeezfO0NgPK6AQYShD8RQYMTtfzbLdBIQkQziv2XJc",
 *                },
 *     "msgtype":"mpvideo"
 * }
 *
 * return correct value:
 * {
 *    "errcode":0,
 *    "errmsg":"send job submission success",
 *    "msg_id":34182
 * }
 *
 */

function wechat_api_send_news_to_all($media_id, $msgtype, $is_to_all, $group) {

    $token_url = t(variable_get('send news to all') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    if ($is_to_all)
    {
        $post_data['filter'] = array(
            'is_to_all' => $is_to_all
        );
    }
    else
    {
        $post_data['filter'] = array(
            'is_to_all' => $is_to_all,
            'group_id' => $group
        );
    }
    /*
     *群发接口新增 send_ignore_reprint 参数，开发者可以对群发接口的 send_ignore_reprint 参数进行设置，指定待群发的文章被判定为转载时，是否继续群发。
     *当 send_ignore_reprint 参数设置为1时，文章被判定为转载时，且原创文允许转载时，将继续进行群发操作。
     *当 send_ignore_reprint 参数设置为0时，文章被判定为转载时，将停止群发操作。
     *send_ignore_reprint 默认为0。
    */
    $post_data['send_ignore_reprint'] = 1;

    switch ($msgtype)
    {
        case "mpnews":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "text":
            $post_data[$msgtype] = array(
                'content' => $media_id
            ); //$media_id is content text
            $post_data['msgtype'] = $msgtype;
        break;
        case "voice":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "image":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "mpvideo":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        default:
            watchdog('wechat send to all', 'message type wrong: @msgtype', array(
                '@msgtype' => $msgtype
            ) , $severity = WATCHDOG_ERROR);
            return '';
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    //check http://loosky.net/3069.html how to protect mess code
    $json_data = json_encode($post_data, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT | JSON_UNESCAPED_UNICODE);

    $result = wechat_php_curl_https_post($token_url, $json_data);
    if (!$result)
    {
        watchdog('wechat send to all', 'return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if ($json_value->errcode)
    {
        watchdog('wechat send to all', 'error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value;
}

/**
 * send news to previews user
 *
 * http请求方式: POST
 * https://api.weixin.qq.com/cgi-bin/message/mass/preview?access_token=ACCESS_TOKEN
 *
 * or send news to openid lists
 * http请求方式: POST
 * https://api.weixin.qq.com/cgi-bin/message/mass/send?access_token=ACCESS_TOKEN
 *
 * POST数据示例如下：
 *
 * 图文消息（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *    "touser":"OPENID",
 *    or
 *    "touser":[
 *    "OPENID1",
 *    "OPENID2"
 *    ],
 *    "mpnews":{
 *             "media_id":"123dsdajkasd231jhksad"
 *              },
 *    "msgtype":"mpnews"
 * }
 * 文本：
 *
 * {
 *     "touser":"OPENID",
 *    or
 *    "touser":[
 *    "OPENID1",
 *    "OPENID2"
 *    ],
 *     "text":{
 *            "content":"CONTENT"
 *            },
 *     "msgtype":"text"
 * }
 * 语音（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *     "touser":"OPENID",
 *    or
 *    "touser":[
 *    "OPENID1",
 *    "OPENID2"
 *    ],
 *     "voice":{
 *             "media_id":"123dsdajkasd231jhksad"
 *             },
 *     "msgtype":"voice"
 * }
 * 图片（其中media_id与根据分组群发中的media_id相同）：
 *
 * {
 *     "touser":"OPENID",
 *    or
 *    "touser":[
 *    "OPENID1",
 *    "OPENID2"
 *    ],
 *     "image":{
 *             "media_id":"123dsdajkasd231jhksad"
 *             },
 *     "msgtype":"image"
 * }
 *
 * only for openid list
 * 请注意，此处视频的media_id需通过POST请求到下述接口特别地得到：
 * https://file.api.weixin.qq.com/cgi-bin/media/uploadvideo?access_token=ACCESS_TOKEN POST数据如下
 *（此处media_id需通过基础支持中的上传下载多媒体文件来得到）：
 *
 * {
 *   "media_id": "rF4UdIMfYK3efUfyoddYRMU50zMiRmmt_l0kszupYh_SzrcW5Gaheq05p_lHuOTQ",
 *   "title": "TITLE",
 *   "description": "Description"
 * }
 * 返回将为
 *
 * {
 *   "type":"video",
 *   "media_id":"IhdaAQXuvJtGzwwc0abfXnzeezfO0NgPK6AQYShD8RQYMTtfzbLdBIQkQziv2XJc",
 *   "created_at":1398848981
 * }
 * 然后，POST下述数据（将media_id改为上一步中得到的media_id），即可进行发送
 *
 * {
 *    "touser":[
 *    "OPENID1",
 *    "OPENID2"
 *    ],
 *    "video":{
 *         "media_id":"123dsdajkasd231jhksad",
 *         "title":"TITLE",
 *        "description":"DESCRIPTION"
 *        },
 *    "msgtype":"video"
 * }
 *
 *
 * only for preview 视频（其中media_id与根据分组群发中的media_id相同）：
 * {
 *     "touser":"OPENID",
 *     "mpvideo":{
 *             "media_id":"IhdaAQXuvJtGzwwc0abfXnzeezfO0NgPK6AQYShD8RQYMTtfzbLdBIQkQziv2XJc",
 *                },
 *     "msgtype":"mpvideo"
 * }
 *
 * return correct value:
 * {
 *    "errcode":0,
 *    "errmsg":"send job submission success",
 *    "msg_id":34182
 * }
 *
 */
function wechat_api_send_preview_to_openid($media_id, $msgtype, $openid)
{

    if (is_array($openid))
    {
        //openids array
        $token_url = t(variable_get('send openid list media') , array(
            '@ACCESS_TOKEN' => variable_get('access_token')
        ));
    }
    else
    {
        //only one openid
        $token_url = t(variable_get('send preview media') , array(
            '@ACCESS_TOKEN' => variable_get('access_token')
        ));
    }

    $post_data['touser'] = $openid;
    switch ($msgtype)
    {
        case "mpnews":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "text":
            $post_data[$msgtype] = array(
                'content' => $media_id
            ); //$media_id is content text
            $post_data['msgtype'] = $msgtype;
        break;
        case "voice":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "image":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        case "mpvideo":
            $post_data[$msgtype] = array(
                'media_id' => $media_id
            );
            $post_data['msgtype'] = $msgtype;
        break;
        default:
            watchdog('wechat send preview', 'message type wrong: @msgtype', array(
                '@msgtype' => $msgtype
            ) , $severity = WATCHDOG_ERROR);
            return '';
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    //check http://loosky.net/3069.html how to protect mess code
    $json_data = json_encode($post_data, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT | JSON_UNESCAPED_UNICODE);

    $result = wechat_php_curl_https_post($token_url, $json_data);

    if (!$result)
    {
        watchdog('wechat send preview', 'return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if ($json_value->errcode)
    {
        watchdog('wechat send preview', 'error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value;
}

/**
 * Upload news to server
 *
 * http请求方式: POST
 * https://api.weixin.qq.com/cgi-bin/media/uploadnews?access_token=ACCESS_TOKEN
 *
 * return correct value:
 * {
 *  "type":"news",
 *  "media_id":"CsEf3ldqkAYJAU6EJeIkStVDSvffUJ54vqbThMgplD-VJXXof6ctX5fI6-aYyUiQ",
 *  "created_at":1391857799
 * }
 *
 * return error value:
 * {"errcode":40004,"errmsg":"invalid media type"}
 */
function wechat_api_upload_news_media($post_data)
{

    $token_url = t(variable_get('upload news') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $post_data);
    if (!$result)
    {
        watchdog('wechat upload media', 'upload news error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('wechat upload media', 'upload news error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value;
}

/**
 * Upload temporary media file to server
 *
 * http请求方式: POST/FORM,需使用https
 * https://api.weixin.qq.com/cgi-bin/media/upload?access_token=ACCESS_TOKEN&type=TYPE
 * 调用示例（使用curl命令，用FORM表单方式上传一个多媒体文件）：
 * curl -F media=@test.jpg "https://api.weixin.qq.com/cgi-bin/media/upload?access_token=ACCESS_TOKEN&type=TYPE"
 *
 * TYPE 媒体文件类型，分别有图片（image）、语音（voice）、视频（video）和缩略图（thumb 要用于视频与音乐格式的缩略图）
 * 图片大小不超过2M，支持bmp/png/jpeg/jpg/gif格式，语音大小不超过5M，长度不超过60秒，支持mp3/wma/wav/amr格式
 * return correct value:
 * {"type":"TYPE","media_id":"MEDIA_ID","created_at":123456789}
 *
 * return error value:
 * {"errcode":40004,"errmsg":"invalid media type"}
 */
function wechat_api_upload_temp_media_file($file, $type)
{

    $token_url = t(variable_get('upload temp media file') , array(
        '@ACCESS_TOKEN' => variable_get('access_token') ,
        '@TYPE' => $type
    ));

    //Need system path instead of file url
    $file = drupal_realpath($file);
    //find file format
    $file_ext = preg_split('/\./', $file);

    //关键是判断curlfile,官网推荐php5.5或更高的版本使用curlfile来实例文件
    if (class_exists('\CURLFile'))
    {
        $post_data = array(
            'media' => new \CURLFile($file, 'image/' . $file_ext[1])
        );
    }
    else
    {
        $post_data = array(
            'media' => '@' . $file
        );
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $post_data);
    if (!$result)
    {
        watchdog('wechat upload media', 'upload temp media file error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('wechat upload media', 'upload temp media file error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value;
}

/**
 * Upload image to server, and get URL for wechat article
 *
 * http请求方式: POST
 * https://api.weixin.qq.com/cgi-bin/media/uploadimg?access_token=ACCESS_TOKEN
 * 调用示例（使用curl命令，用FORM表单方式上传一个图片）：
 * curl -F media=@test.jpg "https://api.weixin.qq.com/cgi-bin/media/uploadimg?access_token=ACCESS_TOKEN"
 * only support jpg/png file, size <= 1M byte
 * return correct value:
 * {
 *   "url":  "http://mmbiz.qpic.cn/mmbiz/gLO17UPS6FS2xsypf378iaNhWacZ1G1UplZYWEYfwvuU6Ont96b1roYs CNFwaRrSaKTPCUdBK9DgEHicsKwWCBRQ/0"
 * }
 *
 * return error value:
 * {"errcode":40004,"errmsg":"invalid media type"}
 */
function wechat_api_upload_image_file($file, $type = '')
{

    $token_url = t(variable_get('upload image url to news') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    //Need system path instead of file url
    $file = drupal_realpath($file);
    watchdog('wechat upload image', 'file <pre>@data</pre>', array(
        '@data' => print_r($file, true)
    ));
    //find file format
    $file_ext = preg_split('/.*\./', $file);
    watchdog('wechat upload image', 'file_ext <pre>@data</pre>', array(
        '@data' => print_r($file_ext, true)
    ));

    //关键是判断curlfile,官网推荐php5.5或更高的版本使用curlfile来实例文件
    if (class_exists('\CURLFile'))
    {
        $post_data = array(
            'media' => new \CURLFile($file, 'image/' . $file_ext[1])
        );
    }
    else
    {
        $post_data = array(
            'media' => '@' . $file
        );
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $post_data);
    if (!$result)
    {
        watchdog('wechat upload image', 'upload image file error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('wechat upload media', 'upload image file error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value;
}

/**
 * Upload other material to wechat server, with permanently and return URL
 *
 * 新增其他类型永久素材
 * 接口调用请求说明
 * 通过POST表单来调用接口，表单id为media，包含需要上传的素材内容，有filename、filelength、content-type等信息。
 * 请注意：图片素材将进入公众平台官网素材管理模块中的默认分组。
 * http请求方式: POST，需使用https
 * https://api.weixin.qq.com/cgi-bin/material/add_material?access_token=ACCESS_TOKEN&type=TYPE
 * 调用示例（使用curl命令，用FORM表单方式新增一个其他类型的永久素材，curl命令的使用请自行查阅资料）
 * parameters:
 * access_token 是  调用接口凭证
 * type 是  媒体文件类型，分别有图片（image）、语音（voice）、视频（video）和缩略图（thumb）
 * media    是  form-data中媒体文件标识，有filename、filelength、content-type等信息
 * return correct value:
 * {
 media_id    新增的永久素材的media_id
 url 新增的图片素材的图片URL（仅新增图片素材时会返回该字段）
 * }
 *
 * return error value:
 * {"errcode":40007,"errmsg":"invalid media id"}
 */
function wechat_api_upload_other_material($file, $type = '')
{

    $token_url = t(variable_get('upload other material') , array(
        '@ACCESS_TOKEN' => variable_get('access_token') ,
        '@TYPE' => $type
    ));

    //Need system path instead of file url
    $file = drupal_realpath($file);
    //watchdog('upload other material', 'file <pre>@data</pre>', array('@data' => print_r($file, TRUE)));
    //find file format
    $file_ext = preg_split('/.*\./', $file);

    //关键是判断curlfile,官网推荐php5.5或更高的版本使用curlfile来实例文件
    if (class_exists('\CURLFile'))
    {
        $post_data = array(
            'media' => new \CURLFile($file, $type . '/' . $file_ext[1])
        );
    }
    else
    {
        $post_data = array(
            'media' => '@' . $file
        );
    }

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $post_data);
    if (!$result)
    {
        watchdog('upload other material', 'upload material file error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return false;
    }

    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('upload other material', 'upload material file error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return false;
    }

    return $json_value;
}




/**
 * get user list from wechat server
 *
 * http请求方式: GET（请使用https协议）
 * next_openid  第一个拉取的OPENID，不填默认从头开始拉取
 * https://api.weixin.qq.com/cgi-bin/user/get?access_token=ACCESS_TOKEN
 * return correct value:
 * {"total":2,"count":2,"data":{"openid":["","OPENID1","OPENID2"]},"next_openid":"NEXT_OPENID1"}
 * total    关注该公众账号的总用户数
 * count    拉取的OPENID个数，最大值为10000
 * data 列表数据，OPENID的列表
 * next_openid  拉取列表的后一个用户的OPENID
 * https://api.weixin.qq.com/cgi-bin/user/get?access_token=ACCESS_TOKEN&next_openid=NEXT_OPENID1
 *
 * return error
 * {"errcode":40013,"errmsg":"invalid appid"}
 *
 */
function wechat_api_get_user_list()
{

    $token_url = t(variable_get('get user list') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_get($token_url);
    if (!$result)
    {
        watchdog('wechat get user list', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    //get user list, it will return maxmum 10000 open id each time.
    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('wechat get user list', 'error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    //return user list json format
    return $json_value;
}

/**
 * get user list from wechat server
 *
 * http请求方式: GET（请使用https协议）
 * https://api.weixin.qq.com/cgi-bin/user/get?access_token=ACCESS_TOKEN&next_openid=NEXT_OPENID
 * return correct value:
 * {"total":2,"count":2,"data":{"openid":["","OPENID1","OPENID2"]},"next_openid":"NEXT_OPENID1"}
 * total    关注该公众账号的总用户数
 * count    拉取的OPENID个数，最大值为10000
 * data 列表数据，OPENID的列表
 * next_openid  拉取列表的后一个用户的OPENID
 * https://api.weixin.qq.com/cgi-bin/user/get?access_token=ACCESS_TOKEN&next_openid=NEXT_OPENID1
 *
 * return error
 * {"errcode":40013,"errmsg":"invalid appid"}
 *
 */
function wechat_api_get_user_list_next($openid)
{

    $token_url = t(variable_get('get user list next') , array(
        '@ACCESS_TOKEN' => variable_get('access_token') ,
        '@NEXT_OPENID' => $openid
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_get($token_url);
    if (!$result)
    {
        watchdog('wechat get user list next', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    //get user list, it return maxmum 10000 open id each time.
    $json_value = json_decode($result);
    if (isset($json_value->errcode))
    {
        watchdog('wechat get user list next', 'error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    //return user list json format
    return $json_value;
}



//reget access token to validate user subscribe for mobile shopping
function wechat_api_validate_user_info($openid)
{

    $token_url = t(variable_get('get user info') , array(
        '@ACCESS_TOKEN' => variable_get('access_token') ,
        '@OPENID' => $openid
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_get($token_url);
    if (!$result)
    {
        watchdog('wechat get user info', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);

    if (isset($json_value->errcode))
    {
        watchdog('wechat get user info', 'error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    //return user list json format
    return $json_value;
}

function wechat_api_get_temp_media($save_path, $media_id)
{
    $file_name = null;

    $token_url = t(variable_get('get temp media id') , array(
        '@ACCESS_TOKEN' => variable_get('access_token') ,
        '@MEDIA_ID' => $media_id
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_get($token_url, true);
    if (!$result)
    {
        watchdog('wechat get bar code', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);

        return $file_name;
    }

    list($header, $body) = explode("\r\n\r\n", $result, 2);

    preg_match('@Content-disposition[ \t]*:[ \t]*attachment[ \t]*;[ \t]*filename[ \t]*=[ \t]*\".*\"@i', $header, $matches);
    preg_match('@\".*\"@i', $matches[0], $filename);
    $filename = trim($filename[0], '"');

    //watchdog('wechat_api_get_temp_media', '$filename = <pre>@data</pre>', array('@data' => print_r($filename, TRUE)));
    $destination = $save_path . "/" . $filename;
    $filename = file_unmanaged_save_data($body, $destination, FILE_EXISTS_RENAME);

    return $filename;

}

/**
 * get user head image file from wechat server
 *
 * http请求方式: GET http url
 * $url:  wechat user head image url  (http://)
 * $md5id: wechat user md5 openid
 * return head image file
 */
function wechat_api_get_user_head_image($url, $md5id = 'empty')
{

    $scheme = variable_get('file_default_scheme', 'public') . '://';

    $ch = curl_init($url);

    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
    $picture = curl_exec($ch);
    if (!$picture)
    {
        watchdog('wechat api', 'can not get head image url, replace locate png image');
        $picture = file_create_url(drupal_get_path('module', 'dld_restful_api') . '/resource/ttf/dld_logo_300_300.jpg');
        $img_info = 'image/png';
    }

    # get the content type
    $img_info = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
    curl_close($ch);

    switch ($img_info)
    {
        case "image/gif":
            $temp_head_image_file = $scheme . "wechat_qrcode/wechat_user_headimg_" . $md5id . ".gif";
            $orig_head_image = file_unmanaged_save_data($picture, $temp_head_image_file, FILE_EXISTS_REPLACE);
            $head_image_im = imagecreatefromgif($orig_head_image);
        break;
        case "image/jpeg":
            $temp_head_image_file = $scheme . "wechat_qrcode/wechat_user_headimg_" . $md5id . ".jpeg";
            $orig_head_image = file_unmanaged_save_data($picture, $temp_head_image_file, FILE_EXISTS_REPLACE);
            $head_image_im = imagecreatefromjpeg($orig_head_image);
        break;
        case "image/png":
            $temp_head_image_file = $scheme . "wechat_qrcode/wechat_user_headimg_" . $md5id . ".png";
            $orig_head_image = file_unmanaged_save_data($picture, $temp_head_image_file, FILE_EXISTS_REPLACE);
            $head_image_im = imagecreatefrompng($orig_head_image);
        break;
        case "image/bmp":
            $temp_head_image_file = $scheme . "wechat_qrcode/wechat_user_headimg_" . $md5id . ".bmp";
            $orig_head_image = file_unmanaged_save_data($picture, $temp_head_image_file, FILE_EXISTS_REPLACE);
            $head_image_im = imagecreatefromwbmp($orig_head_image);
        break;
    }

    return $head_image_im;

}

/**
 * wechat remove conditional menu according to menu id
 * 删除个性化菜单
 * http请求方式：POST（请使用https协议）
 * https://api.weixin.qq.com/cgi-bin/menu/delconditional?access_token=ACCESS_TOKEN
 * 请求示例
 * {
 "menuid":"208379533"
 * }
 * 正确时的返回JSON数据包如下，
 * {"errcode":0,"errmsg":"ok"}
 *
 */
function wechat_conditional_menu_remove($menu_id)
{

    $post_data['menuid'] = $menu_id;
    $json_data = json_encode($post_data, JSON_UNESCAPED_UNICODE);

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $token_url = t(variable_get('remove conditional menu') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    $result = wechat_php_curl_https_post($token_url, $json_data);
    if (!$result)
    {
        watchdog('wechat_conditional_menu_remove', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $result;
}

/**
 * wechat check conditional menu by user open id
 * 测试个性化菜单匹配结果
 * http请求方式：POST（请使用https协议）
 * https://api.weixin.qq.com/cgi-bin/menu/trymatch?access_token=ACCESS_TOKEN
 * 请求示例
 * {
 *  "user_id":"weixin"
 * }
 * user_id可以是粉丝的OpenID，也可以是粉丝的微信号。
 *
 */
function wechat_conditional_menu_trymatch($openID)
{

    $post_data['user_id'] = $openID;
    $json_data = json_encode($post_data, JSON_UNESCAPED_UNICODE);

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $token_url = t(variable_get('try match conditional menu') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    $result = wechat_php_curl_https_post($token_url, $json_data);
    if (!$result)
    {
        watchdog('wechat_conditional_menu_trymatch', 'error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }
    return $result;
}

/**
 * wechat batch remove tag to user
 *
 * http请求方式：POST（请使用https协议）
 * https://api.weixin.qq.com/cgi-bin/tags/members/batchuntagging?access_token=ACCESS_TOKEN
 * POST数据格式：JSON
 * POST数据例子：
 * {
 * "openid_list" : [//粉丝列表
 *   "ocYxcuAEy30bX0NXmGn4ypqx3tI0",
 *   "ocYxcuBt0mRugKZ7tGAHPnUaOW7Y"
 * ],
 * "tagid" : 134
 * }
 * 返回说明（正常时返回的json包示例）
 * {
 *  "errcode":0,
 *  "errmsg":"ok"
 * }
 *
 */
function wechat_batch_untag_to_user($openid_list, $tag_id)
{

    $post_data['openid_list'] = $openid_list;
    $post_data['tagid'] = $tag_id;

    $json_data = json_encode($post_data, JSON_UNESCAPED_UNICODE);

    //watchdog('post user list', 'json_data = <pre>@data</pre>', array('@data' => print_r($json_data, TRUE)));
    $token_url = t(variable_get('batch user untag') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $json_data);
    if (!$result)
    {
        watchdog('wechat_batch_untag_to_user', 'create tag error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if ($json_value->errcode != 0)
    {
        watchdog('wechat_batch_untag_to_user', 'create tag error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value->errcode;
}

/**
 * wechat batch add tag to user
 *
 * http请求方式：POST（请使用https协议）
 * https://api.weixin.qq.com/cgi-bin/tags/members/batchtagging?access_token=ACCESS_TOKEN
 * POST数据格式：JSON
 * POST数据例子：
 * {
 * "openid_list" : [//粉丝列表
 *   "ocYxcuAEy30bX0NXmGn4ypqx3tI0",
 *   "ocYxcuBt0mRugKZ7tGAHPnUaOW7Y"
 * ],
 * "tagid" : 134
 * }
 * 返回说明（正常时返回的json包示例）
 * {
 *  "errcode":0,
 *  "errmsg":"ok"
 * }
 *
 */
function wechat_batch_tag_to_user($openid_list, $tag_id)
{

    $post_data['openid_list'] = $openid_list;
    $post_data['tagid'] = $tag_id;

    $json_data = json_encode($post_data, JSON_UNESCAPED_UNICODE);

    //watchdog('post user list', 'json_data = <pre>@data</pre>', array('@data' => print_r($json_data, TRUE)));
    $token_url = t(variable_get('batch user tag') , array(
        '@ACCESS_TOKEN' => variable_get('access_token')
    ));

    module_load_include('inc', 'wechat_api', 'wechat_api_php5');
    $result = wechat_php_curl_https_post($token_url, $json_data);
    if (!$result)
    {
        watchdog('wechat_batch_tag_to_user', 'create tag error: return null in @line line:@filename', array(
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    $json_value = json_decode($result);
    if ($json_value->errcode != 0)
    {
        watchdog('wechat_batch_tag_to_user', 'create tag error code: @error and errmsg: @errmsg at @line in @filename', array(
            '@error' => $json_value->errcode,
            '@errmsg' => $json_value->errmsg,
            '@line' => __LINE__,
            '@filename' => __FILE__,
        ) , $severity = WATCHDOG_ERROR);
        return '';
    }

    return $json_value->errcode;
}

/**
 * End of wechat api send message to wechat template interface
 */

