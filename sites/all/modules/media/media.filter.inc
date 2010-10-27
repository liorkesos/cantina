<?php
// $Id: media.filter.inc,v 1.11 2010/08/05 16:12:41 JacobSingh Exp $

/**
 * @file
 * Functions related to the WYSIWYG editor and the media input filter.
 *
 * @TODO: Rename this file?
 */

/**
 * Implementation of hook_wysiwyg_include_directory()
 */
function media_wysiwyg_include_directory($type) {
  switch($type) {
    case 'plugins':
      return 'wysiwyg_plugins';
        break;
    }
}

/**
 * Filter callback for media markup filter.
 *
 * @TODO check for security probably pass text through filter_xss
 * @return unknown_type
 */
function media_filter($text) {
  $text = ' ' . $text . ' ';
  $text = preg_replace_callback("/\[\[.*?]]/s",'_media_markup', $text);

  return $text;
}

/**
 *  Filter callback for media url filter.
 *  @TODO: There are currently problems with this. For instance, if a file is
 *  to be loaded from a remote location here, it will be recreated multiple
 *  times, each time this filter is called. If we want to continue supporting
 *  this feature, we would need to probably create a new stream or other way
 *  to lookup a remote file w/ its local version. Probably best as a contributed
 *  module because of this difficulty. ~ aaron.
 */
function media_url_filter($text, $filter) {
  $text = ' '. $text .' ';

  // Need to attach the variables to the callback after the regex.
  $callback = _media_url_curry('_media_url_parse_full_links', 1);

  // Match absolute URLs.
  $text = preg_replace_callback("`(<p>|<li>|<br\s*/?>|[ \n\r\t\(])((http://|https://)([a-zA-Z0-9@:%_+*~#?&=.,/;-]*[a-zA-Z0-9@:%_+*~#&=/;-]))([.,?!]*?)(?=(</p>|</li>|<br\s*/?>|[ \n\r\t\)]))`i", $callback, $text);

  return $text;
}

/**
 * If one of our allowed providers knows what to do with the url,
 * then let it embed the video.
 *
 * @param int $filter
 *  The filter id.
 * @param array $match
 *  The matched text from our regex.
 *
 * @return string
 *  The replacement text for the url.
 */
function _media_url_parse_full_links($match) {
  // Get just the URL.
  $match[2] = check_url(decode_entities($match[2]));
  try {
    $file = media_parse_to_file($match[2]);
  }
  catch (Exception $e) {
    // Ignore errors; pass the original text for other filters to deal with.
    return $match[0];
  }

  if ($file->fid) {
    // Return the embedded media.
    // Now load the desired media to display.
    $medias = entity_load('media', array($file->fid));

    if (!empty($medias)) {
      $media = array_pop($medias);

      // Generate a preview of the file
      // @TODO: Allow user to change the formatter in the filter settings.
      $preview = field_view_field('media', $media, 'file', 'media_large');
      $preview['#show_names'] = TRUE;

      return drupal_render($preview);
    }
  }

  // Nothing was parsed; return the original text.
  return $match[0];
}

