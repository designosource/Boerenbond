<?php

/**
 * @file
 * Field handler that provides a form for changing states.
 */

class state_flow_entity_handler_field_events_form extends state_flow_entity_handler_field_state_flow_states {

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {

    // Make sure these options are set
    if (!empty($this->aliases['active']) && !empty($this->options['active_revision_handling'])  ) {
    $active_revision_handling = $this->options['active_revision_handling'];
    $active_alias = $this->aliases['active'];

    $entity_type = $values->state_flow_states_entity_type;
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $revision_key = $entity_info['entity keys']['revision'];

    // If 'active' matters, then check the active column if necessary.
    if ($active_revision_handling === 'both' ||
    ($active_revision_handling === 'active' && !empty($values->{$active_alias})) ||
    ($active_revision_handling === 'inactive' && empty($values->{$active_alias}))) {

      // Load the relevant entity. Probably a node.
      $entities = entity_load($entity_type, FALSE, array($revision_key => $values->state_flow_states_revision_id));
      $entity = array_pop($entities);

      $form_options = array(
        'event_element_type' => 'select',
        'assemble_page_title' => FALSE,
      );

      module_load_include('inc', 'state_flow_entity', 'state_flow_entity.forms');
      return drupal_get_form('state_flow_entity_events_revision', $entity, $entity_type, NULL, $form_options);
    }
    }
  }
}
