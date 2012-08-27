<?php
/**
 * @file
 * lambeth_open_data_solr.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function lambeth_open_data_solr_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'lambeth_open_data_foi';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_lbl_foi_index';
  $view->human_name = 'Lambeth Open Data FoI';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Lambeth Freedom of Information Requests';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Indexed Lambeth Open Data FoI Entity: Ref */
  $handler->display->display_options['fields']['Ref']['id'] = 'Ref';
  $handler->display->display_options['fields']['Ref']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['fields']['Ref']['field'] = 'Ref';
  $handler->display->display_options['fields']['Ref']['separator'] = '';
  $handler->display->display_options['fields']['Ref']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data FoI Entity: Received */
  $handler->display->display_options['fields']['Received']['id'] = 'Received';
  $handler->display->display_options['fields']['Received']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['fields']['Received']['field'] = 'Received';
  $handler->display->display_options['fields']['Received']['date_format'] = 'custom';
  $handler->display->display_options['fields']['Received']['custom_date_format'] = 'd M Y';
  $handler->display->display_options['fields']['Received']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data FoI Entity: Summary */
  $handler->display->display_options['fields']['Summary']['id'] = 'Summary';
  $handler->display->display_options['fields']['Summary']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['fields']['Summary']['field'] = 'Summary';
  $handler->display->display_options['fields']['Summary']['link_to_entity'] = 0;
  /* Field: Search: Relevance */
  $handler->display->display_options['fields']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['fields']['search_api_relevance']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['fields']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['fields']['search_api_relevance']['exclude'] = TRUE;
  $handler->display->display_options['fields']['search_api_relevance']['separator'] = '';
  $handler->display->display_options['fields']['search_api_relevance']['link_to_entity'] = 0;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_lbl_foi_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'lambeth_foi';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Lambeth FoI';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['lambeth_open_data_foi'] = $view;

  $view = new view();
  $view->name = 'lambeth_open_data_locations';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_lbl_locations_index';
  $view->human_name = 'Lambeth Open Data Locations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Lambeth Open Data Locations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Indexed Lambeth Open Data Locations Entity: index */
  $handler->display->display_options['fields']['index']['id'] = 'index';
  $handler->display->display_options['fields']['index']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['index']['field'] = 'index';
  $handler->display->display_options['fields']['index']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  $handler->display->display_options['fields']['type']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Name';
  $handler->display->display_options['fields']['name']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: address */
  $handler->display->display_options['fields']['address']['id'] = 'address';
  $handler->display->display_options['fields']['address']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['address']['field'] = 'address';
  $handler->display->display_options['fields']['address']['label'] = 'Address';
  $handler->display->display_options['fields']['address']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['address']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: postcode */
  $handler->display->display_options['fields']['postcode']['id'] = 'postcode';
  $handler->display->display_options['fields']['postcode']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['postcode']['field'] = 'postcode';
  $handler->display->display_options['fields']['postcode']['label'] = 'Postcode';
  $handler->display->display_options['fields']['postcode']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: telephone */
  $handler->display->display_options['fields']['telephone']['id'] = 'telephone';
  $handler->display->display_options['fields']['telephone']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['telephone']['field'] = 'telephone';
  $handler->display->display_options['fields']['telephone']['label'] = 'Telephone';
  $handler->display->display_options['fields']['telephone']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: openhours */
  $handler->display->display_options['fields']['openhours']['id'] = 'openhours';
  $handler->display->display_options['fields']['openhours']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['openhours']['field'] = 'openhours';
  $handler->display->display_options['fields']['openhours']['label'] = 'Hours';
  $handler->display->display_options['fields']['openhours']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['openhours']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: other_1 */
  $handler->display->display_options['fields']['other_1']['id'] = 'other_1';
  $handler->display->display_options['fields']['other_1']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['other_1']['field'] = 'other_1';
  $handler->display->display_options['fields']['other_1']['label'] = 'Other A';
  $handler->display->display_options['fields']['other_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['other_1']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: other_2 */
  $handler->display->display_options['fields']['other_2']['id'] = 'other_2';
  $handler->display->display_options['fields']['other_2']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['other_2']['field'] = 'other_2';
  $handler->display->display_options['fields']['other_2']['label'] = 'Other B';
  $handler->display->display_options['fields']['other_2']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['other_2']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: other_3 */
  $handler->display->display_options['fields']['other_3']['id'] = 'other_3';
  $handler->display->display_options['fields']['other_3']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['other_3']['field'] = 'other_3';
  $handler->display->display_options['fields']['other_3']['label'] = 'Other C';
  $handler->display->display_options['fields']['other_3']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['other_3']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: other_4 */
  $handler->display->display_options['fields']['other_4']['id'] = 'other_4';
  $handler->display->display_options['fields']['other_4']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['other_4']['field'] = 'other_4';
  $handler->display->display_options['fields']['other_4']['label'] = 'Other D';
  $handler->display->display_options['fields']['other_4']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['other_4']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Locations Entity: other_5 */
  $handler->display->display_options['fields']['other_5']['id'] = 'other_5';
  $handler->display->display_options['fields']['other_5']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['fields']['other_5']['field'] = 'other_5';
  $handler->display->display_options['fields']['other_5']['label'] = 'Other E';
  $handler->display->display_options['fields']['other_5']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['other_5']['link_to_entity'] = 0;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  /* Filter criterion: Indexed Lambeth Open Data Locations Entity: type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_lbl_locations_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'lambeth-locations';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Lambeth Locations';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['lambeth_open_data_locations'] = $view;

  $view = new view();
  $view->name = 'lambeth_open_data_planning';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_lbl_planning_index';
  $view->human_name = 'Lambeth Open Data Planning';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Lambeth Open Data Planning';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Indexed Lambeth Open Data Planning Entity: REFVAL */
  $handler->display->display_options['fields']['REFVAL']['id'] = 'REFVAL';
  $handler->display->display_options['fields']['REFVAL']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['fields']['REFVAL']['field'] = 'REFVAL';
  $handler->display->display_options['fields']['REFVAL']['label'] = 'Reference';
  $handler->display->display_options['fields']['REFVAL']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Planning Entity: ADDRESS */
  $handler->display->display_options['fields']['ADDRESS']['id'] = 'ADDRESS';
  $handler->display->display_options['fields']['ADDRESS']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['fields']['ADDRESS']['field'] = 'ADDRESS';
  $handler->display->display_options['fields']['ADDRESS']['label'] = 'Address';
  $handler->display->display_options['fields']['ADDRESS']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Planning Entity: PROPOSAL */
  $handler->display->display_options['fields']['PROPOSAL']['id'] = 'PROPOSAL';
  $handler->display->display_options['fields']['PROPOSAL']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['fields']['PROPOSAL']['field'] = 'PROPOSAL';
  $handler->display->display_options['fields']['PROPOSAL']['label'] = 'Proposal';
  $handler->display->display_options['fields']['PROPOSAL']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Planning Entity: DECSN */
  $handler->display->display_options['fields']['DECSN']['id'] = 'DECSN';
  $handler->display->display_options['fields']['DECSN']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['fields']['DECSN']['field'] = 'DECSN';
  $handler->display->display_options['fields']['DECSN']['label'] = 'Decision';
  $handler->display->display_options['fields']['DECSN']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Planning Entity: DATEDECISS */
  $handler->display->display_options['fields']['DATEDECISS']['id'] = 'DATEDECISS';
  $handler->display->display_options['fields']['DATEDECISS']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['fields']['DATEDECISS']['field'] = 'DATEDECISS';
  $handler->display->display_options['fields']['DATEDECISS']['label'] = 'Date';
  $handler->display->display_options['fields']['DATEDECISS']['date_format'] = 'custom';
  $handler->display->display_options['fields']['DATEDECISS']['custom_date_format'] = 'd M Y';
  $handler->display->display_options['fields']['DATEDECISS']['link_to_entity'] = 0;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  /* Filter criterion: Indexed Lambeth Open Data Planning Entity: DATEDECISS */
  $handler->display->display_options['filters']['DATEDECISS']['id'] = 'DATEDECISS';
  $handler->display->display_options['filters']['DATEDECISS']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['filters']['DATEDECISS']['field'] = 'DATEDECISS';
  $handler->display->display_options['filters']['DATEDECISS']['exposed'] = TRUE;
  $handler->display->display_options['filters']['DATEDECISS']['expose']['operator_id'] = 'DATEDECISS_op';
  $handler->display->display_options['filters']['DATEDECISS']['expose']['label'] = 'Date';
  $handler->display->display_options['filters']['DATEDECISS']['expose']['operator'] = 'DATEDECISS_op';
  $handler->display->display_options['filters']['DATEDECISS']['expose']['identifier'] = 'DATEDECISS';
  /* Filter criterion: Indexed Lambeth Open Data Planning Entity: REFVAL */
  $handler->display->display_options['filters']['REFVAL']['id'] = 'REFVAL';
  $handler->display->display_options['filters']['REFVAL']['table'] = 'search_api_index_lbl_planning_index';
  $handler->display->display_options['filters']['REFVAL']['field'] = 'REFVAL';
  $handler->display->display_options['filters']['REFVAL']['exposed'] = TRUE;
  $handler->display->display_options['filters']['REFVAL']['expose']['operator_id'] = 'REFVAL_op';
  $handler->display->display_options['filters']['REFVAL']['expose']['label'] = 'Ref';
  $handler->display->display_options['filters']['REFVAL']['expose']['operator'] = 'REFVAL_op';
  $handler->display->display_options['filters']['REFVAL']['expose']['identifier'] = 'REFVAL';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'lambeth-planning';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Lambeth Planning';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['lambeth_open_data_planning'] = $view;

  $view = new view();
  $view->name = 'lambeth_open_data_spending';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_lbl_spending_index';
  $view->human_name = 'Lambeth Open Data Spending';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Lambeth Open Data Spending';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'Month' => 'Month',
    'Year' => 'Year',
  );
  $handler->display->display_options['row_options']['separator'] = ' ';
  /* Field: Indexed Lambeth Open Data Spending Entity: Index */
  $handler->display->display_options['fields']['Index']['id'] = 'Index';
  $handler->display->display_options['fields']['Index']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['fields']['Index']['field'] = 'Index';
  $handler->display->display_options['fields']['Index']['exclude'] = TRUE;
  $handler->display->display_options['fields']['Index']['separator'] = '';
  $handler->display->display_options['fields']['Index']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Spending Entity: Supplier */
  $handler->display->display_options['fields']['Supplier']['id'] = 'Supplier';
  $handler->display->display_options['fields']['Supplier']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['fields']['Supplier']['field'] = 'Supplier';
  $handler->display->display_options['fields']['Supplier']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Spending Entity: Total */
  $handler->display->display_options['fields']['Total']['id'] = 'Total';
  $handler->display->display_options['fields']['Total']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['fields']['Total']['field'] = 'Total';
  $handler->display->display_options['fields']['Total']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['Total']['alter']['text'] = '£[Total]';
  $handler->display->display_options['fields']['Total']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['Total']['precision'] = '2';
  $handler->display->display_options['fields']['Total']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Spending Entity: Month */
  $handler->display->display_options['fields']['Month']['id'] = 'Month';
  $handler->display->display_options['fields']['Month']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['fields']['Month']['field'] = 'Month';
  $handler->display->display_options['fields']['Month']['link_to_entity'] = 0;
  /* Field: Indexed Lambeth Open Data Spending Entity: Year */
  $handler->display->display_options['fields']['Year']['id'] = 'Year';
  $handler->display->display_options['fields']['Year']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['fields']['Year']['field'] = 'Year';
  $handler->display->display_options['fields']['Year']['label'] = '';
  $handler->display->display_options['fields']['Year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['Year']['link_to_entity'] = 0;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  /* Filter criterion: Indexed Lambeth Open Data Spending Entity: Total */
  $handler->display->display_options['filters']['Total']['id'] = 'Total';
  $handler->display->display_options['filters']['Total']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['filters']['Total']['field'] = 'Total';
  $handler->display->display_options['filters']['Total']['operator'] = '>=';
  $handler->display->display_options['filters']['Total']['group'] = 1;
  $handler->display->display_options['filters']['Total']['exposed'] = TRUE;
  $handler->display->display_options['filters']['Total']['expose']['operator_id'] = 'Total_op';
  $handler->display->display_options['filters']['Total']['expose']['label'] = 'Total greater than';
  $handler->display->display_options['filters']['Total']['expose']['operator'] = 'Total_op';
  $handler->display->display_options['filters']['Total']['expose']['identifier'] = 'Total';
  /* Filter criterion: Indexed Lambeth Open Data Spending Entity: Month */
  $handler->display->display_options['filters']['Month']['id'] = 'Month';
  $handler->display->display_options['filters']['Month']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['filters']['Month']['field'] = 'Month';
  $handler->display->display_options['filters']['Month']['group'] = 1;
  $handler->display->display_options['filters']['Month']['exposed'] = TRUE;
  $handler->display->display_options['filters']['Month']['expose']['operator_id'] = 'Month_op';
  $handler->display->display_options['filters']['Month']['expose']['label'] = 'Month';
  $handler->display->display_options['filters']['Month']['expose']['operator'] = 'Month_op';
  $handler->display->display_options['filters']['Month']['expose']['identifier'] = 'Month';
  /* Filter criterion: Indexed Lambeth Open Data Spending Entity: Year */
  $handler->display->display_options['filters']['Year']['id'] = 'Year';
  $handler->display->display_options['filters']['Year']['table'] = 'search_api_index_lbl_spending_index';
  $handler->display->display_options['filters']['Year']['field'] = 'Year';
  $handler->display->display_options['filters']['Year']['group'] = 1;
  $handler->display->display_options['filters']['Year']['exposed'] = TRUE;
  $handler->display->display_options['filters']['Year']['expose']['operator_id'] = 'Year_op';
  $handler->display->display_options['filters']['Year']['expose']['label'] = 'Year';
  $handler->display->display_options['filters']['Year']['expose']['operator'] = 'Year_op';
  $handler->display->display_options['filters']['Year']['expose']['identifier'] = 'Year';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'lambeth-spending';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Lambeth Spending';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['lambeth_open_data_spending'] = $view;

  return $export;
}
