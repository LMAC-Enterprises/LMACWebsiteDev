<div id="lmacg_imagePage">
	<div id="lmacg_imagePage-navigation">
		<a href="/lmac_gallery" title=""><?php print t('Back to the gallery'); ?></a>
		<a href="/lmac_gallery/<?php print $image['author']; ?>" title=""><?php print t('All images by !author.', array('!author'=>$image['author'])); ?></a>
	</div>
	<div id="lmacg_imagePage-image-container">
	<img src="<?php print $image['url']; ?>" alt="<?php print t('!authors image is not available.', array('!author' => $image['author'])); ?>" />
	</div>
	<div id="lmacg_imagePage-info-container">
		<table>
 			<tbody>
				<tr>
					<td class="lmacg-image-table-label-col"><?php print t('Title:'); ?></td>
					<td class="lmacg-image-table-link-col">
						<?php print $image['title']; ?>
					</td>
				</tr>
				<tr>
					<td class="lmacg-image-table-label-col"><?php print t('Tags:'); ?></td>
					<td class="lmacg-image-table-link-col">
						<?php print $image['tags']; ?>
					</td>
				</tr>
				<tr>
					<td class="lmacg-image-table-label-col"><?php print t('Author:'); ?></td>
					<td class="lmacg-image-table-link-col">
						<a class="lmacg-dapp-link hive-blog-dapp-icon" href="http://hive.blog/@<?php print $image['author']; ?>" title="<?php print t('Open in Hive.blog'); ?>" target="_black"/>&nbsp;</a>
						<a class="lmacg-dapp-link peakd-dapp-icon" href="http://peakd.com/@<?php print $image['author']; ?>" title="<?php print t('Open in PeakD'); ?>" target="_black"/>&nbsp;</a>
						<a class="lmacg-dapp-link ecency-dapp-icon" href="http://ecency.com/@<?php print $image['author']; ?>" title="<?php print t('Open in Ecency'); ?>" target="_black"/>&nbsp;</a>
						<span>By @<?php print $image['author']; ?></span>
					</td>
				</tr>
				<tr>
					<td class="lmacg-image-table-label-col"><?php print t('Hive post:'); ?></td>
					<td class="lmacg-image-table-link-col">
						<a class="lmacg-dapp-link hive-blog-dapp-icon" href="http://hive.blog/<?php print $image['hiveLink']; ?>" title="<?php print t('Open in Hive.blog'); ?>" target="_black"/>&nbsp;</a>
						<a class="lmacg-dapp-link peakd-dapp-icon" href="http://peakd.com/<?php print $image['hiveLink']; ?>" title="<?php print t('Open in PeakD'); ?>" target="_black"/>&nbsp;</a>
						<a class="lmacg-dapp-link ecency-dapp-icon" href="http://ecency.com/<?php print $image['hiveLink']; ?>" title="<?php print t('Open in Ecency'); ?>" target="_black"/>&nbsp;</a>
					</td>
				</tr>
				<tr>
					<td class="lmacg-image-table-label-col"><?php print t('Download image:'); ?></td>
					<td class="lmacg-image-table-link-col"><a id="lmac-gallery-image-download" target="_blank" href="<?php print $image['url']; ?>" title="<?php print t('Click to download'); ?>"/><?php print t('Click to download'); ?></a></td>
				</tr>
			</tbody>
		</table>
	</div>
</div>