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
    foreach ($header as $k => $v) {
      ?>
        <th><?php echo $v; ?></th>
      <?php
    }
    ?>
    </tr>
    <?php
    module_load_include('inc', 'pathauto');
    foreach ($data as $k => $v) {
      ?>
      <tr>
        <td><?php $title = pathauto_cleanstring($v->title); echo l($v->title, 'document/' . $title); ?></td>
        <td><?php echo date('d M Y H:m', $v->changed); ?></td>
        <td><?php echo $v->name; ?></td>
        <td><?php echo $v->s_changed > 0 ? 'queued' : $v->s_changed < 0 ? 'error' :'indexed'; ?></td>
      </tr>
      <?php
    }
    ?>
  </table>
</div>
