<?php
/**
 * Expose themes as context reactions.
 */
class context_reaction_g2_template extends context_reaction_theme {

  function options_form($context) {
    $form = array();
    $form['info'] = array(
      '#type' => 'markup',
      '#markup' => t('Enable the page-g2 page template'),
    );

    $form['g2-template'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    return $form;
  }

  function options_form_submit($values) {
    return array('g2-template' => 1);
  }

  /**
   * Enable additional template.
   */
  function execute(&$vars) {
    foreach ($this->get_contexts() as $name => $context) {
      if (!empty($context->reactions['g2_template']['g2-template']) && !in_array('page-g2', $vars['theme_hook_suggestions'])) {
        $vars['theme_hook_suggestions'][] = 'page__g2';
      }
    }
  }
}
