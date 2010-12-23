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
                <a href="/user"><?php print t("login");?></a>
            </div>
        </div>
    </header>

    <div id="main" class="clearfix">
        <section id="content-inner-intro" class="region clearfix">
            <div class="container_12">
                <div class="block grid_7 alpha">
                    <h2><?php print $site_slogan; ?></h2>
                </div>
                <div class="block grid_5 omega">
                    <div id="logo-small"><a href="/node" class="ir"><?php print $site_name ?></a></div>
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
         <div id="content-main" class="clearfix">
            <div class="container_12">
                <section id="content" class="region grid_9 alpha">

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
            <section id="content-sidebar" class="region grid_3 omega">
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

    </div>

    <footer class="clearfix region">
        <div class="container_12">

            <div class="block grid_7 alpha">
              <?php print render($page['credits']); ?>
            </div>
            <div class="block grid_5 omega">
                לוגואים
            </div>
        </div>
    </footer>
  </div> <!--! end of #container -->
