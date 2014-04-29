<?php

/**
 * @file
 * Prints available state change events for a given entity.
 */

// @todo, redo this in the style of events_form

class state_flow_entity_handler_field_available_events extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['entity_type'] = array('table' => 'state_flow_states', 'field' => 'entity_type');
    $this->additional_fields['entity_id'] = array('table' => 'state_flow_states', 'field' => 'entity_id');
    $this->additional_fields['revision_id'] = array('table' => 'state_flow_states', 'field' => 'revision_id');
    $this->additional_fields['active'] = array('table' => 'state_flow_states', 'field' => 'active');
  }

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {

    // @todo, replace all the hard-coded node assumptions.
    $entities = entity_load('node', FALSE, array('vid' => $this->get_value($values, 'revision_id')));
    $entity = array_pop($entities);
    $machine = state_flow_entity_load_state_machine($entity, 'node');

    // @todo, Eventually, we should just call this method.
    // return $state_machine->event_links();

    $revision_events = $machine->get_available_events();

    foreach ($revision_events as $event_machine_name => $event) {
      // @todo, why run state_flow_access again? Doesn't get_available_events perform that check?
      if (state_flow_access($entity, $event_machine_name)) {
        $state_actions[] = l($event->get_option('label'), 'node/' . $entity->nid . '/revisions/' . $entity->vid . '/workflow/' . $event_machine_name) . ' ';
      }
    }

    return implode(' | ', $state_actions);
  }
}
