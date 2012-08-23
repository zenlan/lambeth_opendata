<?php
/**
 * @file
 */


/**
 *
 * entity label callback
 * @param object $entity
 */
function lambeth_opendata_planning_label($entity) {
  return $entity->REFVAL;
}

/**
 *
 * entity uri callback
 * @param object $entity
 */
function lambeth_opendata_planning_uri($entity) {
  return array(
    'path' => lambeth_opendata_planning_uri_string($entity),
  );
}

function lambeth_opendata_planning_uri_string($entity) {
  if (is_array($entity)) {
    $entity = (object)$entity;
  }
  return  '/lambeth_planning/' . $entity->REFVAL;
}

function lambeth_opendata_planning_get_indexkey() {
  if (user_access('see lambeth_opendata indexkey')) {
    return 'REFVAL';
  }
  return FALSE;
}
function lambeth_opendata_planning_creation_callback($values, $entity_type) {
  return new LBLODPlanningClass($values);
}

/**
 *
 * Main class for LBLOD planning entities.
 */
class LBLODPlanningClass extends Entity {

  var $options = array();

  // special property
  var $id;

  public function __construct(array &$values = array()) {

    //parent assigns the values to properties
    parent::__construct($values, 'lambeth_opendata_planning');
  }

  public function save() {
    return NULL;
  }
}

/**
 * Controller class for planning entities
 */
class LBLODPlanningController extends DrupalDefaultEntityController{

  public function load($values = array(), $conditions = array()) {

    $entities = array();
    try {
      foreach ($values as $data) {
        if (!is_array($data)) {

          $id = $data;
          $data = $this->fetchItem($id);
          if (empty($data)) {
            $data['PROPOSAL'] = 'Missing item ' . $id;
          }
          $data['id'] = $id;
          if (isset($data['error'])) {

            $data['PROPOSAL'] = $data['error']['message'];
          }
        }
        $entities[$data['id']] = entity_create('lambeth_opendata_planning', $data);
      }
    } catch (Exception $e) {

      watchdog('LBLODplanningController::load', '(' . $id . ')' . $e->getMessage());
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
