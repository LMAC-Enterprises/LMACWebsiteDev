<?php
/**
 * Form callback for admin/lmac/lmacg-unrated.
 */
function LMACGallery_admin_unrated_form($form, &$form_state) {

	$form['unrated_description'] = array(
		'#type'	=> 'markup',
		'#markup' => t('This table displays all images that are not yet rated. You can perform bulk ratings for the entries in this table. The rating of the images influences the sorting in the gallery!'),
		'#prefix' => '<p>',
		'#suffix' => '</p>'
	);
	
	$form['rating_select'] = array(
		'#type' => 'select',
		'#title' => t('Your rating'),
		'#options' => array(
			1 => t('★☆☆☆☆'),
			2 => t('★★☆☆☆'),
			3 => t('★★★☆☆'),
			4 => t('★★★★☆'),
			5 => t('★★★★★'),
		)
	);
	
	$form['rating_submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit rating'),
		'#submit' => array('LMACGallery_admin_unrated_form_submit')
	);
 	
	$form['table'] = array(
		'#type' => 'tableselect',
		'#header' => array(
			'links' => array('data' => t('Links'), 'field' => 'links'),
			'author' => array('data' => t('Author'), 'field' => 'author'),
			'title' => array('data' => t('Title'), 'field' => 'title'),
			'tags' => array('data' => t('Tags'), 'field' => 'tags'),
		),
		'#options' => _LMACGallery_admin_getUnratedImages(),
		'#empty' => t('All images were already rated.'),
    );
	
	$form['pager'] = array(
		'#type' => 'markup',
		'#markup' => theme("pager")
	);
	
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'LMACGallery') . '/LMACGallery.unrated.css',
	);
	
	return $form;
}

/**
 * Submit handler of LMACGallery_admin_unrated_form.
 */
function LMACGallery_admin_unrated_form_submit($form, &$form_state) {

	debug ($form_state['input']);
	$imageIds = [];
	foreach($form_state['input']['table'] as $imageId=>$selectionState) {
		if ($selectionState == NULL) {
			continue;
		}
		$imageIds[] = $imageId;
	}
	
	_LMACGallery_admin_bulkRrateImages($imageIds, $form_state['input']['rating_select']);
}

/**
 * Bulk rates images.
 *
 * @param $imageIds	An array of imageIds to rate.
 * @param $score	A five star score.
 *
 */
 function _LMACGallery_admin_bulkRrateImages($imageIds, $score) {
	
	global $user;
	
	foreach($imageIds as $imageId) {
		db_merge('lmacg_ratings')->key(array('imageid'=>$imageId, 'uid'=>$user->uid))->fields(array(
			'imageid' => $imageId,
			'score' => $score,
			'uid' => $user->uid,
		))->execute();
	}	
		
	cache_clear_all('LMACGallery_*', 'cache');
 }
 
/**
 * Retrieves all unrated images from the database.
 *
 * @return
 *   An array of image entities.
 */
function _LMACGallery_admin_getUnratedImages() {
 
	$unratedImages = [];
 
	$query = db_select("images", "li");
	$query->addField("li", "imageid");
	$query->addField("li", "title");
	$query->addField("li", "tags");
	$query->addField("li", "url");
	$query->addField("li", "permlink");
	$query->addField("li", "author");

	$query->where('li.imageid NOT IN (SELECT lr.imageid FROM lmacg_ratings as lr)');
	$query->orderBy('imageid', 'DESC');
	$query = $query->extend('PagerDefault')->limit(30);
	$result = $query->execute();
 
	while ($row = $result->fetchAssoc()) {

		$row['links'] = _LMACGallery_admin_getHivePostLinkCellContent('@' . $row['author'] . '/' . $row['permlink'], $row['url']);
		$unratedImages[$row['imageid']] = $row;		  
	}

	return $unratedImages;
}

/**
 * Retrieves all unrated images from the database.
 *
 * @param $rawLink 		Relative link to the hive post.
 * @param $imageUrl		The Url to the image.
 *
 * @return
 *   A string of HTML links.
 */
function _LMACGallery_admin_getHivePostLinkCellContent($rawLink, $imageUrl) {
	
	$linklist = array(
		'item1' => array( 'title' => 'Open in PeakD', 'href' => 'https://peakd.com/' . $rawLink, 'attributes'=>array('target'=>'_blank')),
		'item2' => array( 'title' => 'Open in Hive.blog', 'href' => 'https://hive.blog/' . $rawLink, 'attributes'=>array('target'=>'_blank')),
		'item3' => array( 'title' => 'Open image', 'href' => $imageUrl, 'attributes'=>array('target'=>'_blank')),
	);
	
	return theme('links', array(
	  'links' => $linklist,
	  'attributes' => array('class' => array('links', 'inline'))));   
}