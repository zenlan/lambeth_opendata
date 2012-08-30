<?php
/**
 * @file
 * lambeth_open_data_solr.features.inc
 */

/**
 * Implements hook_views_api().
 */
function lambeth_open_data_solr_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function lambeth_open_data_solr_default_search_api_index() {
  $items = array();
  $items['lbl_foi_index'] = entity_import('search_api_index', '{
    "name" : "LBL FoI Index",
    "machine_name" : "lbl_foi_index",
    "description" : null,
    "server" : "lbl_foi_server",
    "item_type" : "lambeth_opendata_foi",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "Ref" : { "type" : "integer" },
        "Received" : { "type" : "date" },
        "Summary" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      }
    },
    "enabled" : "1",
    "read_only" : "1"
  }');
  $items['lbl_locations_index'] = entity_import('search_api_index', '{
    "name" : "LBL Locations Index",
    "machine_name" : "lbl_locations_index",
    "description" : null,
    "server" : "lbl_locations_server",
    "item_type" : "lambeth_opendata_locations",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "index" : { "type" : "text" },
        "type" : { "type" : "text" },
        "name" : { "type" : "text", "boost" : "8.0" },
        "address" : { "type" : "text", "boost" : "5.0" },
        "postcode" : { "type" : "text" },
        "telephone" : { "type" : "text" },
        "openhours" : { "type" : "text" },
        "other_1" : { "type" : "text" },
        "other_2" : { "type" : "text" },
        "other_3" : { "type" : "text" },
        "other_4" : { "type" : "text" },
        "other_5" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      }
    },
    "enabled" : "1",
    "read_only" : "1"
  }');
  $items['lbl_planning_index'] = entity_import('search_api_index', '{
    "name" : "LBL Planning Index",
    "machine_name" : "lbl_planning_index",
    "description" : null,
    "server" : "lbl_planning_server",
    "item_type" : "lambeth_opendata_planning",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "REFVAL" : { "type" : "text" },
        "DATEDECISS" : { "type" : "date" },
        "ADDRESS" : { "type" : "text" },
        "PROPOSAL" : { "type" : "text" },
        "DECSN" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      }
    },
    "enabled" : "1",
    "read_only" : "1"
  }');
  $items['lbl_spending_index'] = entity_import('search_api_index', '{
    "name" : "LBL Spending Index",
    "machine_name" : "lbl_spending_index",
    "description" : null,
    "server" : "lbl_spending_server",
    "item_type" : "lambeth_opendata_spending",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "Index" : { "type" : "integer" },
        "Total" : { "type" : "decimal" },
        "Supplier" : { "type" : "text" },
        "Month" : { "type" : "text" },
        "Year" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      }
    },
    "enabled" : "1",
    "read_only" : "1"
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function lambeth_open_data_solr_default_search_api_server() {
  $items = array();
  $items['lbl_foi_server'] = entity_import('search_api_server', '{
    "name" : "LBL FoI Server",
    "machine_name" : "lbl_foi_server",
    "description" : "",
    "class" : "lambeth_opendata_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr\\/lambeth_foi",
      "default_field" : "Ref",
      "http_user" : "admin",
      "http_pass" : "admin",
      "excerpt" : 1,
      "retrieve_data" : 1,
      "highlight_data" : 1
    },
    "enabled" : "1"
  }');
  $items['lbl_locations_server'] = entity_import('search_api_server', '{
    "name" : "LBL Locations Server",
    "machine_name" : "lbl_locations_server",
    "description" : "",
    "class" : "lambeth_opendata_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr\\/lambeth_locations",
      "default_field" : "index",
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 1,
      "retrieve_data" : 1,
      "highlight_data" : 1
    },
    "enabled" : "1"
  }');
  $items['lbl_planning_server'] = entity_import('search_api_server', '{
    "name" : "LBL Planning Server",
    "machine_name" : "lbl_planning_server",
    "description" : "",
    "class" : "lambeth_opendata_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr\\/lambeth_planning",
      "default_field" : "REFVAL",
      "http_user" : "admin",
      "http_pass" : "admin",
      "excerpt" : 1,
      "retrieve_data" : 1,
      "highlight_data" : 1
    },
    "enabled" : "1"
  }');
  $items['lbl_spending_server'] = entity_import('search_api_server', '{
    "name" : "LBL Spending Server",
    "machine_name" : "lbl_spending_server",
    "description" : "",
    "class" : "lambeth_opendata_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr\\/lambeth_spending",
      "default_field" : "Index",
      "http_user" : "admin",
      "http_pass" : "admin",
      "excerpt" : 1,
      "retrieve_data" : 1,
      "highlight_data" : 1
    },
    "enabled" : "1"
  }');
  return $items;
}