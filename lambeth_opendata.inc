<?php

/**
 * @file
 * Utilities for the Lambeth Open Data module.
 *
 * @ingroup
 */
global $_lambeth_opendata_private_dir;
$_lambeth_opendata_private_dir = 'private://lambeth_opendata/';

global $_lambeth_opendata_public_dir;
$_lambeth_opendata_public_dir = 'public://lambeth_opendata/';

global $_lambeth_opendata_types;
$_lambeth_opendata_types = lambeth_opendata_get_document_types();

function lambeth_opendata_get_document_types($reset = FALSE) {
  $result = variable_get('lambeth_opendata_types', array());
  if ($reset || empty($result)) {
    lambeth_opendata_cache_document_types();
    $result = variable_get('lambeth_opendata_types', array());
  }
  return $result;
}

function lambeth_opendata_cache_document_types() {
  $result = array();
  $vocab = taxonomy_vocabulary_machine_name_load('lambeth_opendata_category');
  $tree = taxonomy_get_tree($vocab->vid, 0, 1, TRUE);
  foreach ($tree as $term) {
    $result[$term->name] = array(
      'label' => $term->field_document_type_label[LANGUAGE_NONE][0]['value'],
      'url' => $term->field_document_type_origin['und'][0]['value'],
      'tid' => $term->tid,
    );
  }
  variable_set('lambeth_opendata_types', $result);
}

function lambeth_opendata_get_page_contents($url) {
  if (!$result = file_get_contents($url)) {
    watchdog('lambeth_opendata', t('Error retrieving contents from !url', array('!url', $url)));
    return FALSE;
  }
  return $result;
}

function lambeth_opendata_rewrite_datadoc($type, $url, $name) {
  return call_user_func_array('lambeth_opendata_rewrite_' . $type, array($url, $name));
}

function lambeth_opendata_rewrite_locations($url, $name) {
  if (!preg_match('/Lambeth(.*)/', $name, $matches)) {
    watchdog('lambeth_opendata_rewrite_locations', 'Failed to parse file !name', array('!name' => $name));
    return FALSE;
  }
  $type = $matches[1];
  $fields = lambeth_opendata_map_locations_fields($type);
  if (empty($fields)) {
    watchdog('lambeth_opendata_rewrite_locations', 'Failed to map fields for file !name', array('!name' => $name));
    return FALSE;
  }
  if (!$data = lambeth_opendata_csv_readfile($url)) {
    watchdog('lambeth_opendata_rewrite_locations', 'Failed to read file !name', array('!name' => $name));
    return FALSE;
  }
  foreach ($data[0] as $k => $v) {
    if (in_array($v, $fields)) {
      $header[$k] = array_search($v, $fields);
    }
  }
  unset($data[0]);
  ksort($data);
  $rows = array();
  foreach ($data as $k => $v) {
    $tmp = '';
    foreach ($header as $i => $value) {
      $rows[$k][$i] = $v[$i];
      $tmp .= $v[$i];
    }
    if (!empty($tmp)) {
      $rows[$k][] = sha1($type) . '_' . str_pad($k, 4, '0', STR_PAD_LEFT);
      $rows[$k][] = $type;
    }
    else {
      unset($rows[$k]);
    }
  }
  $header[] = 'index';
  $header[] = 'type';
  if (empty($rows)) {
    watchdog('lambeth_opendata_rewrite_locations', 'No rows to rewrite for file !name', array('!name' => $name));
    return FALSE;
  }
  return lambeth_opendata_csv_writefile($name, $header, $rows);
}

