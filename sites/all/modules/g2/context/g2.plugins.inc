<?php
/**
 * Context registry.
 */
function _g2_context_registry() {
  $registry = array();
  $registry['conditions'] = array(
    'g2' => array(
      'title' => t('G2 Glossary'),
      'description' => t('Set this context on G2 pages and node pages when node type is %type.', array('%type' => G2\NODETYPE)),
      'plugin' => 'context_condition_g2',
    ),
  );

  $registry['reactions'] = array(
    'g2_template' => array(
      'title' => t('G2 Glossary'),
      'description' => t('Enable the page-g2 template as a page template.'),
      'plugin' => 'context_reaction_g2_template',
    ),
  );

  // dsm($registry, __FUNCTION__);
  return $registry;
}

/**
 * Context plugins.
 */
function _g2_context_plugins() {
  $plugins = array();
  $path = drupal_get_path('module', 'g2') . '/plugins';

  /**
   * Conditions.
   */
  $plugins['context_condition_g2'] = array(
    'handler' => array(
      'file' => 'context_condition_g2.inc',
      'class' => 'context_condition_g2',
    ),
  );

  $plugins['context_reaction_g2_template'] = array(
    'handler' => array(
      'file' => 'context_reaction_g2_template.inc',
      'class' => 'context_reaction_g2_template',
    ),
  );

  foreach ($plugins as $name => &$plugin) {
    $plugin['handler']['path'] = $path;
  }

  // dsm($plugins, __FUNCTION__);
  return $plugins;
}
