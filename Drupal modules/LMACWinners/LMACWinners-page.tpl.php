<div id="lmac-contest">
   <?php if (user_access('administer lmac_contest')) { ?>
   <div id="lmac-contest-admin-links">
      <a href="/admin/lmac/lmac-winners/<?php print $contest->contestId; ?>"><?php print t('Edit'); ?></a>
   </div>
   <?php } ?>
	<div id="lmac-contest-navigation" class="header-nav">		
		<?php if (! empty($navigation['previousRound'])) { ?><a href="<?php print $navigation['previousRound']; ?>" title="<?php print t('Previous contest round'); ?>"><?php print t('Previous'); ?></a><?php }else{ ?><span><?php print t('Previous'); ?></span><?php } ?>&nbsp;&bull;&nbsp;    	
		<?php if (! empty($navigation['nextRound'])) { ?><a href="<?php print $navigation['nextRound']; ?>" title="<?php print t('Next contest round'); ?>"><?php print t('Next'); ?></a><?php }else{ ?><span><?php print t('Next'); ?></span><?php } ?>
	</div>
	<div id="lmac-contest-info">
		<h2><?php print $contest->title; ?></h2>
		<div>
			<table>
				<tbody>
				<tr class="odd">
					<td><?php print t('Final poll post:'); ?></td>
					<td><?php print $contest->title; ?></td>
					<td  class="title-links"><?php print $links['pollLinks']; ?></td>
				</tr>
				<tr class="odd">
					<td><?php print t('Contest round:'); ?></td>
					<td>#<?php print $contest->contestId; ?></td>
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
		<?php if (empty($contest->winners)) { ?>
		<p><?php print t('No winners available yet.'); ?></p>
 		<?php } ?>
		<?php if (! empty($contest->templateThumbnailUrl)) { ?>
			<div id="lmac-contest-template-thumbnail-container">
				<div id="lmac-contest-template-thumbnail-footer">
					<?php print t('The template image in round #!round.', array('!round'=>$contest->contestId)); ?>
				</div>
				<img src="<?php print $contest->templateThumbnailUrl; ?>" data-full="<?php print $contest->templateImageUrl; ?>" /> 
			</div>
		<?php } ?>
		<h3><?php print t('Winners'); ?></h3>
      <p><?php print t('Here you can find the winners of this round. Click on an image to see it in a bigger size. You can visit the original Hive post of an image by clicking on the Dapp icons.'); ?></p>
      <?php foreach($contest->groupedWinners as $winnerGroup=>$winners) { ?>
         <br />
         <h4><?php print t('Top !group', array('!group'=>$winnerGroup)); ?></h4>
         <?php foreach($winners as $winner) { ?>
		   <div class="lmac-contest-winner-thumbnail-box">
			   <div class="lmac-contest-winner-thumbnail-footer" class="lmac-winner-place-<?php print $winner['winningPlace']; ?>"><span class="lmac-winner-place-label-<?php print $winner['winningPlace']; ?>"><?php print $winner['placeText']; ?> - By </span><a class="lmac-winner-artist-label" href="https://peakd.com/@<?php print $winner['artist']; ?>" title="<?php print t('Click to open the Hive profile of the artist'); ?>">@<?php print $winner['artist']; ?></a>
			   </div>
            <div class="lmac-contest-winner-thumbnail-box-image-container">
			      <img src="<?php print $winner['imageThumbnailUrl']; ?>" alt="Image" data-full="<?php print $winner['imageUrl']; ?>">
            </div>
			   <div class="lmac-contest-winner-dapps-box">
				   <?php foreach ($winner['dappyfiedPostUrls'] as $dappKey => $dappyfiedPostUrl) { ?> 
				   <a href="<?php print $dappyfiedPostUrl; ?>" title="<?php print t('Open entry in this specific dapp.'); ?>" target="_blank" class="lmac-winner-dapp-link <?php print $dappKey; ?>-dapp-icon"></a>
				   <?php } ?>
			   </div>
		   </div>
      <?php } 
            if (empty($winners)) {
      ?>
      <p><?php print t('No winners in this group.'); ?></p>
          <?php } ?>  
      <?php } ?>
	</div>
	<div id="lmac-contest-navigation" class="footer-nav">		
		<?php if (! empty($navigation['previousRound'])) { ?><a href="<?php print $navigation['previousRound']; ?>" title="<?php print t('Previous contest round'); ?>"><?php print t('Previous'); ?></a><?php }else{ ?><span><?php print t('Previous'); ?></span><?php } ?>&nbsp;&bull;&nbsp;
		<?php if (! empty($navigation['nextRound'])) { ?><a href="<?php print $navigation['nextRound']; ?>" title="<?php print t('Next contest round'); ?>"><?php print t('Next'); ?></a><?php }else{ ?><span><?php print t('Next'); ?></span><?php } ?>
	</div>
</div>
