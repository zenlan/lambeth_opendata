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


function lambeth_opendata_rewrite_datadoc($type, $url, $name) {
  return call_user_func_array('lambeth_opendata_rewrite_' . $type, array($url, $name));
}

function lambeth_opendata_rewrite_locations($url, $name) {
  $result = preg_match('/Lambeth(.*)\.csv/', $name, $matches);
  if ($result) {
    $type = $matches[1];
    $fields = lambeth_opendata_map_locations_fields($type);
    if (!empty($fields)) {

      if ($data = lambeth_opendata_csv_readfile($url)) {
        $header_from = $data[0];
        foreach ($header_from as $k => $v) {
          if (in_array($v, $fields)) {
            $header_to[$k] = array_search($v, $fields);
          }
        }
        unset($data[0]);
        ksort($data);
        $rows = array();
        foreach ($data as $k => $v) {
          $tmp = '';
          foreach ($header_to as $i => $value) {
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
        $header_to[] = 'index';
        $header_to[] = 'type';
        $result = lambeth_opendata_csv_writefile($name, $header_to, $rows);
      }
    }
  }
  return $result;
}

function lambeth_opendata_rewrite_spending($url, $name) {
  $result = preg_match('/LambethPaymentsOver500(.*)\.csv/', $name, $matches);
  if ($result) {
    $array = date_parse_from_format('jMYY', '1' . $matches[1]);
    $timestamp = mktime(0, 0, 0, $array['month'], $array['day'], $array['year']);
    $month = date('M', $timestamp);
    $year = date('Y', $timestamp);
    $cols = array('Supplier', 'Total', 'Index', 'Month', 'Year');
    //$indexfile = $_lambeth_opendata_private_dir . '/LambethSpending_' . $month . '_' . $year . '_index.csv';
    if ($data = lambeth_opendata_csv_readfile($url)) {
      $data = array_splice($data, 4);
      $rows = array();
      foreach ($data as $k => $v) {
        $v[0] = trim($v[0]);
        if (empty($v[0]) || ($v[0] == 'Grand Total')) {
          continue;
        }
        $rows[$k][0] = $v[0];
        $rows[$k][1] = ltrim(str_replace(',', '', $v[1]), '£');
        $rows[$k][2] = $timestamp + $k;
        $rows[$k][3] = $month;
        $rows[$k][4] = $year;
      }
      $result = csv_writefile($name, $cols, $rows);
    }
  }
  return $result;
}

function lambeth_opendata_rewrite_planning($url, $name) {
  $result = FALSE;
  if ($data = lambeth_opendata_csv_readfile($url)) {
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
    $result = lambeth_opendata_csv_writefile($name, $header, $rows);
  }
  return $result;
}

function lambeth_opendata_rewrite_foi($url, $name) {
  $result = FALSE;
  if ($data = lambeth_opendata_csv_readfile($url)) {
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
    $result = lambeth_opendata_csv_writefile($name, $header, $rows);
  }
  return $result;
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
        'openhours'=>'OPENING_TIMES',
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
        'openhours'=>'OPENING_HOURS',
        'other_1'=>'TOILETS',
        'other_2'=>'AREA',
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
        'other_1'=>'No_',
        'other_2'=>'Note_',
      );
      //Easting
      //Northing
      break;
    case 'LeisureCentres':
      $fields = array(
        'name' => 'NAME',
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
        'openhours'=>'HOURS',
      );
      break;
    case 'PollingStations':
      $fields = array(
        'name' => 'VENUE',
        'address' => 'ADDRESS',
        'postcode' => 'POSTCODE',
        'other_1'=>'WARD',
        'other_2'=>'DISTRICT_CODE',
      );
      //EASTING
      //NORTHING
      break;
    case 'PublicConveniences':
      $fields = array(
        'name' => 'BUILDING_NAME',
        'address' => 'PROPERTY',
        'other_1'=>'TYPE',
      );
      //EASTING
      //NORTHING
      break;
    case 'RecyclingBanks':
      $fields = array(
        'name' => 'LOCATION',
        'other_1'=>'CO_MINGLED',
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
        'other_1'=>'TYPE',
        'other_2'=>'ACADEMY',
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
        'other_1'=>'Contact Name',
        'other_2'=>'Email',
        'other_3'=>'Website',
        'other_4'=>'Description of provision',
        'other_5'=>'Target group',
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