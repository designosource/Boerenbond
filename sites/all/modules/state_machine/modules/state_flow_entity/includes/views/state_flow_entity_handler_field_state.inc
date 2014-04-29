<?php

/**
 * @file
 * Field handler for states. Takes state machine name columns and renders the labels.
 */

class state_flow_entity_handler_field_state extends views_handler_field {

  public function render($values) {
    $return = '';

    $value = $this->get_value($values);
    if (!empty($value)) {
      $state_labels = state_flow_entity_state_labels();
      if (!empty($state_labels[$value])) {
        // @todo, check_plain might not be necessary as escaping could happen
        // later. Until I know otherwise, I'll stay on the safe side.
        $return = check_plain($state_labels[$value]);
      }
    }

    return $return;
  }
}

