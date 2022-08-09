function _LMACGalleryTags_countSelectedTags() {
   return jQuery('.selectedTagsContainer').children().length;
}

function _LMACGalleryTags_selectTag(tagElement) {
   if (_LMACGalleryTags_countSelectedTags() > 7) {
      return false;
   }
   if (_LMACGalleryTags_isTagAlreadySelected(tagElement)) {
      return false;
   }


   jQuery('.selectedTagsContainer > em').hide();
   jQuery(tagElement).clone().appendTo('.selectedTagsContainer');   

   if (jQuery('.searchSelectedTagsButton').hasClass('inactive')) {
      jQuery('.searchSelectedTagsButton').removeClass('inactive');
   }
}

function _LMACGalleryTags_unselectTag(tagElement) {
   jQuery(tagElement).empty();
   jQuery(tagElement).remove();

   if (_LMACGalleryTags_countSelectedTags() == 1) {
      jQuery('.selectedTagsContainer > em').show();
   }

   if (_LMACGalleryTags_countSelectedTags()== 1 && ! jQuery('.searchSelectedTagsButton').hasClass('inactive')) {
      jQuery('.searchSelectedTagsButton').addClass('inactive');
   }
}

function _LMACGalleryTags_performSearchQuery() {
   var elementsInTagContainer = jQuery('.selectedTagsContainer').children().length;
   if (elementsInTagContainer < 2) {
      return;
   }

   var q=[];

   jQuery('.selectedTagsContainer > .tag').each(function(index, value){
      q.push(jQuery(value).attr('data-tag'));
   });   

   window.location.href='/lil-gallery/' + q.join(' ') + '?searchmode=mix';
   return;
}

function _LMACGalleryTags_isTagAlreadySelected(tagElement) {
 
   var tagName = jQuery(tagElement).attr('data-tag');
   var found = false;
   
   jQuery('.selectedTagsContainer > .tag').each(function(index, value){
 
      if (tagName == jQuery(value).attr('data-tag')) {
         found = true;         
         return true;
      }
   });   

   return found;
}


jQuery(document).ready(function() {
   jQuery('.allTags > .tag').click(function(e) {
      _LMACGalleryTags_selectTag(this);
      jQuery('html, body').animate({scrollTop: '0px'}, 300);
   });

   jQuery('.selectedTagsContainer > .tag').live('click', function(e) {
      _LMACGalleryTags_unselectTag(this);
   });

   jQuery('.searchSelectedTagsButton').click(function(e) {
      
      _LMACGalleryTags_performSearchQuery();

      return false;
   });
});

