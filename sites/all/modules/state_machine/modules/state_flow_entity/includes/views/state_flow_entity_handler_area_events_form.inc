<?php

/**
 * @file
 * Contains state_flow_entity_handler_area_events_form
 */
/**
 * Views area handlers. Insert a state flow entity form in a view.
 *
 * @ingroup views_area_handlers
 */
class state_flow_entity_handler_area_events_form extends views_handler_area_text {

  function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = array('default' => 'node');
    $options['entity_id'] = array('default' => '');
    $options['entity_to_use'] = array('default' => 'active');
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['content']);
    $form['entity_id'] = array(
      '#title' => 'Entity ID',
      '#description' => 'For what entity id should this form be rendered? For most usages, this will be a node id that is coming in to the View as an argument. If this is the case, use token replacement.',
      '#type' => 'textfield',
      '#default_value' => $this->options['entity_id'],
    );

    // @todo, remove hard-coded node expectation.
    $form['entity_type'] = array(
      '#title' => 'Entity Type',
      '#type' => 'value',
      '#value' => 'node',
      '#default_value' => $this->options['entity_type'],
    );

    $form['entity_to_use'] = array(
      '#type' => 'select',
      '#title' => t('Entity to use'),
      '#options' => array(
        'active' => 'Active',
        'default' => 'Default',
      ),
      '#default_value' => $conf['entity_to_use'],
      '#description' => t('The active entity is the one that most recently has had a state change or edit. This may or may not be the entity that loads from entity_load(). "Active" is recommended for most usages.'),
    );
  }

  /**
   * Render the area
   */
  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {

      if (!empty($this->options['entity_type'])) {
        $entity_type = $this->options['entity_type'];

        // The default text format might return the entity wrapped in p tags and with a line break.
        // Remove those and the result should be a numeric id.
        $entity_id = trim(strip_tags($this->render_textarea($this->options['entity_id'], NULL)));
        if (!empty($entity_id) && is_numeric($entity_id)) {

          // Get the active revision id.
          $default_entity_array = entity_load($entity_type, array($entity_id));
          if (!empty($default_entity_array) && $default_entity = array_pop($default_entity_array)) {

            if (!empty($this->options['entity_to_use']) && $this->options['entity_to_use'] == 'active') {

              $entity_info = entity_get_info($entity_type);
              $revision_key = $entity_info['entity keys']['revision'];

              $machine = state_flow_entity_load_state_machine($default_entity, $entity_type);
              $active_revision_id = $machine->get_active_revision();

              // Get the active revision object.
              $conditions = array($revision_key => $active_revision_id);
              $entities = entity_load($entity_type, array($entity_id), $conditions);
              $entity = array_pop($entities);
            }
            else {
              $entity = $default_entity;
            }

            $form_options = array(
              'assemble_page_title' => FALSE,
              'event_element_type' => 'select',
            );

            module_load_include('inc', 'state_flow_entity', 'state_flow_entity.forms');
            $output = drupal_render(drupal_get_form('state_flow_entity_events_revision', $entity, $entity_type, NULL, $form_options));

            return $output;
          }
        }
      }
    }
  }
}
