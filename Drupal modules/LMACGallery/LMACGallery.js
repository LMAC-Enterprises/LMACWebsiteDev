lmacg_currentSiteState = {title: '', href: ''};

(function ($) {
	
	currentImageId = 0;
	
	function _lmacg_ajaxSend(query) {
	  $.get(query, function(data, status){});
	}		
	
	function _lmacg_updateFiveStarView(anchor, rating) {

		$('.lmacg_fivestar', $(anchor).parent()).each(function(index, element){
			$(element).html((index + 1 > rating) ? '☆' : '★');

		});
	}
	
	function _lmacg_closeImageModal() {
		$("#lmacg_modalImageLayer").hide();
		window.history.replaceState('', lmacg_currentSiteState.title, lmacg_currentSiteState.href);
	}
	
	function _lmacg_openImageModal(imageId) {
				
		$("#lmacg_titleData").html(imagesData[imageId]['title']);
		$("#lmacg_modalImageLayer img").attr("src",imagesData[imageId]['url']);
		$("#lmacg_authorData").html(_lmacg_getDappLinksHtml(imagesData[imageId]['author'], '', '<span>By @' + imagesData[imageId]['author'] + '</span>'));
		$("#lmacg_permlinkData").html(_lmacg_getDappLinksHtml(imagesData[imageId]['author'], imagesData[imageId]['permlink']));
		$("#lmacg_urlData").attr("href", imagesData[imageId]['url']);
		
		currentImageId = imageId;
				
		var imageBy = 'Image by ' + imagesData[imageId]['author'];
		
		$("#lmacg_modalImageLayer").show();
		window.history.replaceState(imageBy, imageBy, '/lil-gallery-image/' + imageId);
	}
	
	function _lmacg_getDappLinksHtml($author, $permlink, $suffix = '') {
		
		return '<a target="_blank" class="lmacg-dapp-link hive-blog-dapp-icon" href="http://hive.blog/@' + $author + '/' + $permlink + '" title="Open in Hive.Blog">&nbsp;</a>' + 
			   '<a target="_blank" class="lmacg-dapp-link peakd-dapp-icon" href="http://peakd.com/@' + $author + '/' + $permlink + '" title="Open in PeakD">&nbsp;</a>' +
			   '<a target="_blank" class="lmacg-dapp-link ecency-dapp-icon" href="http://ecency.com/@' + $author + '/' + $permlink + '" title="Open in Ecency">&nbsp;</a>' + $suffix;
	}
	
	$(document).ready(function(){
	  
		lmacg_currentSiteState.title = window.location.title;
		lmacg_currentSiteState.href = window.location.pathname;
		imagesData = document.getElementById('imageDataJson') != null ? JSON.parse(document.getElementById('imageDataJson').textContent) : [];
		  
		$("#lmacgallery-search-form").submit(function(){
		  $searchString = $("#edit-sq").val().replace(/[^0-9a-z\-, ]/gi, '').toLowerCase ();
		  
		  window.location.href = '/lil-gallery/' +  $searchString;
		  return false;
		});
			
		$(".gallerypick").click(function(e){
			e.stopPropagation();
			var eId = $(this).attr('id');
			var id = eId.replace('gallerypick_', '');
					
			_lmacg_openImageModal(id);
			
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

			return false;
		});
		
	 
		
		$("#lmacg_modalImageLayer").click(function(e){

			if (!$(e.target).is('div')) {
				return true;
			}
					
			e.stopPropagation();
			_lmacg_closeImageModal();

			return false;
		});
		
			
		$("#lmacg_modalExit").click(function(e){

			if (!$(e.target).is('a')) {
				return true;
			}
					
			e.stopPropagation();
			_lmacg_closeImageModal();

			return false;
		});
		

		$("#lmacg_urlData").click(function(e){

			if (!$(e.target).is('a')) {
				return true;
			}
					
			e.stopPropagation();
			_lmacg_closeImageModal();
			
			var author = 'the author';
			
			if (currentImageId != 0) {
				author = '@' + imagesData[currentImageId]['author'];
			}
 
			LMACDialogsController.openDialog('lmacGalleryDownloadInfo', {'#downloadHref': imagesData[currentImageId]['url'], '@authorXY': author, 'galleryImageUrl': 'http://lmac.gallery/lil-gallery-image/' + currentImageId});

			return false;
		});
		
		$('body').delegate("#lmac-gallery-image-download-link", "click", function() { 
			LMACDialogsController.closeDialog('lmacGalleryDownloadInfo');
			return true;
		});

		
	  });
	}
)(jQuery);
