<?php

/**
 * @file
 * State Flow implementation of the State Machine class.
 */

class StateFlowEntity extends StateMachine {
  /**
   * Called from StateMachine::__construct to initialize the states and events.
   */
  public $initial;
  public $current;
  public $object;
  public $plugin;

  public function __construct($plugin = NULL) {
    $this->object = $plugin['object'];
    $this->init($plugin['handler']['workflow_options']);
    $this->plugin = $plugin;
    $this->set_current_state((string) $this->load());
  }

  public function init($workflow = array()) {
    // Initialize states
    foreach ($workflow['states'] as $name => $options) {
      // some methods require $this when called by State Machine e.g. on_exit()
      foreach ($options as $key => $value) {
        if ($key == 'on_exit' || $key == 'on_enter') {
          $options[$key] = array($this, $value);
        }
      }
      $this->create_state($name, $options);
    }

    // Initialize events
    foreach ($workflow['events'] as $name => $options) {
      $this->create_event($name, $options);
    }
  }

  /**
   * Set the appropriate revision id that can be stored in the entity table
   * e.g. which vid is stored in the node table
   *
   * @param array arguments needed for updating the current status.
   * @return boolean
   */
  public function update_entity_state() {

    global $user;
    $user_id = $user->uid;

    $entity = $this->get_object();

    // write history record for all except if a new revision is not being created
    if (!empty($entity->revision)) {
      $this->write_history($user_id);
      $this->write_active();
    }

    $this->state_flow_entity_set_saved();
  }

  public function get_event($key) {
    if (!array_key_exists($key, $this->events)) {
      return FALSE;
    }

    if (is_array($this->events[$key])) {
      $options = $this->events[$key];
      $this->events[$key] = new StateFlow_Event($key, $this, $options);
    }

    return $this->events[$key];
  }

  public function get_object() {
    return $this->object;
  }

  public function set_object($object) {
    $this->object = $object;
  }

  public function get_states_options() {
    return $this->states;
  }

  /**
   * Get the label for the current state
   *
   * @return string
   */
  public function get_label_for_current_state() {
    $state_key = $this->get_current_state();
    $state = $this->get_state($state_key);
    if ($label = $state->get_option('label')) {
      return $label;
    }
    else {
      return drupal_ucfirst($state_key);
    }
  }

  public function get_entity_type() {
    if (isset($this->plugin['handler']['entity_type'])) {
      return $this->plugin['handler']['entity_type'];
    }
  }

  /**
   * Get the revision key for this entity type.
   */
  public function get_revision_key() {

    $entity_type = $this->get_entity_type();
    $entity_info = entity_get_info($entity_type);
    return $entity_info['entity keys']['revision'];
  }

  /**
   * Get the revision key for this entity type.
   */
  public function get_entity_id_key() {

    $entity_type = $this->get_entity_type();
    $entity_info = entity_get_info($entity_type);
    return $entity_info['entity keys']['id'];
  }