function lambeth_opendata_rewrite_spending($url, $name) {
  if (!preg_match('/LambethPaymentsOver500(.*)/', $name, $matches)) {
    watchdog('lambeth_opendata_rewrite_spending', 'Failed to parse file !name', array('!name' => $name));
    return FALSE;
  }
  if (!$data = lambeth_opendata_csv_readfile($url)) {
    watchdog('lambeth_opendata_rewrite_spending', 'Failed to read file !name', array('!name' => $name));
    return FALSE;
  }
  $array = date_parse_from_format('jMYY', '1' . $matches[1]);
  $timestamp = mktime(0, 0, 0, $array['month'], $array['day'], $array['year']);
  $month = date('M', $timestamp);
  $year = date('Y', $timestamp);
  $header = array('Supplier', 'Total', 'Index', 'Month', 'Year');
  $data = array_splice($data, 4);
  $rows = array();
  foreach ($data as $k => $v) {
    $v[0] = trim($v[0]);
    if (empty($v[0]) || ($v[0] == 'Grand Total')) {
      continue;
    }
    if (empty($rows) && count($v) !== 2) {
      watchdog('lambeth_opendata_rewrite_spending', 'Wrong column count in file !name', array('!name' => $name));
      return FALSE;
    }
    $rows[$k][0] = $v[0];
    $rows[$k][1] = ltrim(str_replace(',', '', $v[1]), '£');
    $rows[$k][2] = $timestamp + $k;
    $rows[$k][3] = $month;
    $rows[$k][4] = $year;
  }
  if (empty($rows)) {
    watchdog('lambeth_opendata_rewrite_spending', 'No rows to rewrite for file !name', array('!name' => $name));
    return FALSE;
  }
  return lambeth_opendata_csv_writefile($name, $header, $rows);
}

function lambeth_opendata_rewrite_planning($url, $name) {
  if (!$data = lambeth_opendata_csv_readfile($url)) {
    watchdog('lambeth_opendata_rewrite_planning', 'Failed to read file !name', array('!name' => $name));
    return FALSE;
  }
  $header = $data[0];
  unset($data[0]);
  foreach ($data as $k => $v) {
    if (empty($v[3])) {
      continue;
    }
    $rows[$k][0] = $v[0];
    $rows[$k][1] = $v[1];
    $rows[$k][2] = $v[2];
    $rows[$k][3] = lambeth_opendata_reformat_date_string($v[3], 'd-M-y');
    $rows[$k][4] = $v[4];
  }
  if (empty($rows)) {
    watchdog('lambeth_opendata_rewrite_planning', 'No rows to rewrite for file !name', array('!name' => $name));
    return FALSE;
  }
  return lambeth_opendata_csv_writefile($name, $header, $rows);
}

function lambeth_opendata_rewrite_foi($url, $name) {
  if (!$data = lambeth_opendata_csv_readfile($url)) {
    watchdog('lambeth_opendata_rewrite_foi', 'Failed to read file !name', array('!name' => $name));
    return FALSE;
  }
  $header = $data[1];
  unset($data[0]);
  unset($data[1]);
  foreach ($data as $k => $v) {
    if (empty($v[1])) {
      continue;
    }
    $rows[$k][0] = $v[0];
    $rows[$k][1] = lambeth_opendata_reformat_date_string($v[1], 'd/m/Y');
    $rows[$k][2] = $v[2];
  }
  if (empty($rows)) {
    watchdog('lambeth_opendata_rewrite_foi', 'No rows to rewrite for file !name', array('!name' => $name));
    return FALSE;
  }
  return lambeth_opendata_csv_writefile($name, $header, $rows);
}

function lambeth_opendata_reformat_date_string($string, $format) {
  $array = date_parse_from_format($format, $string);
  $timestamp = mktime(0, 0, 0, $array['month'], $array['day'], $array['year']);
  $timezone = date_default_timezone_get();
  date_default_timezone_set("UTC");
  $utc = date("Y-m-d\TG:i:s\Z", $timestamp);
  date_default_timezone_set($timezone);
  return $utc;
}

function lambeth_opendata_map_locations_fields($type) {

  switch ($type) {
    case 'AdventurePlaygrounds':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'telephone' => 'TELEPHONE_',
          //'other_1'=>'UPRN',
      );
//      'MAP_EAST'
//      'MAP_NORTH'
      break;
    case 'CarParks':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'openhours' => 'OPENING_TIMES',
      );
      break;
    case 'ChildrensCentres':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
      );
      break;
    case 'CommunityToilets':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'telephone' => 'TELEPHONE',
        'openhours' => 'OPENING_HOURS',
        'other_1' => 'TOILETS',
        'other_2' => 'AREA',
      );
//X
//Y
      break;
    case 'CoreOfficeBuildings':
      $fields = array(
        'name' => 'OFFICE',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
      );
//EASTING
//NORTHING
      break;
    case 'FoodGrowingProjects':
      $fields = array(
        'name' => 'Name',
        'address' => 'Description',
        'postcode' => 'Postcode',
        'other_1' => 'No_',
        'other_2' => 'Note_',
      );
//Easting
//Northing
      break;
    case 'LeisureCentres':
      $fields = array(
        'name' => 'CENTRE NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'telephone' => 'TELEPHONE',
      );
