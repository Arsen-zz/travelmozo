<?php
/**
 * @file
 *   template for item_reviews panel
 */

if (!empty($item->CustomerReviews->IFrameURL)) {
?>
<div class="customer_review">
<iframe class="customer_review_iframe" src="<?php print check_url($item->CustomerReviews->IFrameURL);?>"/>
</div>
<?php
}
