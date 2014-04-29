<?php

/**
 * @file
 * Handle rendering the state flow entity form.
 */

$plugin = array(
  'title' => t('State Flow Entity Form'),
  'defaults' => array('entity_to_use' => 'active'),
  'content type' => 'state_flow_entity_form_content_type_content_type',
  'content types' => 'state_flow_entity_form_content_type_content_types',
  'render callback' => 'state_flow_entity_form_content_type_render',

  // @todo, admin title callback is not working.
  'admin title callback' => 'state_flow_entity_form_content_type_admin_title',
);

function state_flow_entity_form_content_type_content_type($subtype) {

  $types = state_flow_entity_form_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

/**
 * Return all field content types available.
 */
function state_flow_entity_form_content_type_content_types() {

  $types = &drupal_static(__FUNCTION__, array());
  if (!empty($types)) {
    return $types;
  }

  // This will hold all the individual field content types.
  $context_types = array();
  $entities = entity_get_info();

  foreach ($entities as $entity_type => $entity) {

    // @todo, remove hardcoded node assumption
    if ($entity_type === 'node') {
      $types[$entity_type] = array(
        'category' => t(ucfirst($entity_type)),
        'required context' => new ctools_context_required(t(ucfirst($entity_type)), $entity_type),
        'title' => t('State Flow Entity form for: @entity_type', array(
          '@entity_type' => t(ucfirst($entity_type)),
        )),
        'description' => t('Allows the state to be changed for a given entity.'),
        'edit form' => array(
          'state_flow_entity_form_content_type_formatter_options' => array(
            'default' => TRUE,
            'title' => t('Formatter options for State Flow Entity Form'),
          ),
        ),
      );
    }
  }

  return $types;
}

/**
* Render the custom content type.
*/
function state_flow_entity_form_content_type_render($subtype, $conf, $panel_args, $context) {

  if (empty($context) || empty($context->data)) {
    return;
  }

  $entity_type = $subtype;

  // The form might be for the entity passed into this function or for the "active" entity.
  if (!empty($conf['entity_to_use']) && $conf['entity_to_use'] == 'active') {
    $entity_info = entity_get_info($entity_type);
    $revision_key = $entity_info['entity keys']['revision'];

    // Get the active revision id.
    $machine = state_flow_entity_load_state_machine($context->data, $entity_type);
    $active_revision_id = $machine->get_active_revision();

    // Get the active revision object.
    $conditions = array($revision_key => $active_revision_id);
    // @todo, can original_argument be trusted as the entity_id?
    $entities = entity_load($entity_type, array($context->original_argument), $conditions);
    $entity = array_pop($entities);
  }
  else {
    $entity = $context->data;
  }

  $form_options = array(
    'assemble_page_title' => FALSE,
    'event_element_type' => 'select',
  );

  module_load_include('inc', 'state_flow_entity', 'state_flow_entity.forms');
  $output =  drupal_get_form('state_flow_entity_events_revision', $entity, $entity_type, NULL, $form_options);

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'state_flow_entity';
  if ($conf['label'] == 'title' && isset($field_output['#title'])) {
    $block->title = '';
  }

  $block->content = $output;
  $block->delta   = $ids[0];

  return $block;
}

/**
* Returns an edit form for custom type settings.
*/
function state_flow_entity_form_content_type_formatter_options($form, &$form_state) {

  if (empty($form_state['conf']['formatter_settings'])) {
    $form_state['conf']['formatter_settings'] = array();
  }
  $conf = $form_state['conf'];


  $form['entity_to_use'] = array(
    '#type' => 'select',
    '#title' => t('Entity to use'),
    '#options' => array(
      'active' => 'Active',
      'supplied' => 'Supplied',
    ),
    '#default_value' => $conf['entity_to_use'],
    // @todo, better description.
    '#description' => t('The active entity is the one that most recently has had a state change or edit. This may or may not be the entity context supplied to this plugin.'),
  );

  return $form;
}

function state_flow_entity_form_content_type_formatter_options_submit($form, &$form_state) {

  $form_state['conf']['entity_to_use'] = $form_state['values']['entity_to_use'];
}

/**
* Returns the administrative title for a type.
*/
function state_flow_entity_form_content_type_admin_title($subtype, $conf, $context) {

  // @todo, this function is not getting called.
  return t('State Flow Entity form');
}
