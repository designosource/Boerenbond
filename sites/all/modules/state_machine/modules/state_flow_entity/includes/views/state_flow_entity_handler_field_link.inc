<?php

/**
 * @file
 * Field handler that provides actionable links related to an entity version.
 */

class state_flow_entity_handler_field_link extends state_flow_entity_handler_field_state_flow_states {

  function option_definition() {
    $options = parent::option_definition();
    $options['link_type'] = array('default' => 'revision_delete_path');
    $options['link_text'] = array('default' => 'Delete');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_type'] = array(
      '#type' => 'select',
      '#title' => t('Link type'),
      '#description' => t('Creates a link to edit, delete or change the state of this entity revision.'),
      '#options' => array(
        'revision_edit_path'   => 'edit',
        'revision_delete_path' => 'delete',
        'revision_workflow_path' => 'activate/workflow',
      ),
      '#default_value' => $this->options['link_type'],
      '#weight' => -102,
    );

    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#description' => t('The text to display for this link'),
      '#default_value' => $this->options['link_text'],
      '#weight' => -102,
    );
  }

  public function render($values) {

    // Make sure these options are set
    if (!empty($this->aliases['active']) && !empty($this->options['active_revision_handling'])  ) {
      $active_revision_handling = $this->options['active_revision_handling'];
      $active_alias = $this->aliases['active'];

      // If 'active' matters, then check the active column if necessary.
      if ($active_revision_handling === 'both' ||
      ($active_revision_handling === 'active' && !empty($values->{$active_alias})) ||
      ($active_revision_handling === 'inactive' && empty($values->{$active_alias}))) {

        // Verify that link text and type are set.
        if (!empty($this->options['link_text']) && !empty($this->options['link_type'])) {
          return $this->render_state_flow_states_link($values, check_plain($this->options['link_text']), $this->options['link_type']);
        }

      }
    }
  }
}
