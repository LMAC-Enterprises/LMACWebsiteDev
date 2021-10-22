jQuery(document).ready(function () { 

		
	jQuery('#lmac-gallery-image-download').click(function(e) {
		
		var image = Drupal.settings['LMACGalleryImage'];
		
		LMACDialogsController.openDialog('lmacGalleryDownloadInfo', {'authorXY': image['author'], 'downloadHref': image['url']});
		return false;
	});
	
	jQuery('body').delegate("#lmac-gallery-image-download-link", "click", function() { 
		LMACDialogsController.closeDialog('lmacGalleryDownloadInfo');
		return true;
	});
});