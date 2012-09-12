<?php
/**
 * @file
 * lambeth_opendata_solr.features.inc
 */

/**
 * Implements hook_views_api().
 */
function lambeth_opendata_solr_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function lambeth_opendata_solr_default_search_api_index() {
  $items = array();
  $items['lbl_document_index'] = entity_import('search_api_index', '{
    "name" : "LBL Document Index",
    "machine_name" : "lbl_document_index",
    "description" : null,
    "server" : "lbl_document_server",
    "item_type" : "node",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "is_new" : { "type" : "boolean" },
        "type" : { "type" : "text" },
        "title" : { "type" : "text" },
        "status" : { "type" : "integer" },
        "created" : { "type" : "date" },
        "changed" : { "type" : "date" },
        "field_tags" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_lambeth_doc_filename" : { "type" : "text" },
        "field_lambeth_doc_name" : { "type" : "text" },
        "field_lambeth_doc_type" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "search_api_language" : { "type" : "string" },
        "body:value" : { "type" : "text" },
        "body:summary" : { "type" : "text" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : {
            "default" : "0",
            "bundles" : { "lambeth_opendata_document" : "lambeth_opendata_document" }
          }
        },
        "search_api_alter_node_access" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "type" : true,
              "title" : true,
              "field_lambeth_doc_filename" : true,
              "field_lambeth_doc_name" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "type" : true,
              "title" : true,
              "field_lambeth_doc_filename" : true,
              "field_lambeth_doc_name" : true
            },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "type" : true,
              "title" : true,
              "field_lambeth_doc_filename" : true,
              "field_lambeth_doc_name" : true
            },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "type" : true,
              "title" : true,
              "field_lambeth_doc_filename" : true,
              "field_lambeth_doc_name" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0"
  }');
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
      },
      "data_alter_callbacks" : {
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : false } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : { "Summary" : true } }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : { "Summary" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "Summary" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "Summary" : true },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0"
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
    "read_only" : "0"
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
    "read_only" : "0"
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
    "read_only" : "0"
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function lambeth_opendata_solr_default_search_api_server() {
  $items = array();
  $items['lbl_document_server'] = entity_import('search_api_server', '{
    "name" : "LBL Document Server",
    "machine_name" : "lbl_document_server",
    "description" : "",
    "class" : "search_api_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr\\/lambeth_docs",
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0
    },
    "enabled" : "1"
  }');
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
      "http_user" : "",
      "http_pass" : "",
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
      "http_user" : "",
      "http_pass" : "",
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
