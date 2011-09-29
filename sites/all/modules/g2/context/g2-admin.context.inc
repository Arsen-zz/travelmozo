<?php
$context = new stdClass;
$context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
$context->api_version = 3;
$context->name = 'g2-admin';
$context->description = 'G2 privileged context';
$context->tag = 'g2';
$context->conditions = array(
  'g2' => array(
    'values' => array(
      'g2_admin' => 'g2_admin',
    ),
  ),
);
$context->reactions = array(
  'theme_html' => array(
    'class' => 'context-g2-admin',
  ),
);
$context->condition_mode = 0;

// Translatables
// Included for use with string extractors like potx.
t('Context d\'admin g2');
t('g2');