//MAP_EAST
//MAP_NORTH
      break;
    case 'Libraries':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'telephone' => 'TELEPHONE',
      );
//MAP_EAST
//MAP_NORTH
      break;
    case 'ParkingCCTVCameras':
      $fields = array(
        'name' => 'LOCATION',
      );
//X
//Y
      break;
    case 'PoliceStations':
      $fields = array(
        'name' => 'POLICE STATION NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'telephone' => 'TELEPHONE',
        'openhours' => 'HOURS',
      );
      break;
    case 'PollingStations':
      $fields = array(
        'name' => 'VENUE',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'other_1' => 'WARD',
        'other_2' => 'DISTRICT_CODE',
      );
//EASTING
//NORTHING
      break;
    case 'PublicConveniences':
      $fields = array(
        'name' => 'BUILDING_NAME',
        'address' => 'PROPERTY',
        'other_1' => 'TYPE',
      );
//EASTING
//NORTHING
      break;
    case 'RecyclingBanks':
      $fields = array(
        'name' => 'LOCATION',
        'other_1' => 'CO_MINGLED',
//        'other_2'=>'',
//        'other_3'=>'',
//        'other_4'=>'',
//        'other_5'=>'',
      );
//MATERIALS
//TEXTILES
//SHOES
//BOOKS
//MEDIA
//PRINTER_CARTRIDGES
//WASTE_ELECTRICAL
//EASTING
//NORTHING
      break;
    case 'Schools':
      $fields = array(
        'name' => 'NAME',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'other_1' => 'TYPE',
        'other_2' => 'ACADEMY',
//        'other_3'=>'',
//        'other_4'=>'',
//        'other_5'=>'',
      );
//MAP_EAST
//MAP_NORTH
      break;
    case 'WorkReadyLocations':
      $fields = array(
        'name' => 'Organisation',
        'address' => 'Address 1',
        'postcode' => 'Postcode',
        'telephone' => 'Telephone',
        'other_1' => 'Contact Name',
        'other_2' => 'Email',
        'other_3' => 'Website',
        'other_4' => 'Description of provision',
        'other_5' => 'Target group',
      );
      break;
    default;
      $fields = array();
  }
  return $fields;
}

function lambeth_opendata_csv_writefile($file, $header, $rows) {
  global $_lambeth_opendata_private_dir;
  file_prepare_directory($_lambeth_opendata_private_dir, FILE_CREATE_DIRECTORY);
  $path = drupal_realpath($_lambeth_opendata_private_dir . $file . '.csv');
  if ($fh = @fopen($path, 'w')) {
    if ($bytes = fputcsv($fh, $header)) {
      foreach ($rows AS $k => $v) {
        if (is_array($v)) {
          if (!($bytes = fputcsv($fh, $v))) {
            break;
          }
        }
      }
    }
    fclose($fh);
    return $path;
  }
  return FALSE;
}

function lambeth_opendata_csv_readfile($file) {
  if ($fh = @fopen($file, 'r')) {
    while ($rows[] = fgetcsv($fh)) {

    }
    return $rows;
  }
  return FALSE;
}

function lambeth_opendata_get_type_options($withall = 'all') {
  global $_lambeth_opendata_types;
  $types = $withall ? array(0 => $withall) : array();
  foreach ($_lambeth_opendata_types as $type => $info) {
    $types[$info['tid']] = $type;
  }
  ksort($types);
  return $types;
}

