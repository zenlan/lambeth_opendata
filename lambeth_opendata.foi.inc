<?php
/**
 * @file
 */


/**
 *
 * entity label callback
 * @param object $entity
 */
function lambeth_opendata_foi_label($entity) {
  return $entity->Summary;
}

/**
 *
 * entity uri callback
 * @param object $entity
 */
function lambeth_opendata_foi_uri($entity) {
  return array(
    'path' => lambeth_opendata_uri_string($entity),
  );
}

function lambeth_opendata_foi_uri_string($entity) {
  if (is_array($entity)) {
    $entity = (object)$entity;
  }
  $id = str_replace($entity->type . '-', '', $entity->id);
  return  '/' . $entity->type . '/' . $id;
}

function lambeth_opendata_foi_get_indexkey() {
  if (user_access('see lambeth_opendata indexkey')) {
    return 'identifier';
  }
  return FALSE;
}
function lambeth_opendata_foi_creation_callback($values, $entity_type) {
  return new LBLODFoIClass($values);
}

/**
 *
 * Main class for LBLOD FoI entities.
 */
class LBLODFoIClass extends Entity {

  var $options = array();

  // special property
  var $id;

  public function __construct(array &$values = array()) {

    //parent assigns the values to properties
    parent::__construct($values, 'lambeth_opendata_foi');
  }

  public function save() {
    return NULL;
  }
}

/**
 * Controller class for FoI entities
 */
class LBLODFoIController extends DrupalDefaultEntityController{

  public function load($values = array(), $conditions = array()) {

    $entities = array();
    try {
      foreach ($values as $data) {
        if (!is_array($data)) {

          $id = $data;
          $data = $this->fetchItemByIdentifier($id);
          if (empty($data)) {
            $data['Summary'] = 'Missing item ' . $id;
          }
          $data['id'] = $identifier;
          if (isset($data['error'])) {

            $data['Summary'] = $data['error']['message'];
          }
        }
        $entities[$data['id']] = entity_create('lambeth_opendata_foi', $data);
      }
    } catch (Exception $e) {

      watchdog('LBLODFoIController::load', '(' . $id . ')' . $e->getMessage());
    }
    return $entities;
  }

  protected function fetchItemByIdentifier($identifier) {
    return '';
  }

  public function delete() {
    return TRUE;
  }
}
