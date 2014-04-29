<?php

/**
 * @file
 * State Flow implementation of the State Machine class
 */

class StateFlowNode extends StateFlowEntity {
  /**
   * Called from StateMachine::__construct to initialize the states and events.
   * Define two states.
   * First revision:
   *  - Expose go to draft button
   *  - Expose go to publish button
   *  - Upon publish, create new revision (handled outside of state machine)
   * Second revision:
   *  - Menu alter edit link to load most recent revision (whether published or revision from states)
   *  - On hook_nodeapi (op: load), force new revision checkbox on node edit form
   *    - Expose go to draft button
   *  - Create new revision, prevent node table from updating new revision as published revision
   *  - Expose go to publish button
   *  - Upon publish, set revision id in node table
   *  - Repeat as needed
   */

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

    // find publishable vid
    $this->set_available_publish_revision();

    // set latest history
    if (!empty($this->object) && isset($this->object->vid)) {
      $this->set_current_history();
    }
  }

  /**
   * Set the vid from the node table as the "Published" revision.
   *
   * If the node is not currently published then there is no Published revision.
   * It only needs to set this id when the entity first loads.  Keep that test
   * to see if it's already set or it gets confused when creating new
   * revisions.
   */
  public function set_available_publish_revision() {
    if (!isset($this->object->published_revision_id) && !empty($this->object->nid)) {
      $this->object->published_revision_id = 0;
      $published_revision = db_select('node')
        ->fields('node', array('vid'))
        ->condition('nid', $this->object->nid)
        ->condition('status', 1)
        ->execute()->fetchCol();
      if (!empty($published_revision)) {
        $this->object->published_revision_id = $published_revision[0];
      }
    }
  }

  public function on_enter_published() {
    state_flow_entity_set_entity_saved('node', $this->object->nid, $this->object);
    $this->object->published_revision_id = $this->object->vid;
  }


  /**
   * Callback for when a node enters the unpublished state. Make sure this
   * revision is the one going to the node table.
   */
  public function on_enter_unpublished() {
    // "published_revision_id" is somewhat of a misnomer here. It's really
    // "the revision_id that is in the node table."
    $this->object->published_revision_id = $this->object->vid;
    $this->object->revision = FALSE;
    $this->object->state_flow_node_on_enter_unpublished = TRUE;
    node_save($this->object);
  }

  public function on_exit_published() {
    $this->set_unpublished();
  }

  /*
   * Set revision to published meaining, the revision is in the entities
   * table and is enabled to be viewed by others.  For example, for nodes we
   * would update the node table to use this vid and set status = 1
   *
   * this method is also called to make sure the currently published
   * revision continues to stay published during other events
   *
   * @todo needs to be smarter about node form submit so it does not node_save()
   * here when node_sumit ddoes it for us. dont' need to call on_enter_published()
   * when the user edits a node and is pushing the new revision to published
   */
  public function set_published($reset = FALSE) {
    $entity = $this->object;
    if (isset($this->object->nid)) {
      $entity_id = $this->object->nid;
    }
    if (isset($this->object->vid)) {
      $revision_id = $this->object->vid;
    }
    $entity_type = $this->plugin['handler']['entity_type'];
    $entity_info = entity_get_info($entity_type);
    $revision_key = $entity_info['entity keys']['revision'];

    // get currently published revision
    if (!empty($entity_id)) {
      $published_revisions = entity_load($entity_type, array($entity_id), array($revision_key => $revision_id));
      $published_revision = $published_revisions[$entity_id];
    }
    else {
      // going straight from node creation to published, no ids exist yet
      $published_revision = $this->object;
    }
    // only set revision to false if we are "re-saving"
    if ($reset) {
      $published_revision->revision = FALSE;
    }
    // this is the little bit that ers doesn't have that state flow wants
    $published_revision->status = 1;
    $published_revision->state_flow_node_set_published = TRUE;

    node_save($published_revision);
    // _node_save_revision() will completely overwrite the UID of the revision
    // which we absolutely do no want in this context. So we store that
    // UID and restore it after the node_save.
    if (isset($published_revision->revision_uid)) {
      $uid = $published_revision->revision_uid;
      if ($uid != $GLOBALS['user']->uid) {
        db_update('node_revision')
          ->fields(array('uid' => $uid))
          ->condition('vid', $entity->published_revision_id)
          ->execute();
      }
    }

    $entity = $published_revision;

    // Update the node access table for this node.
    $node = node_load($this->object->nid, $this->object->vid, TRUE);
    node_access_acquire_grants($node);

    //rebuild taxonomy index
    if (function_exists('taxonomy_delete_node_index')) {
      taxonomy_delete_node_index($node);
    }
    if (function_exists('taxonomy_build_node_index')) {
      taxonomy_build_node_index($node);
    }
  }

  public function set_unpublished() {
    $this->object->status = 0;

    // Set the unpublished status on the node_revision record for this revision
    $res = db_update('node_revision')
      ->fields(array(
        'status' => $this->object->status,
      ))
      ->condition('vid', $this->object->vid)
      ->execute();

    // If the revision to unpublish is the latest published revision for the
    // node, then unpublish the node itself
    if ($this->object->vid == $this->object->published_revision_id) {
      // Set the published status on the node record for this node
      $res = db_update('node')
        ->fields(array(
          'status' => $this->object->status,
        ))
        ->condition('nid', $this->object->nid)
        ->execute();
    }

    // Update the node access table for this node.
    $node = node_load($this->object->nid, $this->object->vid, TRUE);
    node_access_acquire_grants($node);
  }

  public function get_latest_revision($nid) {
    $result = db_query('SELECT MAX(vid) FROM {node_revision} WHERE nid = :nid', array(':nid' => $nid))->fetchCol('vid');
    return !empty($result[0]) ? $result[0] : FALSE;
  }

  /**
   * Provide history with each revision
   */
  // @todo, this does not need to be in state_flow_node.inc. It could easily get
  // abstracted to state_flow_entity.inc
  public function set_current_history() {
    $result = db_select('state_flow_history', 'sfh')
      ->fields('sfh', array('state', 'timestamp', 'log'))
      ->condition('entity_id', $this->object->nid)
      ->condition('revision_id', $this->object->vid)
      ->orderBy('timestamp', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchObject();
    if ($result) {
      $this->object->current_state = $result->state;
      $this->object->current_log = $result->log;
      $this->object->current_state_timestamp = $result->timestamp;
    }
    else {
      $this->object->current_state = '';
      $this->object->current_log = '';
      $this->object->current_state_timestamp = '';
    }
  }

  /**
   * Provide the current state of this revision or entity.
   *
   * @return string machine name of a state.
   */
  public function load() {
    $state = parent::load();
    // When State Flow is installed on an existing site there may be nodes already
    // published. Those nodes should be considered as "published" by State Flow.
    if (empty($state) && !empty($this->object->status) && !empty($this->object->nid)) {
      return 'published';
    }
    return $state;
  }
}
