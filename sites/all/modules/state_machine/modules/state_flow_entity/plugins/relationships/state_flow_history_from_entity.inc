<?php

/**
 * @file
 * Plugin to provide an relationship handler for state_flow_history_entity from node.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('State Flow History Entity from Entity'),
  'keyword' => 'state_flow_history_entity',
  'description' => t('Adds a state'),

  // @todo, take out node assumptions
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'context' => 'state_flow_entity_state_flow_history_from_entity_context',
  'edit form' => 'state_flow_entity_state_flow_history_from_entity_settings_form',
  'defaults' => array('vid' => ''),
);

/**
 * Return a new context based on an existing context.
 */
function state_flow_entity_state_flow_history_from_entity_context($context, $conf) {

  if (!empty($context->data)) {
    $entity = $context->data;

    // @todo, take out node assumptions. This query code is ripped from get_history_entity()
    // should not have this duplication.
    $entity_type = 'node';
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $revision_key = $entity_info['entity keys']['revision'];

    // Make a new empty class for the history entity.
    $history_entity = new stdClass();
    $query = db_select('state_flow_history', 'sfh')
                ->fields('sfh', array('hid'))
                ->orderBy('hid', 'DESC')
                ->range(0, 1)
                ->condition('entity_type', $entity_type);

    // Set a condition for the vid or nid.
    if (!empty($entity->$revision_key)) {
      $query->condition('revision_id', $entity->$revision_key);
    }
    elseif (!empty($entity->$id_key)) {
      $query->condition('entity_id', $entity->$id_key);
    }

    // Get the results of the query.
    $result = $query->execute()->fetchCol('hid');

    // Load up the fields of the history entity.
    if (!empty($result[0])) {
      $hid = $result[0];

      // @todo, Find out why TRUE needs to be passed as the last argument.
      // If TRUE is not passed, there are inconsisent objects returned at node/12/revisions/53/workflow/published
      $history_entities = entity_load('state_flow_history_entity', array($hid), array(), TRUE);

      if (!empty($history_entities[$hid])) {
        $history_entity = $history_entities[$hid];
        return ctools_context_create('entity:state_flow_history_entity', $history_entity);
      }
    }
  }

  return ctools_context_create_empty('entity:state_flow_history_entity', NULL);
}

/**
 * Settings form for the relationship.
 */
function state_flow_entity_state_flow_history_from_entity_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}
