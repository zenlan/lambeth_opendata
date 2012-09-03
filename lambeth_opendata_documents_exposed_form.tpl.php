<?php
/**
 * @file
 * Template file for lambeth_opendata documents exposed form
 */
?>

<div class="container-inline">
  <?php if (empty($variables['form']['#block']->subject)): ?>
    <h2 class="element-invisible"><?php print t('Form'); ?></h2>
  <?php endif; ?>
  <?php print $filters; ?>
</div>


