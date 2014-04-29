<?php

/**
 * @file
 * Views integration for State Flow
 */

/**
 * Implements hook_views_data().
 */
function state_flow_entity_views_data() {

  $data = array();

  // @todo, add vid edit/delete links

  $data['state_flow_states']['table']['group'] = t('State Flow States');
  $data['state_flow_states']['table']['join'] = array(
    'state_flow_history' => array(
      'left_field' => 'hid',
      'field' => 'hid',
      'type' => 'INNER',
    ),
  );

  $data['state_flow_states']['entity_type'] = array(
    'title' => t('Entity Type'),
    'help' => t('The type of entity that had a state change.'),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['state_flow_states']['state_flow_id'] = array(
    'title' => t('state flow id'),
    'help' => t('--'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['state_flow_states']['entity_id'] = array(
    'title' => t('entity id'),
    'help' => t('Useful for linking to entity tables'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['state_flow_states']['state_flow_entity_available_events'] = array(
    'real field' => 'state_flow_id',
    'title' => t('Available event links'),
    'help' => t('Links to the state change events that are available to this entity'),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_available_events',
      'click sortable' => FALSE,
    ),
  );

  $data['state_flow_states']['history_list'] = array(
    'title' => t('History list'),
    'help' => t('All of the State Flow History Entities belong to a given revision rendered in a view mode.'),
    'real field' => 'state_flow_id',
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_state_flow_states_history_list',
    ),
  );

  $data['state_flow_states']['state_flow_events_form'] = array(
    'real field' => 'state_flow_id',
    'title' => t('state flow events form'),
    'help' => t('--'),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_events_form',
      'click sortable' => FALSE,
    ),
  );

  $data['state_flow_states']['state_flow_states_revision_link'] = array(
    'real field' => 'state_flow_id',
    'title' => t('state flow states revision link'),
    'help' => t('--'),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_link',
      'click sortable' => FALSE,
    ),
  );

  // since state_flow_entity does not have a table for states,
  // a simple string filter is used
  $data['state_flow_states']['state'] = array(
    'title' => t('Revision current state'),
    'help' => t('Content moderation state of the entity revisision.'),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_state',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['state_flow_states']['hid'] = array(
    'title' => t('Revision current hid'),
    'help' => t('--'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'relationship' => array(
      'title' => t('State Flow History Entity'),
      'label' => t('State Flow History Entity'),
      'base' => 'state_flow_history',
      'base field' => 'hid',
    ),
  );

  $data['state_flow_states']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The date this revision was moderated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['state_flow_states']['active'] = array(
    'title' => t('Active'),
    'help' => t('Whether or not this is the active revision.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // Begin the state_flow_history section.
  $data['state_flow_history']['table']['group'] = t('State Flow History Record');
  $data['state_flow_history']['table']['base'] = array(
    'field' =>  'hid',
    'title' => 'state_flow_history_entity',
    'weight' => 100,
    'entity type' => 'state_flow_history_entity',
  );

  $data['state_flow_history']['hid'] = array(
    'title' => t('Moderation History ID'),
    'help' => t('Content moderation history record ID.'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'relationship' => array(
      'title' => t('State Flow States Table'),
      'label' => t('State Flow States Table'),
      'base' => 'state_flow_states',
      'base field' => 'hid',
    ),

  );

  $data['state_flow_history']['state'] = array(
    'title' => t('State'),
    'help' => t('Content moderation state of the entity revisision.'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_state',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      // @todo, what is this? Ah, I don't know how to update that for this case since state_flow doesn't own it's state names.
      'name table' => 'workbench_moderation_states',
      'name field' => 'label',
    ),
  );

  $data['state_flow_history']['from_state'] = array(
    'title' => t('From State'),
    'help' => t('Content moderation from state of the entity revisision.'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'state_flow_entity_handler_field_state',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      // @todo, what is this? Ah, I don't know how to update that for this case since state_flow doesn't own it's state names.
      'name table' => 'workbench_moderation_states',
      'name field' => 'label',
    ),
  );


  $data['state_flow_history']['entity_type'] = array(
    'title' => t('Entity Type'),
    'help' => t('The type of entity that had a state change.'),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['state_flow_history']['log'] = array(
    'title' => t('State Change Log'),
    'help' => t('----'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['state_flow_history']['revision_id'] = array(
    'title' => t('Content Revision'),
    'help' => t('---'),
  );

  $data['state_flow_history']['uid'] = array(
    'title' => t('User'),
    'help' => t('User who moderated this revision.'),
    'relationship' => array(
      'title' => t('User'),
      'label' => t('moderator user'),
      'base' => 'users',
      'base field' => 'uid',
    ),
  );

  $data['state_flow_history']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The date this revision was moderated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );


  $data['views']['state_flow_entity_handler_area_events_form'] = array(
    'title' => t('State Flow Entity Events Form'),
    'help' => t('Insert a State Flow Entity events form for a given entity id.'),
    'area' => array(
      'handler' => 'state_flow_entity_handler_area_events_form',
    ),
  );

  return $data;
}
