<?php

/**
 * @file
 * Hooks provided by the XML sitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide information on the type of links this module provides.
 *
 * @see hook_entity_info()
 * @see hook_entity_info_alter()
 */
function hook_xmlsitemap_link_info() {
  return array(
    'mymodule' => array(
      'label' => 'My module',
      'base table' => 'mymodule',
      'entity keys' => array(
        // Primary ID key on {base table}
        'id' => 'myid',
        // Subtype key on {base table}
        'bundle' => 'mysubtype',
      ),
      'path callback' => 'mymodule_path',
      'bundle label' => t('Subtype name'),
      'bundles' => array(
        'mysubtype1' => array(
          'label' => t('My subtype 1'),
          'admin' => array(
            'real path' => 'admin/settings/mymodule/mysubtype1/edit',
            'access arguments' => array('administer mymodule'),
          ),
          'xmlsitemap' => array(
            'status' => XMLSITEMAP_STATUS_DEFAULT,
            'priority' => XMLSITEMAP_PRIORITY_DEFAULT,
          ),
        ),
      ),
      'xmlsitemap' => array(
        // Callback function to take an array of IDs and save them as sitemap
        // links.
        'process callback' => '',
        // Callback function used in batch API for rebuilding all links.
        'rebuild callback' => '',
        // Callback function called from the XML sitemap settings page.
        'settings callback' => '',
      )
    ),
  );
}

/**
 * Alter the data of a sitemap link before the link is saved.
 *
 * @param $link
 *   An array with the data of the sitemap link.
 */
function hook_xmlsitemap_link_alter(&$link) {
  if ($link['type'] == 'mymodule') {
    $link['priority'] += 0.5;
  }
}

/**
 * Index links for the XML sitemaps.
 */
function hook_xmlsitemap_index_links($limit) {
}

/**
 * Provide information about contexts available to XML sitemap.
 *
 * @see hook_xmlsitemap_context_info_alter().
 */
function hook_xmlsitemap_context_info() {
  $info['vocabulary'] = array(
    'label' => t('Vocabulary'),
    'summary callback' => 'mymodule_xmlsitemap_vocabulary_context_summary',
    'default' => 0,
  );
  return $info;
}

/**
 * Alter XML sitemap context info.
 *
 * @see hook_xmlsitemap_context_info().
 */
function hook_xmlsitemap_context_info_alter(&$info) {
  $info['vocabulary']['label'] = t('Site vocabularies');
}

/**
 * Provide information about the current context on the site.
 *
 * @see hook_xmlsitemap_context_alter()
 */
function hook_xmlsitemap_context() {
  $context = array();
  if ($vid = mymodule_get_current_vocabulary()) {
    $context['vocabulary'] = $vid;
  }
  return $context;
}

/**
 * Alter the current context information.
 *
 * @see hook_xmlsitemap_context()
 */
function hook_xmlsitemap_context_alter(&$context) {
  if (user_access('administer taxonomy')) {
    unset($context['vocabulary']);
  }
}

/**
 * Provide options for the url() function based on an XML sitemap context.
 */
function hook_xmlsitemap_context_url_options(array $context) {
}

/**
 * Alter the url() options based on an XML sitemap context.
 */
function hook_xmlsitemap_context_url_options_alter(array &$options, array $context) {
}

/**
 * Alter the query selecting data from {xmlsitemap} during sitemap generation.
 *
 * @param $query
 *   A Query object describing the composite parts of a SQL query.
 *
 * @see hook_query_TAG_alter()
 */
function hook_query_xmlsitemap_generate_alter(QueryAlterableInterface $query) {
  $sitemap = $query->getMetaData('sitemap');
  if (!empty($sitemap->context['vocabulary'])) {
    $node_condition = db_and();
    $node_condition->condition('type', 'taxonomy_term');
    $node_condition->condition('subtype', $sitemap->context['vocabulary']);
    $normal_condition = db_and();
    $normal_condition->condition('type', 'taxonomy_term', '<>');
    $condition = db_or();
    $condition->condition($node_condition);
    $condition->condition($normal_condition);
    $query->condition($condition);
  }
}

/**
 * Provide information about XML sitemap bulk operations.
 */
function hook_xmlsitemap_sitemap_operations() {
}

/**
 * Respond to XML sitemap deletion.
 *
 * This hook is invoked from xmlsitemap_sitemap_delete_multiple() after the XML
 * sitemap has been removed from the table in the database.
 *
 * @param $sitemap
 *   The XML sitemap object that was deleted.
 */
function hook_xmlsitemap_sitemap_delete(stdClass $sitemap) {
  db_query("DELETE FROM {mytable} WHERE smid = '%s'", $sitemap->smid);
}

/**
 * @} End of "addtogroup hooks".
 */
