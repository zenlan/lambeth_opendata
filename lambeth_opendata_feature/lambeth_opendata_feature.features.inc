<?php
/**
 * @file
 * lambeth_opendata_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function lambeth_opendata_feature_node_info() {
  $items = array(
    'lambeth_opendata_document' => array(
      'name' => t('Lambeth OpenData Document'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
