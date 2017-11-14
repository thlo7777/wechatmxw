/*
 * Gif loading plugin
 */
;(function($, window, document, undefined) {
    // Create the defaults once
    var pluginName = 'loading_icon1',
        defaults = {};

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
        this.options.pluginID = '#loading-icon1';
        $loading_fig = $('<div>').attr({'class': "loader-off", 'id': "loading-icon1"}).appendTo(this.element);
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

    $.fn.loading_icon1_on = function() {

        plugin = this.data('plugin_' + pluginName);
        var opts = plugin.options;

        $(opts.pluginID).removeClass('loader-off').addClass('loader-on');
        //var loading_gif_url = window.location.origin + "/sites/default/files/pymxw/preloading_gif";
        //$loading_fig.css("background-image","url(" + loading_gif_url + ")");
        $(opts.pluginID).offset({
            top: ($(window).height() - $(opts.pluginID).height()) / 2,
            left: ($(window).width() - $(opts.pluginID).width()) / 2
        });
    };

    $.fn.loading_icon1_off = function() {
        plugin = this.data('plugin_' + pluginName);
        var opts = plugin.options;

        $(opts.pluginID).removeClass('loader-on').addClass('loader-off');
    };

}(jQuery, window, document));
