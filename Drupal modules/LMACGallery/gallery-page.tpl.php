<?php print $pagination; ?> 
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
	<a href="/lmac_gallery-image/<?php print $image['imageid']; ?>" title="Click to open" class="gallerypick" id="gallerypick_<?php print $image['imageid']; ?>"><img src="<?php print $image['thumbnailUrl']; ?>" alt="Image"/></a>
</div>
<?php
}
?>

</div>

<?php print $pagination; ?> 

<div id="lmacg_modalImageLayer">
	<div id="lmacg_imageContainer"><img src="" /></div>
	<div id="lmacg_dataContainer">	
	<table>
			<tbody>
				<tr>
					<td>Title:</td><td id="lmacg_titleData"></td>
				</tr>
				<tr>
					<td>Author:</td><td id="lmacg_authorData"></td>
				</tr>
				<tr>
					<td>Original post:</td><td id="lmacg_permlinkData"></td>
				</tr>
				<tr>
					<td>Download:</td><td><a id="lmacg_urlData" href="#"><?php print t('Click here'); ?></a></td>
				</tr>
			</tbody>
		</table>
	</div>
	<a id="lmacg_modalExit" href="" target="_blank">&#10006;</a>
</div>
<script type="application/json" id="imageDataJson">
<?php print $imagesJson; ?>
</script>
