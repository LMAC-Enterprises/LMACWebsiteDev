<div id="lmac-contest">
	<div id="lmac-contest-navigation" class="header-nav">		
		<?php if (! empty($navigation['previousRound'])) { ?><a href="<?php print $navigation['previousRound']; ?>" title="<?php print t('Previous contest round'); ?>"><?php print t('Previous'); ?></a><?php }else{ ?><span><?php print t('Previous'); ?></span><?php } ?>
		<?php if (! empty($navigation['nextRound'])) { ?><a href="<?php print $navigation['nextRound']; ?>" title="<?php print t('Next contest round'); ?>"><?php print t('Next'); ?></a><?php }else{ ?><span><?php print t('Next'); ?></span><?php } ?>
	</div>
	<div id="lmac-contest-info">
		<h2><?php print $contest->title; ?></h2>
		<div>
			<table>
				<tbody>
				<?php if (! empty($contest->announcementtitle) && ! empty($contest->announcementurl)) { ?>
				<tr class="even">
					<td><?php print t('Contest announcement post:'); ?></td>
					<td><?php print $contest->announcementtitle; ?></td>
					<td  class="title-links"><?php print $links['announcementLinks']; ?></td>
				</tr>
				<?php } ?>
				<?php if (! empty($contest->polltitle) && ! empty($contest->pollurl)) { ?>
				<tr class="odd">
					<td><?php print t('Final poll post:'); ?></td>
					<td><?php print $contest->polltitle; ?></td>
					<td  class="title-links"><?php print $links['pollLinks']; ?></td>
				</tr>
				<?php } ?>
				<?php if (! empty($contest->winnerstitle) && ! empty($contest->winnersurl)) { ?>
				<tr class="even">
					<td><?php print t('Winner announcement post:'); ?></td>
					<td><?php print $contest->winnerstitle; ?></td>
					<td  class="title-links"><?php print $links['winnersLinks']; ?></td>
				</tr>
				<?php } ?>
				<tr class="odd">
					<td><?php print t('Contest round:'); ?></td>
					<td>#<?php print $contest->roundid; ?></td>
					<td></td>
				</tr>
				<tr class="even">
					<td><?php print t('Winners:'); ?></td>
					<td><?php print count($contest->winners); ?></td>
					<td></td>
				</tr>
				</tbody>
			</table>
		</div>
	</div>
	<div id="lmac-contest-winners" class="gallery">
		<h3><?php print t('Winners'); ?></h3>
		<?php if (empty($contest->winners)) { ?>
		<p><?php print t('No winners available yet.'); ?></p>
 		<?php } ?>
		<?php if (! empty($contest->templateThumbnailUrl)) { ?>
			<div id="lmac-contest-template-thumbnail-container">
				<div id="lmac-contest-template-thumbnail-footer">
					<?php print t('The template photo in round #!round.', array('!round'=>$contest->roundid)); ?>
				</div>
				<a href="<?php print $contest->templateimageurl; ?>" title="<?php print t('Click here to open the template image.'); ?>" target="_blank"><img src="<?php print $contest->templateThumbnailUrl; ?>" /></a>	 
			</div>
		<?php } ?>
		<?php foreach($contest->winners as $winner) { ?>
		<div class="lmac-contest-winner-thumbnail-box">
			<div class="lmac-contest-winner-thumbnail-footer" class="lmac-winner-place-<?php print $winner['place']; ?>"><span class="lmac-winner-place-label-<?php print $winner['place']; ?>"><?php print $winner['placeText']; ?></span><a class="lmac-winner-artist-label" href="https://peakd.com/@<?php print $winner['artist']; ?>" title="<?php print t('Click to open the Hive profile of the artist'); ?>">@<?php print $winner['artist']; ?></a>
			</div>
			<img src="<?php print $winner['imageThumbnailUrl']; ?>" alt="Image" data-full="<?php print $winner['imageurl']; ?>">
			<div class="lmac-contest-winner-dapps-box">
				<?php foreach ($winner['dappyfiedPostUrls'] as $dappKey => $dappyfiedPostUrl) { ?> 
				<a href="<?php print $dappyfiedPostUrl; ?>" title="<?php print t('Open entry in this specific dapp.'); ?>" target="_blank" class="lmac-winner-dapp-link <?php print $dappKey; ?>-dapp-icon"></a>
				<?php } ?>
			</div>
		</div>
		<?php } ?>
	</div>
	<div id="lmac-contest-navigation" class="footer-nav">		
		<?php if (! empty($navigation['previousRound'])) { ?><a href="<?php print $navigation['previousRound']; ?>" title="<?php print t('Previous contest round'); ?>"><?php print t('Previous'); ?></a><?php }else{ ?><span><?php print t('Previous'); ?></span><?php } ?>
		<?php if (! empty($navigation['nextRound'])) { ?><a href="<?php print $navigation['nextRound']; ?>" title="<?php print t('Next contest round'); ?>"><?php print t('Next'); ?></a><?php }else{ ?><span><?php print t('Next'); ?></span><?php } ?>
	</div>
</div>
