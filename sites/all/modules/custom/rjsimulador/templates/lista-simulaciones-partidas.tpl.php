<div class="rjsim simulaciones-partidas-content">
  <div class="upper-left floating-left">
    <?php if (isset($upper_left)): ?>
      <?php print render($upper_left); ?>
    <?php endif; ?>
  </div>
  <div class="upper-right floating-right">
    <?php if (isset($upper_right)): ?>
      <?php print render($upper_right); ?>
    <?php endif; ?>
  </div>
  <div style="clear:both;"></div>
  <?php if (isset($main_content)): ?>
    <div style="float:left;width: 100%">
      <?php print render($main_content); ?>
    </div>
  <?php endif; ?>
</div>