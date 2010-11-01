<?php
// $Id: html.tpl.php,v 1.4 2010/05/26 10:47:20 dries Exp $

/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 */
?><!doctype html>

<!-- paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/ -->
<!--[if lt IE 7 ]> <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" class="no-js ie ie6 lte7 lte8 lte9"> <![endif]-->
<!--[if IE 7 ]>    <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" class="no-js ie ie7 lte7 lte8 lte9"> <![endif]-->
<!--[if IE 8 ]>    <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" class="no-js ie ie8 lte8 lte9"> <![endif]-->
<!--[if IE 9 ]>    <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" class="no-js ie ie9 lte9"> <![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--> <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" class="no-js"> <!--<![endif]-->

<head>

    <?php print $head; ?>

    <!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame
         Remove this if you use the .htaccess -->
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <!--  Mobile viewport optimized: j.mp/bplateviewport -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title><?php print $head_title; ?></title>
    <?php print $styles; ?>

    <!-- All JavaScript at the bottom, except for Modernizr which enables HTML5 elements & feature detects -->
    <script src="<?php print url($directory . '/js/libs/modernizr-1.6.min.js'); ?>"></script>

</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <div class="skip-link visuallyhidden">
    <a href="#main-content"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>

  <?php print $scripts; ?>
  <!--[if lt IE 7 ]>
    <script src="<?php print url($directory . '/js/libs/dd_belatedpng.js')?>"></script>
    <script> DD_belatedPNG.fix('img, .png_bg'); //fix any <img> or .png_bg background-images </script>
  <![endif]-->

  <?php print $page_bottom; ?>
</body>

</html>
