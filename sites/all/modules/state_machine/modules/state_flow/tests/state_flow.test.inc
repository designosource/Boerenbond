<?php
/**
 * @file
 *
 * Helper functions for tests.
 */


/**
 * Install a simple text field on the state_flow_history_entity.
 */
function state_flow_add_test_text_field() {

  /**
  * The field_config array was generated with this debug code.
  *
  * $field_config = field_info_field('field_sfhe_text');
  * dsm(var_export($field_config, TRUE));
  */

  $field_config = array (
    'translatable' => '0',
    'entity_types' =>
    array (
    ),
    'settings' =>
    array (
      'max_length' => '255',
    ),
    'storage' =>
    array (
      'type' => 'field_sql_storage',
      'settings' =>
      array (
      ),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' =>
      array (
        'sql' =>
        array (
          'FIELD_LOAD_CURRENT' =>
          array (
            'field_data_field_sfhe_text' =>
            array (
              'value' => 'field_sfhe_text_value',
              'format' => 'field_sfhe_text_format',
            ),
          ),
          'FIELD_LOAD_REVISION' =>
          array (
            'field_revision_field_sfhe_text' =>
            array (
              'value' => 'field_sfhe_text_value',
              'format' => 'field_sfhe_text_format',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' =>
    array (
      'format' =>
      array (
        'table' => 'filter_format',
        'columns' =>
        array (
          'format' => 'format',
        ),
      ),
    ),
    'indexes' =>
    array (
      'format' =>
      array (
        0 => 'format',
      ),
    ),
    'id' => '6',
    'field_name' => 'field_sfhe_text',
    'type' => 'text',
    'module' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
    'deleted' => '0',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'varchar',
        'length' => '255',
        'not null' => false,
      ),
      'format' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
      ),
    ),
    'bundles' =>
    array (
      'state_flow_history_entity' =>
      array (
        0 => 'state_flow_history_entity',
      ),
    ),
  );

  field_create_field($field_config);

  /**
  * The field_instance array was generated with this debug code.
  *
  * $field_instance = field_info_instance('state_flow_history_entity', 'field_sfhe_text', 'state_flow_history_entity');
  * dsm(var_export($field_instance, TRUE));
  */

  $field_instance = array (
    'label' => 'sfhe_text',
    'widget' =>
    array (
      'weight' => '2',
      'type' => 'text_textfield',
      'module' => 'text',
      'active' => 1,
      'settings' =>
      array (
        'size' => '60',
      ),
    ),
    'settings' =>
    array (
      'text_processing' => '0',
      'user_register_form' => false,
    ),
    'display' =>
    array (
      'default' =>
      array (
        'label' => 'above',
        'type' => 'text_default',
        'settings' =>
        array (
        ),
        'module' => 'text',
        'weight' => 1,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '8',
    'field_id' => '6',
    'field_name' => 'field_sfhe_text',
    'entity_type' => 'state_flow_history_entity',
    'bundle' => 'state_flow_history_entity',
    'deleted' => '0',
  );

  field_create_instance($field_instance);
}

/**
 * Install a simple integer field on the state_flow_history_entity with a minimum
 * of 1 and a maximum of 10.
 */
function state_flow_add_test_integer_field() {

  /**
   *  $field_config = field_info_field('field_sfhe_integer');
  dsm(var_export($field_config, TRUE));
   */

  $field_config = array (
    'translatable' => '0',
    'entity_types' =>
    array (
    ),
    'settings' =>
    array (
    ),
    'storage' =>
    array (
      'type' => 'field_sql_storage',
      'settings' =>
      array (
      ),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' =>
      array (
        'sql' =>
        array (
          'FIELD_LOAD_CURRENT' =>
          array (
            'field_data_field_sfhe_integer' =>
            array (
              'value' => 'field_sfhe_integer_value',
            ),
          ),
          'FIELD_LOAD_REVISION' =>
          array (
            'field_revision_field_sfhe_integer' =>
            array (
              'value' => 'field_sfhe_integer_value',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' =>
    array (
    ),
    'indexes' =>
    array (
    ),
    'id' => '5',
    'field_name' => 'field_sfhe_integer',
    'type' => 'number_integer',
    'module' => 'number',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
    'deleted' => '0',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'int',
        'not null' => false,
      ),
    ),
    'bundles' =>
    array (
      'state_flow_history_entity' =>
      array (
        0 => 'state_flow_history_entity',
      ),
    ),
  );

  field_create_field($field_config);


  /**
  * The field_instance array was generated with this debug code.
  *
  * $field_instance = field_info_instance('state_flow_history_entity', 'field_sfhe_text', 'state_flow_history_entity');
  * dsm(var_export($field_instance, TRUE));
  */

  $field_instance = array (
    'label' => 'Test integer',
    'widget' =>
    array (
      'weight' => '1',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' =>
      array (
      ),
    ),
    'settings' =>
    array (
      'min' => '1',
      'max' => '10',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' =>
    array (
      'default' =>
      array (
        'label' => 'above',
        'type' => 'number_integer',
        'settings' =>
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 0,
          'prefix_suffix' => true,
        ),
        'module' => 'number',
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '7',
    'field_id' => '5',
    'field_name' => 'field_sfhe_integer',
    'entity_type' => 'state_flow_history_entity',
    'bundle' => 'state_flow_history_entity',
    'deleted' => '0',
  );

  field_create_instance($field_instance);
}
