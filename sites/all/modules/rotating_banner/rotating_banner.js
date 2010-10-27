// $Id: rotating_banner.js,v 1.10 2010/10/20 17:02:16 jameselliott Exp $

/*jslint bitwise: true, eqeqeq: true, immed: true, newcap: true, nomen: false,
 onevar: false, plusplus: false, regexp: true, undef: true, white: true, indent: 2
 browser: true */

/*global jQuery: true Drupal: true window: true */

(function ($) {
  Drupal.behaviors.rotatingBanner = {
    attach: function (context) {
      $('.rotating-banner', context).once('jCycleActivated', function () {
        var settings = Drupal.settings.rotatingBanners[this.id].cycle;
        if ($.fn.cycle === 'undefined') {
          alert(Drupal.t('Jquery Cycle is not installed.  See the README.txt'));
          return;
        }
        // This should probably be moved into a change to the form in rotating_banner.module
        settings.pager = "#" + this.id + " .controls";
        settings.fit = 1;
        settings.cleartypeNoBg = true;
				
        $('.rb-slides', this).cycle(settings);
      });
    }
  };
})(jQuery);
