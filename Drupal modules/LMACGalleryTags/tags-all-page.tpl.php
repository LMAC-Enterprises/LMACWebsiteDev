<div class="selectedTagsArea">
   <div class="searchSelectedTagsButton inactive"><a href="#">Go</a></div>
   <div class="selectedTagsContainer">
      <em><?php print t('No tags selected!'); ?></em>
   </div>
</div>
<div>
   <p><?php print t('Here you can find a list of all keywords that have images in the gallery. You can also use this page as search input builder to search the gallery with a combination of keywords.'); ?></p>
</div>
<div class="allTags">
   <?php foreach(array_keys($tags) as $tag) { ?>
         <div class="tag tagColor<?php print (int)(4 * $tagWeights[$tag]);?>" data-tag="<?php print $tag; ?>">            
            <span class="amount"><?php print $tags[$tag]; ?></span><span class="tagName"><?php print $tag; ?></span>
         </div>
   <?php } ?>
</div>
