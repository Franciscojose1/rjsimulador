<div class="simulaciones-partidas-content">
  <?php if (isset($upper_left)): ?>
    <div style="float:left;width:50%;">
      <?php print render($upper_left); ?>
    </div>
  <?php endif; ?>
  <?php if (isset($upper_right)): ?>
    <div style="float:left;width:50%;">
      <?php print render($upper_right); ?>
    </div>
  <?php endif; ?>
  <?php if (isset($main_content)): ?>
    <div style="clear:both;"></div>
    <div style="float:left;width: 100%">
      <?php print render($main_content); ?>
    </div>
  <?php endif; ?>
</div>