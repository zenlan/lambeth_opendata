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
  $form['description'] = array(
    '#markup' => lambeth_opendata_entity_information('lambeth_opendata_foi'),
  );
  return $form;
}

function lambeth_opendata_entity_information($entity_type){
  //make sure we are using the latest version of the entity
  //entity_info_cache_clear();
  //entity_property_info_cache_clear();
  $description = '<strong>Entity Info:</strong><br /><br />';
  $info = entity_get_info($entity_type);
  if ($info && is_array($info)){
    foreach ($info as $k => $v){
      $description.= $k.' => '.print_r($v, TRUE).'<br />';
    }
  }
  $description.= '<br />';

  $description.= '<strong>Properties:</strong><br /><br />';
  $props = entity_get_all_property_info($entity_type);
  if ($props && is_array($props)){
    foreach ($props as $k => $v){
      $description.= 'Name: '.$k.' / ';
      $description.= 'Type: '.$v['type'].' / ';
      $description.= 'Label: '.$v['label'].' / ';
      $description.= 'Description: '.$v['description'].'<br />';
      //$description.= 'Callback: '.$v['query callback'].'<br />';
    }
  }
  $description.= '<br />';

  $description.= '<strong>Uses Hooks:</strong><br /><br />';
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
  foreach ($hooks as $hook)
  {
    $result = module_hook('lambeth_opendata', $hook);
    $description.= print_r($hook, TRUE).' : '.($result ? 'true' : 'false').'<br />';
  }
  $description.= '<br />';
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


  $description.= '<br />';

  $description.= '<strong>Search API Servers:</strong><br /><br />';
  $servers = search_api_server_load_multiple(FALSE);
  foreach ($servers as $server) {
    if ($server->class == 'lambeth_opendata_solr_service') {
      $ping = $server->ping();
      $description .= $server->machine_name;
      $description .= ' // ' . ($server->enabled ? 'enabled' : 'disabled');
      $description .=  ' // ping ' . ($ping ? 'OK' : 'failed') . '<br />';
    }
  }
  return $description;
}



