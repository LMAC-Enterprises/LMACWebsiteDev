<?php
/**
 * Form callback for admin/lmac/lmac-curation.
 **/
function LMACGallery_admin_curation_form($form, &$form_state) {
 
	$handPickedImages = _LMACGallery_getCurationImages();
	$handPickedImagesAvailable = ! empty($handPickedImages);
 
	$curationSettings = _LMACGallery_getCurationSettings();

	$form['#attached']['css'][] = drupal_get_path('module', 'LMACGallery').'/LMACGallery.curation.css';
	
	$form['pickedgroup'] = array(
		'#type' => 'fieldset',
		'#title' => t('Hand picked images'),
		'#collapsible' => False,
		'#collapsed' => False,  
	);
 	
	$form['pickedgroup']['picked_images'] = array(
	  '#type' => 'checkboxes',
	  '#options' => _LMACGallery_getCurationImagesOptions($handPickedImages),
      '#disabled' => ! $handPickedImagesAvailable
	);
	
	if (! $handPickedImagesAvailable) {
		$form['pickedgroup']['empty_picklist'] = array(
			'#type' 	=> 'markup',
			'#markup' 	=> t('Pick List is empty. Navigate to the gallery site and rate some images.'),
			'#prefix'	=> '<p>',
			'#suffix'	=> '</p>'
		);
	}	
	
	$form['pickedgroup']['unpick_checked'] = array(
		'#type' 	=> 'submit',
		'#value' 	=> t('Unpick checked images'),
		'#submit' 	=> array('LMACGallery_unpickCheckedSubmit'),		
	);
	
	$form['pickedgroup']['unpick_all'] = array(
		'#type' 	=> 'submit',
		'#value' 	=> t('Unpick all images'),
		'#submit' 	=> array('LMACGallery_unpickAllSubmit'),		
	);
	
	$form['markdowngroup'] = array(
		'#type' => 'fieldset',
		'#title' => t('Curation post markdown'),
		'#collapsible' => False,
		'#collapsed' => False,  
		'#disabled' => ! $handPickedImagesAvailable
	);
	
	$form['markdowngroup']['post_benefication'] = array(
		'#type' => 'textfield',
		'#title' => t('Beneficiary reward factor'),
		'#description' => t('The Reward Factor is multiplied by the FiveStar score to compute the percentage share of the rewards. The Reward shares add up per author if an author has multiple rated images.'),
		'#maxlength' => 6,
		'#size' => 4,
		'#default_value' => $curationSettings['post_benefication']
	);
	
	$form['markdowngroup']['post_default_beneficiary'] = array(
		'#type' => 'textfield',
		'#title' => t('Default beneficiary'),
		'#description' => t('Leave empty if the whole rest of the rewards should got to the post author. (Enter a Hive username without leading @.)'),
		'#default_value' => $curationSettings['post_default_beneficiary']
	);
	
	$form['markdowngroup']['post_sender'] = array(
		'#type' => 'textfield',
		'#title' => t('Post sender'),
		'#description' => t('The sender of the top picks post. (Enter a Hive username without leading @.)'),
		'#default_value' => $curationSettings['post_sender'],
		'#required' => TRUE
	);
	
		$form['markdowngroup']['post_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Post title'),
		'#description' => t('Title for the post)'),
		'#default_value' => $curationSettings['post_title'],
		'#required' => TRUE
	);
	
	$form['markdowngroup']['post_tags'] = array(
		'#type' => 'textfield',
		'#title' => t('Post tags'),
		'#description' => t('Comma separated tags.)'),
		'#default_value' => $curationSettings['post_tags'],
		'#required' => TRUE
	);
	
	$form['markdowngroup']['post_permlink'] = array(
		'#type' => 'textfield',
		'#title' => t('Post permlink'),
		'#description' => t('A suggested permlink for the post.)'),
		'#default_value' => $curationSettings['post_permlink'],
		'#required' => TRUE
	);
	
	$form['markdowngroup']['image_markdown_text'] = array(
		'#type' => 'textarea',
		'#title' => t('Image markdown'),
		'#description' => t('Replacement variables to use in this markdown: {image_url} {image_thumbnail_url} {post_author} {post_url} {post_title} {image_tags}'),
		'#default_value' => $curationSettings['image_markdown_text']
	);
	
	$form['markdowngroup']['post_markdown_text'] = array(
		'#type' => 'textarea',
		'#title' => t('Post body markdown'),
		'#description' => t('Replacement variables to use in this markdown: {images} {image_authors}'),
		'#rows' => 32,
		'#default_value' => $curationSettings['post_markdown_text']
	);
		
	$form['markdowngroup']['save_markdown'] = array(
		'#type' 	=> 'submit',
		'#value' 	=> t('Save markdowns'), 
		'#submit' 	=> array('LMACGallery_saveMarkdownHandler'),
	);

	$form['markdowngroup']['preview_markdown'] = array(
		'#type' 	=> 'submit',
		'#value' 	=> t('Preview markdowns'), 
		'#description' => t('In order to preview the results of the image markdown you have to add at least one {images} variable to the body markdown.'),
		'#submit' 	=> array('LMACGallery_previewMarkdownHandler'),
	); 
  
	$form['#rebuild'] = TRUE;
	
	return $form;
}