function _media_url_curry($func, $arity) {
  return create_function('', "
    \$args = func_get_args();
    if(count(\$args) >= $arity)
        return call_user_func_array('$func', \$args);
    \$args = var_export(\$args, 1);
    return create_function('','
        \$a = func_get_args();
        \$z = ' . \$args . ';
        \$a = array_merge(\$z,\$a);
        return call_user_func_array(\'$func\', \$a);
    ');
  ");
}

/**
 * Replace callback to convert tag into markup
 * @param string $match
 * Takes a match of tag code
 * @param boolean $wysiwyg
 *   Set to TRUE if called from within the WYSIWYG text area editor.
 * @return
 * Return the replaced markup
 */
function _media_markup($match, $wysiwyg = FALSE) {
  $match = str_replace("[[","",$match);
  $match = str_replace("]]","",$match);
  $tag = $match[0];
  
  try {
    if (!is_string($tag)) {
      throw new Exception('Unable to find matching tag');
    }
    
    $media = drupal_json_decode($tag);
  
    if (!isset($media['fid'])) {
      throw new Exception('No file Id');
    }
    if (!isset($media['view_mode'])) {
      // Should we log or throw an exception here instead?
      // Do we need to validate the view mode for fields API?
      $media['view_mode'] = media_variable_get('wysiwyg_default_view_mode');
    }
    $media_obj = media_load($media['fid']);
    if (!$media_obj) {
     throw new Exception('Could not load media object');
    }
    $settings = is_array($media['attributes']) ? $media['attributes'] : array();
    $attribute_whitelist = media_variable_get('wysiwyg_allowed_attributes');
    $settings = array_intersect_key($settings, array_flip($attribute_whitelist));
    
    // @TODO: What case does this provide for?  Can we add this logic in JS when we embed it?
    // This doesn't look great to me.  Also won't work if the style has anything
    // between width and height (or if they are in reverse order).
    if (isset($settings['style'])) {
      if (preg_match('@width: (.+?)px; height: (.+?)px;@i', $settings['style'], $matches)) {
        $settings['width'] = $matches[1];
        $settings['height'] = $matches[2];
      }
    }
    if ($wysiwyg) {
      $settings['wysiwyg'] = $wysiwyg;
    }
  }
  catch (Exception $e) {
    watchdog('media', 'Unable to render media from %tag. Error: %error', array('%tag' => $tag, '%error' => $e->getMessage()));
    return '';
  }

  $file_field = media_get_file_without_label($media_obj, $media['view_mode'], $settings);
  return drupal_render($file_field);
}

/**
 * Process a text format widget to load and attach editors.
 *
 * The element's #id is used as reference to attach client-side editors.
 */
function media_pre_render_text_format($element) {
  // filter_process_format() copies properties to the expanded 'value' child
  // element.
  if (!isset($element['format'])) {
    return $element;
  }

  $format_field = &$element['format'];
  $field = &$element['value'];
  $settings = array(
    'field' => $field['#id'],
  );

  $tagmap = _media_generate_tagMap($field['#value']);

  if (isset($tagmap)) {
    drupal_add_js(array('tagmap' => array_unique($tagmap)), 'setting');
  }
  return $element;
}

/**
 * Generates an array of [inline tags] => <html> to be used in filter
 * replacement and to add the mapping to JS.
 * @param
 * The String containing text and html markup of textarea
 * @return
 * An associative array with tag code as key and html markup as the value.
 *
 * @see
 * media_process_form
 * _media_markup
 */
function _media_generate_tagMap($text) {
  // Making $tagmap static as this function is called many times and
  // adds duplicate markup for each tag code in Drupal.settings JS,
  // so in media_process_form it adds something like tagCode:<markup>,
  // <markup> and when we replace in attach see two duplicate images
  // for one tagCode. Making static would make function remember value
  // between function calls. Since media_process_form is multiple times
  // with same form, this function is also called multiple times.
  static $tagmap = array();
  preg_match_all("/\[\[.*?]]/s", $text, $matches, PREG_SET_ORDER);
  foreach($matches as $match) {
    // We see if tagContent is already in $tagMap, if not we add it
    // to $tagmap otherwise return a empty error
    if(empty($tagmap[$match[0]])) {
      // @TODO: Total HACK, but better than nothing.
      // We should find a better way of cleaning this up.
      if ($markup_for_media = _media_markup($match, TRUE)) {
        $tagmap[$match[0]] = $markup_for_media;
      } else {
        $tagmap[$match[0]] = '<div><img src="/broken.jpg" height="50px" width="100px"/></div>';
      }
    }
    else {
      return array();
    }
  }
  return $tagmap;
}

/**
 * Used when embedding media.  Allows the user to pick a format for their media file.
 *
 * Can also have additional params depending on the media type.
 *
 * @param unknown_type $form
 * @param unknown_type $media
 * @return unknown_type
 */
function media_format_form($form, $form_state, $media) {
  // This will vary depending on the media type.
  $form = array();
  $form['#media'] = $media;

  $instance_info = field_info_instance('media', 'file', $media->type);
  $entity_info = entity_get_info('media');
  $view_modes = $entity_info['view modes'];

  $options = array();
  
  drupal_alter('media_wysiwyg_allowed_view_modes', $media_type, $view_modes);

  foreach ($view_modes as $key => $title) {
    $format = $instance_info['display'][$key];
    //If the format is set to hidden, don't offer it
    if ($format['type'] == 'hidden') {
      continue;
    }

    //@TODO: Display more verbose information about which formatter and what it does.
    $options[$key] = $title['label'];
    $file_field = media_get_file_without_label($media, $key, array('wysiwyg' => TRUE));

    /**
     * Currently not used in UI.
     * Will provide more information in style selection from file_styles.
     */
    if (preg_match('/styles_file_(.*)/', $format['type'], $matches)) {
      $style_name = $matches[1];
      $style_info = styles_containers_available_styles('file', $media->type, $style_name);
    }
    
    // Make a pretty name out of this.
    $formats[$key] = drupal_render($file_field);
  }

  if (!count($formats)) {
    throw new Exception('Unable to continue, no available formats for displaying media.');
    return;
  }

  $default_view_mode = media_variable_get('wysiwyg_default_view_mode');
  if (!isset($formats[$default_view_mode])) {
    $default_view_mode = key($formats);
  }
  

  // Add JS and settings array of formats.
  $settings = array();
  $settings['media'] = array('formatFormFormats' => $formats);
  drupal_add_js($settings, 'setting');

  drupal_add_library('media', 'media_base');
  drupal_add_library('system', 'form');

  $path = drupal_get_path('module', 'media');
  $form['#attached']['js'][] = $path . '/javascript/media-format-form.js';
  $form['#attached']['css'][] = $path . '/css/media-format-form.css';

  $form['heading'] = array(
    '#type' => 'markup',
    '#prefix' => '<h1 class="title">',
    '#suffix' => '</h1>',
    '#markup' => t('Embedding %filename', array('%filename' => $media->filename)),
  );
  
  $preview = media_get_thumbnail_preview($media);

  $form['preview'] = array(
    '#type' => 'markup',
    '#title' => basename($media->uri),
    '#markup' => drupal_render($preview),
  );

  // These will get passed on to WYSIWYG
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('options'),
  );

  $form['options']['format'] = array(
    '#type' => 'select',
    '#title' => t('Current format is'),
    '#options' => $options,
    '#default_value' => $default_view_mode
  );

  // Similar to a form_alter, but we want this to run first so that media.types.inc
  // can add the fields specific to a given type (like alt tags on media).
  // If implemented as an alter, this might not happen, making other alters not
  // be able to work on those fields.
  // @TODO: We need to pass in existing values for those attributes.
  drupal_alter('media_format_form_prepare', $form, $form_state, $media);

  if (!element_children($form['options'])) {
    $form['options']['#attributes'] = array('style' => 'display:none');
  }

  return $form;
}



/**
 * Wrapper around field_view_field, returns the file field w/o a label
 *
 * @param Object $media
 * @param string $view_mode
 * @param array $settings
 *  Any attribute overrides to pass to the style formatter.
 * @return array
 *  drupal_renderable array
 */
function media_get_file_without_label($media, $view_mode, $settings = array()) {
  $instance = field_info_instance('media', 'file', $media->type);
  $format = $instance['display'][$view_mode];
  $format['label'] = 'hidden';
  $format['settings'] = array_merge($format['settings'], $settings);
  $media->override = $settings;
  return field_view_field('media', $media, 'file', $format);
}
