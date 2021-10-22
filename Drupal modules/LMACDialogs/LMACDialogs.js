/**
 * @author QuantumG
 */

function LMACDialogs() {

	this.closeButtonDialogMap = createCloseButtonMap();
	this.closeCallbacks = {};
	
	var self=this;
	
	jQuery('.lmac-dialogs-dialog-header-control-close').click(function(e) {
			 
		self.closeDialog(self.closeButtonDialogMap[jQuery(this).attr('id')]);

		return false;
	});
	
	jQuery('.lmac-dialogs-dialog-footer > input[type=button]').click(function(e) {
			 
		var buttonId = jQuery(this).attr('id');
 
		if (typeof Drupal.settings.LMACDialogs.buttons !== 'undefined' && typeof Drupal.settings.LMACDialogs.buttons[buttonId] !== 'undefined') {
			var buttonSettings = Drupal.settings.LMACDialogs.buttons[buttonId];
			
			if (typeof buttonSettings['#ajax'] !== 'undefined') {
				var url = buttonSettings['#ajax'];
				console.debug(url);
				url = url.replace('%(buttonId)' , buttonId);
				url = url.replace('%(dialogId)' ,  buttonSettings['#parentDialog']);
				jQuery.ajax({
					url: url,
					success: function(){
						self.closeDialog(buttonSettings['#parentDialog']);	
					},
					error: function(){
						self.closeDialog(buttonSettings['#parentDialog']);	
					}
				});
			}else{
				self.closeDialog(buttonSettings['#parentDialog']);	
			}
		}
		
		return false;
	});
	
	function createCloseButtonMap() {

		var mapping = []
	
		jQuery('.lmac-dialogs-dialog-header-control-close').each(function(index, item) {
			 
			mapping [jQuery(item).attr('id')] = jQuery(item).parents('.lmac-dialogs-dialog').attr('id');
			
		});
	
		return mapping;
	}
	
	/**
	 * Opens a dialog by its Id.
	 *
	 * @param dialogId
	 *   The html id of the dialog as defined in a hook_lmacDialog implementation.
	 *  @param replacements
	 *   An associative array of to-replace/replacement elements for replacing texts in the dialog message.
	 *
	 * @return
	 *   Nothing
	 */
	self.openDialog = function(dialogId, replacements = [], closeCallback = null) {
 
		var dialog = jQuery('#' + dialogId + '> .lmac-dialogs-dialog-content');
		if (dialog.length == 0) {
			return;
		}
 
		if (Object.keys(replacements).length > 0) {
			content = jQuery('#' + dialogId + ' > .lmac-dialogs-dialog-content').html();
			for(replacementKey in replacements) {

				content = content.replace(replacementKey, replacements[replacementKey]);
			}
			jQuery('#' + dialogId + ' > .lmac-dialogs-dialog-content').html(content);
			
		}
	 
		
		if (closeCallback !== null) {
			self.closeCallbacks[dialogId] = closeCallback;
		}
		
		jQuery('#lmac-dialogs-popup-overlay').show();
		jQuery('#' + dialogId).show();
	};
	
	/**
	 * Closes a dialog by its Id.
	 *
	 * @param dialogId
	 *   The html id of the dialog as defined in a hook_lmacDialog implementation.
	 *
	 * @return
	 *   Nothing
	 */
	self.closeDialog = function(dialogId) {
		jQuery('#lmac-dialogs-popup-overlay').hide();
		jQuery('#' + dialogId).hide();
		if (typeof self.closeCallbacks[dialogId] !== 'undefined') {
			self.closeCallbacks[dialogId](dialogId);
		}
	}	
}

jQuery(document).ready(function () { 

	LMACDialogsController = new LMACDialogs(); 
	
	if (typeof Drupal.settings.LMACDialogs !== 'undefined' && typeof Drupal.settings.LMACDialogs.initiallyOpen !== 'undefined') {
		
		LMACDialogsController.openDialog(Drupal.settings.LMACDialogs.initiallyOpen['dialogId'], Drupal.settings.LMACDialogs.initiallyOpen['replacements']);
	}
});