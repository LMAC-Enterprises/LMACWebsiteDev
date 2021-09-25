<?php
function LMACGallery_ajax_fiveStarCallback($imageId, $score, $cid) {
 
	_LMACGallery_ajax_rateImage($imageId, $score, $cid);
	
	return;
}

function LMACGallery_ajax_curationPickCallback($imageId) {

	$removed = _LMACGallery_ajax_performCurationPick($imageId);
 
	return drupal_json_output(array('result'=> ($removed ? 'removed' : 'added')));
}

function _LMACGallery_ajax_rateImage($imageId, $rating, $cid) {
	global $user;
	
	cache_clear_all($cid, 'cache');
	
	db_delete(' lmacg_ratings')
	->condition('imageid', $imageId)
	->condition('uid', $user->uid)
	->execute();

	
	db_merge('lmacg_ratings')->key(array('imageid'=>$imageId, 'uid'=>$user->uid))->fields(array(
    'imageid' => $imageId,
    'score' => $rating,
	'uid' => $user->uid,
  ))->execute();
}

function _LMACGallery_ajax_performCurationPick($imageId) {
	
	$pickedImageIds = variable_get('LMACGallery_curationPicks', array());
	$remove = FALSE;
	if (in_array($imageId, $pickedImageIds)) {
		$remove = TRUE;
	}
	
	if (!$remove) {
		$pickedImageIds[] = $imageId;
	}else{
		$pickedImageIds = array_diff($pickedImageIds, array($imageId));
	}
	
	variable_set('LMACGallery_curationPicks', $pickedImageIds);
	
	return $remove;
}