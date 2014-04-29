<?php

/**
 * @file
 * Default version of the state_flow_states_by_node View
 */

$view = new view();
$view->name = 'state_flow_states_by_node';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node_revision';
$view->human_name = 'state_flow_states_by_node';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer nodes';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['row_class'] = 'state-flow--is-published--[is_published_1]';
$handler->display->display_options['style_options']['columns'] = array(
  'vid' => 'vid',
  'hid' => 'hid',
  'title' => 'title',
  'timestamp' => 'timestamp',
  'state' => 'state',
  'log' => 'log',
  'log_1' => 'log_1',
  'state_flow_events_form' => 'state_flow_events_form',
  'state_flow_states_revision_link' => 'state_flow_states_revision_link',
  'state_flow_states_revision_link_1' => 'state_flow_states_revision_link_1',
  'is_published' => 'is_published',
  'history_list' => 'history_list',
  'is_published_1' => 'is_published_1',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'vid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'hid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'timestamp' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'state' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'log' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'log_1' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'state_flow_events_form' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'state_flow_states_revision_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'state_flow_states_revision_link_1' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'is_published' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'history_list' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'is_published_1' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: State Flow States: State Flow History Entity */
$handler->display->display_options['relationships']['hid']['id'] = 'hid';
$handler->display->display_options['relationships']['hid']['table'] = 'state_flow_states';
$handler->display->display_options['relationships']['hid']['field'] = 'hid';
/* Relationship: Content revision: Content */
$handler->display->display_options['relationships']['vid']['id'] = 'vid';
$handler->display->display_options['relationships']['vid']['table'] = 'node_revision';
$handler->display->display_options['relationships']['vid']['field'] = 'vid';
/* Field: Content revision: Vid */
$handler->display->display_options['fields']['vid']['id'] = 'vid';
$handler->display->display_options['fields']['vid']['table'] = 'node_revision';
$handler->display->display_options['fields']['vid']['field'] = 'vid';
$handler->display->display_options['fields']['vid']['exclude'] = TRUE;
/* Field: State Flow States: Revision current hid */
$handler->display->display_options['fields']['hid']['id'] = 'hid';
$handler->display->display_options['fields']['hid']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['hid']['field'] = 'hid';
$handler->display->display_options['fields']['hid']['label'] = 'hid';
$handler->display->display_options['fields']['hid']['exclude'] = TRUE;
/* Field: Content revision: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node_revision';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node_revision'] = TRUE;
/* Field: State Flow States: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'Last Updated';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
/* Field: State Flow States: Workbench State Filter */
$handler->display->display_options['fields']['state']['id'] = 'state';
$handler->display->display_options['fields']['state']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['state']['field'] = 'state';
$handler->display->display_options['fields']['state']['label'] = 'Revision State';
/* Field: State Flow History Record: State Change Log */
$handler->display->display_options['fields']['log']['id'] = 'log';
$handler->display->display_options['fields']['log']['table'] = 'state_flow_history';
$handler->display->display_options['fields']['log']['field'] = 'log';
$handler->display->display_options['fields']['log']['relationship'] = 'hid';
$handler->display->display_options['fields']['log']['exclude'] = TRUE;
/* Field: Content revision: Log message */
$handler->display->display_options['fields']['log_1']['id'] = 'log_1';
$handler->display->display_options['fields']['log_1']['table'] = 'node_revision';
$handler->display->display_options['fields']['log_1']['field'] = 'log';
$handler->display->display_options['fields']['log_1']['label'] = 'Revision log';
$handler->display->display_options['fields']['log_1']['exclude'] = TRUE;
/* Field: State Flow States: state flow events form */
$handler->display->display_options['fields']['state_flow_events_form']['id'] = 'state_flow_events_form';
$handler->display->display_options['fields']['state_flow_events_form']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['state_flow_events_form']['field'] = 'state_flow_events_form';
$handler->display->display_options['fields']['state_flow_events_form']['label'] = 'Moderate';
$handler->display->display_options['fields']['state_flow_events_form']['exclude'] = TRUE;
$handler->display->display_options['fields']['state_flow_events_form']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['state_flow_events_form']['active_revision_handling'] = 'active';
$handler->display->display_options['fields']['state_flow_events_form']['show_only_for_active_revision_ids'] = TRUE;
/* Field: State Flow States: state flow states revision link */
$handler->display->display_options['fields']['state_flow_states_revision_link']['id'] = 'state_flow_states_revision_link';
$handler->display->display_options['fields']['state_flow_states_revision_link']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['state_flow_states_revision_link']['field'] = 'state_flow_states_revision_link';
$handler->display->display_options['fields']['state_flow_states_revision_link']['label'] = 'Revision Edit';
$handler->display->display_options['fields']['state_flow_states_revision_link']['exclude'] = TRUE;
$handler->display->display_options['fields']['state_flow_states_revision_link']['link_type'] = 'revision_edit_path';
$handler->display->display_options['fields']['state_flow_states_revision_link']['link_text'] = 'Edit';
/* Field: State Flow States: state flow states revision link */
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['id'] = 'state_flow_states_revision_link_1';
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['field'] = 'state_flow_states_revision_link';
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['label'] = 'Revision Actions';
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['state_flow_states_revision_link_1']['alter']['text'] = '[state_flow_states_revision_link] | [state_flow_states_revision_link_1]';
/* Field: Content revision: Is this revision published? */
$handler->display->display_options['fields']['is_published']['id'] = 'is_published';
$handler->display->display_options['fields']['is_published']['table'] = 'node_revision';
$handler->display->display_options['fields']['is_published']['field'] = 'is_published';
$handler->display->display_options['fields']['is_published']['label'] = 'Published?';
$handler->display->display_options['fields']['is_published']['exclude'] = TRUE;
$handler->display->display_options['fields']['is_published']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['is_published']['alter']['text'] = '<strong class="state-flow--is-published">[is_published]</strong>';
$handler->display->display_options['fields']['is_published']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['is_published']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['is_published']['yes_text'] = 'This is the published revision';
$handler->display->display_options['fields']['is_published']['no_text'] = '';
/* Field: State Flow States: History list */
$handler->display->display_options['fields']['history_list']['id'] = 'history_list';
$handler->display->display_options['fields']['history_list']['table'] = 'state_flow_states';
$handler->display->display_options['fields']['history_list']['field'] = 'history_list';
$handler->display->display_options['fields']['history_list']['label'] = '';
$handler->display->display_options['fields']['history_list']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['history_list']['alter']['text'] = '[is_published]
[state_flow_events_form]
[history_list]';
$handler->display->display_options['fields']['history_list']['element_label_colon'] = FALSE;
/* Field: Content revision: Is this revision published? */
$handler->display->display_options['fields']['is_published_1']['id'] = 'is_published_1';
$handler->display->display_options['fields']['is_published_1']['table'] = 'node_revision';
$handler->display->display_options['fields']['is_published_1']['field'] = 'is_published';
$handler->display->display_options['fields']['is_published_1']['label'] = 'Is this revision published -- used in CSS class for the row.';
$handler->display->display_options['fields']['is_published_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['is_published_1']['yes_text'] = 'yes';
$handler->display->display_options['fields']['is_published_1']['no_text'] = 'no';
/* Sort criterion: State Flow States: Timestamp */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'state_flow_states';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
/* Sort criterion: Content revision: Vid */
$handler->display->display_options['sorts']['vid']['id'] = 'vid';
$handler->display->display_options['sorts']['vid']['table'] = 'node_revision';
$handler->display->display_options['sorts']['vid']['field'] = 'vid';
$handler->display->display_options['sorts']['vid']['order'] = 'DESC';
/* Contextual filter: State Flow States: entity id */
$handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['arguments']['entity_id']['table'] = 'state_flow_states';
$handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['path'] = 'node/%/revisions-state-flow-states';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Revisions';
$handler->display->display_options['menu']['description'] = 'each vid and its state';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
