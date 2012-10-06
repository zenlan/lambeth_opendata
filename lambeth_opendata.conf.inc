<?php

Class LambethOpendataConf {

  private $conf_file;
  private $conf_path;
  private $conf_array;

  public function __construct($filename = NULL, $filepath = NULL) {
    if (is_null($filename)) {
      $filename = 'lambeth_opendata.xml';
    }
    if (is_null($filepath)) {
      $filepath = DRUPAL_ROOT . '/' . drupal_get_path('module', 'lambeth_opendata') . '/conf/';
    }
    $this->conf_path = rtrim(trim($filepath), '/') . '/';
    $this->conf_file = $this->conf_path . trim($filename);
    $this->conf_array = array();
  }

  public function get_conf_path() {
    return $this->conf_path;
  }

  public function get_conf_file() {
    return $this->conf_file;
  }

  public function get_conf_array() {
    return $this->conf_array;
  }

  public function get_conf_datasets() {
    return $this->conf_array['datastore']['datasets'];
  }

  public function get_conf_dataset_array($dataset) {
    return $this->conf_array['datastore']['datasets'][$dataset]['tables'];
  }

  public function load_conf() {
    $result = array();
    try {
      $result = $this->parse_main_conf();
      foreach ($result['datastore']['datasets'] as $k => $v) {
        $conf_file = $this->conf_path . $v['conf'];
        $result['datastore']['datasets'][$k]['tables'] = $this->parse_dataset_conf($this->conf_path . $v['conf']);
      }
      $result['conf_file'] = $this->conf_file;
      $result['conf_path'] = $this->conf_path;
      $this->conf_array = $result;
      //file_put_contents(__DIR__ . '/conf_array.txt', print_r($result,1));
      return $this->conf_array;
    } catch (Exception $e) {
      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
    }
    return FALSE;
  }

  private function parse_main_conf() {
    try {
      if (!file_exists($this->conf_file)) {
        watchdog('lambeth_opendata', 'Not found: !file', array('!file' => $this->conf_file));
      }
      else {
        $xml = simplexml_load_file($this->conf_file);
        $result = array();
        foreach ($xml->attributes() as $name => $value) {
          $result[$name] = (string) $xml->attributes()->$name;
        }
        $source = $xml->xpath('//schema/source');
        while (list(, $node) = each($source)) {
          foreach ($node as $k => $v) {
            $result['source'][$k] = (string) $v;
          }
        }
        foreach ($xml->xpath('//schema/datastore') as $node) {
          foreach ($node->attributes() as $name => $value) {
            $result['datastore'][$name] = (string) $node->attributes()->$name;
          }
          foreach ($node as $name => $props) {
            foreach ($props as $k => $v) {
              $result['datastore']['datasets'][$name][$k] = (string) $v;
            }
          }
        }
        return $result;
      }
    } catch (Exception $e) {
      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
    }
    return FALSE;
  }

  public function parse_dataset_conf($conf_file) {
    try {
      if (!file_exists($conf_file)) {
        watchdog('lambeth_opendata', 'Not found: !file', array('!file' => $conf_file));
      }
      else {
        $xml = simplexml_load_file($conf_file);
        $result = array();
        foreach ($xml->attributes() as $name => $value) {
          $result[$name] = (string) $xml->attributes()->$name;
        }
        foreach ($xml->xpath('//dataset/property') as $node) {
          $property = NULL;
          foreach ($node->attributes() as $name => $value) {
            $property[$name] = (string) $node->attributes()->$name;
          }
          if (is_array($property) && isset($property['name'])) {
            $result['properties'][$property['name']] = $property;
          }
        }
        foreach ($xml->xpath('//dataset/map') as $node) {
          $table = array('fields' => array());
          foreach ($node->attributes() as $name => $value) {
            $table[$name] = (string) $node->attributes()->$name;
          }
          foreach ($node as $fields) {
            $field = array();
            foreach ($fields->attributes() as $k => $v) {
              $field[$k] = (string) $v;
            }
            $table['fields'][$field['key']] = $field;
          }
          $result['maps'][$table['name']] = $table;
        }
        return $result;
      }
    } catch (Exception $e) {
      watchdog('lambeth_opendata', 'Exception: !e', array('!e' => $e->getMessage()));
    }
    return FALSE;
  }

  function xml_to_array($xml) {
    $result = array();
    foreach ($xml->attributes() as $name => $value) {
      $result[$name] = (string) $xml->attributes()->$name;
    }
    for ($xml->rewind(); $xml->valid(); $xml->next()) {
      if (!array_key_exists($xml->key(), $result)) {
        $result[$xml->key()] = array();
      }
      if ($xml->hasChildren()) {
        $result[$xml->key()][] = lambeth_opendata_xml_to_array($xml->current());
      }
      else {
        $xlm = $xml->current();
        $val = '';
        foreach ($xlm->attributes() as $name => $value) {
          $val[$name] = (string) $xlm->attributes()->$name;
        }
        if (empty($val)) {
          $val = strval($xlm);
        }
        $result[$xml->key()] = $val;
      }
    }
    return $result;
  }

}