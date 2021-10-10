jQuery(document).ready(function () { 

	jQuery('body').append('<img src="#" id="hoverImageTooltip"/>');
	jQuery('#hoverImageTooltip').hide();
	
	jQuery('.image-tooltip').hover(
		function(e) {
			var tooltip = jQuery('#hoverImageTooltip');
			tooltip.css({position:"absolute", left:e.pageX + 15,top:e.pageY + 15});
			tooltip.show();
			tooltip.attr('src', 'https://images.hive.blog/280x0/' + jQuery(e.target).attr('href'));
		},
		function(e) {
			jQuery('#hoverImageTooltip').hide();
		}
	);
 
});