  /**
   * Get the state flow history entity.
   */
  public function get_history_entity() {
    if (empty($this->history_entity)) {
      $entity_type = $this->get_entity_type();
      $entity_info = entity_get_info($entity_type);
      $id_key = $entity_info['entity keys']['id'];
      $revision_key = $entity_info['entity keys']['revision'];

      // Make a new empty class for the history entity.
      $history_entity = new stdClass();

      // If this object has a revision key or id key, get it's previous history.
      // If this is a new object, like on the node add form, it won't have a
      // previous history object.
      if (!empty($this->object->$revision_key) || !empty($this->object->$id_key)) {

        $query = db_select('state_flow_history', 'sfh')
                  ->fields('sfh', array('hid'))
                  ->orderBy('hid', 'DESC')
                  ->range(0, 1)
                  ->condition('entity_type', $entity_type);

        // Set a condition for the vid or nid.
        if (!empty($this->object->$revision_key)) {
          $query->condition('revision_id', $this->object->$revision_key);
        }
        elseif (!empty($this->object->$id_key)) {
          $query->condition('entity_id', $this->object->$id_key);
        }

        // Get the results of the query.
        $result = $query->execute()->fetchCol('hid');

        // Load up the fields of the history entity.
        if (!empty($result[0])) {
          $hid = $result[0];
          $history_entities = entity_load('state_flow_history_entity', array('hid' => $hid));
          if (!empty($history_entities[$hid])) {
            $history_entity = clone $history_entities[$hid];
          }
        }

        // Unset the hid and the log message so those don't carry over.
        unset($history_entity->hid);
        unset($history_entity->log);
      }

      // Populate the entity with current information.
      // Not sure how much of this is necessary.
      if (!empty($this->object->$revision_key)) {
        $history_entity->revision_id = $this->object->$revision_key;
      }
      else {
        $history_entity->revision_id = NULL;
      }

      if (!empty($this->object->$id_key)) {
        $history_entity->entity_id = $this->object->$id_key;
      }
      else {
        $history_entity->entity_id = NULL;
      }

      $history_entity->state = $this->get_current_state();
      $history_entity->timestamp = REQUEST_TIME;
      $history_entity->entity_type = $entity_type;
      $this->history_entity = $history_entity;
    }
    return $this->history_entity;
  }

  /**
   * Get available events as an array suitable for FAPI.
   *
   * @param boolean $validate_options
   *   A boolean for whether events should be validated. For instance, a node in
   *   'Draft' state might have 'Needs Review' and 'Published' as available next
   *   states/events. However the currently logged in user might not have permission
   *   to perform the publishing event. Passing TRUE to $validate_events would
   *   exclude the 'Published' option for that user.
   */
  public function get_available_events_options($validate_options = TRUE) {
    $events = $this->get_available_events();
    $return = array();
    foreach ($events as $event_machine_name => $event) {
      if (empty($validate_options) || $event->validate()) {
        $return[$event_machine_name] = $event->get_option('label');
      }
    }
    return $return;
  }

  /**
   * Set the state flow history entity.
   */
  public function set_history_entity($history_entity) {

    $this->history_entity = $this->clean_history_entity($history_entity);
  }

  /**
   * Removes all properties from the history_entity that are not fields or properties
   * declared by Entity API.
   */
  public function clean_history_entity($history_entity) {

    // If entity module is available, uses it's function to get all the properties
    // and fields on the state_flow_history_entity.
    if (module_exists('entity')) {
      $sfhe_properties = array_keys(entity_get_all_property_info('state_flow_history_entity'));
    }
    // Otherwise get the properties and fields directly.
    else {
      $property_info = state_flow_entity_entity_property_info();
      $fields = field_info_instances('state_flow_history_entity', 'state_flow_history_entity');
      $sfhe_properties = array_merge(array_keys($fields), array_keys($property_info['state_flow_history_entity']['properties']));
    }

    // @todo, These should be in the properties already.
    $sfhe_properties = array_merge(array('event', 'event_comment'), $sfhe_properties);

    $clean_history_entity = new stdClass();
    foreach($sfhe_properties as $property) {
      if (!empty($history_entity->{$property})) {
        $clean_history_entity->{$property} = $history_entity->{$property};
      }
    }

    return $clean_history_entity;
  }

  /**
   * Validate the filed API values within a form that has a state_flow_history_enitity.
   */
  public function history_entity_form_field_validate($form, &$form_state) {
    entity_form_field_validate('state_flow_history_entity', $form, $form_state);
  }

  /**
   * Build up a full state flow history entity from form values.
   *
   * Called from state_flow_events_revision_submit().
   */
  public function history_entity_form_submit_build_entity($form, &$form_state) {
    $history_entity = $this->get_history_entity();
    entity_form_submit_build_entity('state_flow_history_entity', $history_entity, $form, $form_state);
    $this->set_history_entity($history_entity);
  }

