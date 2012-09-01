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
  $properties['Ref'] = array('type' => 'integer', 'label' => 'Ref', 'description' => 'Ref',
    'datasource controller' => 'LambethOpenDataEntityDataSourceController',
  );
  $properties['Received'] = array('type' => 'date', 'label' => 'Received', 'description' => 'Received');
  $properties['Summary'] = array('type' => 'text', 'label' => 'Summary', 'description' => 'Summary');

  $properties = &$info['lambeth_opendata_locations']['properties'];
  $properties['index'] = array('type' => 'text', 'label' => 'index', 'description' => 'index');
  $properties['type'] = array('type' => 'text', 'label' => 'type', 'description' => 'type');
  $properties['name'] = array('type' => 'text', 'label' => 'name', 'description' => 'name');
  $properties['address'] = array('type' => 'text', 'label' => 'address', 'description' => 'address');
  $properties['postcode'] = array('type' => 'text', 'label' => 'postcode', 'description' => 'postcode');
  $properties['telephone'] = array('type' => 'text', 'label' => 'telephone', 'description' => 'telephone');
  $properties['openhours'] = array('type' => 'text', 'label' => 'openhours', 'description' => 'openhours');
  $properties['other_1'] = array('type' => 'text', 'label' => 'other_1', 'description' => 'other_1');
  $properties['other_2'] = array('type' => 'text', 'label' => 'other_2', 'description' => 'other_2');
  $properties['other_3'] = array('type' => 'text', 'label' => 'other_3', 'description' => 'other_3');
  $properties['other_4'] = array('type' => 'text', 'label' => 'other_4', 'description' => 'other_4');
  $properties['other_5'] = array('type' => 'text', 'label' => 'other_5', 'description' => 'other_5');

  $properties = &$info['lambeth_opendata_spending']['properties'];
  $properties['Index'] = array('type' => 'integer', 'label' => 'Index', 'description' => 'Index');
  $properties['Total'] = array('type' => 'decimal', 'label' => 'Total', 'description' => 'Total');
  $properties['Supplier'] = array('type' => 'text', 'label' => 'Supplier', 'description' => 'Supplier');
  $properties['Month'] = array('type' => 'text', 'label' => 'Month', 'description' => 'Month');
  $properties['Year'] = array('type' => 'text', 'label' => 'Year', 'description' => 'Year');


  $properties = &$info['lambeth_opendata_planning']['properties'];
  $properties['REFVAL'] = array('type' => 'text', 'label' => 'REFVAL', 'description' => 'REFVAL');
  $properties['DATEDECISS'] = array('type' => 'date', 'label' => 'DATEDECISS', 'description' => 'DATEDECISS');
  $properties['ADDRESS'] = array('type' => 'text', 'label' => 'ADDRESS', 'description' => 'ADDRESS');
  $properties['PROPOSAL'] = array('type' => 'text', 'label' => 'PROPOSAL', 'description' => 'PROPOSAL');
  $properties['DECSN'] = array('type' => 'text', 'label' => 'DECSN', 'description' => 'DECSN');

  return $info;
}
