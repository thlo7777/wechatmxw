(function($) {

    console.log(jsObj);

    //$(document).config_wechat_only_location_api();

    var form = $('.wm-content.container').hotel_form({
        formID: "hotel-add-form",
        formField: jsObj.form,
        rest_url: jsObj.rest_url
    });

//    $('<button>').attr({'class': "btn btn-success copy-btn"}).html('拷贝').appendTo($fieldset);
//    $('.copy-btn').on('click', function(e) {
//        e.preventDefault();
//        $(this).copyToClipboard('平遥美途驿栈');
//    });
//
//    $.fn.copyToClipboard = function (string) {
//        var $temp = $("<input>");
//        $("body").append($temp);
//        $temp.val(string).select();
//        document.execCommand("copy");
//        $temp.remove();
//    }
//

})(jQuery);
