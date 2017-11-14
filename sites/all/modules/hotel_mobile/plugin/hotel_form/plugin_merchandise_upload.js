/*
 * add upload new merchandise
 */
;(function($, window, document, undefined) {
 
    // Create the defaults once
    var pluginName = 'upload_new_merchandise',
        defaults = {
            nid: 0,
            intervalID: -1,
            progressBar: 10,  //10%
            apply_form: {},
            first_selectize: null,
            second_selectize: null,
            shop_sub_category: null
        };
 
    // The actual plugin constructor
    function Plugin( element, options ) {
        this.element = element;
        // jQuery has an extend method that merges the
        // contents of two or more objects, storing the
        // result in the first object. The first object
        // is generally empty because we don't want to alter
        // the default options for future instances of the plugin
        this.options = $.extend( {}, defaults, options) ;
        this._defaults = defaults;
        this._name = pluginName;
        this.init();
    }

    Plugin.prototype.init = function () {
        // Place initialization logic here
        // You already have access to the DOM element and
        // the options via the instance, e.g. this.element
        // and this.options
    };

    // A really lightweight plugin wrapper around the constructor,
    // preventing against multiple instantiations
    $.fn[pluginName] = function ( options ) {
        return this.each(function () {
            if (!$.data(this, 'plugin_' + pluginName)) {
                $.data(this, 'plugin_' + pluginName,
                new Plugin( this, options ));
            }
        });
    };

    /*
     * create shopkeeper register form
     */
    $.fn.upload_new_merch_form = function() {

        plugin = this.data('plugin_' + pluginName);

        var self_form_name = "form-new-merchandise";
        plugin.options.self_form_name = self_form_name;

        var opts = plugin.options;

        $form = $('<form>').attr({
            'class': "form-horizontal " + self_form_name
        }).appendTo(this);

        $fieldset = $('<fieldset>').appendTo($form);
        $('<legend>').html('录入商品').appendTo($fieldset);

        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="select-shop-term' + opts.nid + '" class="col-lg-2 control-label">选择店铺</label>' +
            '<div class="col-lg-10">' +
                '<select class="form-control" data-vid="' + shop_category_voc + '" id="select-shop-term">' +
                    '<option></option>' +
                '</select>' +
            '</div>'
        ).appendTo($fieldset);

        json_data = {
            xyz: opts.xyz,
            sk_xyz: opts.sk_xyz,
            type: "get_all_my_shop_term_list",
        };
        var create_select2 = function(data, $select2) {
            //create select2 object
            $select2.select2({
                placeholder: "选择店铺",
                minimumResultsForSearch: Infinity,
                language: "zh-CN",
                data: data.terms
            });

            $select2.val(data.terms[0].id).trigger("change");
        };

        $.dld_ajax_submit("GET", json_data, $("#select-shop-term"), create_select2, 'create_select2');
//        $("#select-shop-term").select2({
//
//            placeholder: "选择店铺",
//            minimumResultsForSearch: Infinity,
//            language: "zh-CN",
//
//            //ajax call
//            cache: true,
//            ajax: {
//                url: $.dld_get_restful_url(),
//                dataType: "json",
//                //json data
//                data: function(params) {
//                    return {
//                        data: {
//                            type: "get_all_my_shop_term_list",
//                            xyz: opts.xyz
//                        } 
//                    };
//                },
//
//                //process result data
//                processResults: function (data, params) {
//                    return {
//                        results: data.data.terms
//                    };
//                }
//            }
//
//        }); //dld shop category term

        $("#select-shop-term").on("change", function (e) {
            opts.apply_form.shop_term = {vid: $(this).data('vid'), tid: $(this).val()};
            /*
             * get shop term first level category
             */
            json_data = {
                type: "get_my_shop_sub_term",
                vid: opts.apply_form.shop_term.vid,
                pid: opts.apply_form.shop_term.tid,
                xyz: opts.xyz
            };

            $.dld_ajax_submit("GET", json_data, opts, render_shop_first_category, 'get_my_shop_sub_term');

        });
        $("#select-shop-term").on('select2:select', function (event) {
            //console.log($(this).data('vid'));
            //console.log(event.params.data.id);
            $(this).val(event.params.data.id).trigger("change");
        });

        $('<div>').attr({'class': "alert alert-info"}).html(
            '如果没有合适的一级分类，可以输入新的一级分类。促销专区没有二级分类'
        ).appendTo($fieldset);
        //create my shop first category element
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="first-category-' + opts.nid + '" class="col-lg-2 control-label">商品一级分类</label>' +
            '<div class="col-lg-10">' +
                '<select class="form-control" id="first-category" placeholder="选择/输入一级分类">' +
                    '<option></option>' +
                '</select>' +
            '</div>'
        ).appendTo($fieldset);

        //create my shop second category element
        $('<div>').attr({'class': "alert alert-info"}).html(
            opts.node_help.field_dld_shop_term.description
        ).appendTo($fieldset);
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="second-category-' + opts.nid + '" class="col-lg-2 control-label">商品二级分类</label>' +
            '<div class="col-lg-10">' +
                '<select class="form-control" id="second-category" placeholder="选择/输入二级分类">' +
                    '<option></option>' +
                '</select>' +
            '</div>'
        ).appendTo($fieldset);
        /*
         * create supermarket sub category
         */
        var $select = $('#first-category').selectize({
            create: true,
            sortField: 'value', //order option by term id
            onChange: function(value) {

                delete opts.apply_form.first_tid;
                delete opts.apply_form.first_tname;

                if (value === "" || value === null || typeof(value) === "undefined") {
                    console.log('nothing');
                    return ;
                }

                text = opts.first_selectize.getOption(value)[0].textContent;
                //console.log(opts.first_selectize.getOption(value));
                opts.second_selectize.clearOptions();
                if (text == '促销专区') {
                    opts.second_selectize.disable();
                } else {
                    opts.second_selectize.enable();
                }

                if (value != text) {
                    if (opts.shop_sub_category) {
                        $.each(opts.shop_sub_category[value], function(i, term) {
                            if (i != 0) {
                                opts.second_selectize.addOption({'value': i, 'text': term});
                            }
                        });
                    }
                    opts.apply_form.first_tid = value;
                } else {
                    opts.apply_form.first_tid = -1;  //new input first category tid
                    opts.apply_form.first_tname = value;  //new input first category name
                }

            }
        });
        opts.first_selectize = $select[0].selectize;

        var $select = $('#second-category').selectize({
            create: true,
            sortField: 'value', //order option by term id
            onChange: function(value) {

                delete opts.apply_form.second_tid;
                delete opts.apply_form.second_tname;

                if (value === "" || value === null || typeof(value) === "undefined") {
                    console.log('nothing');
                    return ;
                }
                //console.log(value);
                //console.log(second_selectize.getOption(value));
                //
                text = opts.second_selectize.getOption(value)[0].textContent;
                if (value != text) {
                    opts.apply_form.second_tid = value;
                } else {
                    opts.apply_form.second_tid = -1;  //new input second category tid
                    opts.apply_form.second_tname = value;  //new input second category name
                }
            }
        });
        opts.second_selectize = $select[0].selectize;

        /*
         * workaround solution to capture Chinese character on mobile device
         * 1. unbind keyup from input
         */
        $('#first-category').on('input', function(event) {
            $(this).off('keyup');
        });
        /*
         * 2. bind (keyp input) event with input element
         */
        $('#first-category').on('keyup input', function(e) {
            //alert(event.target.value);
            if (opts.first_selectize) {
                var self = opts.first_selectize;
                if (self.isLocked) return e && e.preventDefault();
                var value = self.$control_input.val() || '';
                if (self.lastValue !== value) {
                    self.lastValue = value;
                    self.onSearchChange(value);
                    self.refreshOptions();
                    self.trigger('type', value);
                }
            } else {
                console.log('can\'t find first_selectize instance');
            }

        });

        $('#second-category').on('input', function(event) {
            $(this).off('keyup');
        });
        $('#second-category').on('keyup input', function(e) {
            //alert(event.target.value);
            if (opts.second_selectize) {
                var self = opts.second_selectize;
                if (self.isLocked) return e && e.preventDefault();
                var value = self.$control_input.val() || '';
                if (self.lastValue !== value) {
                    self.lastValue = value;
                    self.onSearchChange(value);
                    self.refreshOptions();
                    self.trigger('type', value);
                }
            } else {
                console.log('can\'t find second_selectize instance');
            }

        });


        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="input-name-' + opts.nid + '" class="col-lg-2 control-label">商品名称</label>' +
            '<div class="col-lg-10">' +
                '<input type="text" class="form-control" id="input-name-' + opts.nid + '" placeholder="输入商品名称">' +
            '</div>'
        ).appendTo($fieldset);

        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="product-price-' + opts.nid + '" class="col-lg-2 control-label">原价</label>' +
            '<div class="col-lg-10">' +
                '<div class="input-group">' +
                    '<input type="number" class="form-control" id="product-price" placeholder="输入原价">' +
                    '<div class="input-group-addon">元.00</div>' +
                '</div>' +
            '</div>'
        ).appendTo($fieldset);

        $('<div>').attr({'class': "alert alert-info"}).html(
            opts.node_help.field_product_discount_price.description
        ).appendTo($fieldset);
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="discount-price-' + opts.nid + '" class="col-lg-2 control-label">促销价</label>' +
            '<div class="col-lg-10">' +
                '<div class="input-group">' +
                    '<input type="number" class="form-control" id="discount-price" placeholder="输入促销价">' +
                    '<div class="input-group-addon">元.00</div>' +
                '</div>' +
            '</div>'
        ).appendTo($fieldset);

        $('<div>').attr({'class': "alert alert-info"}).html(
            opts.node_help.field_product_weight.description
        ).appendTo($fieldset);
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="product-weight-' + opts.nid + '" class="col-lg-2 control-label">商品重量</label>' +
            '<div class="col-lg-10">' +
                '<div class="input-group">' +
                    '<input type="number" class="form-control" id="product-weight" placeholder="输入商品重量">' +
                    '<div class="input-group-addon">克</div>' +
                '</div>' +
            '</div>'
        ).appendTo($fieldset);

        $('<div>').attr({'class': "alert alert-info"}).html(
            opts.node_help.field_product_stock_qty.description
        ).appendTo($fieldset);
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="product-stock-' + opts.nid + '" class="col-lg-2 control-label">商品库存</label>' +
            '<div class="col-lg-10">' +
                '<div class="input-group">' +
                    '<input type="number" class="form-control" id="product-stock" placeholder="输入商品库存">' +
                    '<div class="input-group-addon">件</div>' +
                '</div>' +
            '</div>'
        ).appendTo($fieldset);

        $('<div>').attr({'class': "alert alert-info"}).html(
            opts.node_help.field_product_pictures_big.description
        ).appendTo($fieldset);
        $form_group = $('<div>').attr({'class': 'form-group pics-form'}).html(
            '<label for="input-picture" class="col-lg-2 control-label">商品图片</label>' +
            '<div class="col-lg-10">' +
                '<a href="javascript:void(0)" class="btn btn-primary btn-sm input-picture" data-id="' + opts.nid + '">' +
                    '<i class="zmdi zmdi-collection-item-3 zmdi-hc-lg zmdi-hc-fw"></i>' +
                    '添加商品图片</a>' +
            '</div>'
        ).appendTo($fieldset);

        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<label for="input-desc-' + opts.nid + '" class="col-lg-2 control-label">商品详情</label>' +
            '<div class="col-lg-10">' +
                '<textarea class="form-control" rows="5" id="input-desc" placeholder="商品文字介绍"></textarea>' +
            '</div>'
        ).appendTo($fieldset);
        $('#input-desc').summernote({
            height: 300,
            lang: 'zh-CN',
            toolbar: [
                // [groupName, [list of button]]
                ['style', ['style']],
                //['style', ['bold', 'italic', 'underline', 'clear']],
                ['font', ['strikethrough', 'underline', 'clear']],
                ['fontsize', ['fontsize']],
                ['para', ['ul', 'ol', 'paragraph']],
            ]
        });


        $(document).on('click', '.input-picture', function(event) {
            event.preventDefault();
            $('.form-group.pics-form').input_pics_button(3);
        });

        //submit register page
        $form_group = $('<div>').attr({'class': 'form-group'}).html(
            '<div class="col-lg-12 text-center">' +
                '<button type="submit" class="btn btn-primary submit-register-form">提交商品信息</button>' +
            '</div>'
        ).appendTo($fieldset);
        $(document).on('click', '.submit-register-form', { plugin : plugin }, function(event) {

            event.preventDefault();

            plugin = event.data.plugin;
            opts = plugin.options;
            
            if (! $('.' + opts.self_form_name).form_validation(plugin)) {
                return ;
            }

            $(this).prop("disabled", true); //disable button before send data to server
            $('.' + opts.self_form_name).submit_register_form(plugin, '.submit-register-form');

        });

        return this;

    };  //$.fn.upload_new_merch_form

    //form validation
    $.fn.form_validation = function(plugin) {

        opts = plugin.options;

        //validate data
        $input = $('#select-shop-term');
        if (! opts.apply_form.hasOwnProperty('shop_term')) {
            alert('请选择店铺');
            $input.focus();
            return false;
        }

        if (! opts.apply_form.hasOwnProperty('first_tid')) {
            alert('请选择或输入商品一级分类');
            $('#first-category').focus();
            return;
        }

        $input = $('#input-name-' + opts.nid);
        if (! $input.val().length) {
            alert('请输入商品名称');
            $input.focus();
            return false;
        } else {
            opts.apply_form.merch_name = $input.val();
        }

        $input_price1 = $('#product-price');
        if ($input_price1.val().length) {
            if (isNaN($input_price1.val())) {
                alert('请输入商品原价金额数字');
                $input_price1.focus();
                return;
            }
            opts.apply_form.product_po = $input_price1.val();
        } else {
            alert('请输入商品原价');
            $input_price1.focus();
            return;
        }

        $input_price2 = $('#discount-price');
        if ($input_price2.val().length) {
            if (isNaN($input_price2.val())) {
                alert('请输入商品促销价金额数字');
                $input_price2.focus();
                return;
            }
            opts.apply_form.product_pd = $input_price2.val();
        } 

        $input = $('#product-weight');
        if ($input.val().length) {
            opts.apply_form.product_w = $input.val();
        }

        $input = $('#product-stock');
        if ($input.val().length) {
            opts.apply_form.product_stock = $input.val();
        }

        $input = $('#input-desc');
        if ($input.summernote('code').length) {
            opts.apply_form.product_desc = $input.summernote('code');
        }

        img_num = $('.form-group.pics-form').find('.row.pic-grid .pic-item img');
        if (! img_num.length) {
            $('.input-picture').focus();
            alert('请添加商品照片');
            return false;
        } else {
            opts.apply_form.pics_form = {};
            opts.apply_form.pics_form.localids = [];
            img_num.each(function() {
                opts.apply_form.pics_form.localids.push($(this).attr('src'));
            });
        }

        return true;

    };

    $.fn.submit_register_form = function(plugin, buttonClass) {
        var opts = plugin.options;

        //notify url for apply to super admin
        //opts.apply_form.notify_url = opts.notify_url;

        $(buttonClass).prop("disabled", true); //disable button before send data to server

        //show pregressbar modal
        $('#upload-progressbar').modal({
            backdrop: 'static',
            show: true
        });
        $('#form-upload-progress').css({'width': "0px"});
        $('.progress-completed').html("0%");

        //upload shop license pics and get media ids
        var pics_form = upload_deferred('pics-form', opts.apply_form.pics_form.localids);
        $.when(pics_form).done(function(var1) {
            //console.log(var1);
            if (var1.length){
                opts.apply_form.shop_pics = var1;
            }
            width = $('#form-upload-progress').css({'width': "100%"});
            $('.progress-completed').html("100%");
            setTimeout(function(){
                $('#upload-progressbar').modal('hide');
            }, 500);

            //call ajax send form to server
            var json_data = {
                type: "add_new_merch_to_shop",
                xyz: opts.xyz,
                sk_xyz: opts.sk_xyz,
                apply_form: opts.apply_form
            };
            var form_submit_finish = function(data, that) {
                //clear submit button, move to top and aler info waiting audit
                //window.location.href = opts.manage_url;
                clear_submit_button(that, buttonClass);
            };
            $.dld_ajax_submit("POST", json_data, plugin, form_submit_finish, 'form_submit_finish');

        }).fail(function(var1) {
            alert(var1);
            console.log(var1);
        });

    };

    //call wechat to select pictures for each input pics button
    $.fn.input_pics_button = function(num) {

        var render_local_pics = function(localids, that) {

            //alert(JSON.stringify(localids));
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

    };

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
                    var upload_image_callback = function(data) {
                        //alert(JSON.stringify(data));
                        if (data.message == "ok" && data.count == 1) {

                            var serverId = data.serverId; // 返回图片的服务器端ID
                            media_ids.push(serverId);

                            if(localIds.length > 0){
                                syncUpload(localIds);
                            } else {
                                //get all meida ids and return value
                                dtd.resolve(media_ids);
                            }
                        }
                    };
                    $.dld_uploadImage(localId, upload_image_callback);

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
 
    /*
     * clear submit button for create and modified form
     */
    function clear_submit_button(plugin, button_type) {
        var opts = plugin.options;
        //resume submit button
        $(button_type).prop("disabled", false); //disable button before send data to server

        //form clear
        $('#input-name-' + opts.nid).val('');
        $('#product-price').val('');
        $('#discount-price').val('');
        $('#product-weight').val('');
        $('#input-desc').summernote('reset');

        //clear first level or second level term and refrest whole shop term
        if (opts.apply_form.first_tid == -1 || opts.apply_form.second_tid == -1) {
            //refresh shop sub term
            json_data = {
                type: "get_my_shop_sub_term",
                vid: opts.apply_form.shop_term.vid,
                pid: opts.apply_form.shop_term.tid,
                xyz: opts.xyz
            };

            $.dld_ajax_submit("GET", json_data, opts, render_shop_first_category, 'get_my_shop_sub_term');
        }

        $('.form-group.pics-form > .row.pic-grid').each(function() {
            $(this).remove();
        });
        
        //move to top
        $('html, body').animate({
            scrollTop: $('body').offset().top
        }, 500);
    }

    /*
     * render my shop sub term select
     */
    function render_shop_first_category(data, opts) {
        //console.log(data);

        var $fs_id = opts.first_selectize;
        var $ss_id = opts.second_selectize;

        if (data.count) {

            opts.shop_sub_category = data; //save sub category
            $fs_id.clearOptions();
            $ss_id.clearOptions();

            $.each(data, function(i, term) {
                if (i != 'count') {
                    $fs_id.addOption({'value': i, 'text': term[0]});
                }
            });

        }
    }

}(jQuery, window, document));


