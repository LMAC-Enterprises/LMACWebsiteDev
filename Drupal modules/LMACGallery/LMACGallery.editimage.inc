<?php
function LMACGallery_admin_addImage_form($form, &$form_state) {

	$formTitle = '';
    $imageId = empty($form_state['build_info']['args']) ? 'new' : $form_state['build_info']['args'][0];
 
	$imageDefaults = array(
		'imageid' => 0,
		'title' => '',
		'tags' => '',
		'url' => '',
		'permlink' => '',
		'author' => ''
	);	
	
	$editMode = FALSE;
 
	if ($imageId == 'new') {
	
		$formTitle = t('Add new image');
	}else{
		$imageDefaults = _LMACGallery_admin_loadImageById($imageId);
		if ($imageDefaults == NULL) {
			drupal_set_message(t('Image not found!'), 'error');
			return array();
		}else{
			$formTitle = t('Edit image');
			$editMode = TRUE;
		}
	}
	
	
	$form['imageid'] = array(
		'#type' => 'value', 
		'#value' => $imageDefaults['imageid']
	);
	
	$form['header'] = array(
		'#type' => 'markup',
		'#markup' => $formTitle,
		'#prefix' => '<h2>',
		'#suffix' => '</h2>',
	);

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#default_value' => $imageDefaults['title'],
		'#size' => 96,
		'#maxlength' => 300
	);
	
	$form['tags'] = array(
		'#type' => 'textfield',
		'#title' => t('Tags'),
		'#description' => t('Comma separated tags/keywords.'),
		'#default_value' => $imageDefaults['tags'],
		'#maxlength' => 300
	);
	
	$form['url'] = array(
		'#type' => 'textfield',
		'#title' => t('Image Url'),
		'#description' => t('The Url of the single image.'),
		'#default_value' => $imageDefaults['url'],
		'#maxlength' => 300
	);
	
	if (! empty($imageDefaults['url'])) {
		$form['url_preview'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="lmacg_edit-image-url-preview-box"><img src="https://images.hive.blog/300x0/'. $imageDefaults['url']  . '" /></div>',
		);
	}
	
	$form['author'] = array(
		'#type' => 'textfield',
		'#title' => t('Author'),
		'#description' => t('The author of the image post.'),
		'#default_value' => $imageDefaults['author'],
		'#size' => 32
	);
	
	$form['permlink'] = array(
		'#type' => 'textfield',
		'#title' => t('Permlink'),
		'#description' => t('The permlink of the source post. (Is part of the post url, right to the username segment.) In example: my-first-lil-contribution'),
		'#default_value' => $imageDefaults['permlink'],
		'#maxlength' => 300
		
	);
	
	if ($editMode) {
		$readonly = array('readonly'=>'readonly');
		$form['author']['#attributes'] = $readonly;
		$form['url']['#attributes'] = $readonly;
		$form['permlink']['#attributes'] = $readonly;
	}
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#submit' => array('LMACGallery_admin_addImageForm_submit')
	);

	$form['new_image'] = array(
		'#type' => 'link',
		'#title' => t('New image entry'),
		'#href' => '/admin/lmac/lmacg-edit-image',
	);

	return $form;
}
 
function LMACGallery_admin_addImageForm_submit($form, &$form_state) {
	drupal_set_message(t('Image saved.'));
	$oldImageId = isset($form_state['values']['imageId']) ? $form_state['values']['imageId'] : 0;
	$newImageId = _LMACGallery_admin_saveImage($form_state['values']);	
	if ($newImageId != $oldImageId) {
		drupal_goto('admin/lmac/lmacg-edit-image/' . $newImageId);
	}
}

function _LMACGallery_admin_saveImage($image) {
	$status = db_merge('images')->key(array('imageid'=>$image['imageid']))->fields(array(
    'imageid' => $image['imageid'],
    'title' => $image['title'],
	'tags' => $image['tags'],
	'url' => $image['url'],
	'author' => $image['author'],
	'permlink' => $image['permlink']
  ))->execute();
  
	if( $status == MergeQuery::STATUS_INSERT ) {
		$image['imageid'] = db_query("SELECT MAX(imageid) FROM images")->fetchField();
	}
	
	cache_clear_all('LMACGallery_*', 'cache', TRUE);
	
	return $image['imageid'];
}

function _LMACGallery_admin_loadImageById($imageId) {
 
	$result= db_query('SELECT * FROM images WHERE imageid=:imageid LIMIT 1', array('imageid'=>$imageId));

	if (!$result) {
		return NULL;
	}

	return $result->fetchAssoc();
}