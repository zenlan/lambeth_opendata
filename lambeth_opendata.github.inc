<?php

class LambethOpenDataGitHub {

  private $error = FALSE;
  private $message = '';
  protected $base_url = 'https://api.github.com/';
  private $datasets = array();
  private $repo_name;
  private $repo_owner;
  private $download_directory;
  private $data_directory;
  private $tmpfile;

  public function __construct($options = array()) {
    if (!function_exists('curl_init')) {
      throw new Exception('Lambeth Open Data GitHub class needs the CURL PHP extension.');
    }
    if (!function_exists('json_decode')) {
      throw new Exception('Lambeth Open Data GitHub class needs the JSON PHP extension.');
    }
    if (isset($options['download_directory'])) {
      $this->download_directory = $options['download_directory'];
    }
    else {
      $this->download_directory = sys_get_temp_dir();
    }
    if (isset($options['data_directory'])) {
      $this->data_directory = $options['data_directory'];
    }
    else {
      $this->data_directory = __DIR__;
    }
    if (isset($options['repo_owner'])) {
      $this->repo_owner = $options['repo_owner'];
    }
    if (isset($options['repo_name'])) {
      $this->repo_name = $options['repo_name'];
    }
  }

  public function get_datasets() {
    return $this->datasets;
  }

  public function get_data_directory() {
    return $this->data_directory;
  }

  public function get_download_directory() {
    return $this->download_directory;
  }

  public function get_repo_owner() {
    return $this->repo_owner;
  }

  public function get_repo_name() {
    return $this->repo_name;
  }

  public function create_packed_filename() {
    $this->tmpfile = tempnam($this->download_directory, 'lambeth.');
    if (file_exists($this->tmpfile)) {
      return $this->tmpfile;
    }
    return FALSE;
  }

  public function get_packed_filename() {
    return $this->tmpfile;
  }

  public function delete_packed_filename() {
    @@unlink($this->tmpfile);
    if (file_exists($this->tmpfile)) {
      return FALSE;
    }
    $this->tmpfile = FALSE;
    return TRUE;
  }

  function get_repo_url() {
    return $this->base_url . 'repos/' . $this->repo_owner . '/' . $this->repo_name;
  }

