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
    $result = array();
    $type = lambeth_opendata_get_type_from_index_name($index->machine_name);
    try {
      $this->connect();
      $curl = new Apache_Solr_HttpTransport_Curl();
      $this->solr->setHttpTransport($curl);
      foreach ($items as $id => $item) {
        $node = node_load($id, NULL, TRUE);
        if (!is_object($node)) {
          watchdog('LambethOpendDataSolrService', 'Failed to load node %id', array('%id' => $id));
          continue;
        }
        $node_file = (object) $node->field_lambeth_doc_file[LANGUAGE_NONE][0];
        if (!is_file($node_file->uri)) {
          watchdog('LambethOpendDataSolrService', 'File %file not found', array('%file' => $node_file->uri));
          continue;
        }
        $url = file_create_url($node_file->uri);
        if (!$tmpfile = lambeth_opendata_rewrite_datadoc($type, $url, $node->field_lambeth_doc_name[LANGUAGE_NONE][0]['value'])) {
          watchdog('LambethOpendDataSolrService', 'Failed to rewrite %file', array('%file' => $node_file->uri));
          continue;
        }
        if (!$post_data = file_get_contents($tmpfile)) {
          watchdog('LambethOpendDataSolrService', 'Failed to get contents of %file', array('%file' => $node_file->uri));
          continue;
        }
        $response = $this->solr->addCSV($post_data);
        if ($response->getHttpStatus() == 200) {
          $result[] = $id;
        }
        else {
          watchdog('LambethOpendDataSolrService', 'HTTP status @status: @msg.', array('@status' => $response->getHttpStatus(), '@msg' => $response->getHttpStatusMessage()));
        }
      }
      if (!empty($result) && !empty($index->options['index_directly'])) {
        $this->scheduleCommit();
      }
    } catch (Exception $e) {
      watchdog('LambethOpendDataSolrService', t('Error indexing node @id: @msg.', array('@id' => $id, '@msg' => $e->getMessage())), NULL, WATCHDOG_WARNING);
    }
    return $result;
  }

  /**
   * This method has a custom, Solr-specific extension:
   * If $ids is a string other than "all", it is treated as a Solr query. All
   * items matching that Solr query are then deleted. If $index is additionally
   * specified, then only those items also lying on that index will be deleted.
   * It is up to the caller to ensure $ids is a valid query when the method is
   * called in this fashion.
   */
  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    $this->connect();
    if (is_array($ids)) {
      $this->solr->deleteByMultipleIds($ids);
    }
    elseif ($ids == 'all') {
      $this->solr->deleteByQuery('*:*');
    }
    else {
      $this->solr->deleteById($ids);
    }
    $this->scheduleCommit();
  }

}
