<?php
/**
 * @file
 */

/**
 * Implements hook_entity_property_info()
 */
function lambeth_opendata_entity_property_info() {
  $info = array();
  $properties = &$info['lambeth_opendata_foi']['properties'];
  $properties['Ref'] = array('type' => 'integer', 'label' => 'Ref', 'description' => 'Ref');
  $properties['Received'] = array('type' => 'string', 'label' => 'Received', 'description' => 'Received');
  $properties['Summary'] = array('type' => 'text', 'label' => 'Summary', 'description' => 'Summary');
  return $info;
}