//
//class LambethOpendataConf {
//
//  private $conf_file;
//  private $conf_path;
//  private $conf_array;
//
//  public function __construct($filename = NULL, $filepath = NULL) {
//    if (is_null($filename)) {
//      $filename = 'lambeth_opendata.xml';
//    }
//    if (is_null($filepath)) {
//      $filepath = DRUPAL_ROOT . '/' . drupal_get_path('module', 'lambeth_opendata') . '/conf/';
//    }
//    $this->conf_path = rtrim(trim($filepath), '/') . '/';
//    $this->conf_file = $this->conf_path . trim($filename);
//    $this->conf_array = array();
//  }
//
//  public function get_conf_path() {
//    return $this->conf_path;
//  }
//
//  public function get_conf_file() {
//    return $this->conf_file;
//  }
//
//  public function get_conf_array() {
//    return $this->conf_array;
//  }
//
//  public function get_conf_datasets() {
//    return $this->conf_array['datastore']['datasets'];
//  }
//
//  public function get_conf_dataset_array($dataset) {
//    return $this->conf_array['datastore']['datasets'][$dataset]['tables'];
//  }
//
//  public function load_conf() {
//    $result = array();
//    try {
//      $result = $this->parse_main_conf();
//      foreach ($result['datastore']['datasets'] as $k => $v) {
//        $conf_file = $this->conf_path . $v['conf'];
//        $result['datastore']['datasets'][$k]['tables'] = $this->parse_dataset_conf($this->conf_path . $v['conf']);
//      }
//      $result['conf_file'] = $this->conf_file;
//      $result['conf_path'] = $this->conf_path;
//      $this->conf_array = $result;
//      //file_put_contents(__DIR__ . '/conf_array.txt', print_r($result,1));
//      return $this->conf_array;
//    } catch (Exception $e) {
//      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
//    }
//    return FALSE;
//  }
//
//  private function parse_main_conf() {
//    try {
//      if (!file_exists($this->conf_file)) {
//        watchdog('lambeth_opendata', 'Not found: !file', array('!file' => $this->conf_file));
//      }
//      else {
//        $xml = simplexml_load_file($this->conf_file);
//        $result = array();
//        foreach ($xml->attributes() as $name => $value) {
//          $result[$name] = (string) $xml->attributes()->$name;
//        }
//        $source = $xml->xpath('//schema/source');
//        while (list(, $node) = each($source)) {
//          foreach ($node as $k => $v) {
//            $result['source'][$k] = (string) $v;
//          }
//        }
//        foreach ($xml->xpath('//schema/datastore') as $node) {
//          foreach ($node->attributes() as $name => $value) {
//            $result['datastore'][$name] = (string) $node->attributes()->$name;
//          }
//          foreach ($node as $name => $props) {
//            foreach ($props as $k => $v) {
//              $result['datastore']['datasets'][$name][$k] = (string) $v;
//            }
//          }
//        }
//        return $result;
//      }
//    } catch (Exception $e) {
//      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
//    }
//    return FALSE;
//  }
//
//  public function parse_dataset_conf($conf_file) {
//    try {
//      if (!file_exists($conf_file)) {
//        watchdog('lambeth_opendata', 'Not found: !file', array('!file' => $conf_file));
//      }
//      else {
//        $xml = simplexml_load_file($conf_file);
//        $result = array();
//        foreach ($xml->attributes() as $name => $value) {
//          $result[$name] = (string) $xml->attributes()->$name;
//        }
//        foreach ($xml->xpath('//dataset/property') as $node) {
//          $property = NULL;
//          foreach ($node->attributes() as $name => $value) {
//            $property[$name] = (string) $node->attributes()->$name;
//          }
//          if (is_array($property) && isset($property['name'])) {
//            $result['properties'][$property['name']] = $property;
//          }
//        }
//        foreach ($xml->xpath('//dataset/map') as $node) {
//          $table = array('fields' => array());
//          foreach ($node->attributes() as $name => $value) {
//            $table[$name] = (string) $node->attributes()->$name;
//          }
//          foreach ($node as $fields) {
//            $field = array();
//            foreach ($fields->attributes() as $k => $v) {
//              $field[$k] = (string) $v;
//            }
//            $table['fields'][$field['key']] = $field;
//          }
//          $result['maps'][$table['name']] = $table;
//        }
//        return $result;
//      }
//    } catch (Exception $e) {
//      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
//    }
//    return FALSE;
//  }
//
//  function xml_to_array($xml) {
//    $result = array();
//    foreach ($xml->attributes() as $name => $value) {
//      $result[$name] = (string) $xml->attributes()->$name;
//    }
//    for ($xml->rewind(); $xml->valid(); $xml->next()) {
//      if (!array_key_exists($xml->key(), $result)) {
//        $result[$xml->key()] = array();
//      }
//      if ($xml->hasChildren()) {
//        $result[$xml->key()][] = lambeth_opendata_xml_to_array($xml->current());
//      }
//      else {
//        $xlm = $xml->current();
//        $val = '';
//        foreach ($xlm->attributes() as $name => $value) {
//          $val[$name] = (string) $xlm->attributes()->$name;
//        }
//        if (empty($val)) {
//          $val = strval($xlm);
//        }
//        $result[$xml->key()] = $val;
//      }
//    }
//    return $result;
//  }
//
//}