  /**
   * Provide entire state_flow history.
   *
   * @todo, this method should use dbtng and specify entity_type.
   */
  public function get_full_history() {
    $history = db_query('
    SELECT sfh.*, u.uid, u.name AS user_name
    FROM {state_flow_history} sfh
    LEFT JOIN {users} u ON u.uid = sfh.uid
    WHERE sfh.entity_id = :entity_id
    ORDER BY sfh.timestamp DESC',
          // Add entity_type where
    array(
      ':entity_id' => $this->object->entity_id
    ))->fetchAll();
  return $history;
  }

  /**
   * Extending fire_event() from state_machine's base.inc to add uid and log
   * arguments.
   */
  public function fire_event($key, $uid = NULL, $log = '') {
    $event = $this->get_event($key);
    $history_entity = $this->get_history_entity();
    $history_entity->event = $key;
    $this->set_history_entity($history_entity);

    if ($event && ($new_state = $event->execute())) {
      // Allow the previous state to run its 'on_exit' callbacks.
      $this->get_state($this->get_current_state())->on_exit();

      // Set and save the new state.
      $this->set_current_state($new_state);

      // Write a history record for this state change.
      if (empty($uid)) {
        global $user;
        $uid = $user->uid;
      }
      $this->write_history($uid, $log);

      // Set this revision as active and update the current status in the table.
      $this->write_active();

      // Allow the new state to run its 'on_enter' callbacks.
      $this->get_state($this->get_current_state())->on_enter();

      // Allow the event to "finish".
      $event->finish();

      // Allow state_flow to provide other hooks or event triggers.
      /**
       * @todo the original hook in 7.x-2.x looks like this:
       *    state_flow_invoke_event_handlers($this, $key, $uid, $log);
       * We should review to see if other code uses this hook and update as
       * needed.
       */

      state_flow_entity_invoke_event_handlers($this->object, $new_state, $this->get_history_entity());

      // Update entity tables.
      $this->state_flow_entity_set_saved();
    }
    else {
      $this->on_event_fail($event);
      return FALSE;
    }
  }

  /**
   * Helper function for calling state_flow_entity_set_entity_saved()
   * from update_state
   */
  public function state_flow_entity_set_saved() {

    // Get the published revision and resave it if it is different that the revision
    // in this entity.

    $entity = $this->get_object();
    $revision_key = $this->get_revision_key();
    $entity_id_key = $this->get_entity_id_key();
    $entity_type = $this->get_entity_type();
    $revision_id = $entity->{$revision_key};
    $entity_id = $entity->{$entity_id_key};

    if (!empty($entity->published_revision_id) && $revision_id <> $entity->published_revision_id) {
      $conditions = array($revision_key => $entity->published_revision_id);
      $published_revisions = entity_load($entity_type, array($entity_id), $conditions);
      $published_revision = $published_revisions[$entity_id];
      state_flow_entity_set_entity_saved($entity_type, $entity_id, $published_revision);
    }
  }

  /**
   * Write the transaction to the history table.
   *
   * @param int $uid
   * @param string $log
   * @return object
   */
  public function write_history($uid, $log = '') {
    // start with what the db has
    $history_entity = $this->get_history_entity();

    // @todo, this was added to accomodate the double saving that happens with node forms.
    // reevaluate if necessary.
    if (empty($log) && !empty($history_entity->event_comment)) {
      $log = $history_entity->event_comment;
    }

    $entity_type = $this->get_entity_type();
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $revision_key = $entity_info['entity keys']['revision'];

    // apply current settings
    if (!empty($this->object->$revision_key)) {
      $history_entity->revision_id = $this->object->$revision_key;
    }
    else {
      $history_entity->revision_id = NULL;
    }
    if (!empty($this->object->$id_key)) {
      $history_entity->entity_id = $this->object->$id_key;
    }
    else {
      $history_entity->entity_id = NULL;
    }

    $history_entity->state = $this->get_current_state();
    $history_entity->timestamp = REQUEST_TIME;
    $history_entity->uid = $uid;
    $history_entity->log = $log;
    if (!empty($state)) {
      $history_entity->state = $state;
    }
    $history_entity->entity_type = $entity_type;

    // Often we are writing a new history entity and this property doesn't exist.
    // The way node form submitting requires writing a history on form submit
    // and then updating it in entity_update when the new vid is available. That
    // means a hid might be present.
    if (empty($history_entity->hid)) {
      $primary_keys = array();
    }
    else {
      $primary_keys = array('hid');
    }

    // @todo, tests needed here.
    field_attach_presave('state_flow_history_entity', $history_entity);

    // Update the state_flow_history table
    drupal_write_record('state_flow_history', $history_entity, $primary_keys);
    // Update field tables.
    field_attach_update('state_flow_history_entity', $history_entity);

    $this->set_history_entity($history_entity);
    return $history_entity;
  }

