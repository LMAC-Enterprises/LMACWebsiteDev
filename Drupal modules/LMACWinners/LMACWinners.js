
jQuery(document).ready(function () { 
	jQuery('#edit-contest-round').change(function() {		
		window.location.href = '/lmac-winners/' +  jQuery(this).val();
	}); 
	
 
    jQuery('.gallery').gallery();
});

 