  function get_repo_info() {
    $url = $this->get_repo_url();
    $content = $this->curl_request($url);
    try {
      //file_put_contents('lambeth_opendata_info.txt', print_r(json_decode($content['data']), 1));
      return json_decode($content['data']);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  function get_repo_commits($since = NULL) {
    $url = $this->get_repo_url() . '/commits';
    if ($since) {
      $url .= '?since=' . date(DATE_ISO8601, strtotime($since));
    }
    $content = $this->curl_request($url);
    try {
      //file_put_contents('lambeth_opendata_coms.txt', print_r(json_decode($content['data']), 1));
      return json_decode($content['data']);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  function get_repo_references() {
    $url = $this->get_repo_url() . '/git/refs';
    $content = $this->curl_request($url);
    try {
      //file_put_contents('lambeth_opendata_refs.txt', print_r(json_decode($content['data']), 1));
      return json_decode($content['data']);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  function get_repo_tree($sha, $recursive = FALSE) {
    $url = $this->get_repo_url() .'/git/trees/' . $sha;
    if ($recursive) {
      $url .=  '?recursive=1';
    }
    $content = $this->curl_request($url);
    try {
      //file_put_contents('lambeth_opendata_tree_' . $sha . '.txt', print_r(json_decode($content['data']), 1));
      return json_decode($content['data']);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  function get_repo_blob($sha) {
    $url = $this->get_repo_url() .'/git/blobs/' . $sha;
    $content = $this->curl_request($url);
    try {
      //file_put_contents('lambeth_opendata_blob.txt', print_r(json_decode($content['data']), 1));
      return json_decode($content['data']);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  function parse_repo_tree($tree) {
    foreach ($tree->tree as $i => $branch) {
      if ($branch->type == 'tree') {
        $this->datasets[$branch->path] = $branch;
      }
      elseif ($branch->type == 'blob') {
        $tmp = pathinfo($branch->path);
        if (array_key_exists($tmp['dirname'], $this->datasets)) {
          $this->datasets[$tmp['dirname']][] = $branch;
        }
      }
    }
    return $this->datasets;
  }

  function parse_repo_tree_to_array($tree) {
    foreach ($tree->tree as $i => $branch) {
      if ($branch->type == 'tree') {
        $this->datasets[$branch->path] = array();
      }
      elseif ($branch->type == 'blob') {
        $tmp = pathinfo($branch->path);
        if (array_key_exists($tmp['dirname'], $this->datasets)) {
          $this->datasets[$tmp['dirname']][$tmp['basename']] = $tmp + (array)$branch;
          ksort($this->datasets[$tmp['dirname']]);
        }
      }
    }
    ksort($this->datasets);
    return $this->datasets;
  }

  function download_repo() {
    $url = $this->get_repo_url();
    $opts = array(CURLOPT_FOLLOWLOCATION => 1);
    $url.= '/zipball/master';  //archive: tarball or zipball
    $content = $this->curl_request($url, $opts);
    try {
      switch ($content['type']) {
        case 'application/zip':
          $file = $this->create_packed_filename();
          file_put_contents($file, print_r($content['data'], 1));
          return $file;
//        case 'application/x-gzip':
//          $file = $this->create_packed_filename();
//          file_put_contents($file, print_r($content['data'], 1));
//          return $file;
        case 'application/json':
        default:
      }
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
    }
    return FALSE;
  }

  public function unpack_repo($zipfile) {
    $result = array();
    try {
      if ($fh = zip_open($zipfile)) {
        $basedir = $this->get_data_directory();
        $i = 0;
        $zip_dir = FALSE;
        while ($zip_entry = zip_read($fh)) {
          if ($i == 0) {
            // ignore the first entry
            $i = 1;
            continue;
          }
          $file = basename(zip_entry_name($zip_entry));
          if (zip_entry_open($fh, $zip_entry, "r")) {
            $buf = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));
            zip_entry_close($zip_entry);
            if (empty($buf) || $buf === FALSE) {
              // it is a directory
              $zip_dir = $basedir . $file;
              @@mkdir($zip_dir);
              $this->datasets[] = $file;
              continue;
            }
          }
          if ($zip_dir) {
            $target = $zip_dir . '/' . $file;
            if ($fp = fopen($target, "w+")) {
              fwrite($fp, $buf);
            }
            fclose($fp);
            $result[] = $target;
          }
        }
      }
      zip_close($fh);
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
      return FALSE;
    }
    return $result;
  }

  function curl_request($url, $opts = array()) {
    try {
      $ch = curl_init($url);
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
      foreach ($opts as $k => $v) {
        curl_setopt($ch, $k, $v);
      }
      $this->clear_error();
      $response = curl_exec($ch);
      if ($response === FALSE) {
        $error = curl_error($ch);
        $this->set_error('The GitHub API returned false to ' . $url . ' with cURL error ' . $error);
        return FALSE;
      }
      $info = curl_getinfo($ch);
      curl_close($ch);
      if ($info['http_code'] != 200) {
        $msg = 'The GitHub API returned code ' . $info['http_code'] . ' to ' .$url . '(' . $info['content_type'] . ')';
        if ($info['content_type'] == 'application/json') {
          $msg .= ' :: Response: ' . print_r(json_decode($response), TRUE);
        }
        $this->set_error($msg, $info['http_code']);
        return FALSE;
      }
    } catch (Exception $e) {
      $this->set_error($e->getMessage(), $e->getCode());
      return FALSE;
    }
    return array('type' => $info['content_type'], 'data' => $response);
  }

  public function get_authorization() {
    $opts = array(CURLOPT_USERPWD => json_encode($this->repo_owner . ':' . 'password'));
    if ($response = $this->curl_request($this->base_url, $opts)) {
      $result = json_decode($response);
      $token = $result->token;
      //$this->set_access_token($dataset, $token);
      return $result;
    }
    return FALSE;
  }

  function set_error($msg = 'unkown', $code = 1) {
    $this->error = $code;
    $this->message = $msg;
  }

  function get_error_code() {
    return $this->error;
  }

  function get_error_message() {
    return $this->message;
  }

  function clear_error() {
    $this->error = FALSE;
    $this->message = '';
  }

}
