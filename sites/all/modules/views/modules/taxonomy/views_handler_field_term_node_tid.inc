<?php
// $Id: views_handler_field_term_node_tid.inc,v 1.4.4.5 2010/10/17 10:42:44 dereine Exp $

/**
 * Field handler for terms.
 */
class views_handler_field_term_node_tid extends views_handler_field_prerender_list {
  function init(&$view, &$options) {
    parent::init($view, $options);
    if ($view->base_table == 'node_revisions') {
      $this->additional_fields['vid'] = array('table' => 'node_revisions', 'field' => 'vid');
    }
    else {
      $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
    }

    // Convert legacy vids option to machine name vocabularies.
    if (!empty($this->options['vids'])) {
      $vocabularies = taxonomy_get_vocabularies();
      foreach ($this->options['vids'] as $vid) {
        if (isset($vocabularies[$vid], $vocabularies[$vid]->machine_name)) {
          $this->options['vocabularies'][$vocabularies[$vid]->machine_name] = $vocabularies[$vid]->machine_name;
        }
      }
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vocabularies'] = array('default' => array());

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit terms by vocabulary'),
      '#default_value'=> $this->options['limit'],
    );

    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $options[$voc->machine_name] = check_plain($voc->name);
    }

    $form['vocabularies'] = array(
      '#prefix' => '<div><div id="edit-options-vocabularies">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vocabularies'],
      '#process' => array('form_process_checkboxes'),
      '#states' => array(
        'visible' => array(
          ':input[name="options[limit]"]' => array('checked' => TRUE),
        ),
      ),
   );
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $query = db_select('taxonomy_term_data', 'td');
      $query->innerJoin('taxonomy_index', 'tn', 'td.tid = tn.tid');
      $query->innerJoin('taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
      $query->fields('td');
      $query->addField('tn', 'nid', 'node_vid');
      $query->addField('tv', 'name', 'vocabulary');
      $query->addField('tv', 'machine_name', 'vocabulary_machine_name');
      $query->orderby('td.weight');
      $query->orderby('td.name');
      $query->condition('tn.nid', $vids);
      $query->addTag('term_access');
      if (!empty($this->options['limit']) && !empty($this->options['vocabularies'])) {
        $query->condition('tv.machine_name', $this->options['vocabularies']);
      }
      $result = $query->execute();

      foreach ($result as $term) {
        $this->items[$term->node_vid][$term->tid]['name'] = check_plain($term->name);
        $this->items[$term->node_vid][$term->tid]['tid'] = $term->tid;
        $this->items[$term->node_vid][$term->tid]['vocabulary_machine_name'] = check_plain($term->vocabulary_machine_name);
        $this->items[$term->node_vid][$term->tid]['vocabulary'] = check_plain($term->vocabulary);

        if (!empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid]['make_link'] = TRUE;
          $this->items[$term->node_vid][$term->tid]['path'] = 'taxonomy/term/' . $term->tid;
        }
      }
    }
  }

  function render_item($count, $item) {
    return $item['name'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = t('The taxonomy term ID for the term.');
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The taxonomy term name for the term.');
    $tokens['[' . $this->options['id'] . '-vocabulary-machine-name' . ']'] = t('The machine name for the vocabulary the term belongs to.');
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = t('The name for the vocabulary the term belongs to.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = $item['tid'];
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['name'];
    $tokens['[' . $this->options['id'] . '-vocabulary-machine-name' . ']'] = $item['vocabulary_machine_name'];
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = $item['vocabulary'];
  }
}
