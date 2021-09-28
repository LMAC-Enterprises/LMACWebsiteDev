jQuery(document).ready(function () { 
	jQuery('.lmac-manual-inspection-status').change(function() {		
 
		if (this.value == 3 && ! confirm('Are you sure to delete the entry?')) {
			jQuery(this).val('0').change();
			return true;
		}
		
		 
		var link = jQuery(this).data('link');
		var rowId = '#' + jQuery(this).data('rowid');
		var status = this.value;

		jQuery.ajax({
				type: 'POST',
				url: '/ajax/manual-inspection/status-change',
				dataType: 'json',
				success: function() {

					if (status == 3) {	
				
						jQuery(rowId).remove();
					}else{
						jQuery(rowId).removeClass();
						jQuery(rowId).addClass('lmac-manual-inspection-row');

						switch(status) {
							case '0':
								jQuery(rowId).addClass('status-unseen');
								break;
							case '1':
								jQuery(rowId).addClass('status-inreview');
								break;
							case '2':
								jQuery(rowId).addClass('status-unsolvable');
								break;								
						}
					}
				},
				data: {link: link, status: this.value},
			});
		
		return true;
	});
 
});