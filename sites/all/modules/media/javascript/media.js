// $Id: media.js,v 1.5 2010/07/16 20:42:49 JacobSingh Exp $

/**
 *  @file
 *  This file handles the JS for Media Module functions.
 */

/*

The naming convetion for items in this file revolves around three different
components: tabs, subtabs and panes. Tabs are a primary navigation or grouping
mechanism. Subtabs are related to a specific tab. Panes are the content
related to the subtab.

   ---------------------------------------
   |                  |tab | tab2 | tab3 |
   |                  --------------------
   |__________ __________________________|
   | Subtab  |                           |
   -----------                           |
   | Subtab2 |        Pane for           |
   -----------      active subtab        |
   | Subtab3 |                           |
   -----------                           |
   |                                     |
   |                                     |
   |                                     |
   ---------------------------------------



*/
// $Id: media.js,v 1.5 2010/07/16 20:42:49 JacobSingh Exp $

(function ($) {

/**
 * Loads media browsers and callbacks, specifically for media as a field.
 */
Drupal.behaviors.mediaElement = {
  attach: function (context, settings) {
    // Options set from media.fields.inc for the types, etc to show in the browser.
    
    // For each widget (in case of multi-entry)
    $('.media-widget', context).once('mediaBrowserLaunch', function () {
      var options = settings.media.elements[this.id];
      globalOptions = {};
      if (options.global != undefined) {
        var globalOptions = options.global;
      }
      //options = Drupal.settings.media.fields[this.id];
      var fidField = $('.fid', this);
      var previewField = $('.preview', this);
      // When someone clicks the link to pick media (or clicks on an existing thumbnail)
      $('.launcher', this).bind('click', function () {
        // Launch the browser, providing the following callback function
        // @TODO: This should not be an anomyous function.
        Drupal.media.popups.mediaBrowser(function (mediaFiles) {
          if (mediaFiles.length < 0) {
            return;
          }
          var mediaFile = mediaFiles[0];
          // Set the value of the filefield fid (hidden).
          fidField.val(mediaFile.fid);
          // Set the preview field HTML
          previewField.html(mediaFile.preview);
        }, globalOptions);
        return false;
      });

      $('.media-edit-link', this).bind('click', function () {
        var fid = fidField.val();
        if (fid) {
          Drupal.media.popups.mediaFieldEditor(fid, function (r) { alert(r); });
        }
        return false;
      });

    });
  }
};

})(jQuery);
