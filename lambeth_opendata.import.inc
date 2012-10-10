<?php

/**
 * @file
 */
function lambeth_opendata_import_from_github($type, $data, $options) {
  global $_lambeth_opendata_public_dir;
  $dir = $_lambeth_opendata_public_dir . $type;
  $result = array();
  $git = new LambethOpenDataGitHub($options);
  foreach ($data as $path) {
    $content = $git->get_repo_content($path);
    $node['nid'] = _lambeth_opendata_get_node_by_sha($content->sha);
    $pathinfo = pathinfo($content->path);
    $node['sha'] = $content->sha;
    $node['path'] = $content->path;
    $node['title'] = $pathinfo['basename'];
    switch ($content->encoding) {
      case 'base64':
        $pathinfo['content'] = base64_decode($content->content);
        break;
      default:
    }
    if ($node['file_obj'] = _lambeth_opendata_upload_file_unmanaged($pathinfo, $dir)) {
      $result[] = _lambeth_opendata_node_new_datadoc($type, $node);
    }
  }
  return $result;
}

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
  global $_lambeth_opendata_public_dir;
  $dir = $_lambeth_opendata_public_dir . $type;
  $result = array();
  foreach ($data as $k => $v) {
    $v['nid'] = _lambeth_opendata_get_node_by_uid($v['uid']);
    if ($v['file_obj'] = _lambeth_opendata_upload_file_unmanaged($v, $dir)) {
      $result[] = _lambeth_opendata_node_new_datadoc($type, $v);
    }
  }
  return $result;
}

function lambeth_opendata_download($type, $data) {
  global $_lambeth_opendata_private_dir;
  $dir = $_lambeth_opendata_private_dir . $type;
  $result = array();
  foreach ($data as $k => $v) {
    $result[] = _lambeth_opendata_upload_file_unmanaged($v, $dir);
  }
  return $result;
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
      $node->status = 1;
      $node->type = 'lambeth_opendata_document';
      $node->body = '';
    }
    $node->title = $data['title'];
    $node->field_lambeth_dataset[$node->language][0]['value'] = $type;
    $node->field_lambeth_git_sha[$node->language][0]['value'] = $data['sha'];
    $node->field_lambeth_git_url[$node->language][0]['value'] = $data['path'];
    $field_doc = array(
      'fid' => $data['file_obj']->fid,
      'filename' => $data['file_obj']->filename,
      'filemime' => $data['file_obj']->filemime,
      'uid' => 1,
      'uri' => $data['file_obj']->uri,
      'status' => 1,
      'display' => 1,
    );
    $node->field_lambeth_file[$node->language][0] = $field_doc;
    node_object_prepare($node);
    node_save($node);
    if (!$node->nid) {
      watchdog('lambeth_opendata', t('Node did not return nid for !type', array('!type' => $type)));
      return FALSE;
    }
  } catch (Exception $e) {
    watchdog('lambeth_opendata', t('Exception: !e', array('!e' => $e->getMessage())));
    return FALSE;
  }
  return $node->nid;
}

function _lambeth_opendata_upload_file_unmanaged($data, $dir) {
  if (!file_valid_uri($dir)) {
    watchdog('lambeth_opendata', t('Invalid uri !dir', array('!dir', $dir)));
    return FALSE;
  }
  if (!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    watchdog('lambeth_opendata', t('Error preparing directory !dir', array('!dir', $dir)));
    return FALSE;
  }
  if (!isset($data['content'])) {
    $url = $data['dirname'] . '/' . $data['basename'];
    if (!$data['content'] = file_get_contents($url)) {
      watchdog('lambeth_opendata', t('Error retrieving contents from !url', array('!url', $url)));
      return FALSE;
    }
  }
  $file = $dir . '/' . $data['basename'];
  if (!$file_obj = file_save_data($data['content'], $file, FILE_EXISTS_REPLACE)) {
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

function _lambeth_opendata_get_node_by_sha($sha) {
  $query = db_select('field_revision_field_lambeth_git_sha', 'f');
  $query->addField('f', 'entity_id');
  $query->condition('f.field_lambeth_git_sha_value', $sha, '=');
  $res = $query->execute();
  $nid = $res->fetchField();
  return $nid;
}
