<?php
/**
 * @file
 * Views integration
 *
 * - define g2_node as a base table
 * - allow g2_node access from node views (g2 fields in node views)
 *
 * @copyright Copyright (C) 2005-2011 Frederic G. MARAND for Ouest Systèmes Informatiques (OSInet, OSI)
 *
 * @license Licensed under the CeCILL, version 2 and General Public License version 2 or later
 *
 * License note: G2 is distributed by OSInet to its customers under the
 * CeCILL 2.0 license. OSInet support services only apply to the module
 * when distributed by OSInet, not by any third-party further down the
 * distribution chain.
 *
 * If you obtained G2 from drupal.org, that site received it under the General
 * Public License version 2 or later (GPLv2+) and can therefore distribute it
 * under the same terms, and so can you and just anyone down the chain as long
 * as the GPLv2+ terms are abided by, the module distributor in that case being
 * the drupal.org organization or the downstream distributor, not OSInet.
 */

/**
 * Implements hook_views_data().
 */
function g2_views_data() {

  $data['g2_node'] = array(
    'table' => array(
      'group' => t('G2'),
      'base' => array(
        'field' => 'nid',
        'title' => t('G2 Glossary'),
        'help' => t('G2 Views are node views limited to glossary entries.'),
      ),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
          // 'type' => 'INNER',
        ),
      ),
    ),
    'nid' => array(
      'field' => array(
        'handler' => 'views_handler_field_node',
        'click sortable' => TRUE,
        'title' => t('Node ID'),
        'help' => t('The ID of the glossary entry'),
      ),
    ),
    'period' => array(
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
        'title' => t('Entry life period'),
        'help' => t('This is plain text, no formatting at all.'),
      ),
    ),
    'complement' => array(
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
        'title' => t('Extra information'),
        'help' => t('This is hidden text, only displayed on the node edit form.'),
      ),
    ),
    'origin' => array(
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
        'title' => t('Origin information'),
        'help' => t('This is hidden text, only displayed on the node edit form'),
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function g2_views_data_alter(&$data) {
  $data['node']['table']['join']['g2_node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
    'type' => 'INNER',
  );
}
