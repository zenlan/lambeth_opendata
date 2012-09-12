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
        <td><?php $title = pathauto_cleanstring($v->node_title); echo l($v->node_title, 'document/' . $title); ?></td>
        <td><?php echo date('d M Y H:m', $v->node_changed); ?></td>
        <td><?php echo $v->term_name; ?></td>
        <td><?php echo ($v->search_changed > 0 ? 'queued' : ($v->search_changed < 0 ? 'error' :'indexed')); ?></td>
      </tr>
      <?php
    }
    ?>
  </table>
</div>
