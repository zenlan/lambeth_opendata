<?php
/**
 * @file
 */


/**
 *
 * entity label callback
 * @param object $entity
 */
function lambeth_opendata_spending_label($entity) {
  return $entity->Index;
}

/**
 *
 * entity uri callback
 * @param object $entity
 */
function lambeth_opendata_spending_uri($entity) {
  return array(
    'path' => lambeth_opendata_spending_uri_string($entity),
  );
}

function lambeth_opendata_spending_uri_string($entity) {
  if (is_array($entity)) {
    $entity = (object)$entity;
  }
  return  '/lambeth_spending/' . $entity->Index;
}

function lambeth_opendata_spending_get_indexkey() {
  if (user_access('see lambeth_opendata indexkey')) {
    return 'Index';
  }
  return FALSE;
}
function lambeth_opendata_spending_creation_callback($values, $entity_type) {
  return new LBLODSpendingClass($values);
}

/**
 *
 * Main class for LBLOD spending entities.
 */
class LBLODSpendingClass extends Entity {

  var $options = array();

  // special property
  var $id;

  public function __construct(array &$values = array()) {

    //parent assigns the values to properties
    parent::__construct($values, 'lambeth_opendata_spending');
  }

  public function save() {
    return NULL;
  }
}

/**
 * Controller class for spending entities
 */
class LBLODSpendingController extends DrupalDefaultEntityController{

  public function load($values = array(), $conditions = array()) {

    $entities = array();
    try {
      foreach ($values as $data) {
        if (!is_array($data)) {

          $id = $data;
          $data = $this->fetchItem($id);
          if (empty($data)) {
            $data['Supplier'] = 'Missing item ' . $id;
          }
          $data['id'] = $id;
          if (isset($data['error'])) {

            $data['Supplier'] = $data['error']['message'];
          }
        }
        $entities[$data['id']] = entity_create('lambeth_opendata_spending', $data);
      }
    } catch (Exception $e) {

      watchdog('LBLODSpendingController::load', '(' . $id . ')' . $e->getMessage());
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
