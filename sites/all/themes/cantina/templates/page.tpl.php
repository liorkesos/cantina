<!doctype html>

<!-- paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/ -->
<!--[if lt IE 7 ]> <html lang="he" class="no-js ie ie6 lte7 lte8 lte9"> <![endif]-->
<!--[if IE 7 ]>    <html lang="he" class="no-js ie ie7 lte7 lte8 lte9"> <![endif]-->
<!--[if IE 8 ]>    <html lang="he" class="no-js ie ie8 lte8 lte9"> <![endif]-->
<!--[if IE 9 ]>    <html lang="he" class="no-js ie ie9 lte9"> <![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--> <html lang="he" class="no-js"> <!--<![endif]-->
<head>
  <meta charset="utf-8">

  <!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame
       Remove this if you use the .htaccess -->
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">

  <title>The Cantina</title>
  <meta name="description" content="">
  <meta name="author" content="">

  <!--  Mobile viewport optimized: j.mp/bplateviewport -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <!-- CSS : implied media="all" -->
  <link rel="stylesheet" href="css/style.css?v=2">
  <link rel="stylesheet" href="css/grid-rtl.css?v=2">

  <!-- All JavaScript at the bottom, except for Modernizr which enables HTML5 elements & feature detects -->
  <script src="js/libs/modernizr-1.6.min.js"></script>

</head>

<body dir="rtl">

  <div id="container">

    <header class="region clearfix">
        <div class="container_12">
            <nav class="block grid_10 alpha">
             <?php print theme('links__system_main_menu', array(
                  'links' => $main_menu,
                  'attributes' => array(
                    'id' => 'main-menu-links',
                    'class' => array('links', 'clearfix'),
                  ),
                  'heading' => array(
                    'text' => t('Main menu'),
                    'level' => 'h2',
                    'class' => array('element-invisible'),
                  ),
                )); ?>
            </nav>
            <div id="login" class="block grid_2 omega">
                <a href="#">כניסה</a>
            </div>
        </div>
    </header>

    <div id="main" class="clearfix">

        <section id="content-intro" class="region clearfix">
            <div class="container_12">
                <div class="block grid_7 alpha">
                    <p><?php
                        $node = node_load(2);
                        print $node->body['he'][0]['value']; ?></p>
                    <h2><?php print $site_slogan; ?></h2>
                </div>
                <div class="block grid_5 omega">
                    <div id="logo-video"><a href="/node" class="ir">לוגו/וידאו</a></div>
                </div>
            </div>
        </section>
    <?php if ($page['featured']): ?>
         <section id="content-crousel" class="region clearfix">
               <div class="container_12">
                 <div id="featured">

                      <?php print render($page['featured']); ?>

                  </div> <!-- /.section, /#featured -->
              </div>
         </section>
     <?php endif; ?>
         <div class="container_12">
                <section id="content-main" class="region grid_7 alpha">

                <?php if ($messages): ?>
                    <div id="messages">
                      <?php print $messages; ?>
                    </div> <!-- /.section, /#messages -->
                <?php endif; ?>
                <div class="node">

                      <?php if ($page['highlighted']): ?>
                          <div id="highlighted">
                                <?php print render($page['highlighted']); ?>
                          </div>
                      <?php endif; ?>
                      <a id="main-content"></a>

                      <?php print render($title_prefix); ?>

                      <?php if ($title): ?>
                        <h1 class="title" id="page-title">
                          <?php print $title; ?>
                        </h1>
                      <?php endif; ?>

                      <?php print render($title_suffix); ?>

                      <?php if ($tabs): ?>
                        <div class="tabs">
                          <?php print render($tabs); ?>
                        </div>
                      <?php endif; ?>

                      <?php print render($page['help']); ?>

                      <?php if ($action_links): ?>
                        <ul class="action-links">
                          <?php print render($action_links); ?>
                        </ul>
                      <?php endif; ?>

                      <?php print render($page['content']); ?>
                      <?php print $feed_icons; ?>

                </div>
            </section>

            <?php //if (drupal_is_front_page()): ?>
            <section id="content-social" class="region grid_5 omega">
                <div class="block">
                 <?php if ($page['sidebar_first']): ?>
                      <div id="sidebar-first" class="column sidebar"><div class="section">
                        <?php print render($page['sidebar_first']); ?>
                      </div></div> <!-- /.section, /#sidebar-first -->
                 <?php endif; ?>
                </div>
            </section>
        </div>

    </div>

    <footer class="clearfix region">
        <div class="container_12">

            <div class="block grid_7 alpha">
                ניווט תחתון וקופי
            </div>
            <div class="block grid_5 omega">
                לוגואים
            </div>
        </div>
    </footer>
  </div> <!--! end of #container -->


  <!-- Javascript at the bottom for fast page loading -->
  <script src="js/libs/jquery-1.4.3.min.js"></script>
  <script src="js/script.js"></script>

  <!--[if lt IE 7 ]>
    <script src="js/libs/dd_belatedpng.js"></script>
    <script> DD_belatedPNG.fix('img, .png_bg'); //fix any <img> or .png_bg background-images </script>
  <![endif]-->

  <!-- asynchronous google analytics: mathiasbynens.be/notes/async-analytics-snippet
       change the UA-XXXXX-X to be your site's ID -->
  <script>
  /*
   var _gaq = [['_setAccount', 'UA-XXXXX-X'], ['_trackPageview']];
   (function(d, t) {
    var g = d.createElement(t),
        s = d.getElementsByTagName(t)[0];
    g.async = true;
    g.src = ('https:' == location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    s.parentNode.insertBefore(g, s);
   })(document, 'script');
  */
  </script>

</body>
</html>
