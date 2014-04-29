<?php

/**
 * @file
 *   Template for a rendered state_flow_history_entity.
 */
?>
<?php
/**
 * @todo All classes should come fully formed from preprocess
 */
?>
<div class="state-flow-history-entity-hid-<?php print $hid ?>">
  <span><?php print $from_state ?> --> <?php print $state ?> on <?php print $formatted_timestamp ?> by <?php print $formatted_username ?></span>

  <?php // @todo What html element show wrap the log message? <p> ? Probably
        // not blockquote. ?>
  <blockquote><?php print $log; ?></blockquote>
    <div class="state-flow-history-entity-content">
      <?php print render($content) ?>
    </div>
</div>
