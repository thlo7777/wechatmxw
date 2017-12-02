/*
 * load ampa
 * for hotel gps location and user handler map
 */
;(function($, window, document, undefined) {
    // Create the defaults once
    var pluginName = 'hotel_map',
        defaults = {
            nid: 0,
            address: {},
            height: "400px"
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
        this.options.amapID = "map_container";
        $map_container = $('<div>').attr({'id': "map_container"}).insertBefore(this.element);
        $map_container.height(this.options.height);
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

    $.fn.load_map_init = function(gps_loc) {

        plugin = this.data('plugin_' + pluginName);
        opts = plugin.options;

        loding_icon1 = this.loading_icon1();
        loding_icon1.loading_icon1_on();

        get_gpslocation_callback = function (address, target_id, dragType) {

            AMapUI.loadUI(['misc/PositionPicker'], function(PositionPicker) {
                map = new AMap.Map(target_id, {
                    zoom: 16,
                    center:[address.longitude, address.latitude]
                });
                if (dragType == 'dragMarker') {
                    var positionPicker = new PositionPicker({
                        mode:'dragMarker',//设定为拖拽地图模式，可选'dragMap'、'dragMarker'，默认为'dragMap'
                        map:map//依赖地图对象
                    });
                } else {
                    var positionPicker = new PositionPicker({
                        mode:'dragMap',//设定为拖拽地图模式，可选'dragMap'、'dragMarker'，默认为'dragMap'
                        map:map//依赖地图对象
                    });
                }
                //TODO:事件绑定、结果处理等
                //

                //add toolBar plugin
                map.plugin(["AMap.ToolBar"],function(){
                    //加载工具条
                    var tool = new AMap.ToolBar({
                        //position: 'LT'
                        offset: new AMap.Pixel(10,100)
                    });
                    map.addControl(tool);
                });

                positionPicker.on('success', function(positionResult) {
                    //console.log(positionResult);

                    var province = positionResult.regeocode.addressComponent.province;
                    var city = positionResult.regeocode.addressComponent.city;
                    var district = positionResult.regeocode.addressComponent.district;

                    if (city == "") {
                        city = province;
                    }

                    address = positionResult.address.slice(province.length);
                    $(opts.addressID).val( address );
                    gps_loc.latitude = positionResult.position.getLat();
                    gps_loc.longitude = positionResult.position.getLng();

                    //console.log(opts);
                });

                //start dragmap mode
                positionPicker.start();

            });

            loding_icon1.loading_icon1_off();
        };

        if (gps_loc.value === undefined) {
            this.amap_getaddress_from_wechat_geo(opts.amapID, get_gpslocation_callback, 'get_gpslocation_callback');
        } else {
            get_gpslocation_callback({latitude: gps_loc.value.lat, longitude: gps_loc.value.lon}, opts.amapID, 'dragMarker');
        }

        return this;
    }

    $.fn.front_map = function(gps_loc) {
    }

}(jQuery, window, document));

