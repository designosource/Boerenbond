<?php

/**
 * @file
 * Views integration for State Flow
 */

/**
 * Implements hook_views_default_views().
 */
function state_flow_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'state_flow') . '/includes/views/views', '/inc$/');
  foreach ($files as $filename => $file) {
    require $filename;
    $views[$file->name] = $view;
  }

  return $views;
}

/**
 * Implements hook_views_data_alter().
 */
function state_flow_views_data_alter(&$data) {

  $data['state_flow_states']['table']['join']['node_revision'] = array(
    'left_field' => 'vid',
    'field' => 'revision_id',
    'type' => 'INNER',
    'extra' => array(
      array(
        'field' => 'entity_type',
        'value' => 'node',
        'operator' => '='
      )
    )
  );

  $data['state_flow_history']['table']['join']['node_revision'] = array(
    'left_field' => 'vid',
    'field' => 'revision_id',
    'type' => 'INNER',
  );

  $data['state_flow_history']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'entity_id',
    'type' => 'INNER',
    'extra' => array(
      array(
        'field' => 'entity_type',
        'value' => 'node',
        'operator' => '='
      )
    )
  );

  $data['state_flow_history']['revision_id']['relationship'] = array(
    'title' => t('Content Revision'),
    'label' => t('Content Revision'),
    'base' => 'node_revision',
    'base field' => 'vid',
   'type' => 'INNER',
  );

  $data['state_flow_history']['nid'] = array(
     'title' => t('Node'),
     'help' => t('The node according to the State Flow.'),
     'relationship' => array(
         'base' => 'node',
         'field' => 'entity_id',
         'handler' => 'views_handler_relationship',
         'label' => t('Node'),
     ),
   );

  $data['node_revision']['is_published'] = array(
    'real field' => 'vid',
    'title' => t('Is this revision published?'),
    'help' => t('Checks whether this is the revision that is also in the node table and that its status is 1'),
    'field' => array(
      'handler' => 'state_flow_handler_field_is_node_revision_published',
      'click sortable' => FALSE,
    ),
  );
}
