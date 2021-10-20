<div id="<?php print $dialogId; ?>" class="lmac-dialogs-dialog" <?php if (! empty($styles)) { print 'style="' . $styles . '"'; } ?>>
	<div class="lmac-dialogs-dialog-header">
		<div class="lmac-dialogs-dialog-header-title"><?php print $title; ?></div>
		<div class="lmac-dialogs-dialog-header-control"><a href="#" class="lmac-dialogs-dialog-header-control-close">✖</a></div>
	</div>
	<div class="lmac-dialogs-dialog-content"><?php print $content; ?></div>
	<div class="lmac-dialogs-dialog-footer"><?php print $footer; ?></div>
</div>
