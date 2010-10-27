// $Id: media-add_from_url.js,v 1.4 2010/08/30 19:06:43 jameselliott Exp $

/**
 * @file
 * This creates an AJAX preview for the 'from_url' browser form.
 */

(function ($) {

/**
 * Change the preview on textfield change.
 */
Drupal.behaviors.mediaAddFromURLPreview = {
  attach: function (context, settings) {
    $('.media-add-from-url', context).once('mediaAddFromURLPreview', function () {
      /*  If you paste into the textbox and then click submit directly, the AJAX GET
       *  returns a 404.  As I don't think this is doing anything currently, I've
       *  commented it out.
      $(this).bind('change', function () {
        $preset = $(this);
        if ($preset.val()) {
          $.getJSON(Drupal.settings.media.add_from_url_preview + '?url=' + $preset.val(), function (data) {
            // @todo: Perform better error checking.
            if (data && data.preview) {
              $('#media-add-from-url-preview').html(data.preview);
            }
          });
        }
      });
      */
    });
  }
};

})(jQuery);
