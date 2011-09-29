<?php

$context = new stdClass;
$context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
$context->api_version = 3;
$context->name = 'g2-unprivileged';
$context->description = 'G2 unprivileged context';
$context->tag = 'g2';
$context->conditions = array(
  'g2' => array(
    'values' => array(
      'g2_node' => 'g2_node',
      'g2_user' => 'g2_user',
    ),
  ),
);
$context->reactions = array(
  'g2_template' => array(
    'g2-template' => 1,
  ),
  'theme_html' => array(
    'class' => 'context-g2',
  ),
);
$context->condition_mode = 0;


// Translatables
// Included for use with string extractors like potx.
t('G2 unprivileged context');
t('g2');
