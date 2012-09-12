<?php
/**
 * @file
 * Template file for lambeth_opendata documents table
 */
?>
<div class="<?php echo $class; ?>">
  <table>
    <tr>
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
        <th style="background:<?php echo $class; ?>;align:center;">
          <?php echo 'Index ' . $index->item_type . '<br /> is currently ' . $status; ?>
        </th>
        <?php
      }
      ?>
    </tr>
  </table>
</div>
