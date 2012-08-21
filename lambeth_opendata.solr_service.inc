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

  /**
   * This method avoids an attempt by Search API Solr
   * to delete read-only indexes when deleting a server object
   *
   * @see SearchApiSolrService::removeIndex()
   * patch pending to removeIndex to check for read-only index
   * this can then be deprecated
   */
  public function removeIndex($index) {
    if (module_exists('search_api_views')) {
      views_invalidate_cache();
    }
  }

}
