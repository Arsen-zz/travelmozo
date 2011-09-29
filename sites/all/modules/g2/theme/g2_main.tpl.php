<?php
/**
 * @file
 * Template for G2 main page.
 *
 * - $alphabar: the configured alphabar
 * - $text: the text used to build the page
 */
?>
<p><?php echo $alphabar; ?></p><?php
echo $text;
?><p><?php if (!empty($text)) echo $alphabar; ?></p>