  /**
   * Get the active revision. The active revision is not necessarily the
   * published revision.
   *
   * @todo I am not sure I like this method. confirm that we need this. There
   * may be a better way.
   *
   * @return int revision_id
   */
  function get_active_revision() {
    if (empty($this->object->active_revision_id)) {
      $entity_type = $this->get_entity_type();
      $entity_info = entity_get_info($entity_type);
      $id_key = $entity_info['entity keys']['id'];
      $this->object->active_revision_id = 0;

      $revision_id = db_select('state_flow_states', 'sfs')
        ->fields('sfs', array('revision_id'))
        ->condition('entity_type', $entity_type)
        ->condition('entity_id', $this->object->$id_key)
        ->condition('active', 1)
        ->execute()
        ->fetchCol();
      if (!empty($revision_id[0])) {
        $this->object->active_revision_id = $revision_id[0];
      }
    }
    return $this->object->active_revision_id;
  }

  /**
   * Update the current status record for the revision.  Only one revision
   * may be active at a time.  If additional revisions need to be active,
   * please use fields for now.
   *
   * Using $this->history_entity assumes that write_history() has already
   * been called.
   *
   * called from fire_event() and update_entity_state() when the user is
   * submitting the entity's edit form (like node edit for example).  When
   * called twice we must match the hid but if it's straight from the
   * state_flow_entity transition form
   * @see state_flow_entity_events_revision()
   * we know that the revision exists and is not new
   *
   */
  public function write_active() {
    // first default all revisions to inactive status
    // @todo, can this happen in a less hard-coded fashion to accommodate sites
    // That may need multiple active revisions?
    db_update('state_flow_states')
      ->fields(array(
        'active' => 0,
      ))
      ->condition('entity_id', $this->history_entity->entity_id)
      ->condition('entity_type', $this->history_entity->entity_type)
      ->execute();

    // Store current status, add a new record if it's a new revision.
    if (isset($this->object->revision)) {
      if ($this->object->revision) {
        db_insert('state_flow_states')
          ->fields(array(
            'entity_type' => $this->history_entity->entity_type,
            'entity_id' => $this->history_entity->entity_id,
            'revision_id' => $this->history_entity->revision_id,
            'hid' => $this->history_entity->hid,
            'state' => $this->get_current_state(),
            'active' => 1,
            'timestamp' => REQUEST_TIME,
          ))
          ->execute();
      }
      else {
        /**
         * Set existing revision as active so we can hit this call twice when
         * editing a node. Require hid so that only one record is updated.
         */
        db_update('state_flow_states')
          ->fields(array(
            'active' => 1,
            'state' => $this->get_current_state(),
            'hid' => $this->history_entity->hid,
          ))
          ->condition('entity_id', $this->history_entity->entity_id)
          ->condition('entity_type', $this->history_entity->entity_type)
          ->condition('revision_id', $this->history_entity->revision_id)
          ->execute();
      }
    }
    else {
        /**
         * Not a new revision AND transition only
         * i.e. the user did not come from an entity edit
         * form like node/%node/edit
         * unlike the db_update above, we do not need to require the hid
         * because a transition-only operation only hits this method once
         */
        db_update('state_flow_states')
          ->fields(array(
            'active' => 1,
            'state' => $this->get_current_state(),
            'hid' => $this->history_entity->hid,
          ))
          ->condition('entity_id', $this->history_entity->entity_id)
          ->condition('entity_type', $this->history_entity->entity_type)
          ->condition('revision_id', $this->history_entity->revision_id)
          ->execute();
    }
  }

