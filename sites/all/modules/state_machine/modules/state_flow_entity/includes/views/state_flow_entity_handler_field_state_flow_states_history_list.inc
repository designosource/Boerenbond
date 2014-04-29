<?php

/**
 * @file
 *  A field handler to take a record form {state_flow_states} and load all of the relevant history
 *  entities.
 */

class state_flow_entity_handler_field_state_flow_states_history_list extends views_handler_field {


  /**
   * Override the construct method to get additional values from the given database row. These will
   * be used to query for related state_flow_history_entities.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['entity_type'] = array('table' => 'state_flow_states', 'field' => 'entity_type');
    $this->additional_fields['revision_id'] = array('table' => 'state_flow_states', 'field' => 'revision_id');
  }

  /**
   * Override pre_render() to load all relevant state_flow_history_entities
   * in one query. They get rendered in chunks in the render() method.
   */
  public function pre_render(&$values) {

    // Call the parent pre_render method on general principle.
    parent::pre_render($values);

    // An array of revision ids. Corresponding state_flow_history_entities will be loaded based on
    // these values.
    // Change to $relevant_revision_ids
    $relevant_revision_ids = array();
    foreach ($values as $result_set) {
      $revision_id = $this->get_value($result_set, 'revision_id');
      $entity_type = $this->get_value($result_set, 'entity_type');
      $relevant_revision_ids[$entity_type][$revision_id] = $revision_id;
    }

    // Use EntityFieldQuery to get the hids of the relevant state_flow_history_entities based on the parent
    // entity_type and revision_ids.
    foreach ($relevant_revision_ids as $parent_entity_type => $keys) {
      $efq = new EntityFieldQuery();
      $efq->entityCondition('entity_type', 'state_flow_history_entity');
      $efq->propertyCondition('entity_type', $parent_entity_type);
      $efq->propertyCondition('revision_id', $keys, 'IN');
      $efq->propertyOrderBy('hid', 'DESC');
      // Execute query and collect results
      $result = $efq->execute();

      // An array for all the relevant state_flow_history_entity hids.
      $hids = array();
      foreach ($result['state_flow_history_entity'] as $result_object) {
        $hids[$result_object->hid] = $result_object->hid;
      }

      // Load all the relevant history entities.
      $state_flow_history_entities = entity_load('state_flow_history_entity', $hids);

      // Results get rendered in groups by parent revision_id so group the state_flow_history_entities
      // accordingly.
      $state_flow_history_entities_by_revision_id = array();
      foreach($state_flow_history_entities as $hid => $state_flow_history_entity) {
        $state_flow_history_entities_by_revision_id[$state_flow_history_entity->revision_id][$hid] = $state_flow_history_entity;
      }
      // Store the array on this object so it is available in the render method.
      $this->state_flow_history_entities_by_revision_id[$parent_entity_type] = $state_flow_history_entities_by_revision_id;
    }
  }

  public function render($values) {

    // @todo, There may be a better place to add this CSS.
    drupal_add_css(drupal_get_path('module', 'state_flow_entity') . '/css/state_flow_entity.css');

    // Get the entity type and revision id for the parent entity.
    $revision_id = $this->get_value($values, 'revision_id');
    $entity_type = $this->get_value($values, 'entity_type');

    // An array of state_flow_history_entities should have been stored on this object in the above
    // pre_render method relevant to this record.
    if (!empty($this->state_flow_history_entities_by_revision_id[$entity_type][$revision_id])) {

      // These are the state_flow_history_entities relevant only to this state_flow_states/parent
      // entity revision_id record.
      $state_flow_history_entities = $this->state_flow_history_entities_by_revision_id[$entity_type][$revision_id];

       // A renderable array.
      $output = array();
      // Add each state_flow_history_entity to the renderable array.
      foreach ($state_flow_history_entities as $hid => $state_flow_history_entity) {
        $output[]['#markup'] = state_flow_entity_state_flow_history_entity_view($state_flow_history_entity, $view_mode = 'full');
      }
      return $output;
    }

    // Return an empty string if there is a fail above.
    return '';
  }
}

