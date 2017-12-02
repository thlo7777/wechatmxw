(function($) {

    console.log(jsObj);

    var opts = {
            formField: {
                    field_promotion_leaf_tpl: {}
                }
        };
    var target = $('.wm-content.container');
    
    //init ajaxSetup
    $.ajaxSetup({
        cache: false,
        headers: { "cache-control": "no-cache" },
    });

    var json_data = {
        type: "hotel_get_leaf_tpl",
        xyz: jsObj.xyz,
        nid: jsObj.nid
    };
    var get_leaf_page = function(data, that) {
        console.log(data);

        $row = $('<div>').attr({'class': "row"}).appendTo(that);
        if (data.count) {
            insert_image_button_field($row, 'leaf-upload', '上传宣传页模版', data.leaf_tpl);
        } else {
            insert_image_button_field($row, 'leaf-upload', '上传宣传页模版');
        }

        $('<hr>').appendTo(that);

        $row = $('<div>').attr({'class': "row"}).appendTo(that);
        if (data.leaflet_file !== undefined) {
            insert_submit_button_field($row, '生成我的宣传页', opts, data.leaflet_file);
        } else {
            insert_submit_button_field($row, '生成我的宣传页', opts);
        }

        //clear submit button, move to top and aler info waiting audit
        //window.location.href = opts.manage_url;
        //clear_submit_button(that, buttonClass);
        //go to generate shop page
        //new_url = gen_shopqr_url.replace(/%/, data.shop_nid);

        //window.location.href = new_url;    //go to shop qr page
    };

    $.wechat_ajax_submit("GET", json_data, target, get_leaf_page, 'get_leaf_page', jsObj.rest_url);

    function insert_image_button_field($parent, id, label, value) {
        //hotel face photo
        $form_group = $('<div>').attr({'class': 'form-group pics-form'}).appendTo($parent);
        $('<label>').attr({
            'for': id,
            'class': "col-lg-2 control-label"
        }).html(label).appendTo($form_group);
        $col_10 = $('<div>').attr({'class': "col-lg-10"}).appendTo($form_group);
        $('<a>').attr({
            'href': "javascript:void(0)",
            'class': "btn btn-primary btn-sm",
            'id': id
        }).html(
            '<i class="zmdi zmdi-collection-item-1 zmdi-hc-lg zmdi-hc-fw"></i>' +
            '添加宣传页</a>'
        ).appendTo($col_10);

        if ( value !== undefined || value != null ) {
            $row = $('<div>').attr({'class': "row pic-grid"}).appendTo($form_group);
            $col_6 = $('<div>').attr({'class': "col-xs-6 pic-item text-center"}).html(
                '<img src="' + value + '" class="img-responsive">'
            ).appendTo($row);
            $('<a>').attr({
                'href': "javascript:void(0)",
                'class': "delete-image"
                }).html('<i class="zmdi zmdi-minus-circle-outline zmdi-hc-lg"></i>').appendTo($col_6);
        }
        
        $(document).on('click', '#' + id, function(event) {
            event.preventDefault();
            $('.form-group.pics-form').input_pics_button(1);
        });

        //for input pics delete image
        $(document).on('click', '.delete-image', function(event) {
            event.preventDefault();
            var $p_element = $(this).parent().parent();
            $(this).parent().children().each(function() {
                $(this).remove();
            });

            var count = $p_element.find('img').length;
            if ( ! count) {
                $p_element.remove();
            }
        });
        
        return $('#' + id);
    }

    //call wechat to select pictures for each input pics button
    $.fn.input_pics_button = function(num) {

        var render_local_pics = function(localids, that) {

            //find all imgs number
            var imgs_num = that.find('.row.pic-grid img').length;

            //can plug how many pics to form pics
            var rest_imgs = num - imgs_num;
            if (! rest_imgs) {
                return ;    //enough pics by num size
            }

            //rest_imgs = rest_imgs >= localids.length ? localids.length : rest_imgs;
            var add_ids = localids.slice(0, rest_imgs);

            //have to check if row pic-grid exists then plugin a pic or create new row pic-grid
            var row_grid = that.find('.row.pic-grid').each(function() {

                image_width = $(this).width()/2 - 3;

                if ($(this).find('.pic-item img').length < 2) {

                    $(this).find('.pic-item').each(function() {
                        var item_this = this;
                        if (! $(item_this).find('img').length && add_ids.length) {
                            $('<img>').attr({
                                "src": add_ids.shift(),
                                "class": "img-responsive"
                            }).css({"width": image_width, "height": "100%"}).appendTo(item_this);

                            pic_a = $('<a>').attr({
                                "href": "javascript:void(0)",
                                "class": "delete-image"
                            }).css({"margin-left": image_width/2 - 7}).html('<i class="zmdi zmdi-minus-circle-outline zmdi-hc-lg"></i>').appendTo(item_this);

                        }
                    });

                    if ($(this).find('.pic-item').length != 2 && add_ids.length) {

                        pic_grid_6 = $('<div>').attr({
                            "class": "col-xs-6 pic-item",
                            "data-left": 0
                        }).appendTo(this);

                        $('<img>').attr({
                            "src": add_ids.shift(),
                            "class": "img-responsive"
                        }).css({"width": image_width, "height": "100%"}).appendTo(pic_grid_6);

                        pic_a = $('<a>').attr({
                            "href": "javascript:void(0)",
                            "class": "delete-image"
                        }).css({"margin-left": image_width/2 - 7}).html('<i class="zmdi zmdi-minus-circle-outline zmdi-hc-lg"></i>').appendTo(pic_grid_6);

                    }
                }
                //console.log(self);
            });

            //define each of row has 2 pic
            var rows = Math.ceil(add_ids.length/2);
            for (var i = 0; i < rows; i++) {
                pic_row = $('<div>').attr({"class": "row pic-grid"}).appendTo(that);
                image_width = pic_row.width()/2 - 3;

                    for (var j = 0; j < 2; j++) {
                        if (add_ids.length) {

                            if (j % 2) {
                                pic_grid_6 = $('<div>').attr({
                                    "class": "col-xs-6 pic-item",
                                    "data-left": 0
                                }).appendTo(pic_row);
                            } else {
                                pic_grid_6 = $('<div>').attr({
                                    "class": "col-xs-6 pic-item",
                                    "data-left": 1
                                }).appendTo(pic_row);
                            }

                            $('<img>').attr({
                                "src": add_ids.shift(),
                                "class": "img-responsive"
                            }).css({"width": image_width, "height": "100%"}).appendTo(pic_grid_6);

                            pic_a = $('<a>').attr({
                                "href": "javascript:void(0)",
                                "class": "delete-image"
                            }).css({"margin-left": image_width/2 - 7}).html('<i class="zmdi zmdi-minus-circle-outline zmdi-hc-lg"></i>').appendTo(pic_grid_6);

                    }
                }

            }
        };

        var that = this;    //protect self element for sucess function overwrite this
        //select image form wechat
        wx.chooseImage({
            count: num, // 默认9
            sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地ID列表，localId可以作为img标签的src属性显示图片, how many rows
                render_local_pics(res.localIds, that);
            }
        });

    }; //$.fn.input_pics_button()

    function insert_submit_button_field($parent, label, opts, leaf_file) {

        if ( leaf_file !== undefined || leaf_file != null ) {

            $row = $('<div>').attr({
                'class': "row container",
                'style': "margin-bottom: 20px"
            }).appendTo($parent);

            $col_12 = $('<div>').attr({'class': "col-xs-12 text-center"}).html(
                '<img src="' + leaf_file + '" class="img-responsive">'
            ).appendTo($row);
        }
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<div class="col-lg-12 text-center">' +
                '<button type="submit" class="btn btn-primary submit-register-form">' + label + '</button>' +
            '</div>'
        ).appendTo($parent);


        $(document).on('click', '.submit-register-form', {opts: opts}, function(event) {

            event.preventDefault();

            opts = event.data.opts;
            if ( !$('.submit-register-form').form_validation(opts) ) {
                return ;
            }

            $(this).prop("disabled", true); //disable button before send data to server
            $('.submit-register-form').submit_register_form(opts, '.submit-register-form');

        });



        return $('.submit-register-form');
    }

    $.fn.form_validation = function(opts) {

        //validate image upload
        img_num = $('.form-group.pics-form').find('.row.pic-grid .pic-item img');
        if (! img_num.length) {
            $('.input-picture').focus();
            alert('请添加宣传页图片');
            return false;
        } else {
            opts.formField.field_promotion_leaf_tpl.localids = [];
            img_num.each(function() {
                if ( $(this).attr('src').indexOf(window.location.hostname) < 0 ) {
                    opts.formField.field_promotion_leaf_tpl.localids.push($(this).attr('src'));
                }
            });
        }

        return true;
    }

    $.fn.submit_register_form = function(that, buttonClass) {
        var opts = that;

        $(buttonClass).prop("disabled", true); //disable button before send data to server

        //show pregressbar modal
        var proBar = $('.wm-page').progressbar().openModal();
        proBar.setProgress({width: "0px", per: "0%"});

        //upload shop license pics and get media ids
        var pics_form = upload_deferred('pics-form', opts.formField.field_promotion_leaf_tpl.localids);
        $.when(pics_form).done(function(var1) {
            //console.log(var1);
            if (var1 !== undefined && var1 != null && var1.length){
                opts.formField.field_promotion_leaf_tpl.new_img = var1;
            }

            proBar.setProgress({width: "100%", per: "100%"});
            //call ajax send form to server
            var json_data = {
                type: "hotel_generate_leaf",
                xyz: jsObj.xyz,
                nid: jsObj.nid,
                apply_form: opts
            };
            var form_submit_finish = function(data, $button) {
                //clear submit button, move to top and aler info waiting audit
                //window.location.href = opts.manage_url;
                //clear_submit_button(that, buttonClass);
                //go to generate shop page
                //new_url = gen_shopqr_url.replace(/%/, data.shop_nid);

                proBar.closeModal();
                $button.prop("disabled", false); //disable button to false

                console.log(data);
                $('.row.container').empty();
                $col_12 = $('<div>').attr({'class': "col-xs-12 pic-item text-center"}).html(
                    '<img src="' + data.leaf_file + '" class="img-responsive">'
                ).appendTo($('.row.container'));

                //window.location.href = new_url;    //go to shop qr page
            };

            $.wechat_ajax_submit("POST", json_data, $(buttonClass), form_submit_finish, 'form_submit_finish', jsObj.rest_url);

        }).fail(function(var1) {
            alert(var1);
            console.log(var1);
        });

    };

    //recursive function for upload locaid to wechat server and get media id
    var upload_deferred = function(type, localids) {
        var dtd = $.Deferred(); //在函数内部，新建一个Deferred对象
        var media_ids = [];

        //sync pics with wechat server
        var syncUpload = function(localIds) {
            //alert(localIds.length);

            if (localIds.length) {
                var localId = localIds.shift();

                if (window.__wxjs_is_wkwebview === true) {
                    //https://mp.weixin.qq.com/advanced/wiki?t=t=resource/res_main&id=mp1483682025_enmey
                    //iOS WKWebview 网页开发适配指南 
                    //二：页面通过LocalID预览图片 
                    //变化：1.2.0以下版本的JSSDK不再支持通过使用chooseImage api返回的
                    //localld以如：”img src=wxLocalResource://50114659201332”的方式预览图片。 
                    //适配建议：直接将JSSDK升级为1.2.0最新版本即可帮助页面自动适配，但在部分场景下可能无效，此时可以使用
                    //getLocalImgData 接口来直接获取数据。
                    //
//                    var upload_image_callback = function(data) {
//                        //alert(JSON.stringify(data));
//                        if (data.message == "ok" && data.count == 1) {
//
//                            var serverId = data.serverId; // 返回图片的服务器端ID
//                            media_ids.push(serverId);
//
//                            if(localIds.length > 0){
//                                syncUpload(localIds);
//                            } else {
//                                //get all meida ids and return value
//                                dtd.resolve(media_ids);
//                            }
//                        }
//                    };
                    //$.dld_uploadImage(localId, upload_image_callback);
                    //alert(localId);
                    media_ids.push(localId);    //base64 file

                    if(localIds.length > 0){
                        syncUpload(localIds);
                    } else {
                        //get all meida ids and return value
                        dtd.resolve(media_ids);
                    }

                } else {

                    wx.uploadImage({

                        localId: localId,
                        isShowProgressTips: 0,  //don't show wechat instead use progress bar
                        success: function (res) {
                            var serverId = res.serverId; // 返回图片的服务器端ID
                            media_ids.push(serverId);

                            if(localIds.length > 0){
                                syncUpload(localIds);
                            } else {
                                //get all meida ids and return value
                                dtd.resolve(media_ids);
                            }
                        },

                        //error handle
                        fail: function(res) {
                            alert(JSON.stringify(res));
                            dtd.reject(type + ": upload error");
                        }

                    });
                }

            } else {
                dtd.resolve(null);
            }
        };

        //call upload multip images
        syncUpload(localids);

        return dtd.promise(); // 返回promise对象
    };


})(jQuery);
