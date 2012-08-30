<?php
/**
 * @file
 * Utilities for the Lambeth Open Data module.
 *
 * @ingroup
 */

global $_lambeth_opendata_public_dir;
$_lambeth_opendata_public_dir = 'public://lambeth/';

global $_lambeth_opendata_types;
$baseurl = 'http://www.lambeth.gov.uk/Services/CouncilDemocracy/DataProtectionFOI/';
$_lambeth_opendata_types_defaults = array(
  'foi' => array(
    'label' => 'Freedom of Information',
    'url' => $baseurl . 'FoIDisclosureLog.htm',
  ),
  'locations' => array(
    'label' => 'Locations',
    'url' => $baseurl . 'LocationOpenData.htm',
  ),
  'planning' => array(
    'label' => 'Planning Permissions',
    'url' => $baseurl . 'PlanningPermissionsOpenData.htm',
  ),
  'spending' => array(
    'label' => 'Expenditure',
    'url' => $baseurl . 'ExpenditureOpenDataCSV.htm',
  ),
);

$_lambeth_opendata_types = unserialize(variable_get('lambeth_opendata_types', serialize($_lambeth_opendata_types_defaults)));

function lambeth_opendata_get_page_contents($url) {
  if (!$result = file_get_contents($url)) {
    watchdog('lambeth_opendata', t('Error retrieving contents from !url', array('!url', $url)));
    return FALSE;
  }
  return $result;
}
