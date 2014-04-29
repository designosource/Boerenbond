<?php

/**
 * @file
 * Rules integration for State Flow.
 */

/**
 * Implements hook_rules_event_info().
 *
 * Define Rules events that are triggered by State Flow.
 */
function state_flow_entity_rules_event_info() {
  $items = array(
    'state_flow_entity_event_fired' => array(
      'label' => t('After a State Flow Event occurs'),
      'group' => t('State Flow'),
      'variables' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'description' => t('The entity for which the current state should be checked.'),
        ),
        'author' => array('type' => 'user', 'label' => t('content author')),
        'state_flow_state' => array('type' => 'text', 'label' => t('state')),
        'state_flow_event' => array('type' => 'text', 'label' => t('event')),
        // @todo, this needs a select list of available states.
        'state_flow_history_entity' => array(
          'type' => 'state_flow_history_entity',
          'label' => t('state flow history entity'),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function state_flow_entity_rules_condition_info() {
  $items = array();
  $items['state_flow_entity_rules_condition_entity_is_state'] = array(
    'label' => t('Entity is in state'),
    'group' => t('State Flow'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The entity for which the current state should be checked.'),
      ),
      'entity_type' => array(
        'type' => 'text',
        'label' => 'Entity Type',
        // @todo, need to convert this to a select list or allow a variable.
      ),
      'state_flow_state' => array(
        'type' => 'text',
        'label' => t('State'),
      ),
    ),
    'base' => 'state_flow_entity_rules_condition_entity_is_state',
  );
  return $items;
}

/**
 * Callback for the "Content is in state" Rules condition.
 */
function state_flow_entity_rules_condition_entity_is_state($entity, $entity_type, $state) {

  // The incoming entity might be in a wrapper. If so, get the raw entity.
  if (method_exists($entity, 'raw')) {
    $raw_entity = $entity->raw();
  }
  else {
    $raw_entity = $entity;
  }

  $machine = state_flow_entity_load_state_machine($raw_entity, $entity_type);
  if (!$machine->ignore()) {
    return $machine->get_current_state() === $state;
  }
  return FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function state_flow_entity_rules_action_info() {
  $items = array();

  $items['state_flow_entity_perform_event'] = array(
    'label' => t("Perform State Flow event"),
    'group' => t("State Flow"),
    'base' => 'state_flow_entity_perform_event',
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t("Entity"),
      ),
      'entity_type' => array(
        'type' => 'text',
        'label' => 'Entity Type',
        'options list' => 'rules_entity_action_type_options',
        'description' => t('What type of entity is this?'),
        'restriction' => 'input',
      ),
      'event' => array(
        'type' => 'text',
        'label' => t("State Flow Event Machine Name"),
        //@todo, need a function to get a list of all available transitions.
        // 'options list' => 'callback_function',
        'restriction' => 'input',
      ),
    ),
  );

  $items['state_flow_entity_fetch_active_id'] = array(
    'label' => t("Fetch active id for an entity"),
    'group' => t("State Flow"),
    'base' => 'state_flow_entity_fetch_active_id',
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t("Entity"),
      ),
      'entity_type' => array(
        'type' => 'text',
        'label' => 'Entity Type',
        'options list' => 'rules_entity_action_type_options',
        'description' => t('What type of entity is this?'),
        'restriction' => 'input',
      ),
    ),
    'provides' => array(
    'active_id' => array('type' => 'integer', 'label' => t('Active id')),
    ),
  );



  return $items;
}

/**
 * Perform an event on an entity.
 */
function state_flow_entity_perform_event($entity, $entity_type, $event) {

  // The incoming entity might be in a wrapper. If so, get the raw entity.
  if (method_exists($entity, 'raw')) {
    $raw_entity = $entity->raw();
  }
  else {
    $raw_entity = $entity;
  }

  global $user;
  $machine = state_flow_entity_load_state_machine($raw_entity, $entity_type);
  // @todo, configurable log message.
  $machine->fire_event($event, $user->uid, 'Published on cron.');

  return array('entity' => $entity);
}

/**
 * Get the active revision id for an entity.
 */
function state_flow_entity_fetch_active_id($entity, $entity_type) {

  // The incoming entity might be in a wrapper. If so, get the raw entity.
  if (method_exists($entity, 'raw')) {
    $raw_entity = $entity->raw();
  }
  else {
    $raw_entity = $entity;
  }

  $active_id = state_flow_entity_get_active_revision_id($raw_entity, $entity_type);

  return array('active_id' => $active_id);
}
