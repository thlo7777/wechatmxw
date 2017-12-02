/*
 * hotel footer plugin
 */
;(function($, window, document, undefined) {
    // Create the defaults once
    var pluginName = 'hotel_footer',
        defaults = {
            xyz: '',
            buttons: {}
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
        // insert all this.options.buttons to page
        insert_footer(this.element, this.options.buttons);
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


    function insert_footer($pEl, element) {

        $footer = $('<footer>').attr({'class': "hotel-footer navbar-fixed-bottom"}).insertAfter($pEl);
        $cf = $('<div>').attr({'class': "container-fluid"}).appendTo($footer);
        $row = $('<div>').attr({'class': "row"}).appendTo($cf);

        col_num = 'col-xs-' + 12/element.length;

        for (var i = 0; i < element.length; i++) {
            element[i]
            $col = $('<div>').attr({'class': col_num + " text-center"}).appendTo($row);
            $a = $('<a>').attr({
                'class': "main-menu",
                'href': element[i].url
                }).html(
                '<i class="' + element[i].icon_class + '"></i>' + element[i].label
            ).appendTo($col);
        };

    }


}(jQuery, window, document));

