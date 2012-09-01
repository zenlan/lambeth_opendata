<?php
/**
 * @file
 * lambeth_opendata_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function lambeth_opendata_feature_taxonomy_default_vocabularies() {
  return array(
    'lambeth_opendata_category' => array(
      'name' => 'Lambeth Open Data Category',
      'machine_name' => 'lambeth_opendata_category',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
