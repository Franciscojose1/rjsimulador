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
  <div class="clearfix"></div>
  <?php if (isset($main_content)): ?>
    <div class="main-content">
      <?php print render($main_content); ?>
    </div>
  <?php endif; ?>
  <?php if (isset($action_section)): ?>
    <div class="action-section">
      <?php print render($action_section); ?>
    </div>
  <?php endif; ?>
</div>