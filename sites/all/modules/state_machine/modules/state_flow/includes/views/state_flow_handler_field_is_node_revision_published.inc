<?php

/**
 * @file
 * Views field handler that checks whether this is the revision that is also in the node table
 * and that its status is 1
 */

class state_flow_handler_field_is_node_revision_published extends views_handler_field {

  function construct() {
    parent::construct();
    // @todo, these lines were commented out as part of this issue: http://drupal.org/node/1775540
    // $this->additional_fields['node_vid'] = array('table' => 'node', 'field' => 'vid');
    // $this->additional_fields['node_status'] = array('table' => 'node', 'field' => 'status');
    $this->additional_fields['nid'] = array('field' => 'nid');
  }

  function option_definition() {
    // @todo, do we need the parent options? A lot of the variables don't apply at all.
    $options = parent::option_definition();

    $options['yes_text'] = array('default' => 'Yes', 'translatable' => TRUE);
    $options['no_text'] = array('default' => 'No', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {

    $form['yes_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display if node is published'),
      '#default_value' => $this->options['yes_text'],
    );

    $form['no_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display if node not published'),
      '#default_value' => $this->options['no_text'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * get_live_vid() and get_live_status() are bad work-arounds to this
   * problem: http://drupal.org/node/1775540
   */
  function get_live_vid($nid) {

    if (!empty($nid)) {

      $live_revision = db_select('node', 'n')
        ->fields('n', array('vid'))
        ->condition('nid', $nid)
        ->execute()
        ->fetchCol();

      if (!empty($live_revision[0])) {
        return $live_revision[0];
      }
    }
    return FALSE;
  }

  function get_live_status($nid) {

    if (!empty($nid)) {
      $live_revision = db_select('node', 'n')
        ->fields('n', array('status'))
        ->condition('nid', $nid)
        ->execute()
        ->fetchCol();

      if (!empty($live_revision[0])) {
        return $live_revision[0];
      }
    }
    return FALSE;
  }

  public function render($values) {

    $nid = $this->get_value($values, 'nid');
    $vid = $this->get_value($values);

    // @todo, this should be moved to pre_render
    $live_vid = $this->get_live_vid($nid);
    $live_status = $this->get_live_status($nid);

    if (!empty($live_vid) && !empty($vid) && !empty($live_status)) {
      if ( $live_status && $live_vid == $vid) {
        return check_plain($this->options['yes_text']);
      }
    }
    return check_plain($this->options['no_text']);
  }
}
