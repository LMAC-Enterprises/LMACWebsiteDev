(function ($) {
	
	function _lmacg_ajaxSend(query) {
	  $.get(query, function(data, status){});
	}		
	
	function _lmacg_updateFiveStarView(anchor, rating) {

		$('.lmacg_fivestar', $(anchor).parent()).each(function(index, element){
			$(element).html((index + 1 > rating) ? '☆' : '★');

		});
	}
	
  $(document).ready(function(){
    $("#lmacgallery-search-form").submit(function(){
	  
	  window.location.href = '/lmac_gallery/' +  $("#edit-sq").val();
	  return false;
	});
	
	var imagesData = JSON.parse(document.getElementById('imageDataJson').textContent);
	
	$(".gallerypick").click(function(e){
		e.stopPropagation();
		var eId = $(this).attr('id');
		var id = eId.replace('gallerypick_', '');
		
		dapp = Drupal.settings['LMACGallery']['dapp'];
		
		$("#lmacg_modalImageLayer img").attr("src",imagesData[id]['url']);
		$("#lmacg_authorData").attr("href",dapp + '@' + imagesData[id]['author']);
		$("#lmacg_authorData").html('@' + imagesData[id]['author']);
		$("#lmacg_permlinkData").attr("href",dapp + '@' + imagesData[id]['author'] + '/' + imagesData[id]['permlink']);
		$("#lmacg_urlData").attr("href", imagesData[id]['url']);
		$("#lmacg_modalImageLayer").show();
		
		return false;
	});
	
	$(".lmacg_fivestar").click(function(e){
		e.stopPropagation(); 		
	
		var addr = $(this).attr("href"); 
		_lmacg_ajaxSend(addr);		
		_lmacg_updateFiveStarView($(this), addr.match(/\/(\d+)\/\w+$/)[1]);
		
		return false;
	});
	
	$(".lmacg_currationPostLink").click(function(e){
		e.stopPropagation(); 		
	
		var addr = $(this).attr("href"); 
		var self = this;
		
		$.get(addr, function(data){

			if (data.result == 'removed') {
				$(self).attr('class', 'lmacg_currationPostLink')
			}else{
				$(self).attr('class', 'lmacg_currationPostLink selected')
			}
		});
		 console.debug(1);
		return false;
	});
	
 
	
	$("#lmacg_modalImageLayer").click(function(e){

		if (!$(e.target).is('div')) {
			return true;
		}
				
		e.stopPropagation();
		$(this).hide();

		return false;
	});
  });
})(jQuery);
