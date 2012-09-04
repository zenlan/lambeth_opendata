<?php

/**
 * @file
 * Admin page callbacks for the Lambeth Open Data module.
 *
 * @ingroup
 */
function lambeth_opendata_junk(array $form, array &$form_state) {
  //$result = lambeth_opendata_get_document_types();
  //$result = taxonomy_vocabulary_machine_name_load('lambeth_opendata_category');
  //$result = taxonomy_get_tree($result->vid, 0, 1, TRUE);
  //$result = lambeth_opendata_get_document_types(); //TRUE);
//  $result = lambeth_opendata_get_node_type_by_tid(4);
//  $result = lambeth_opendata_get_node_tid_by_type('locations');
//  $result = lambeth_opendata_get_type_options(0);

  $markup = '';
  module_load_include('inc', 'lambeth_opendata_feature', 'lambeth_opendata_feature.features.field');
  $fields = lambeth_opendata_feature_field_default_fields();
  foreach ($fields as $k => $v) {
    $instance = $v['field_instance'];
    $config = $v['field_config'];
    $type = $instance['entity_type'];
    $markup .= '<pre>' .$k . '<br />' . print_r($type,1) . '</pre>';
  }


//  $type = lambeth_opendata_get_type_from_index_name('lbl_foi_index');
//  $tax = array_pop(taxonomy_get_term_by_name($type, 'lambeth_opendata_category'));
//  if (empty($tax)) {
//    $markup = t('Failed to get tid for !type', array('!type' => $type));
//  }
//  else {
//    $query = db_select('node', 'n');
//    $query->addField('n', 'nid');
//    //$query->addField('f', 'field_lambeth_doc_type_tid');
//    $query->leftJoin('field_data_field_lambeth_doc_type', 'f', 'f.entity_id=n.nid');
//    $query->condition('n.type', 'lambeth_opendata_document', '=');
//    $query->condition('f.field_lambeth_doc_type_tid', $tax->tid, '=');
////SELECT n.nid AS nid, f.field_lambeth_doc_type_tid AS field_lambeth_doc_type_tid
////FROM node n
////LEFT OUTER JOIN field_data_field_lambeth_doc_type f ON f.entity_id=n.nid
////WHERE n.type = 'lambeth_opendata_document'
////AND f.field_lambeth_doc_type_tid = 1
//    $res = $query->execute();
//    $result = $res->fetchAll();
//    $markup = '<pre>' . print_r($result,1) . '</pre>';
//  }
  $form['description'] = array(
    '#markup' => $markup,
  );
  return $form;
}

function lambeth_opendata_admin_settings(array $form, array &$form_state) {
  global $_lambeth_opendata_types;
  $markup = '<p>';
  $markup .= 'These categories are defined in vocabulary ';
  $markup .= l('Lambeth Open Data Category', 'admin/structure/taxonomy/lambeth_opendata_category',
      array('attributes' => array(
        'target' => '_blank',
      )));
  $markup .= '</p>';
  foreach ($_lambeth_opendata_types as $field => $values) {
    $markup .= '<p>Key: ' . t($field);
    $markup .= '<br />ID: ' . $values['tid'];
    $markup .= '<br />Label: ' . t($values['label']);
    $markup .= '<br />Origin: ' . $values['url'] . '</p>';
  }
  $form['description'] = array(
    '#markup' => $markup,
  );
  return $form;
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
  module_load_include('inc', 'lambeth_opendata', 'lambeth_opendata.scraper');
  lambeth_opendata_batch_init();
}

//function lambeth_opendata_cache(array $form, array &$form_state) {
//  global $_lambeth_opendata_types;
//  $form['#tree'] = TRUE;
//  $form['read'] = array(
//    '#name' => 'cancel',
//    '#type' => 'submit',
//    '#value' => t('Read Cache'),
//    '#submit' => array('lambeth_opendata_cache_submit_read'),
//  );
//  $form['clear'] = array(
//    '#name' => 'cancel',
//    '#type' => 'submit',
//    '#value' => t('Clear Cache'),
//    '#submit' => array('lambeth_opendata_cache_submit_clear'),
//  );
//  return $form;
//}
//
//function lambeth_opendata_cache_submit_read(array $form, array &$form_state) {
//  global $_lambeth_opendata_types;
//  $result = array();
//  foreach ($_lambeth_opendata_types as $type => $values) {
//    $result['lambeth_opendata_' . $type] = cache_get('lambeth_opendata_' . $type);
//  }
//  drupal_set_message(print_r($result, TRUE));
//  return $result;
//}
//
//function lambeth_opendata_cache_submit_clear(array $form, array &$form_state) {
//  global $_lambeth_opendata_types;
//  foreach ($_lambeth_opendata_types as $type => $values) {
//    cache_clear_all('lambeth_opendata_' . $type, 'cache');
//  }
//}

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

