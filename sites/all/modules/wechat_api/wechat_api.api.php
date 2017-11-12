<?php

/**
 * @file
 * Hooks provided by wechat_api.
 */

/**
 *
 * Allow modules to provide their own caching mechanism for the display editor.
 *
 * @param $conent
 *   String or array XML
 */
function hook_wapi_welcome_page($content) {
    return $content;
}

/**
 * Allow modules to handle receive message
 **/

function hook_wapi_recv_scan_msg($eventkey, $postArray) {
    return TRUE;
}

/**
 *
 * Allow module create user account when user subscribe wechat
 * 
 **/
function hook_wapi_subscribe_user($postArray) {
    //create user in Drupal
}
