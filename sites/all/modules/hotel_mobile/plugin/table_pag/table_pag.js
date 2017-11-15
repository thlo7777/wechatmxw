/*
 * customer table list
 */
;(function($, window, document, undefined) {

    // Create the defaults once
    var pluginName = 'table_list',
        defaults = {
            xyz: '',
            rest_api: '',
            edit_url: '',
            list_node_type: '',
            table_header: '',
            table_name: 'list-table-container'
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

        json_data = {
            list_node_type: this.options.list_node_type,
            type: 'all_list_count',
            xyz: this.options.xyz
        };

        render_table = function(data, that) {
            console.log(data);
            $(that.element).
                create_table_box(that.options.table_name, that.options.table_header).
                create_list_table().
                create_list_table_header().
                create_list_table_body(data, that);

            $('.col-xs-12.table-append').create_list_table_pager(
                $('.row.list-table-container .table tbody'), data, that
            );
        };

        $.wechat_ajax_submit("GET", json_data, this, render_table, 'render_table', this.options.rest_api);

        return this;

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
    }
 
    //create table container
    $.fn.create_table_box = function(table_name, table_header) {
        $box = $('<div>').attr({
            'class': 'row ' + table_name
            }).appendTo(this);

        $table_title = $('<h4>').attr({
            'class': "col-xs-12 text-center table-title"
            }).html(table_header).appendTo($box);

        $col12 = $('<div>').attr({
            'class': 'col-xs-12 text-center table-append'
            }).appendTo($box);
        return $col12;
    }

    //create table
    $.fn.create_list_table = function() {
        $table = $('<table>').attr({
            'class': "table table-striped table-hover"
            }).appendTo(this);
        return $table;
    }

    //create user list table header
    $.fn.create_list_table_header = function() {
        $thead = $('<thead>').html(
        '<tr>' +
            '<th class="col-xs-4 text-center">店铺名称</th>' +
            '<th class="col-xs-4 text-center">店铺地址</th>' +
            '<th class="col-xs-4 text-center">店铺电话</th>' +
        '</tr>'
        ).appendTo(this);
        return this;
    }

    //create user list table header
    $.fn.create_list_table_body = function(data, that) {

        $tbody = $('<tbody>').appendTo(this);

        if (data.table_list === undefined) {
            //no records
            $tbody.html(
                '<td colspan="12">' +
                    '<div class="alert alert-dismissible alert-info">' +
                        '<button type="button" class="close" data-dismiss="alert">&times;</button>' +
                        '<strong>没有记录</strong>' +
                    '</div>' +
                '</td>'
            );
            return this;
        }

        $.each(data.table_list, function(i, user) {


            if (user.cs_name === undefined && user.cs_wechat === undefined) {
                tr_str = 
                    '<td class="col-xs-4">' + user.hotel_name + '</td>' +
                    '<td class="col-xs-4">' + user.hotel_address + '</td>' +
                    '<td class="col-xs-4">' + user.hotel_phone + '</td>';
            }

            $tr = $('<tr>').attr({
                'class': 'user-data',
                'data-id': user.id
            }).html(tr_str).appendTo($tbody);
        });

        //add row click event
        $(document).on('click', 'tr.user-data', function(event) {

            var oauth2_edit_url = that.options.edit_url.replace(/%/, $(this).data('id'));
            //console.log(oauth2_edit_url);
            window.location.href = oauth2_edit_url;

        });

        return this;
    }

    //create user list table pager
    $.fn.create_list_table_pager = function($tbody, data, that) {

        $pager = $('<div>').attr({'class': 'table_pager'}).appendTo(this);

        $('.table_pager').bootpag({
            total: data.table.pages,
            page_numbers: data.table.page_number,
            page: 1,
            maxVisible: 3,
            leaps: true,
            firstLastUse: true,
            first: '第一页',
            last: '最后页',
            next: '>',
            prev: '<',
            wrapClass: 'pagination',
            activeClass: 'active',
            disabledClass: 'disabled',
            nextClass: 'next',
            prevClass: 'prev',
            lastClass: 'last',
            firstClass: 'first'
        }).on("page", function(event, num){
            var settings = $(this).data('settings');
            $(this).get_list_pager($tbody, num, settings, that);
        }); 
    }


    $.fn.get_list_pager = function($tbody, page, settings, that) {
        var min_num = settings.page_numbers * (page - 1);
        //call ajax to fetch user account list
        json_data = {
            xyz: that.options.xyz,
            type: "get_list_pager",
            list_node_type: that.options.list_node_type,
            page_num: settings.page_numbers,
            min_num: min_num
        };

        pager_refresh_table_body = function(data, target) {
            $tbody.empty();

            if (data.table_list !== undefined) {

                console.log(data);
                $.each(data.table_list, function(i, user) {

                    if (user.cs_name === undefined && user.cs_wechat === undefined) {
                        tr_str = 
                            '<td class="col-xs-4">' + user.hotel_name + '</td>' +
                            '<td class="col-xs-4">' + user.hotel_address + '</td>' +
                            '<td class="col-xs-4">' + user.hotel_phone + '</td>';
                    } 
                    $tr = $('<tr>').attr({
                        'class': 'user-data',
                        'data-id': user.id
                    }).html(tr_str).appendTo($tbody);
                });
            }
        };

        $.wechat_ajax_submit(
            "POST",
            json_data,
            $tbody,
            pager_refresh_table_body,
            'pager_refresh_table_body',
            that.options.rest_api
        );
    }
 
}(jQuery, window, document));


