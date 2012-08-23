<?php

/**
 * @file
 * Admin page callbacks for the Lambeth Open Data module.
 *
 * @ingroup
 */
function lambeth_opendata_admin_settings(array $form, array &$form_state) {
  return $form;
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
    $result = '<strong>Entity Info (' .$entity_type. '):</strong><br /><br />';
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

