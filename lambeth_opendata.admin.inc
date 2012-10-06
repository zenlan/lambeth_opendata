<?php

/**
 * @file
 * Admin page callbacks for the Lambeth Open Data module.
 *
 * @ingroup
 */
function lambeth_opendata_junk(array $form, array &$form_state) {
  $desc = '';
  $form['description'] = array(
    '#markup' => $desc,
  );
  return $form;
}

function lambeth_opendata_repo(array $form, array &$form_state) {

  $desc = '<p>data cleaning routines</p>';
  $desc .= '<ul>';
  $desc .= '<li>select all/dataset/file</li>';
  $desc .= '<li>apply regex/function</li>';
  $desc .= '</ul>';
  $desc .= '<p>dataset configuration routines</p>';
  $desc .= '<ul>';
  $desc .= '<li>select all/dataset</li>';
  $desc .= '<li>create/map vocabulary</li>';
  $desc .= '<li>create/update entity/properties</li>';
  $desc .= '</ul>';
  $desc .= '<p>data import routines</p>';
  $desc .= '<ul>';
  $desc .= '<li>select all/dataset/file</li>';
  $desc .= '<li>import</li>';
  $desc .= '</ul>';
  $form['description'] = array(
    '#markup' => $desc,
  );
  $conf = lambeth_opendata_conf();
  if (!$gfiles = lambeth_opendata_github_repo_cache_get($conf['datastore']['owner'], $conf['datastore']['name'])) {
    drupal_set_message('Github cache is empty!', '', WATCHDOG_ALERT);
  }
  else {
    drupal_set_message('Github cache date: ' . date('d M Y h:m', $gfiles->created), '', WATCHDOG_ALERT);
  }
  if ($gfiles) {
    foreach ($conf['datastore']['datasets'] as $name => $info) {
      $form['dataset'][$name] = array(
        '#title' => $name,
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($gfiles->data['sets'][$name] as $filename => $info) {
        $data[] = array('filename' => $filename);
      }
      $tmp = array_keys($gfiles->data['sets'][$name]);
      $header['filename'] = array('data' => 'File', 'field' => 'filename');
      $form['dataset'][$name]['table'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $data,
        '#multiple' => TRUE,
        '#default_value' => array(),
        // '#js_select' => FALSE,
        '#empty' => t('Empty'),
      );
    }
  }
  return $form;
}

function lambeth_opendata_tableselect($form, $form_state) {
  $header = array(
    'fieldName' => array('data' => t('fieldName'), 'field' => 'fieldName'),
    'type' => array('data' => t('Drupal property type'), 'field' => 'type'),
    'metatype' => array('data' => t('Adlib metadata'), 'field' => 'metatype'),
    'isIndexed' => array('data' => t('isIndexed'), 'field' => 'isIndexed'),
    'displayName' => array('data' => t('displayName'), 'field' => 'displayName'),
    'group' => array('data' => t('group'), 'field' => 'group'),
    'repeated' => array('data' => t('repeated'), 'field' => 'repeated'),
    'isLinked' => array('data' => t('isLinked'), 'field' => 'isLinked'),
    'tag' => array('data' => t('tag'), 'field' => 'tag'),
    'length' => array('data' => t('length'), 'field' => 'length'),
  );
  $form['searchfields'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => isset($entity_dataobj->properties) ? $entity_dataobj->properties : array(),
    '#multiple' => TRUE,
    '#default_value' => isset($entity_dataobj->searchfields) ? $entity_dataobj->searchfields : array(),
    // '#js_select' => FALSE,
    '#empty' => t('Properties (fields) not found, click save to retrieve
    metadata from the Adlib server.'),
  );
}

function lambeth_opendata_configuration(array $form, array &$form_state) {
  $desc = '';
  $conf = lambeth_opendata_conf();

  $desc .= '<p>Config: ' . $conf['conf_file'] . '</p>';
  $desc .= '<p>Schema: ' . $conf['name'] . ' v' . $conf['version'] . '</p>';
  $desc .= '<p>Source: ' . '<ul>';
  foreach ($conf['source'] as $k => $v) {
    $desc .= '<li>' . $k . ': ' . $v . '</li>';
  }
  $desc .= '</ul></p>';

  $desc .= '<p>GitHub Datastore: ' . $conf['datastore']['owner'] . '/' . $conf['datastore']['name'] . '</p>';
  $desc .= '<div>Datasets: ';
  foreach ($conf['datastore']['datasets'] as $k => $v) {
    $desc .= '<p><ul>';
    $desc .= '<li>key: ' . $k . '</li>';
    $desc .= '<li>label: ' . $v['label'] . '</li>';
    $desc .= '<li>url: ' . $v['url'] . '</li>';
    $desc .= '<li>source: ' . $v['source'] . '</li>';
    $desc .= '<li>conf: ' . $conf['conf_path'] . $v['conf'] . '</li>';
    $desc .= '</ul></p>';
  }
  $desc .= '</div>';
  $form['description'] = array(
    '#markup' => $desc,
  );
  return $form;
}

function lambeth_opendata_cache(array $form, array &$form_state) {
  $desc = '';
  $conf = lambeth_opendata_conf();
  if (!$sfiles = lambeth_opendata_github_scrape_cache_get()) {
    drupal_set_message('Source cache is empty!', '', WATCHDOG_ALERT);
  }
  else {
    drupal_set_message('Source cache date: ' . date('d M Y h:m', $sfiles->created), '', WATCHDOG_ALERT);
  }
  if (!$gfiles = lambeth_opendata_github_repo_cache_get($conf['datastore']['owner'], $conf['datastore']['name'])) {
    drupal_set_message('Github cache is empty!', '', WATCHDOG_ALERT);
  }
  else {
    drupal_set_message('Github cache date: ' . date('d M Y h:m', $gfiles->created), '', WATCHDOG_ALERT);
  }
  if ($sfiles && $gfiles) {
    $stmp['root'] = array_diff($sfiles->data['sets'], $gfiles->data['sets']);
    $gtmp['root'] = array_diff($gfiles->data['sets'], $sfiles->data['sets']);
    foreach ($conf['datastore']['datasets'] as $name => $info) {
      $stmp[$name] = array_diff($sfiles->data['sets'][$name], $gfiles->data['sets'][$name]);
      $gtmp[$name] = array_diff($gfiles->data['sets'][$name], $sfiles->data['sets'][$name]);
    }
    $desc .= '<p>Cached differences:</p>';
    $desc .= '<p>Missing from Github: ' . print_r($stmp, 1) . '</p>';
    $desc .= '<p>Missing from source: ' . print_r($gtmp, 1) . '</p>';

    $desc .= '<div style="float:left;">Source: ' . '<ul>';
    foreach ($sfiles->data['sets'] as $setname => $files) {
      $desc .= '<li>' . $setname . ' (' . count($files) . ' files)</li><ul>';
      foreach ($files as $filename => $info) {
        $desc .= '<li>' . $filename . '</li>';
      }
      $desc .= '</ul>';
    }
    $desc .= '</ul></div>';
    $desc .= '<div style="float:right;">Datastore: ' . '<ul>';
    foreach ($gfiles->data['sets'] as $setname => $files) {
      $desc .= '<li>' . $setname . ' (' . count($files) . ' files)</li><ul>';
      foreach ($files as $filename => $info) {
        $desc .= '<li>' . $filename . '</li>';
      }
      $desc .= '</ul>';
    }
    $desc .= '</ul></div>';
  }
  $form['description'] = array(
    '#markup' => $desc,
  );
  $form['#tree'] = TRUE;
  $form['srccache'] = array(
    '#name' => 'srccache',
    '#type' => 'submit',
    '#value' => t('Refresh source cache'),
    '#submit' => array('lambeth_opendata_source_build_cache'),
  );
  $form['gitcache'] = array(
    '#name' => 'gitcache',
    '#type' => 'submit',
    '#value' => t('Refresh Github cache'),
    '#submit' => array('lambeth_opendata_github_build_cache'),
  );
  return $form;
}

function lambeth_opendata_github_repo_cache_get($owner, $name) {
  return cache_get('lambeth_opendata_' . $owner . '_' . $name);
}

function lambeth_opendata_github_repo_cache_set($owner, $name, $info, $refs, $tree, $sets) {
  cache_set('lambeth_opendata_' . $owner . '_' . $name, array(
    'info' => $info,
    'refs' => $refs,
    'tree' => $tree,
    'sets' => $sets,
  ));
  return lambeth_opendata_github_repo_cache_get($owner, $name);
}

function lambeth_opendata_github_scrape_cache_get() {
  return cache_get('lambeth_opendata_scrape');
}

function lambeth_opendata_github_scrape_cache_set($tree) {
  cache_set('lambeth_opendata_scrape', array(
    'sets' => $tree,
  ));
  return lambeth_opendata_github_scrape_cache_get();
}

function lambeth_opendata_source_build_cache() {
  $conf = lambeth_opendata_conf();
  $result = array();
  module_load_include('inc', 'lambeth_opendata', 'lambeth_opendata.scraper');
  foreach ($conf['datastore']['datasets'] as $name => $info) {
    $result[$name] = lambeth_opendata_scrape($info['source']);
  }
  return lambeth_opendata_github_scrape_cache_set($result);
}

function lambeth_opendata_conf() {
  $result = array();
  try {
    $conf = lambeth_opendata_static_conf();
    $result = $conf->get_conf_array();
  } catch (Exception $e) {
    $result = array('error' => $e->getMessage());
  }
  return $result;
}

function lambeth_opendata_github_build_cache() {
  $options = array(
    'repo_owner' => 'zenlan',
    'repo_name' => 'lambeth_opendatastore',
  );
//$cache = lambeth_opendata_github_repo_cache_get($options['repo_owner'], $options['repo_name']);
  $obj = new LambethOpenDataGitHub($options);
  $info = $obj->get_repo_info();
  $refs = $obj->get_repo_references();
  $sha = $refs[0]->object->sha; // root sha
  $tree = $obj->get_repo_tree($sha, TRUE);
  $datasets = $obj->parse_repo_tree_to_array($tree);

//  $since = '2012-01-21T19:35:53Z';
//  $commits = $obj->get_repo_commits($since);
//  foreach ($commits as $k => $v) {
//    $sha = $v->commit->tree->sha;
//    $tree = $obj->get_repo_tree($sha, TRUE);
// check cached blob info - size, sha?
// if changed, get blob
// $blob = $obj->get_repo_blob('7a571698696957ceba0d7c403875bc97ee58c38a');
// process blob, update cache
//  }

  if ($obj->get_error_code()) {
    drupal_set_message(print_r($obj->get_error_message(), 1));
  }
  elseif ($info) {
    //drupal_set_message(print_r($info, 1));
    $cache = lambeth_opendata_github_repo_cache_set($options['repo_owner'], $options['repo_name'], $info, $refs, $tree, $datasets);
    $time_cache = strtotime($cache->data['info']->pushed_at);
    $time_live = strtotime($info->pushed_at);
    foreach ($datasets as $set => $info) {
      drupal_set_message($set . ':' . count($info) . ' blobs<br />');
    }
    if ($time_live > $time_cache) {
      drupal_set_message('REPO HAS BEEN UPDATED SINCE LAST DOWNLOAD', '', WATCHDOG_ALERT);
    }
    else {
      drupal_set_message('Repo has not been updated since last download', '', WATCHDOG_ALERT);
    }
    return $cache;
  }
  return FALSE;
}

//function lambeth_opendata_github_download() {
//  $dir = DRUPAL_ROOT . '/' . variable_get('file_public_path') . '/lambeth_opendata/';
//  if (!file_exists($dir)) {
//    @@mkdir($dir);
//    if (!file_exists($dir)) {
//      drupal_set_message('Failed to create ' . $dir);
//      return FALSE;
//    }
//  }
//  $options = array(
//    'download_directory' => variable_get('file_temporary_path'),
//    'data_directory' => $dir,
//    'repo_owner' => 'zenlan',
//    'repo_name' => 'lambeth_opendatastore',
//  );
//  $obj = new LambethOpenDataGitHub($options);
//  $file = $obj->download_repo();
//  if ($obj->get_error_code()) {
//    $message = print_r($obj->get_error_message(), 1);
//  }
//  elseif ($file) {
//    $message = print_r($obj->unpack_repo($file), 1);
//  }
//  drupal_set_message($message);
//}

function lambeth_opendata_scraper(array $form, array &$form_state) {
  global $_lambeth_opendata_public_dir;
  global $_lambeth_opendata_private_dir;
  $form['#tree'] = TRUE;
  $form['description'] = array(
    '#markup' => 'Scrape files from Lambeth Open Data pages and '
//    . '<br /><br />'
//    . 'Upload and attach to nodes (files managed), store in ' . $_lambeth_opendata_public_dir
//    . '<br /><br />'
    . 'store in ' . $_lambeth_opendata_private_dir
    . '<br /><br />',
  );
//  $form['import'] = array(
//    '#name' => 'import',
//    '#type' => 'submit',
//    '#value' => t('Upload'),
//    '#submit' => array('lambeth_opendata_scraper_submit'),
//  );
  $form['download'] = array(
    '#name' => 'download',
    '#type' => 'submit',
    '#value' => t('Download'),
    '#submit' => array('lambeth_opendata_scraper_submit'),
  );
  return $form;
}

function lambeth_opendata_scraper_submit(array $form, array &$form_state) {
  module_load_include('inc', 'lambeth_opendata', 'lambeth_opendata.scraper');
  lambeth_opendata_batch_init($form_state['triggering_element']['#name']);
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

