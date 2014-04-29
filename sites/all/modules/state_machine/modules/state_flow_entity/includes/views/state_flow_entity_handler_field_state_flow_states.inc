<?php

/**
 * @file
 * A helper class that is extended by state_flow_entity_handler_field_link and state_flow_entity_handler_field_events_form.
 */

class state_flow_entity_handler_field_state_flow_states extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['entity_type'] = array('table' => 'state_flow_states', 'field' => 'entity_type');
    $this->additional_fields['entity_id'] = array('table' => 'state_flow_states', 'field' => 'entity_id');
    $this->additional_fields['revision_id'] = array('table' => 'state_flow_states', 'field' => 'revision_id');
    $this->additional_fields['active'] = array('table' => 'state_flow_states', 'field' => 'active');
  }

  function option_definition() {
    // @todo, do we need the parent options? A lot of the variables don't apply at all.
    $options = parent::option_definition();
    $options['active_revision_handling'] = array('default' => 'both');

    // This option has been deprecated for active_revision_handling.
    $options['show_only_for_active_revision_ids'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  function init(&$view, &$options) {
    // An older version of this handler used a boolean option show_only_for_active_revision_ids
    // which corresponds to the 'active' setting in 'active_revision_handling'
    if (!empty($options['show_only_for_active_revision_ids']) && empty($options['active_revision_handling'])) {
      $options['active_revision_handling'] = 'active';
    }
    parent::init($view, $options);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->option_definition();
    $form['active_revision_handling'] = array(
      '#type' => 'select',
      '#title' => t('Show for these revisions'),
      '#description' => t('"Active" revisions are those with the most recent state change or edit.'),
      '#options' => array(
        'active' => 'Active',
        'inactive' => 'Inactive',
        'both' => 'Both',
      ),
      '#default_value' => $this->options['active_revision_handling'],
      '#weight' => -103,
    );
  }

  public function render_state_flow_states_link($values, $link_text, $path_key) {

    $entity_type = $values->state_flow_states_entity_type;
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $revision_key = $entity_info['entity keys']['revision'];

    if (!empty($entity_info['state_flow_entity'][$path_key])) {

      $search = array(
        '%entity_id',
        '%revision_id',
      );

      $replacements = array(
        $values->state_flow_states_entity_id,
        $values->state_flow_states_revision_id,
      );

      $path = str_replace($search, $replacements, $entity_info['state_flow_entity'][$path_key]);

      return l($link_text, $path);
    }
  }
}
