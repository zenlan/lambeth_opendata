<?php

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

/**
 * @file
 * Admin page callbacks for the Lambeth Open Data module.
 *
 * @ingroup
 */
function lambeth_opendata_admin_settings(array $form, array &$form_state) {
  global $_lambeth_opendata_types;

  $form['#tree'] = TRUE;
  $form['urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('URLs'),
  );
  foreach ($_lambeth_opendata_types as $field => $values) {
    $form['urls'][$field] = array(
      '#type' => 'textfield',
      '#size' => 128,
      '#title' => t($values['label']),
      '#default_value' => $values['url'],
      '#required' => TRUE,
    );
  }
  $form['urls']['save'] = array(
    '#name' => 'cancel',
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('lambeth_opendata_admin_settings_submit'),
  );
  return $form;
}

function lambeth_opendata_admin_settings_submit(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  foreach ($_lambeth_opendata_types as $type => $values) {
    $url = $form_state['values']['urls'][$type];
    if (lambeth_opendata_get_page_contents($url)) {
      $_lambeth_opendata_types[$type]['url'] = $url;
    }
  }
  variable_set('lambeth_opendata_types', serialize($_lambeth_opendata_types));
}

function lambeth_opendata_scraper(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  $form['#tree'] = TRUE;
  $form['scrape'] = array(
    '#name' => 'cancel',
    '#type' => 'submit',
    '#value' => t('Scrape'),
    '#submit' => array('lambeth_opendata_scraper_submit'),
  );
  return $form;
}

function lambeth_opendata_scraper_submit(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  foreach ($_lambeth_opendata_types as $type => $values) {
    if ($cache = lambeth_opendata_scrape($values['url'])) {
      cache_set('lambeth_opendata_' . $type, $cache);
    }
  }
}
function lambeth_opendata_cache(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  $form['#tree'] = TRUE;
  $form['read'] = array(
    '#name' => 'cancel',
    '#type' => 'submit',
    '#value' => t('Read Cache'),
    '#submit' => array('lambeth_opendata_cache_submit_read'),
  );
  $form['clear'] = array(
    '#name' => 'cancel',
    '#type' => 'submit',
    '#value' => t('Clear Cache'),
    '#submit' => array('lambeth_opendata_cache_submit_clear'),
  );
  return $form;
}

function lambeth_opendata_cache_submit_read(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  $result = array();
  foreach ($_lambeth_opendata_types as $type => $values) {
    $result['lambeth_opendata_' . $type] = cache_get('lambeth_opendata_' . $type);
  }
  drupal_set_message(print_r($result, TRUE));
  return $result;
}

function lambeth_opendata_cache_submit_clear(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  foreach ($_lambeth_opendata_types as $type => $values) {
    cache_clear_all('lambeth_opendata_' . $type, 'cache');
  }
}

function lambeth_opendata_get_page_contents($url) {
  if (!$result = file_get_contents($url)) {
    drupal_set_message('Error retrieving contents from ' . $url);
    return FALSE;
  }
  return $result;
}

/**
 *    // check if node exists for this file
      // create empty file /tmp/$name
      // put contents of href into new file
      // analyse file
      // create datadocument node
      // attach file
 */

function lambeth_opendata_scrape($url) {

  if (!$tmp = lambeth_opendata_get_page_contents($url)) {
    return FALSE;
  }
  $result = array();
  $tagformat = '/\<a name="(?P<name>(.*))" href="(?P<href>(.*))".*\>.*\<\/a\>/Ui';
  if ($i = preg_match_all($tagformat, $tmp, $matches)) {
    drupal_set_message(count($matches['name']) . ' files found at ' . $url);
    $guidformat = '/[0-9A-Z]{8}-[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{4}-[0-9A-Z]{12}/';
    foreach ($matches['name'] as $k => $name) {
      $tmp = pathinfo('http://www.lambeth.gov.uk' . $matches['href'][$k]);
      $tmp['guid'] = '';
      if (strtolower($tmp['extension']) == 'csv') {
        if ($i = preg_match($guidformat, $tmp['dirname'], $gmatches)) {
          $tmp['guid'] = $gmatches[0];
        }
        $result[] = $tmp;
      }
      else {
        $msg = t('EXCEPTION: FILE IS NOT CSV !file', array('!file' => $tmp['basename']));
        drupal_set_message($msg);
        watchdog('lambeth_opendata', $msg);
      }
    }
    drupal_set_message(print_r($result, TRUE));
    return $result;
  }
  drupal_set_message('No CSV files found at ' . $url);
  return FALSE;
}

