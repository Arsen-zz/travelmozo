<?php
/**
 * @file
 * Default contexts.
 *
 * This file should really be named g2.context_defaults.inc once this is fixed:
 * http://drupal.org/node/1242632
 */

/**
 * Implements hook_context_default_contexts().
 *
 * This is a ctools defaults hook.
 */
function g2_context_default_contexts() {
  $path = drupal_get_path('module', 'g2') . '/context';
  $contexts = file_scan_directory($path, '/^g2-.*\.context\.inc$/');
  $ret = array();
  foreach ($contexts as $k => $v) {
    if (file_exists(DRUPAL_ROOT . '/' . $k)) {
      require_once(DRUPAL_ROOT . '/' . $k);
    $ret[$context->name] = $context;
    }
  }
  return $ret;
}
