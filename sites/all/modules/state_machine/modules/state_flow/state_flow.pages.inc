<?php

/**
 * @file
 * Defines the page callback for a node's workflow page.
 */

/**
 * Page callback for a node's Workflow page.
 */
function state_flow_events($node) {
  // Gather information on the current published revision

  $machine = state_flow_entity_load_state_machine($node, 'node');
  //$state_flow_entity->events = $state_flow_entity->plugin->handler->workflow_options->events;
  $events = $machine->get_available_events();
  $current_revision_actions = array();
  $revisions = node_revision_list($node);

  // setup the revision that is available to be published
  foreach ($events as $event_machine_name => $event) {
    if (state_flow_access($node, $event_machine_name)) {
      $current_revision_actions[] = l($event->get_option('label'), 'node/' . $node->nid . '/revisions/' . $node->published_revision_id . '/workflow/' . $event_machine_name);
    }
  }
  $current_revision_actions_str = implode(' | ', $current_revision_actions);

  // Build table for the active draft revisions and other revisions of this node
  $drafts_table = array(
    'header' => array(
      t('ID'),
      t('Title'),
      t('Status'),
      t('Last Updated'),
      t('Actions'),
    ),
    'rows' => array(),
  );
  $others_table = array(
    'header' => array(
      t('ID'),
      t('Title'),
      t('Status'),
      t('Last Updated'),
      t('Actions'),
    ),
    'rows' => array(),
  );

  // build revision table for other revisions
  foreach ($revisions as $revision) {
    $vid = intval($revision->vid);
    $path_view = ($vid == $node->vid) ?
      'node/' . $node->nid : 'node/' . $node->nid . '/revisions/' . $vid . '/view';
    $state_actions = array();

      $revision_node = node_load($node->nid, $vid);
      $revision_machine = state_flow_entity_load_state_machine($revision_node, 'node');
      $revision_date_str = format_date($revision_machine->object->current_state_timestamp, 'short');
      $revision_events = $revision_machine->get_available_events();
      $revision_actions = array();
        if (node_access('update', $revision_node)) {
          $state_actions[] = l(t('Edit'), 'node/' . $node->nid . '/revisions/' . $vid . '/edit');
        }
      if (_node_revision_access($revision_node, 'delete')) {
        $state_actions[] = l(t('Delete'), 'node/' . $node->nid . '/revisions/' . $vid . '/delete');
      }

      foreach ($revision_events as $event_machine_name => $event) {
        if (state_flow_access($revision_node, $event_machine_name)) {
          $state_actions[] = l($event->get_option('label'), 'node/' . $node->nid . '/revisions/' . $vid . '/workflow/' . $event_machine_name) . ' ';
        }
      }
      $revision_actions_str = implode(' | ', $state_actions);
      $item = array(
        l($vid, $path_view),
        l($revision_node->title, $path_view),
        t($revision_machine->object->current_state),
        $revision_date_str,
        $revision_actions_str,
      );
      $others_table['rows'][] = $item;
  }

  // Build a table for the history of this node
  $history_table = array(
    'header' => array(
      t('Date'),
      t('Message'),
    ),
    'rows' => array(),
  );

  $history = state_flow_get_all_history($node->nid);
  foreach ($history as $state) {
    $vid = intval($state->revision_id);
    $path_view = ($vid == $node->vid) ? 'node/' . $node->nid : 'node/' . $node->nid . '/revisions/' . $vid . '/view';
    $state_date_str = format_date($state->timestamp, 'short');
    $history_str = '';
    $history_str = t('!user transitioned revision !vid-link to %state.',
      array(
        '!user' => l($state->user_name, 'user/' . $state->uid),
        '!vid-link' => l($vid,  $path_view),
        '%state' => $state->state
      )
    );
    if (!empty($state->log)) {
      $history_str .= '<br/>' . t('@user said: @log', array('@user' => $state->user_name, '@log' => $state->log));
    }
    $history_table['rows'][] = array(
      $state_date_str,
      $history_str,
    );
  }

  // Set the title for this page
  drupal_set_title(t('Workflow for %title', array('%title' => $node->title)), PASS_THROUGH);

  // Build the render array
  $output = array(
    '#theme' => 'page',
    '#type' => 'page',
    'content' => array(
      'current_revision' => array(
        '#type' => 'fieldset',
        '#title' => t('Current Revision'),
        'current_revision_status' => array(
          '#type' => 'item',
          '#title' => t('Status'),
          '#markup' => check_plain($node->current_state),
        ),
        'current_revision_vid' => array(
          '#type' => 'item',
          '#title' => t('Revision'),
          '#markup' => l($node->published_revision_id, 'node/' . $node->nid),
        ),
      ),
      '#sorted' => TRUE,
    ),
  );
  if (!empty($current_revision_actions_str)) {
    $output['content']['current_revision']['current_revision_actions'] = array(
      '#type' => 'item',
      '#title' => t('Actions'),
      '#markup' => $current_revision_actions_str,
    );
  }
  if (count($drafts_table['rows'])) {
    $output['content']['draft_revisions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drafts'),
      'draft_revisions_table' => array(
        '#markup' => theme('table', $drafts_table),
      ),
    );
  }
  if (count($others_table['rows'])) {
    $output['content']['other_revisions'] = array(
      '#type' => 'fieldset',
      '#title' => t('All Revisions'),
      'other_revisions_table' => array(
        '#markup' => theme('table', $others_table),
      ),
    );
  }
  if (count($history_table['rows'])) {
    $output['content']['history'] = array(
      '#type' => 'fieldset',
      '#title' => t('History'),
      'history_table' => array(
        '#markup' => theme('table', $history_table),
      ),
    );
  }

  return $output;
}