function LMACGallery_unpickCheckedSubmit($form, &$form_state) {
	if (! isset($form_state['input']['picked_images'])) {
		return;
	}
	
	$imageIds = [];
	
	foreach($form_state['input']['picked_images'] as $imageId=>$state) {
		if ($state == NULL) {
			continue;
		}
		$imageIds[] = $imageId;
	}

	_LMACGallery_unpickImages($imageIds);
}

function LMACGallery_unpickAllSubmit($form, &$form_state) {
	_LMACGallery_unpickImages(array_keys($form_state['values']['picked_images']));
}

function _LMACGallery_getCurationSettings() {
	
	return variable_get('LMACGallery_curation', array(
		'post_markdown_text'	=> html_entity_decode('Hello Community,&#10;We would like to show you this weeks top picks from the library. I proudly present the beautiful images contributed by {image_authors}.&#10;&#10;## Images&#10;{images}&#10;&#10;Best regards&#10;LMAC Team'), 
		'image_markdown_text'	=> html_entity_decode('![{post_author}\'s image]{{image_url}}&#10;Original post: [{post_title}]({post_url})&#10;Image tags: {image_tags}&#10;Author: {post_author}'),
		'post_benefication' 	=> 2,
		'post_default_beneficiary' => '',
		'post_sender' => 'lmac',
		'post_tags' => 'letsmakeacollage,lil,collage,art,curation,photography',
		'post_permlink' => 'lmac-lil-top-picks-1',
		'post_title' => 'LMAC Image Library - Top picks #1'
	)); 
}

function LMACGallery_saveMarkdownHandler($form, &$form_state) {

	variable_set('LMACGallery_rating', array(
		'post_markdown_text'		=> $form_state['values']['post_markdown_text'], 
		'image_markdown_text'		=> $form_state['values']['image_markdown_text'],
		'post_benefication' 		=> $form_state['values']['post_benefication'],
		'post_default_beneficiary'	=> $form_state['values']['post_default_beneficiary'],
		'post_sender'				=> $form_state['values']['post_sender'],
		'post_tags'					=> $form_state['values']['post_tags'],
		'post_permlink'				=> $form_state['values']['post_permlink'],
		'post_title'				=> $form_state['values']['post_title'],
	));
 
	
	drupal_set_message(t('The markdowns has been saved.'));
}

function LMACGallery_previewMarkdownHandler($form, &$form_state) {
	LMACGallery_saveMarkdownHandler($form, $form_state);
	drupal_goto('admin/lmac/lmac_ratings/preview_markdown');
}

