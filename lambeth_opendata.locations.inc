<?php
/**
 * @file
 */


/**
 *
 * entity label callback
 * @param object $entity
 */
function lambeth_opendata_locations_label($entity) {
  return $entity->index;
}

/**
 *
 * entity uri callback
 * @param object $entity
 */
function lambeth_opendata_locations_uri($entity) {
  return array(
    'path' => lambeth_opendata_locations_uri_string($entity),
  );
}

function lambeth_opendata_locations_uri_string($entity) {
  if (is_array($entity)) {
    $entity = (object)$entity;
  }
  return  '/lambeth_locations/' . $entity->index;
}

function lambeth_opendata_locations_get_indexkey() {
  if (user_access('see lambeth_opendata indexkey')) {
    return 'index';
  }
  return FALSE;
}

function lambeth_opendata_locations_creation_callback($values, $entity_type) {
  return new LBLODLocationsClass($values);
}

/**
 *
 * Main class for LBLOD locations entities.
 */
class LBLODLocationsClass extends Entity {

  var $options = array();

  // special property
  var $id;

  public function __construct(array &$values = array()) {

    //parent assigns the values to properties
    parent::__construct($values, 'lambeth_opendata_locations');
  }

  public function save() {
    return NULL;
  }
}

/**
 * Controller class for locations entities
 */
class LBLODLocationsController extends DrupalDefaultEntityController{

  public function load($values = array(), $conditions = array()) {

    $entities = array();
    try {
      foreach ($values as $data) {
        if (!is_array($data)) {

          $id = $data;
          $data = $this->fetchItem($id);
          if (empty($data)) {
            $data['name'] = 'Missing item ' . $id;
          }
          $data['id'] = $id;
          if (isset($data['error'])) {

            $data['name'] = $data['error']['message'];
          }
        }
        $entities[$data['id']] = entity_create('lambeth_opendata_locations', $data);
      }
    } catch (Exception $e) {

      watchdog('LBLODlocationsController::load', '(' . $id . ')' . $e->getMessage());
    }
    return $entities;
  }

  protected function fetchItem($id) {
    return '';
  }

  public function delete() {
    return TRUE;
  }
}
