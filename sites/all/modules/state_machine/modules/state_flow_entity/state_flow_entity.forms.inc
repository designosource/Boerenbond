<?php

/**
 * @file
 * Form API related functions used by state_flow_entity and state_flow.
 */


function state_flow_entity_events_revision($form, &$form_state, $entity_revision, $entity_type = 'node', $new_event = NULL, $form_options = array()) {

  $form_options += array(
    'assemble_page_title' => TRUE,
  );

  $form = _state_flow_entity_events_revision($form, $form_state, $entity_revision, $entity_type, $new_event, $form_options);


  // confirm_form() always calls drupal_set_title() and that is unwanted in some
  // usages of this function.
  if (!empty($form['#page_title'])) {
    drupal_set_title($form['#page_title'], PASS_THROUGH);
  }

  // This user might not have access to any events. If so, do not render any
  // form buttons.
  if (!empty($form['event'])) {

    $yes = t('Update State');
    $no = t('Cancel');
    $path = entity_uri($entity_type, $entity_revision);
    $description = '';
    $name = 'confirm';

    // Prepare cancel link.
    if (isset($_GET['destination'])) {
      $options = drupal_parse_url(urldecode($_GET['destination']));
    }
    elseif (is_array($path)) {
      $options = $path;
    }
    else {
      $options = array('path' => $path);
    }

    $form['#attributes']['class'][] = 'confirmation';
    $form['description'] = array('#markup' => $description);
    $form[$name] = array('#type' => 'hidden', '#value' => 1);

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $yes ? $yes : t('Confirm'),
    );
    $form['actions']['cancel'] = array(
      '#type' => 'link',
      '#title' => $no ? $no : t('Cancel'),
      '#href' => $options['path'],
      '#options' => $options,
    );
    // By default, render the form using theme_confirm_form().
    if (!isset($form['#theme'])) {
      $form['#theme'] = 'confirm_form';
    }
  }
  return $form;
}

/**
 * Form builder for the node revision workflow transition form.
 */
function _state_flow_entity_events_revision($form, &$form_state, $entity_revision, $entity_type = 'node', $new_event = NULL, $form_options = array()) {

  // Set up basic variables.
  $entity_info = entity_get_info($entity_type);
  $revision_key = $entity_info['entity keys']['revision'];
  $entity_id_key = $entity_info['entity keys']['id'];
  $entity_label_key = $entity_info['entity keys']['label'];

  $machine = state_flow_entity_load_state_machine($entity_revision, $entity_type);

  // Add additional settings to form options from the state_flow plugin.
  if (isset($machine->plugin['handler']['event_form_options'])) {
    $form_options += $machine->plugin['handler']['event_form_options'];
  }
  // Set any other form options that haven't been set already.
  $form_options += array(
    'event_element_label' =>  t('Choose the event to fire'),
    'event_element_type' => 'radios',
    'show_only_validated_events' => TRUE,
    'require_log_message' => TRUE,
    'assemble_page_title' => FALSE,
  );

  // Get event options.
  $events = $machine->get_available_events_options($form_options['show_only_validated_events']);

  if (empty($events)) {
    drupal_set_message(t('You cannot perform any workflow transitions on this revision.'), 'warning');
  }
  else {
    if (empty($new_event) || !array_key_exists($new_event, $events) ) {

      // Don't set a title based on the revision key if there isn't one.
      // @todo, titles should only be set when this is a state alone form.
      if (!empty($entity_revision->{$revision_key}) && $form_options['assemble_page_title'] === TRUE) {

        // Construct a page title that can be passed to confirm_form().
        $form['#page_title'] =  t('Transition revision @rev-vid to a new state', array('@rev-vid' => $entity_revision->{$revision_key}));
      }

      $form['event'] = array(
        '#type' => $form_options['event_element_type'],
        '#title' => $form_options['event_element_label'],
        '#options' => $events,
        '#required' => TRUE,
      );

      // Set the default event.
      $default_event = $machine->get_default_event_name();
      if (array_key_exists($default_event, $events)) {
        $form['event']['#default_value'] = $default_event;
      }
    }
    else {

      if (!empty($entity_revision->{$revision_key}) && $form_options['assemble_page_title'] === TRUE) {
        // Construct a page title that can be passed to confirm_form().
        $form['#page_title'] =  t('Transition revision @rev-vid to %state', array('@rev-vid' => $entity_revision->{$revision_key}, '%state' => $new_event));
      }

      $form['event'] = array(
        '#type' => 'value',
        '#value' => $new_event,
      );
    }

    $form['event_comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Log message for this state change'),
      '#default_value' => '',
      '#required' => $form_options['require_log_message']
    );

    $form['entity_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type,
    );

    $form['from_state'] = array(
      '#type' => 'value',
      '#value' => $machine->get_current_state(),
    );

    $form['entity_revision'] = array(
      '#type' => 'value',
      '#value' => $entity_revision,
    );
    $form['state_flow'] = array(
      '#type' => 'value',
      '#value' => $machine,
    );
    // Add Field API fields to the State Flow history entity.
    $state_flow_history_entity = $machine->get_history_entity();
    field_attach_form('state_flow_history_entity', $state_flow_history_entity, $form, $form_state, $langcode = NULL);
  }

  return $form;
}

/**
 * Form validate callback for state_flow_events_revision.
 */
function state_flow_entity_events_revision_validate($form, &$form_state) {
  // Validate the Field API fields.
   $state_flow = $form_state['values']['state_flow'];
   $state_flow->history_entity_form_field_validate($form, $form_state);
}

/**
 * Form submission callback for state_flow_events_revision.
 */
function state_flow_entity_events_revision_submit($form, &$form_state) {
  $entity_revision = $form_state['values']['entity_revision'];
  $entity_type = $form_state['values']['entity_type'];
  $entity_info = entity_get_info($entity_type);
  $entity_label_key = $entity_info['entity keys']['label'];

  $state_flow = $form_state['values']['state_flow'];
  $event = $form_state['values']['event'];
  $event_comment = $form_state['values']['event_comment'];
  if (!empty($event)) {
    global $user;
    $state_flow->history_entity_form_submit_build_entity($form, $form_state);
    $rv = $state_flow->fire_event($event, $user->uid, $event_comment);
    if ($rv !== FALSE) {
      $state = $state_flow->get_current_state();
      drupal_set_message(t('%title transitioned to the @state state.', array('%title' => $entity_revision->{$entity_label_key}, '@state' => $state)));
      $entity_uri = entity_uri($entity_type, $entity_revision);
      $form_state['redirect'] = $entity_uri['path'];
    }
  }
}
