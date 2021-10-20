<?php

function LMACWinners_admin_contests_form($form, &$form_state) {
 	
	$form['table'] = array(
		'#type' => 'tableselect',
		'#header' => array(
			'roundid' => array('data' => t('Round'), 'field' => 'roundid'),
			'title' => array('data' => t('Title'), 'field' => 'title'),
			'links' => array('data' => t('Links'), 'field' => 'links'),
		),
		'#options' => _LMACWinners_admin_getContests(),
		'#empty' => t('No contests available.'),
    );
	
	$form['delete_checked_submit'] = array(
		'#type' => 'submit',
		'#value' => t('Delete checked contests'),
		'#submit' => array('LMACWinners_admin_deleteCheckedRounds_form_submitHandler')
	);
	
	$form['new_contest_link'] = array(
		'#type' => 'link',
		'#title' => t('Add new contest round'),
		'#href' => '/admin/lmac/new-contest'
	);
	
	$form['pager'] = array(
		'#type' => 'markup',
		'#markup' => theme("pager")
	);
	
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACWinners').'/LMACWinners.admin.css';
	
	return $form;
}

function LMACWinners_admin_winners_form($form, &$form_state) {
 	
	if (empty($form_state['build_info']['args'])) {
		
		drupal_set_title(t('Error'));
		
		drupal_set_message(t('Contest round not found!'), 'error');
		
		return array();
	}
	
	$roundId = $form_state['build_info']['args'][0];
	
	drupal_set_title(t('Winners of round #!round',array('!round'=>$roundId)));
	
	$form['table'] = array(
		'#type' => 'tableselect',
		'#header' => array(
			'place' => array('data' => t('Place'), 'field' => 'place'),
			'artist' => array('data' => t('Artist'), 'field' => 'artist'),
			'title' => array('data' => t('Title'), 'field' => 'title'),
			'links' => array('data' => t('Links'), 'field' => 'links'),
		),
		'#options' => _LMACWinners_admin_getWinnersByRoundId($roundId),
		'#empty' => t('Not winners available.'),
    );
	
	$form['delete_checked_submit'] = array(
		'#type' => 'submit',
		'#value' => t('Delete checked winners'),
		'#submit' => array('LMACWinners_admin_deleteCheckedWinners_form_submitHandler')
	);
	
	$form['new_contest_link'] = array(
		'#type' => 'link',
		'#title' => t('Add new winner'),
		'#href' => '/admin/lmac/new-winner/' . $roundId
	);
	
	$form['cancel_link'] = array(
		'#type' => 'link',
		'#title' => t('Cancel'),
		'#href' => '/admin/lmac/contests'
	);
	
	$form['pager'] = array(
		'#type' => 'markup',
		'#markup' => theme("pager")
	);
	
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACWinners').'/LMACWinners.admin.css';
	
	return $form;
}

