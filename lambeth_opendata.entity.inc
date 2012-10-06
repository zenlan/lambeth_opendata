<?php

/**
 * @file
 * Entry point for the LambethOpendata module.
 *
 * @ingroup LambethOpendata
 */
//define('LAMBETH_OPENDATA_ADMINUI_URL', 'admin/structure/lambeth_opendata');

/**
 * Dataset entity loader
 *
 * @param array $id
 *   The id of a dataset entity to load
 *   or array of ids
 *   or FALSE to return all entities.
 *
 * @return object
 *   A dataset entity array or FALSE if $id does not exist.
 */
function lambeth_opendata_dataset_load($id, $entity_type) {
  if ($id) {
    $result = entity_load($entity_type, is_array($id) ? $id : array($id));
    return ((!is_array($id)) && isset($result[$id]) ? $result[$id] : $id);
  }
  else {
    $result = entity_load_multiple_by_name($entity_type, FALSE);
    return $result;
  }
}


/**
 * Entity creation callback
 *
 * @param array $values
 *   Array of values to assign as object properties
 */
function lambeth_opendata_dataset_creation_callback($values) {
  return new LambethDatasetEntity($values);
}

/**
 * Controller class for Dataset entities
 */
class LambethDatasetEntityController extends DrupalDefaultEntityController {

  /**
   * @var object
   *   The LambethOpendata entity that holds the record data for this entity
   */
  //public $datasetEntityType;

  /**
   * Constructor requires an entityType
   *
   * @param string $entityType
   *   Entity object
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
    $this->hookLoadArguments = array($entityType);
    //$this->datasetEntityType = lambeth_opendata_load($entityType);
  }

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
        $entities[$data['id']] = entity_create('lambeth_opendata_' . $this->entityType, $data);
      }
    } catch (Exception $e) {

      watchdog('LambethDatasetEntityController::load', '(' . $id . ')' . $e->getMessage());
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

/**
 * Return the default url for this entity
 *
 * @param object $entity
 *   Entity object
 */
function lambeth_opendata_dataset_entity_url($entity) {
  return array(
    'path' => $entity->entity_type . '/' . $entity->{$entity->getIdKey()});
}
/**
 * Debug Dataset entities
 *
 * @param array $entities
 *   Array of entities
 * @param string $entity_type
 *   Entity type
 */
function lambeth_opendata_dataset_view_debug($entities, $entity_type) {
  $result = array();
  foreach ($entities as $k => $entity) {
    $props = get_object_vars($entity);
    foreach ($props as $k => $v) {
      if (is_scalar($v)) {
        $mltlink = FALSE;
        if ($k == 'title') {

        }
        else {
          if (in_array($k, $entity->lambeth_opendataEntity->indexed)) {
            $mlt = lambeth_opendata_utils_morelikethisfield($entity, array($k => $k));
            if (isset($mlt[$k])) {
              $mltlink = l(t(' (@count more like this)', array(
                    '@count' => $mlt[$k]['count'])
                  ), $mlt[$k]['url'], array(
                'attributes' => array(
                  'target' => '_blank',
                ),
                  )
              );
            }
          }
        }
        $result[] = $k . ': ' . $v . ($mltlink ? $mltlink : '');
      }
      elseif (is_array($v)) {
        $result[] = $k . ': ' . print_r($v, TRUE);
      }
    }
  }
  return $result;
}
