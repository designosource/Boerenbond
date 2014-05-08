<?php
/**
 * Implements hook_form_BASE_FORM_ID_alter().
 * boerebond_backend_publicatiefiche_node_form_alter().
 *
 * Forcing new reversion and publishing.
 */
// function hook_form_BASE_FORM_ID_alter()(&$form, $form_state) {

function workbench_moderation_form_node_form_alter(&$form, $form_state) {
  $content_type = $form['#node']->type;
  if($content_type == 'publicatiefiche') {
    global $user;
    // This must be a node form and a type that has moderation enabled
    if (!workbench_moderation_node_type_moderated($form['type']['#value'])) {
      return;
    }
    // Set a moderation state even if there is not one defined
    if (isset($form['#node']->workbench_moderation['current']->state)) {
      $moderation_state = $form['#node']->workbench_moderation['current']->state;
    }
    else {
      $moderation_state = variable_get('workbench_moderation_default_state_' . $form['type']['#value'], workbench_moderation_state_none());
    }
    // Store the current moderation state
    $form['workbench_moderation_state_current'] = array(
      '#type' => 'value',
      '#value' => $moderation_state
    );
    // We have a use case where a live node is being edited. This will always
    // revert back to the original node status.
    if ($moderation_state == workbench_moderation_state_published()) {
      $moderation_state = workbench_moderation_state_none();
    }
    // Get all the states *this* user can access. If states is false, this user
    // can not change the moderation state
    if ($states = workbench_moderation_states_next($moderation_state, $user, $form['#node'])) {
      $current = array($moderation_state => t('Current: @state', array('@state' => workbench_moderation_state_label($moderation_state))));
      $states = array_merge($current, $states);
      $form['revision_information']['workbench_moderation_state_new'] = array(
        '#title' => t('Moderation state'),
        '#type' => 'select',
        '#options' => $states,
        '#default_value' => $moderation_state,
        '#description' => t('Set the moderation state for this content.'),
        '#access' => $states ? TRUE: FALSE,
      );
    }
    else {
      // Store the current moderation state
      $form['workbench_moderation_state_new'] = array(
        '#type' => 'value',
        '#value' => $moderation_state
      );
    }
    // Always create new revisions for nodes that are moderated
    $form['revision_information']['revision'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    // Set a default revision log message.
    $logged_name = (user_is_anonymous() ? variable_get('anonymous', t('Anonymous')) : $user->name);
    if (!empty($form['#node']->nid)) {
      $form['revision_information']['log']['#default_value'] = t('Edited by @user.', array('@user' => $logged_name));
    }
    else {
      $form['revision_information']['log']['#default_value'] = t('Created by @user.', array('@user' => $logged_name));
    }
    // Move the revision log into the publishing options to make things pretty.
    if ($form['options']['#access']) {
      $form['options']['log'] = $form['revision_information']['log'];
      $form['options']['log']['#title'] = t('Moderation notes');    
      // $form['options']['workbench_moderation_state_new'] = $form['revision_information']['workbench_moderation_state_new'];
      /*
       * Patch start for unknown 'workbench_moderation_state_new'
      */
      $form['options']['workbench_moderation_state_new'] = isset($form['revision_information']['workbench_moderation_state_new']) ? $form['revision_information']['workbench_moderation_state_new'] : '';
      /*
       * Patch end for unknown 'workbench_moderation_state_new'
      */
      // Unset the old placement of the Revision log.
      unset($form['revision_information']['log']);
      unset($form['revision_information']['workbench_moderation_state_new']);
      // The revision information section should now be empty.
      $form['revision_information']['#access'] = FALSE;
    }
    // Setup the JS for the vertical tabs summary. The heavy weight allows this
    // script to replace the default node summary callbacks that get registered by
    // "lighter" scripts.
    // Note: Form API '#attached' does not allow to set a weight.
    drupal_add_js(drupal_get_path('module', 'workbench_moderation') . '/js/workbench_moderation.js', array('weight' => 90));
    // Users can not choose to publish content; content can only be published by
    // setting the content's moderation state to "Published".
    $form['options']['status']['#access'] = FALSE;
    $form['actions']['submit']['#submit'][] = 'workbench_moderation_node_form_submit';
    workbench_moderation_messages('edit', $form['#node']);
  }
}