function LMACWinners_admin_editContest_form($form, $form_state) {

	$formTitle = '';
    $roundId = empty($form_state['build_info']['args']) ? 'new' : $form_state['build_info']['args'][0];

 
	$contestDefaults = array(
		'roundid' => 0,
		'templateimageurl' => '',
		'announcementtitle' => '',
		'announcementurl' => '',
		'polltitle' => '',
		'pollurl' => '',
		'winnerstitle' => '',
		'winnersurl' => ''
	);	
	
	$editMode = FALSE;
 
	if ($roundId == 'new') {
	
		$formTitle = t('Add new contest');
	}else{
		$contestDefaults = _LMACWinner_admin_loadContestById($roundId);
		if ($contestDefaults == NULL) {
			drupal_set_message(t('Contest not found!'), 'error');
			return array();
		}else{
			$formTitle = t('Edit contest');
			$editMode = TRUE;
		}
	}
	
	 
	drupal_set_title($formTitle);
	
	$form['header'] = array(
		'#type' => 'markup', 
		'#markup' => $formTitle,
		'#prefix' => '<h2>',
		'#suffix' => '</h2>'
	);
	

	$form['roundid'] = array(
		'#type'  => 'textfield',
		'#title' => t('LMAC Round:'),
		'#description' => t('Enter the number of the contest round. WARNING: IF THE CONTEST WITH THIS NUMBER ALREADY EXISTS, IT WILL BE OVERWRITTEN WHEN THIS CONTEST IS SAVED! IF YOU CHANGE THIS ON AN EXISTING CONTEST, A NEW CONTEST WILL BE CREATED!'),
		'#size' => 5,
		'#maxlength' => 5,
		'#default_value' => $contestDefaults['roundid'],
	);

	
	$form['templateimageurl'] = array(
		'#type'  => 'textfield',
		'#title' => t('Template image url:'),
		'#description' => t('The Url of the template image used for this specific round.'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['templateimageurl'],
		'#attributes' => array('placeholder'=>'https://files.peakd.com/file/peakd-hive/lmac/EqL9E...p.jpg')
	);
	
	$form['announcement'] = array(
		'#type'  => 'fieldset',
		'#title' => t('Announcement post'),
		'#description' => t('Enter title and Url of the announcement post of this specific contest round.'),
		'#tree' => TRUE
	);
	
	$form['announcement']['title'] = array(
		'#type'  => 'textfield',
		'#title' => t('Title:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['announcementtitle']
	);
	
	$form['announcement']['url'] = array(
		'#type'  => 'textfield',
		'#title' => t('Url:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['announcementurl'],
		'#attributes' => array('placeholder'=>'https://peakd.com/hive-174695/@shaka/lets-make-a-collage-...')
	);
	
	$form['poll'] = array(
		'#type'  => 'fieldset',
		'#title' => t('Poll post'),
		'#description' => t('Enter title and Url of the poll post of this specific contest round.'),
		'#tree' => TRUE
	);
	
	$form['poll']['title'] = array(
		'#type'  => 'textfield',
		'#title' => t('Title:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['polltitle'],
	);
	
	$form['poll']['url'] = array(
		'#type'  => 'textfield',
		'#title' => t('Url:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['pollurl'],
		'#attributes' => array('placeholder'=>'https://peakd.com/hive-174695/@shaka/lets-make-a-collage-...')
	);
	
	$form['winners'] = array(
		'#type'  => 'fieldset',
		'#title' => t('Winners post'),
		'#description' => t('Enter title and Url of the winners post of this specific contest round.'),
		'#tree' => TRUE
	);
	
	$form['winners']['title'] = array(
		'#type'  => 'textfield',
		'#title' => t('Title:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['winnerstitle'],
	);
	
	$form['winners']['url'] = array(
		'#type'  => 'textfield',
		'#title' => t('Url:'),
		'#size' => 64,
		'#maxlength' => 512,
		'#default_value' => $contestDefaults['winnersurl'],
		'#attributes' => array('placeholder'=>'https://peakd.com/hive-174695/@shaka/lets-make-a-collage-...')
	);
	
	$form['save_submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save contests'),
		'#submit' => array('LMACWinners_admin_saveContest_form_submitHandler')
	);
	
	$form['cancel_link'] = array(
		'#type' => 'link',
		'#title' => t('Cancel'),
		'#href' => '/admin/lmac/contests'
	);
	
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACWinners').'/LMACWinners.admin.css';
	

	return $form;
}

 function LMACWinners_admin_winnerEdit_form($form, &$form_state) {
	 
	$formTitle = '';
	
	if (count($form_state['build_info']['args']) == 1) {
		$roundId = $form_state['build_info']['args'][0];
		$winnerId = 'new';
	}else if (count($form_state['build_info']['args']) == 2) {
		$roundId = $form_state['build_info']['args'][0];
		$winnerId = $form_state['build_info']['args'][1];
	}else {
		drupal_not_found();
		return;
	}

	if ($roundId < 1 || ! _LMACWinner_admin_isContestAvailable($roundId)) {
	
			drupal_set_message(t('Contest not found!'), 'error');
			return array();
	}
 
 
	$winnerDefaults = array(
		'winnerid' => 0,
		'round' => $roundId,
		'place' => '',
		'artist' => '',
		'imageurl' => '',
		'posturl' => '',
		'title' => ''
	);	
	
	$editMode = FALSE;
 
	if ($winnerId == 'new') {
	
		$formTitle = t('Add new winner to round #!round', array('!round' => $roundId));
	}else{
		$winnerDefaults = _LMACWinner_admin_loadWinnerById($winnerId);
		if ($winnerDefaults == NULL) {
			drupal_set_message(t('Winner not found!'), 'error');
			return array();
		}else{
			$formTitle = t('Edit winner');
			$editMode = TRUE;
		}
	}
		 
	drupal_set_title($formTitle);
	
	$form['winnerid'] = array(
		'#type' => 'value', 
		'#value' => $winnerDefaults['winnerid']
	);	 
	 
	$form['round'] = array(
		'#type'  => 'value',
		'#value' => $winnerDefaults['round'],
	);
	
	$form['place'] = array(
		'#type'  => 'select',
		'#title' => t('Won place:'),
		'#description' => t('The place the artist has won.'),
		'#options' => array(
			1 => t('Place 1'),
			2 => t('Place 2'),
			3 => t('Place 3'),
			4 => t('Place 4'),
			5 => t('Place 5'),
			6 => t('Place 6'),
			7 => t('Place 7'),
			8 => t('Place 8'),
			9 => t('Place 9'),
			10 => t('Place 10'),
			11 => t('Place 11'),
			12 => t('Place 12'),
			13 => t('Place 13'),
			14 => t('Place 14'),
			15 => t('Place 15'),
		),
		'#default_value' => $winnerDefaults['place'],
		
	);
	
	$form['artist'] = array(
		'#type'  => 'textfield',
		'#title' => t('Artist:'),
		'#description' => t('The Hive username of the artist (without the leading @).'),
		'#size' => 32,
		'#maxlength' => 64,
		'#default_value' => $winnerDefaults['artist'],
	);
	
	$form['imageurl'] = array(
		'#type'  => 'textfield',
		'#title' => t('Image Url:'),
		'#description' => t('The Url of the collage image used in the entry post.'),
		'#size' => 80,
		'#maxlength' => 512,
		'#default_value' => $winnerDefaults['imageurl'],
		'#attributes' => array('placeholder'=>'https://files.peakd.com/file/peakd-hive/lmacian/EqL9E...p.jpg')
	);
	$form['title'] = array(
		'#type'  => 'textfield',
		'#title' => t('Hive post title:'),
		'#description' => t('The title of the Hive post the artist participated in the contest with.'),
		'#size' => 80,
		'#maxlength' => 512,
		'#default_value' => $winnerDefaults['title'],
	);
	$form['posturl'] = array(
		'#type'  => 'textfield',
		'#title' => t('Hive post Url:'),
		'#description' => t('The Url of the collage entry post.'),
		'#size' => 80,
		'#maxlength' => 512,
		'#default_value' => $winnerDefaults['posturl'],
		'#attributes' => array('placeholder'=>'https://peakd.com/hive-174695/@lmacian/my-collage-for-round-...')
	);

	$form['save'] = array(
		'#type'  => 'submit',
		'#value' => t('Save'),
		'#submit' => array('LMACWinners_admin_winnerEdit_saveSubmitHandler')
	);
	$form['save_and_next'] = array(
		'#type'  => 'submit',
		'#value' => t('Save and create next'),
		'#submit' => array('LMACWinners_admin_winnerEdit_saveSubmitHandler')
	);
	
	$form['cancel_link'] = array(
		'#type' => 'link',
		'#title' => t('Cancel'),
		'#href' => '/admin/lmac/winners/' . $roundId
	);
	
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACWinners').'/LMACWinners.admin.css';
	
	return $form;
 }
 
 function LMACWinners_admin_winnerEdit_saveSubmitHandler($form, &$form_state) {
	
	$inputData = $form_state['values'];
	$redirectUrl = '/admin/lmac/winners/' . $inputData['round'];
 
	if ($form_state['input']['op'] == $form['save_and_next']['#value']) {
		$redirectUrl = '/admin/lmac/new-winner/' . $inputData['round'];
	}

	
	unset($inputData['save']);
	unset($inputData['save_and_next']);
	unset($inputData['form_build_id']);
	unset($inputData['form_token']);
	unset($inputData['op']);
	unset($inputData['form_id']);
	
	_LMACWinner_admin_saveWinner($inputData);
	
	drupal_goto($redirectUrl);
 }
 
 function LMACWinners_admin_deleteCheckedRounds_form_submitHandler($form, &$form_state) {
	
	$roundIds = $form_state['values']['table'];
	$roundsToDelete = [];
	foreach($roundIds as $roundId) {
		
		if ($roundId === 0) {
			continue;
		}
		$roundsToDelete[] = $roundId;
	}
	
	_LMACWinner_admin_deleteContests($roundsToDelete);
 }
 
 function LMACWinners_admin_deleteCheckedWinners_form_submitHandler($form, &$form_state) {
	
	$winnerIds = $form_state['values']['table'];
	$winnersToDelete = [];
	foreach($winnerIds as $winnerId) {
		
		if ($winnerId === 0) {
			continue;
		}
		$winnersToDelete[] = $winnerId;
	}
	
	_LMACWinner_admin_deleteWinners($winnersToDelete);
 }
 
 function _LMACWinner_admin_deleteContests($roundsToDelete) {
	 
	db_delete('lmacrounds')
	->condition('roundid', $roundsToDelete, 'IN')
	->execute();
		 
	db_delete('lmacwinners')
	->condition('round', $roundsToDelete, 'IN')
	->execute();
 }
 
 function _LMACWinner_admin_deleteWinners($winnersToDelete) {

	db_delete('lmacwinners')
	->condition('winnerid', $winnersToDelete, 'IN')
	->execute();
 }
 
 function LMACWinners_admin_saveContest_form_submitHandler($form, &$form_state) {
	
	$inputData = $form_state['values'];
	
	_LMACWinner_admin_saveContest(array(
		'roundid' => $inputData['roundid'],
		'templateimageurl' => $inputData['templateimageurl'],
		'announcementtitle' => substr($inputData['announcement']['title'],0, 512),
		'announcementurl' => $inputData['announcement']['url'],
		'polltitle' => substr($inputData['poll']['title'],0, 512),
		'pollurl' => $inputData['poll']['url'],		
		'winnerstitle' => substr($inputData['winners']['title'],0, 512),
		'winnersurl' => $inputData['winners']['url'],	
	));
	drupal_goto('/admin/lmac/contests');
 }
 
 function _LMACWinner_admin_saveContest($contestData) {
	db_merge('lmacrounds')
	  ->key(array('roundid' => $contestData['roundid']))
	  ->fields($contestData)
	  ->execute();
 }
 
 function _LMACWinner_admin_saveWinner($winnerData) {
	db_merge('lmacwinners')
	  ->key(array('winnerid' => $winnerData['winnerid']))
	  ->fields($winnerData)
	  ->execute();
 }
 
 function _LMACWinner_admin_loadContestById($roundId) {
	 
	$contest = db_query('SELECT * FROM {lmacrounds} WHERE roundid = :id LIMIT 1', array(':id'=>$roundId))->fetchAssoc(); 
	if (! $contest) {
		return NULL;
	}

	return $contest;
 }
 
 function _LMACWinners_admin_getContests() {
	$rounds = [];
 
	$query = db_select("lmacrounds", "lr");
	$query->addField("lr", "roundid");
	$query->addField("lr", "winnerstitle");
	$query->addField("lr", "announcementtitle");
	$query->addField("lr", "polltitle");
 
	$result = $query->execute();
	
	while ($row = $result->fetchAssoc()) {
 
		$rounds[$row['roundid']] = [
			'roundid' => $row['roundid'],
			'links' => _LMACWinners_admin_getContestLinksColumn($row),
			'title' => _LMACWinners_admin_getContestTitleColumn($row)
		];		  
	}

	return $rounds ;
}

function _LMACWinners_admin_getContestTitleColumn($row) {

	if (! empty($row['announcementtitle'])) {
		return $row['announcementtitle'];
	}
	
	if (! empty($row['winnerstitle'])) {
		return $row['winnerstitle'];
	}
	
	if (! empty($row['polltitle'])) {
		return $row['polltitle'];
	}
	
	return t('No title found!');
}

function _LMACWinners_admin_getContestLinksColumn($row) {
	
	$linklist = array(
		'item1' => array( 'title' => 'Edit', 'href' => '/admin/lmac/edit-contest/' . $row['roundid']),
		'item2' => array( 'title' => 'Manage winners', 'href' => '/admin/lmac/winners/' . $row['roundid'])
	);
	
	return theme('links', array(
	  'links' => $linklist,
	  'attributes' => array('class' => array('links', 'inline'))));   
}

function _LMACWinners_admin_getWinnersByRoundId($roundId) {
	$winners = [];
 
	$query = db_select("lmacwinners", "lw");
	$query->addField("lw", "winnerid");
	$query->addField("lw", "round");
	$query->addField("lw", "place");
	$query->addField("lw", "artist");
	$query->addField("lw", "imageurl");
	$query->addField("lw", "posturl");
	$query->addField("lw", "title");
	$query->where('lw.round = :roundid', array(':roundid'=>$roundId));
	$query->orderBy("place", 'ASC');
	
	$result = $query->execute();
 
 
	while ($row = $result->fetchAssoc()) {
 
		$winners[$row['winnerid']] = [
			'place' => $row['place'],
			'artist' => $row['artist'],
			'links' => _LMACWinners_getWinnersLinksColumn($row),
			'title' => $row['title']
		];		  
	}

	return $winners;
}

function _LMACWinner_admin_isContestAvailable($roundId) {

	return _LMACWinner_admin_loadContestById($roundId) != NULL;
}

function _LMACWinners_getWinnersLinksColumn($row) {
	
	$linklist = array(
		'item1' => array( 'title' => 'Edit', 'href' => '/admin/lmac/edit-winner/' . $row['round'] . '/' . $row['winnerid'])
	);
	
	return theme('links', array(
	  'links' => $linklist,
	  'attributes' => array('class' => array('links', 'inline'))));   
}

function _LMACWinner_admin_loadWinnerById($winnerId) {
	 
	$winner = db_query('SELECT * FROM {lmacwinners} WHERE winnerid = :id LIMIT 1', array(':id'=>$winnerId))->fetchAssoc(); 
	if (! $winner ) {
		return NULL;
	}

	return $winner ;
 }