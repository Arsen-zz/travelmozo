<?php

/**
 * @file
 * Settings forms alteration for taxonomy_menu_trails.
 *
 * @author Dmitriy.trt      <http://drupal.org/user/329125>
 */

/**
 * Callback for _taxonomy_menu_trails_sort_by_weight().
 */
function _taxonomy_menu_trails_sort_by_weight_helper($a, $b) {
  $vars = &drupal_static('_taxonomy_menu_trails_sort_by_weight');
  $a_info = field_info_instance($vars['entity_type'], $a, $vars['bundle']);
  $b_info = field_info_instance($vars['entity_type'], $b, $vars['bundle']);
  return $a_info['widget']['weight'] - $b_info['widget']['weight'];
}

/**
 * Sorts instances array by its weight fetched from field info API.
 *
 * @param array $instances
 * @param string $entity_type
 * @param string $bundle
 */
function _taxonomy_menu_trails_sort_by_weight(&$instances, $entity_type, $bundle) {
  $static = &drupal_static(__FUNCTION__, array());
  $static = compact('entity_type', 'bundle');
  // make sure fields info is collated already to workaround warning on 
  // field creation
  _field_info_collate_fields();
  uksort($instances, '_taxonomy_menu_trails_sort_by_weight_helper');
  $static = array();
}

/**
 * Add Taxonomy Menu Trails settings to bundle form.
 *
 * @param array $form
 * @param string $entity_type
 * @param mixed $bundle
 */
function _taxonomy_menu_trails_alter_bundle_form(&$form, $entity_type, $bundle) {
  $bundle = field_extract_bundle($entity_type, $bundle);
  list($settings) = _taxonomy_menu_trails_get_settings($entity_type, $bundle);

  $all_instances = field_info_instances($entity_type, $bundle);
  $instances_options = array();
  foreach ($all_instances as $instance) {
    $field_info = field_info_field($instance['field_name']);
    if ($field_info['type'] == 'taxonomy_term_reference') {
      $instances_options[$instance['field_name']] = $instance['label'];
    }
  }
  
  if (!empty($instances_options)) {
    _taxonomy_menu_trails_sort_by_weight($instances_options, $entity_type, $bundle);

    $subform = array(
      '#tmt_entity' => $entity_type,
      '#tmt_bundle' => $bundle,
      'instances' => array(
        '#type' => 'checkboxes',
        '#title' => t('Term references for setting active trail'),
        '#options' => $instances_options,
        '#default_value' => $settings['instances'],
      ),
      'selection_method' => array(
        // TODO convert it to 'value' if there is one term reference with
        // Number of values == 1
        '#type' => 'select',
        '#title' => t("Term selection method"),
        '#description' => t("This option defines how module chooses term for processing:<ul><li><b>First/Last</b> - select first/last term with menu item.</li><li><b>Deepest in menu</b> - use term with deepest menu item.</ul>"),
        '#default_value' => $settings['selection_method'],
        '#options' => array(
          'first' => t('First term'),
          'last' => t('Last term'),
          'deepest-in-menu' => t('Deepest in menu'),
        ),
        /*TODO depends on http://drupal.org/node/1057748
        '#states' => array(
          'invisible' => array(
            'input[name^="taxonomy_menu_trails[instances]"]' => array('checked' => FALSE),
          ),
        ),*/
      ),
      'only_without_menu' => array(
        '#type' => 'checkbox',
         //TODO replace "entity" with human-friendly entity type
        '#title' => t("Only if entity doesn't have enabled menu item"),
        '#description' => t("You should think twice before turning this option on, because it'll reduce site performance a bit."),
        '#default_value' => $settings['only_without_menu'],
        /*TODO also depends on http://drupal.org/node/1057748
        '#states' => array(
          'invisible' => array(
            'input[name^="taxonomy_menu_trails[instances]"]' => array('checked' => FALSE),
          ),
        ),*/
      ),
      'tm_integration' => module_exists('taxonomy_menu') ? array(
        '#type' => 'checkbox',
        '#title' => t("Get term's path from Taxonomy Menu modules"),
        '#description' => t("If you've configured non-default paths for menu items in the Taxonomy Menu, module will use them. Turning this option on will reduce site performance a bit too."),
        '#default_value' => $settings['tm_integration'],
        /*TODO also depends on http://drupal.org/node/1057748
        '#states' => array(
          'invisible' => array(
            'input[name^="taxonomy_menu_trails[instances]"]' => array('checked' => FALSE),
          ),
        ),*/
      ) : array(
        '#type' => 'value',
        '#value' => $settings['tm_integration'],
      ),
    );
    
    // adding our submit function before default handler because
    // we don't want our elements to be saved as persistent variables
    // by default node type submit handler
    array_unshift($form['#submit'], 'taxonomy_menu_trails_bundle_form_submit');
  }
  else {
    $subform = array(
      'notice' => array(
        '#markup' => '<p>' . t('Add some "Term reference" fields to use Taxonomy Menu Trails.') . '</p>'
      ),
    );
  }
  
  $form['taxonomy_menu_trails'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Taxonomy menu trails'),
  ) + $subform;
}

