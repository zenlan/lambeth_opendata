<?php

// $Id$

/**
 * @file
 */

/**
 * read cache
 * for each row:
 *  check if node exists for this file
 *  create empty file /tmp/$name
 *  put contents of href into new file
 *  analyse file
 *  create datadocument node
 *  attach file
 *  log errors
 *  log details
 */
function lambeth_opendata_import($type, $data) {
  foreach ($data as $k => $v) {
    $v['nid'] = _lambeth_opendata_get_node_by_uid($v['uid']);
    if ($v['file_obj'] = _lambeth_opendata_upload_file_managed($type, $v)) {
      _lambeth_opendata_node_new_datadoc($type, $v);
    }
    return TRUE;
  }
  return TRUE;
}

function _lambeth_opendata_node_new_datadoc($type, $data) {

  try {
    if ($data['nid']) {
      //file_usage_add($data['file_obj'], 'file', 'node', $data['nid']);
      $node = node_load($data['nid']);
    }
    else {
      $node = node_type_set_defaults();
      $node->language = LANGUAGE_NONE;
      $node->path = array();
      $node->path['language'] = $node->language;
      $node->path['pathauto'] = 1;
      $node->path['alias'] = '';
      $node->log = 'This node revision was imported on ' . date('c');
      $node->status = 0;
      $node->type = 'lambeth_opendata_document';
      $node->body = '';
    }
    $node->title = $data['title'];
    $node->field_lambeth_doc_uid[$node->language][0]['value'] = $data['uid'];
    $node->field_lambeth_doc_name[$node->language][0]['value'] = $data['filename'];
    $node->field_lambeth_doc_url[$node->language][0]['value'] = $data['dirname'];
    $node->field_lambeth_doc_filename[$node->language][0]['value'] = $data['basename'];

    $tax = array_pop(taxonomy_get_term_by_name($type, 'lambeth_opendata_category'));
    if (empty($tax)) {
      watchdog('lambeth_opendata', t('Failed to get tid for !type', array('!type' => $type)));
      $tax = new stdClass();
      $tax->tid = 0;
    }
    $node->field_lambeth_doc_type[$node->language][0] = $tax->tid;

    $field_doc = array(
      'fid' => $data['file_obj']->fid,
      'filename' => $data['file_obj']->filename,
      'filemime' => $data['file_obj']->filemime,
      'uid' => 1,
      'uri' => $data['file_obj']->uri,
      'status' => 1,
      'display' => 1,
    );
    $node->field_lambeth_doc_file[$node->language][0] = $field_doc;

    node_object_prepare($node);
    node_save($node);
    if (!$node->nid) {
      watchdog('lambeth_opendata', t('Node did not return nid for !type', array('!type' => $type)));
    }
  } catch (Exception $e) {
    watchdog('lambeth_opendata', t('Exception: !e', array('!e' => $e->getMessage())));
  }
  return TRUE;
}

function _lambeth_opendata_upload_file_managed($type, $data) {
  global $_lambeth_opendata_public_dir;
  $dir = $_lambeth_opendata_public_dir . $type;
  if (!file_valid_uri($dir)) {
    watchdog('lambeth_opendata', t('Invalid uri !dir', array('!dir', $dir)));
    return FALSE;
  }
  if (!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    watchdog('lambeth_opendata', t('Error preparing directory !dir', array('!dir', $dir)));
    return FALSE;
  }
  $url = $data['dirname'] . '/' . $data['basename'];
  if (!$content = file_get_contents($url)) {
    watchdog('lambeth_opendata', t('Error retrieving contents from !url', array('!url', $url)));
    return FALSE;
  }
  $file = $dir . '/' . $data['basename'];
  if (!$file_obj = file_save_data($content, $file, FILE_EXISTS_REPLACE)) {
    watchdog('lambeth_opendata', t('Error saving !file to !dir', array('!file' => $file)));
    return FALSE;
  }
  return $file_obj;
}

function _lambeth_opendata_get_node_by_uid($duid) {
  $query = db_select('field_revision_field_lambeth_doc_uid', 'f');
  $query->addField('f', 'entity_id');
  $query->condition('f.field_lambeth_doc_uid_value', $duid, '=');
  $res = $query->execute();
  $nid = $res->fetchField();
  return $nid;
}
