function LMACWinners_openImageModal(imageSrc) {
 
	console.debug(imageSrc);
	document.body.scrollTop = document.documentElement.scrollTop = 0;
	jQuery('body').append('<div id="lmac-contest-image-modal-layer"><div id="lmac-contest-imageContainer"><img src="' + imageSrc + '"/></div><a href="' + window.location.href + '" title="Back to the winners gallery">X</a></div>');
	jQuery('#lmac-contest-image-modal-layer > a').click(function(e) {
		jQuery('#lmac-contest-image-modal-layer').remove();
		return false;
	});
}

jQuery(document).ready(function () { 
	jQuery('#edit-contest-round').change(function() {		
		window.location.href = '/lmac-winners/' +  jQuery(this).val();
	}); 
	
 
    jQuery('.gallery').gallery();
});

 