function lambeth_opendata_entities(array $form, array &$form_state) {

  $markup = '<strong>Uses Hooks:</strong><br /><br />';
  $hooks = array(
    'search_api_service_info',
    'search_api_solr_query_alter',
    'search_api_solr_field_mapping_alter',
    'format_search_api_solr_query_string',
    'search_api_solr_search_results_alter',
    'theme',
    'menu',
    'views_api',
    'views_plugins',
    'exposed_form',
    'entity_info',
    'entity_info_alter',
    'entity_property_info',
    'tokens',
    'token_info',
    'token_values',
    'tokens_alter',
  );
  foreach ($hooks as $hook) {
    $result = module_hook('lambeth_opendata', $hook);
    $markup.= print_r($hook, TRUE) . ' : ' . ($result ? 'true' : 'false') . '<br />';
  }
  $markup.= '<br />';

//  $description.= '<strong>Token Tree:</strong><br /><br />';
//  $tokens = token_build_tree($entity_type);
//  foreach ($tokens as $token)
//  {
//    $description.= print_r($token,true).'<br />';
//  }
//  $description.= '<br />';
//  $description.= '<strong>Miscellaneous:</strong><br /><br />';
//  $vapifunc = $entity_type.'_views_api';
//  $viewsapi = $vapifunc();
//  $description.= 'Views API version: '.print_r($viewsapi['api'],true).'<br /><br />';
//    $markup.= '<br />';


  $markup.= '<strong>Search API Servers:</strong><br /><br />';
  $servers = search_api_server_load_multiple(FALSE);
  foreach ($servers as $server) {
    if ($server->class == 'lambeth_opendata_solr_service') {
      $ping = $server->ping();
      $markup .= $server->machine_name;
      $markup .= ' // ' . ($server->enabled ? 'enabled' : 'disabled');
      $markup .= ' // ping ' . ($ping ? 'OK' : 'failed') . '<br />';
    }
  }
  $markup.= '<br />';

  $types = array(
    'lambeth_opendata_foi',
    'lambeth_opendata_locations',
    'lambeth_opendata_planning',
    'lambeth_opendata_spending',
  );
  foreach ($types as $type) {
    $markup .= lambeth_opendata_entity_information($type);
  }
  $form['description'] = array(
    '#markup' => $markup,
  );
  return $form;
}

function lambeth_opendata_entity_information($entity_type) {
  //make sure we are using the latest version of the entity
  //entity_info_cache_clear();
  //entity_property_info_cache_clear();
  $result = '';
  $info = entity_get_info($entity_type);
  if ($info && is_array($info)) {
    $result = '<strong>Entity Info (' . $entity_type . '):</strong><br /><br />';
    foreach ($info as $k => $v) {
      $result.= $k . ' => ' . print_r($v, TRUE) . '<br />';
    }

    $result .= '<br />';
    $result .= '<strong>Properties:</strong><br /><br />';
    $props = entity_get_all_property_info($entity_type);
    if ($props && is_array($props)) {
      foreach ($props as $k => $v) {
        $result .= 'Name: ' . $k . ' / ';
        $result .= 'Type: ' . $v['type'] . ' / ';
        $result .= 'Label: ' . $v['label'] . ' / ';
        $result .= 'Description: ' . $v['description'] . '<br />';
        //$result .= 'Callback: '.$v['query callback'].'<br />';
      }
    }
    $result .= '<br />';
    $result .= '<br />';
  }
  return $result;
}