function _LMACGallery_getCurationImagesOptions($handPickedImages) {
	
	if (empty($handPickedImages)) {
		return $handPickedImages;
	}
	
	$options = [];
	
	foreach($handPickedImages as $pickedImage) {
		$options[$pickedImage['imageid']] = "(" . (int)$pickedImage['rating'] . ") <a href=\"" . $pickedImage['post'] . "\">Post</a><br/><a href=\"" . $pickedImage['url'] . "\" target=\"_blank\"><img src=\"" . $pickedImage['thumbnailUrl']. "\" /></a>";
	}
	
	return $options;
}

function _LMACGallery_getCurationImages() {
 
	$dapp = variable_get('LMACGallery_dapp', 'https://hive.blog/');
 
	$imageIds = variable_get('LMACGallery_curationPicks', array());
	if (empty($imageIds)) {
		return [];
	}

 
	$query = db_select("images", "li");
	$query->addField("li", "imageid");
	$query->addField("li", "title");
	$query->addField("li", "tags");
	$query->addField("li", "url");
	$query->addField("li", "permlink");
	$query->addField("li", "author");
	$query->addExpression('(SELECT AVG(score) FROM lmacg_ratings as lr WHERE lr.imageid=li.imageid)', 'ratingscore');
	$query->where('imageid IN (:imageids)', array(':imageids'=>$imageIds));

	$result = $query->execute();
 
	while ($row = $result->fetchAssoc()) {
 
		$row['post'] = $dapp . '@' . $row['author'] . '/' . $row['permlink'];
		$row['thumbnailUrl'] = 'https://images.hive.blog/120x0/' . str_replace('https://images.hive.blog/0x0/"', '', $row['url']);			
		$row['truncatedTitle'] = (strlen($row['title']) > MAX_IMAGE_TITLE_LENGTH) ? substr($row['title'], 0, MAX_IMAGE_TITLE_LENGTH - strlen(TRUNCATE_DOTS)) . TRUNCATE_DOTS : $row['title'];
		$row['rating'] = $row['ratingscore'];
		$handPickedImages[$row['imageid']] = $row;		  
	}
 
 
	return $handPickedImages;
}

function _LMACGallery_formatMarkdown($markdown) {
 
	$markdown = str_replace("\r", '', $markdown);
 
	return check_markup($markdown, 'markdown');
}
 
function LMACGallery_previewMarkdown_form($form, &$form_state) {
	 
	
	$curationSettings = _LMACGallery_getCurationSettings();
	$handPickedImages = _LMACGallery_getCurationImages();
	$beneficiaries = _LMACGallery_getBeneficiaries($handPickedImages, $curationSettings);
	$postMarkdown = _LMACGallery_compileMarkdown($handPickedImages, $curationSettings);
	$images = _LMACGallery_getImagesFromMarkdown($postMarkdown);
	$users = _LMACGallery_getUsersFromMarkdown($postMarkdown);
 
	drupal_add_js(array('hivePost' => array(
		'images' => $images,
		'postMarkdown' => $postMarkdown,
		'beneficiaries' => $beneficiaries,
		'sender' => $curationSettings['post_sender'],
		'title' => $curationSettings['post_title'],
		'permlink' => $curationSettings['post_permlink'],
		'customJson'=> 
			array(
				'app'	=>'lmac.gallery',
				'format' => 'markdown',
				'description' => $curationSettings['post_title'],
				'tags' => explode(',', $curationSettings['post_tags']),
				'users' => $users,
				'image' => $images,
		 
			)
		
	)), 'setting');
	
	$form['#rebuild'] = TRUE;
	
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACGallery').'/LMACGallery.rating.css';
	$form['#attached']['js'][] = drupal_get_path('module', 'LMACGallery').'/LMACGallery.rating.js';
	
	$form['markdown_preview'] = array(
		'#type' 			=> 'textarea',
		'#title'			=> t('Markdown to post on Hive.'),
		'#value' 			=> html_entity_decode($postMarkdown),
		'#rows' 			=> 7,
		'#attributes' 		=> array('readonly' => array('readonly'))
	);
	$form['send_post'] = array(
		'#type' 			=> 'submit',
		'#value'			=> t('Send Hive post')
	);
	$form['cancel_send_post'] = array(
		'#type' 			=> 'submit',
		'#value'			=> t('Cancel')
	);
	$form['markdown_rendered_preview'] = array(
		'#type'   			=> 'item',
		'#title'			=> t('Rendered markdown'),
		'#markup' 			=> _LMACGallery_formatMarkdown($postMarkdown),
		'#format' 			=> 'markdown',
	);
	$form['beneficiaries_preview'] = array(
		'#type'   			=> 'item',
		'#title'			=> t('Beneficiaries'),
		'#markup' 			=> _LMACGallery_viewBeneficiaries($beneficiaries),
	);
	return $form;
}

