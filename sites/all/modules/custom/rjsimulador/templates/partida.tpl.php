<div class="rjsim partida-content">
  <?php if (isset($upper_content)): ?>
    <div>
      <?php print render($upper_content); ?>
    </div>
  <?php endif; ?>
  <div class="upper-left floating-left min-table">
    <?php if (isset($upper_left)): ?>
      <?php print render($upper_left); ?>
    <?php endif; ?>
  </div>
  <div class="upper-right floating-right min-table">
    <?php if (isset($upper_right)): ?>
      <?php print render($upper_right); ?>
    <?php endif; ?>
  </div>
  <div class="clearfix"></div>
  <div class="main-content">
    <?php if (isset($main_content)): ?>
      <?php print render($main_content); ?>
    <?php endif; ?>
  </div>
  <?php if (isset($action_section)): ?>
    <div class="action-section">
      <?php print render($action_section); ?>
    </div>
  <?php endif; ?>
</div>