/**
 * Submit handler for bundle form.
 */
function taxonomy_menu_trails_bundle_form_submit($form, &$state) {
  $entity_type = $form['taxonomy_menu_trails']['#tmt_entity'];
  $old_bundle = $form['taxonomy_menu_trails']['#tmt_bundle'];
  list($settings, $old_var_name, $var_exists) = _taxonomy_menu_trails_get_settings($entity_type, $old_bundle);
  switch ($entity_type) {
    case 'node':
      $new_bundle = $state['values']['type'];
      break;
    default:
      return;
  }
  $new_var_name = 'taxonomy_menu_trails_' . $entity_type . '_' . $new_bundle;
  $values = !empty($state['values']['taxonomy_menu_trails']) ? $state['values']['taxonomy_menu_trails'] : array();
  $settings = array_merge($settings, $values);
  $settings['instances'] = array_filter($settings['instances']);
  if (!empty($settings['instances'])) {
    //sort by instances weight again, because it could be changed since form generation
    //we're using old_bundle because renaming did not processed yet (it will be in original handler)
    _taxonomy_menu_trails_sort_by_weight($settings['instances'], $entity_type, $old_bundle);
    
    variable_set($new_var_name, $settings);
  }
  if ($var_exists && (empty($settings['instances']) || $old_bundle !== $new_bundle)) {
    variable_del($old_var_name);
  }
  unset($state['values']['taxonomy_menu_trails']);
}

/**
 * Add Taxonomy Menu Trails settings to field instance form.
 */
function _taxonomy_menu_trails_alter_field_form(&$form) {
  list($settings) = _taxonomy_menu_trails_get_settings($form['#instance']['entity_type'], $form['#instance']['bundle'], FALSE);

  $form['instance']['taxonomy_menu_trails'] = array(
    '#tree' => FALSE,
    '#type' => 'fieldset',
    '#title' => t('Taxonomy menu trails'),
    'taxonomy_menu_trails_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Use this term reference to set active trail'),
      '#default_value' => !empty($settings['instances'][$form['#field']['field_name']]),
    ),
  );
  $form['#submit'][] = 'taxonomy_menu_trails_field_form_submit';
}

/**
 * Submit handler for field instance form.
 */
function taxonomy_menu_trails_field_form_submit($form, &$state) {
  $entity_type = $form['#instance']['entity_type'];
  $bundle = $form['#instance']['bundle'];
  list($settings, $var_name, $var_exists) = _taxonomy_menu_trails_get_settings($entity_type, $bundle);
  $field_name = $form['#field']['field_name'];
  if (!empty($state['values']['taxonomy_menu_trails_enabled'])) {
    $settings['instances'][$field_name] = $field_name;
    _taxonomy_menu_trails_sort_by_weight($settings['instances'], $entity_type, $bundle);
  }
  else {
    unset($settings['instances'][$field_name]);
  }
  if (!empty($settings['instances'])) {
    variable_set($var_name, $settings);
  }
  elseif ($var_exists) {
    variable_del($var_name);
  }
}

/**
 * Deletes instance from settings and saves them.
 */
function _taxonomy_menu_trails_delete_instance($instance) {
  list($settings, $var_name, $var_exists) = _taxonomy_menu_trails_get_settings($instance['entity_type'], $instance['bundle'], FALSE);
  if ($var_exists) {
    unset($settings['instances'][$instance['field_name']]);
    if (!empty($settings['instances'])) {
      variable_set($var_name, $settings);
    }
    else {
      variable_del($var_name);
    }
    //TODO replace with specific cache entries deleting
    menu_cache_clear_all();
  }
}

/**
 * Add custom submit handler to fields overview form.
 */
function _taxonomy_menu_trails_alter_overview_form(&$form) {
  $form['#submit'][] = 'taxonomy_menu_trails_overview_form_submit';
}

/**
 * Submit handler for fields overview form. Sorts instances and saves bundle settings.
 */
function taxonomy_menu_trails_overview_form_submit($form, &$state) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  list($settings, $var_name, $var_exists) = _taxonomy_menu_trails_get_settings($entity_type, $bundle, FALSE);
  if ($var_exists) {
    _taxonomy_menu_trails_sort_by_weight($settings['instances'], $entity_type, $bundle);
    variable_set($var_name, $settings);
    //TODO replace with specific cache entries deleting
    menu_cache_clear_all();
  }
}
