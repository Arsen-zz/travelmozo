/**
 * @file
 * Vertical tabs UI support for g2.module
 *
 * @copyright Copyright (C) 2011 Frederic G. MARAND for Ouest Systèmes Informatiques (OSInet, OSI)
 *
 * @license Licensed under the CeCILL, version 2 and General Public License version 2 or later
 *
 * License note: G2 is distributed by OSInet to its customers under the
 * CeCILL 2.0 license. OSInet support services only apply to the module
 * when distributed by OSInet, not by any third-party further down the
 * distribution chain.
 *
 * If you obtained G2 from drupal.org, that site received it under the General
 * Public License version 2 or later (GPLv2+) and can therefore distribute it
 * under the same terms, and so can you and just anyone down the chain as long
 * as the GPLv2+ terms are abided by, the module distributor in that case being
 * the drupal.org organization or the downstream distributor, not OSInet.
 */

(function ($) {

Drupal.behaviors.g2FieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-publishing', context).drupalSetSummary(function (context) {
      var complementLength = $('#edit-complement', context).val().length;
      var originLength = $('#edit-origin', context).val().length;
      var ret = null;

      if (complementLength) {
        ret = originLength
          ? Drupal.t('Complement + IP/Origin')
          : Drupal.t('Complement only');
      }
      else {
        ret = originLength
          ? Drupal.t('IP/Origin only')
          : Drupal.t('No editor info');
      }
      return ret;
    });
  }
};

})(jQuery);
