<?php
/**
 * Search API Solr service class extension
 */
class LambethOpendDataSolrService extends SearchApiSolrService {

  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);
    $form['default_field'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Field'),
      '#description' => t('The field to use in a ping request.'),
      '#default_value' => $this->options['default_field'],
    );
    return $form;
  }

  /**
   * This method avoids an exception that can be thrown
   * by Search API Solr service which expects all string fields
   * to be prefaced with "ss_" and all facet fields prefaced with "f_ss"
   *
   * @see SearchApiSolrService::getFacetField()
   */
  protected function getFacetField($field) {
    return $field;
  }

  public function indexItems(SearchApiIndex $index, array $items) {
    $documents = array();
    $ret = array();
    $index_id = $index->machine_name;
    $fields = $this->getFieldNames($index);

    foreach ($items as $id => $item) {
      try {
        $doc = new Apache_Solr_Document();
        $doc->setField('id', $this->createId($index_id, $id));
        $doc->setField('index_id', $index_id);
        $doc->setField('item_id', $id);

        foreach ($item as $key => $field) {
          if (!isset($fields[$key])) {
            throw new SearchApiException(t('Unknown field @field.', array('@field' => $key)));
          }
          $this->addIndexField($doc, $fields[$key], $field['value'], $field['type']);
        }

        $documents[] = $doc;
        $ret[] = $id;
      }
      catch (Exception $e) {
        watchdog('search_api_solr', t('An error occurred while indexing @type with ID @id: @msg.', array('@type' => $index->item_type, '@id' => $id, '@msg' => $e->getMessage())), NULL, WATCHDOG_WARNING);
      }
    }

    if (!$documents) {
      return array();
    }
    try {
      $this->connect();
      $response = $this->solr->addDocuments($documents);
      if ($response->getHttpStatus() == 200) {
        if (!empty($index->options['index_directly'])) {
          $this->scheduleCommit();
        }
        return $ret;
      }
      throw new SearchApiException(t('HTTP status @status: @msg.',
          array('@status' => $response->getHttpStatus(), '@msg' => $response->getHttpStatusMessage())));
    }
    catch (Exception $e) {
      watchdog('search_api_solr', t('An error occurred while indexing: @msg.', array('@msg' => $e->getMessage())), NULL, WATCHDOG_ERROR);
    }
    return array();
  }


}