function _LMACGallery_compileMarkdown($handPickedImages, $curationSettings) {
	
	$authors = [];	
	$images = '';
	foreach($handPickedImages as $pickedImage) {
		$images .= _LMACGallery_processMarkdown($curationSettings['image_markdown_text'], array('image_tags'=>strtolower($pickedImage['tags']),'image_thumbnail_url'=>$pickedImage['thumbnailUrl'], 'image_url'=>$pickedImage['url'], 'post_author'=>$pickedImage['author'], 'post_url'=>$pickedImage['post'], 'post_title'=>$pickedImage['title']));
		$authors['@' . $pickedImage['author']] = TRUE;
	}

	$markdown = _LMACGallery_processMarkdown($curationSettings['post_markdown_text'], array('images'=>$images, 'image_authors'=>implode(', ', array_keys($authors))));
	
	return str_replace("\r", '', $markdown);
}

function _LMACGallery_processMarkdown($markdownTemplate, $replacementVariables) {
	
	foreach($replacementVariables as $key=>$value) {
		$markdownTemplate = str_replace("{{$key}}", $value, $markdownTemplate);
	}
	
	return $markdownTemplate;
}

function _LMACGallery_viewBeneficiaries($beneficiaries) {
	$output = '<ul>';
	
	foreach($beneficiaries as $beneficiary) {
		$output .= '<li>' . $beneficiary['account'] . ' (' . ((100.0 / 10000.0) * (float)$beneficiary['weight']) . '%)</li>';
	}
	
	$output .= '</ul>';
	
	return $output;
}

function _LMACGallery_getBeneficiaries($handPickedImages, $curationSettings) {
	$authorShares = [];
	$sharedTotal = 0.0;
	
	foreach($handPickedImages as $pickedImage) {
		if (! array_key_exists($pickedImage['author'], $authorShares)) {
			$authorShares[$pickedImage['author']] = 0;	
		}
		$share = (10000.0 / 100.0) * ((float)$pickedImage['rating'] * (float)$curationSettings['post_benefication']);
		$authorShares[$pickedImage['author']] += $share;
		$sharedTotal += $share;		
	}
	
	$authorShares[empty($curationSettings['post_default_beneficiary']) ? $curationSettings['post_sender'] :  $curationSettings['post_default_beneficiary']] = 10000 - $sharedTotal;
	
	$beneficiaries = [];
	foreach($authorShares as $author=>$shares) {
		$beneficiaries[] = array('account'=>$author, 'weight'=>$shares);
	}	
	
	return $beneficiaries;
}

function _LMACGallery_getImagesFromMarkdown($postMarkdown) {
	
	$matches = [];
	preg_match_all('/!\[.*?\]\((.*?)\)/m', $postMarkdown, $matches, PREG_PATTERN_ORDER);
	
	if (count($matches) < 2) {
		return [];
	}
 	
	return $matches[1];
}

function _LMACGallery_getUsersFromMarkdown($postMarkdown) {
	
	$matches = [];
	preg_match_all('/@([a-zA-Z0-9_]*)/m', $postMarkdown, $matches, PREG_PATTERN_ORDER);
	
	if (count($matches) < 2) {
		return [];
	}
	
	$users = [];
	foreach($matches[1] as $match) {
		$users[$match] = $match;
	}
 	
	return array_keys($users);
}

function _LMACGallery_unpickImages($imageIds) {
	
}

