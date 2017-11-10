/*
 * JavaScript for wechat api test api clear textarea when unchecked checkbox.
 */
(function($) {

  // Re-enable form elements that are disabled for non-ajax situations.
  Drupal.behaviors.wechat_api = {
    attach: function(context, settings) {

      //clear .test-result textarea value
      $(".test-checkbox").change(function() {
          if(!this.checked) {
            $(".test-result-text").attr("value", '');
          }
      });

      //test link button
      $("#edit-css-button").click(function() {
        alert("hello");
      });

/************************ End of attach function **************************/
    } //attach function
  };
})(jQuery);
