<?php
/**
 * @file
 * Template file for lambeth_opendata documents table
 */
?>
<?php
foreach ($indexes as $index) {
  if ($index->enabled) {
    $class = '#98C5AB';
    $status = 'enabled';
  }
  else {
    $class = '#CE7898';
    $status = 'disabled';
  }
  ?>
  <div style="background:<?php echo $class; ?>;text-align:center;">
    <?php echo $index->name . ' is currently ' . $status; ?>
  </div>
  <?php
}
?>
