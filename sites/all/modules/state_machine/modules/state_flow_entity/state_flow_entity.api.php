<?php

/**
 * @file
 * Define a new StateMachine for the node
 */

/**
 * Implements hook_state_flow_entity_plugins().
 *
 * Define the ctools plugin to add a new state machine type.
 */
function hook_state_flow_entity_plugins() {
  $info = array();
  $path = drupal_get_path('module', 'state_flow') . '/plugins';

  $workflow_options = array(
    'states' => array(
      'draft' => array('label' => t('Draft')),
      'published' => array(
        'label' => t('Published'),
        'on_enter' => 'on_enter_published',
        'on_exit' => 'on_exit_published',
      ),
      'unpublished' => array('label' => t('Unpublished')),
    ),
    'events' => array(
      'publish' => array(
        'label' => t('Publish'),
        'origin' => 'draft',
        'target' => 'published',
      ),
      'unpublish' => array(
        'label' => t('Unpublish'),
        'origin' => 'published',
        'target' => 'unpublished',
        'permission' => 'publish and unpublish content',
      ),
      'to draft' => array(
        'label' => t('To Draft'),
        'origin' => 'unpublished',
        'target' => 'draft',
      ),
    ),
  );

  $info['state_flow_node'] = array(
    'handler' => array(
      'class' => 'StateFlowNode',
      'file' => 'state_flow_node.inc',
      'path' => drupal_get_path('module', 'state_flow') . '/plugins',
      'parent' => 'state_flow_entity',
      'workflow_options' => $workflow_options,
      'entity_type' => 'node',
      'event_form_options' => array(), // see state_flow.forms.inc for details
    ),
  );
  return $info;
}

/**
 * Implements hook_state_flow_machine_type_alter().
 *
 * @param string $machine_type
 * @param object $entity
 * @param string $entity_type
 */
function hook_state_flow_entity_machine_type_alter(&$machine_type, $entity, $entity_type) {
  $machine_type = 'state_flow_test';
}


/**
 * Define a new workflow for a custom entity type.
 */
class StateFlowTest extends StateFlowEntity {

  /**
   * Callback for when a node enters the published state.
   */
  public function on_enter_published() {
    // @todo.
  }

  /**
   * Callback for when a node enters the unpublished state.
   */
  public function on_enter_unpublished() {
    // @todo.
  }

  /**
   * Callback for when a node exits the published state.
   */
  public function on_exit_published() {
    // @todo.
  }

}
