<?php

/**
 * Expose G2-related pages as a context condition.
 */
class context_condition_g2 extends context_condition {
  function condition_values() {
    $values = array();
    $values['g2_admin'] = t('G2 admin pages: settings');
    $values['g2_node'] = t('G2 node pages');
    $values['g2_user'] = t('G2 user pages: main, per-initial, entries list');
    return $values;
  }

  function execute($value) {
    foreach ($this->get_contexts($value) as $key => $context) {
      $this->condition_met($context, $value);
    }
  }
}
