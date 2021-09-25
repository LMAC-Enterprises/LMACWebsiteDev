<div class="lmacg_flowContainer">
<?php
foreach($images as $image) {
?>
<div class="lmacg_thumbnail">
	<div class="lmacg_thumbnailFooter">
		<?php print $image['truncatedTitle']; ?>
	</div>
	<?php
	if (user_access('administer lmac_gallery')) {
		$imageRating = $image['ratingscore'] == null ? 0 : $image['ratingscore']; 
	?>	
	<div class="lmacg_thumbnailFiveStar">
		<a class="lmacg_editImageLink" href="/admin/lmac/lmacg-edit-image/<?php print $image['imageid']; ?>" title="<?php print t('Edit image'); ?>">&#9874;</a>
		<a class="lmacg_currationPostLink<?php print in_array($image['imageid'], $handPicked) ? ' selected' : '' ;?>" href="/ajax/lmac_gallery-curate/<?php print $image['imageid']; ?>" title="Add to curation post.">&#9752;</a>
		<?php
		for ($i=1; $i<=5; $i++) {
		?>
		<a title="Give <?php print $i; ?> stars." href="/ajax/lmac_gallery/<?php print $image['imageid'] . '/' . $i . '/' . $image['cid']; ?>" class="lmacg_fivestar"><?php print  $imageRating >= $i  ? '★' : '☆'; ?></a>
		<?php
		}
		?>
	</div>
	
	<?php
	}
	?>
	<a href="<?php print $image['url']; ?>" title="Click to open" class="gallerypick" id="gallerypick_<?php print $image['imageid']; ?>"><img src="<?php print $image['thumbnailUrl']; ?>" alt="Image"/></a>
</div>
<?php
}
?>

</div>

<div class="lmacg_pagination clearfix">
	<?php if ($pageNo > 0) { ?><a href="<?php print $previousPageUrl; ?>"><?php print t('Previous'); ?></a><?php } ?>
	<span>(<?php print ($pageNo + 1) . '/' . $pagesTotal; ?>)</span>
	<?php if ($pageNo < $pagesTotal - 1) { ?><a href="<?php print $nextPageUrl; ?>"><?php print t('Next'); ?></a><?php } ?>
</div>
<div id="lmacg_modalImageLayer">
	<div id="lmacg_imageContainer"><img src="" /></div>
	<div id="lmacg_dataContainer">
	<table>
			<tbody>
				<tr>
					<td>Author:</td><td><a id="lmacg_authorData" href="" target="_blank">@</a></td>
				</tr>
				<tr>
					<td>Original post:</td><td><a id="lmacg_permlinkData" href="" target="_blank">Click here</a></td>
				</tr>
				<tr>
					<td>Download:</td><td><a id="lmacg_urlData" href="" target="_blank" download>Click here</a></td>
				</tr>
			</tbody>
		</table>
	</div>
</div>
<script type="application/json" id="imageDataJson">
<?php print $imagesJson; ?>
</script>