  public function delete_state_flow_revision($revision_id, $entity_type) {
      // @todo Deleting field data needs testing.
    field_attach_delete_revision('state_flow_history_entity', $this->get_history_entity());
    // Delete entity records.
    db_delete('state_flow_history')
      //->condition('entity_id', $entity_id)
      ->condition('revision_id', $revision_id)
      ->condition('entity_type', $entity_type)
      ->execute();
    db_delete('state_flow_states')
      //->condition('entity_id', $entity_id)
      ->condition('revision_id', $revision_id)
      ->condition('entity_type', $entity_type)
      ->execute();
  }

  /**
   * Provide the current state of this revision or entity.
   *
   * @return string state machine name
   */
  public function load() {
    $state = FALSE;
    $entity_type = $this->get_entity_type();
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $revision_key = $entity_info['entity keys']['revision'];

    if (!empty($this->object->$revision_key)) {
      $state = $this->revision_state($this->object->$id_key, $this->object->$revision_key);
    }
    elseif (!empty($this->object->$id_key)) {
      $state = $this->latest_state($this->object->$id_key);
    }
    return $state;
  }

    // Return the state of the revision
  public function revision_state($entity_id, $revision_id) {
    $latest_state = db_select('state_flow_history', 'sfh')
      ->fields('sfh', array('state'))
      ->condition('entity_id', $entity_id)
      ->condition('revision_id', $revision_id)
      ->orderBy('hid', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchCol('state');

    return !empty($latest_state[0]) ? $latest_state[0] : FALSE;
  }

  public function latest_state($entity_id) {
    $latest_state = db_select('state_flow_history', 'sfh')
      ->fields('sfh', array('state'))
      ->condition('entity_id', $entity_id)
      ->orderBy('hid', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchCol('state');

    return !empty($latest_state[0]) ? $latest_state[0] : FALSE;
  }

  /**
   * Get a default event.
   *
   * When the event form is embedded on the node edit form or elsewhere it may be
   * the case that the state does not change. The method checks for an event that
   * has a target state that matches the current state.
   *
   * @return a string for an event name or NULL.
   */
  public function get_default_event_name() {

    foreach ($this->get_available_events() as $event) {

      $target = $event->get_option('target');
      if ($target === $this->current) {
        return $event->name;
      }
    }

    return NULL;
  }


  /**
  * @todo, this may be the best way to do validation on form submission.
    public function validate_event($event_machine_name, $form, &$form_state) {
      $event = $this->get_event($event_machine_name);
      return $event->validate();
    }
  *
  */


  public function on_exit_scheduled() {
    if (module_exists('state_flow_schedule')) {
      db_query('
        DELETE FROM state_flow_schedule
        WHERE nid = :nid
        AND vid = :vid',
        array(
          ':nid' => $this->object->nid,
          ':vid' => $this->object->vid,
        )
      );
    }
  }

}

/**
 * @todo, should this class be renamed to StateFlow_Event?
 */
class StateFlow_Event extends StateMachine_Event {
  public function get_machine() {
    return $this->machine;
  }

  public function validate() {
    if (parent::validate()) {
      if (!empty($this->options['permissions'])) {
        foreach ($this->options['permissions'] as $permission) {
          if (!user_access($permission)) {
            return FALSE;
          }
        }
      }
      return TRUE;
    }
    return FALSE;
  }
}
