<?php
/**
 * @file
 * Utilities for the Lambeth Open Data module.
 *
 * @ingroup
 */

module_load_include('inc', 'lambeth_opendata', 'lambeth_opendata.import');

function lambeth_opendata_batch_init() {
  $batch = array(
    'title' => t('Batch Scrape Lambeth Open Data'),
    'operations' => array(
      array('lambeth_opendata_batch_function', array()),
    ),
    'init_message' => t('Lambeth Open Data scraper is starting...'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Lambeth Open Data scraper has encountered an error.'),
    'finished' => 'lambeth_opendata_batch_finished',
    'file' => drupal_get_path('module', 'lambeth_opendata') . '/lambeth_opendata.scraper.inc',
  );
  batch_set($batch);
}

function lambeth_opendata_batch_function(&$context) {
  global $_lambeth_opendata_types;
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($_lambeth_opendata_types);
  }
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $item = array_slice($_lambeth_opendata_types, $context['sandbox']['progress'], 1);
    $context['finished'] = $context['sandbox']['progress']++ / $context['sandbox']['max'];
    foreach ($item as $type => $values) {
      if ($data = lambeth_opendata_scrape($values['url'])) {
        lambeth_opendata_import($type, $data);
        $context['results'][$type] = count($data);
      }
    }
    $context['message'] = $type . ' (' . $context['sandbox']['progress'] . ' of ' . $context['sandbox']['max'] . ')';
  }
  else {
    $context['finished'] = TRUE;
  }
}

function lambeth_opendata_batch_finished($success, $results, $operations) {
  if ($success) {
    $count = count($results);
    $message = ($count == 1 ? 'One type processed.' : $count . ' types processed.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message, 'lambeth_opendata');
  foreach ($results as $type => $rows) {
    drupal_set_message($type . ' => ' . $rows . ' rows', 'lambeth_opendata');
  }
}

function lambeth_opendata_scrape($url) {

  if (!$tmp = lambeth_opendata_get_page_contents($url)) {
    $msg = t('Failed to retrieve content from !url', array('!url' => $url));
    watchdog('lambeth_opendata', $msg);
    return FALSE;
  }
  $result = array();
  $tagformat = '/\<a name="(?P<name>(.*))" href="(?P<href>(.*))".*\>.*\<\/a\>/Ui';
  if ($i = preg_match_all($tagformat, $tmp, $matches)) {
    $uidformat = '/[0-9A-Z]{8}-[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{12}/';
    foreach ($matches['name'] as $k => $name) {
      $tmp = pathinfo('http://www.lambeth.gov.uk' . $matches['href'][$k]);
      $tmp['uid'] = '';
      $tmp['title'] = $name;
      if (strtolower($tmp['extension']) == 'csv') {
        if ($i = preg_match($uidformat, $tmp['dirname'], $umatches)) {
          $tmp['uid'] = $umatches[0];
        }
        $result[] = $tmp;
      }
      else {
        $msg = t('EXCEPTION: FILE IS NOT CSV !file', array('!file' => $tmp['basename']));
        watchdog('lambeth_opendata', $msg);
      }
    }
    return $result;
  }
  $msg = t('No CSV files found at !url', array('!url' => $url));
  watchdog('lambeth_opendata', $msg);
  return FALSE;
}
