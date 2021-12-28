<?php
/*
 * Form callback for editing a contest round.
**/
function LMACWinners_editRound_form($form, &$form_state) {

   if (count($form_state['build_info']['args']) == 0) {
      drupal_not_found();
      return;
   }

   if ($form_state['build_info']['args'][0] == 'new') {
      $contest = _LMACWinners_createContestNullObject();
   }else{
      $contestId = (int)$form_state['build_info']['args'][0];
      $contest = _LMACWinners_loadContest($contestId);
   }

   if ($contest->contestId == 0) {
      $form['contestId'] = array(
         '#type' => 'textfield',
         '#title' => t('Contest Id:'),
         '#description' => t('The number of the contest round.'),
         '#default_value' => $contest->contestId,
      );
      $form['isNew'] = array(
         '#type' => 'value',
         '#value' => TRUE
      );
   }else{
      $form['contestId'] = array(
         '#type' => 'value',
         '#value' => $contest->contestId,
      );
   }

   $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title:'),
      '#default_value' => $contest->title,
      '#size' => 120,
      '#maxlength' => 512,
      '#required' => TRUE
   );

   $form['postUrl'] = array(
      '#type' => 'textfield',
      '#title' => t('Post url:'),
      '#default_value' => $contest->postUrl,
      '#size' => 120,
      '#maxlength' => 512,
      '#required' => TRUE
   );

   $form['templateImageUrl'] = array(
      '#type' => 'textfield',
      '#title' => t('Template image url:'),
      '#default_value' => $contest->templateImageUrl,
      '#size' => 120,
      '#maxlength' => 512,
      '#required' => TRUE
   );

   $form['winners'] = array(
      '#type' => 'fieldset',
      '#title' => t('Winners'),
      '#collapsible' => FALSE, 
      '#collapsed' => FALSE,
      '#tree' => TRUE
   );
   
   if ($contest->contestId == 0) {
         $form['winners']['message'] = array(
            '#type' => 'markup',
            '#markup' => t('Save the contest first, then add winners.'),
            '#prefix' => '<p>',
            '#suffix' => '</p>'
         );
   }else{
      foreach($contest->winners as $artist=>$winner) {
         $form['winners'][$artist] = array(
            '#type' => 'fieldset',
            '#title' => $artist,
            '#collapsible' => TRUE, 
            '#collapsed' => TRUE,
            '#tree' => TRUE
         );
         $form['winners'][$artist]['artist'] = array(
            '#type' => 'textfield',
            '#title' => t('Artist:'),
            '#default_value' => $winner['artist'],
            '#size' => 32,
            '#maxlength' => 512
         );
         $form['winners'][$artist]['winningPlace'] = array(
            '#type' => 'textfield',
            '#title' => t('Winning place:'),
            '#default_value' => $winner['winningPlace'],
            '#size' => 2,
            '#maxlength' => 2
         );
         $form['winners'][$artist]['postUrl'] = array(
            '#type' => 'textfield',
            '#title' => t('Post url:'),
            '#default_value' => $winner['postUrl'],
            '#size' => 120,
            '#maxlength' => 512
         );
         $form['winners'][$artist]['imageUrl'] = array(
            '#type' => 'textfield',
            '#title' => t('Image url:'),
            '#default_value' => $winner['imageUrl'],
            '#size' => 120,
            '#maxlength' => 512
         );
         $form['winners'][$artist]['delete'] = array(
            '#type' => 'checkbox',
            '#title' => t('Delete entry')
         );
      }
   }

   $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
      '#submit' => array('_LMACWinners_editRound_submit'),
   );

   $form['back_to_the_gallery'] = array(
      '#type' => 'submit',
      '#value' => t('Back to the gallery'),
      '#submit' => array('_LMACWinners_back_submit'),
   );
   
   if ($contest->contestId > 0) {
      $form['newWinner'] = array(
         '#type' => 'fieldset',
         '#title' => t('New winner entry'),
         '#collapsible' => FALSE, 
         '#collapsed' => FALSE,
         '#tree' => TRUE
      );

      $form['newWinner']['artist'] = array(
         '#type' => 'textfield',
         '#title' => t('Artist:'),
         '#size' => 32,
         '#maxlength' => 512
      );
      $form['newWinner']['winningPlace'] = array(
         '#type' => 'textfield',
         '#title' => t('Winning place:'),
         '#size' => 2,
         '#maxlength' => 2
      );
      $form['newWinner']['postUrl'] = array(
         '#type' => 'textfield',
         '#title' => t('Post url:'),
         '#size' => 120,
         '#maxlength' => 512
      );
      $form['newWinner']['imageUrl'] = array(
         '#type' => 'textfield',
         '#title' => t('Image url:'),
         '#size' => 120,
         '#maxlength' => 512
      );
      $form['newWinner']['submit'] = array(
         '#type' => 'submit',
         '#value' => t('Add'),
         '#submit' => array('_LMACWinners_editRound_addWinner_submit'),
      );
   }


   return $form;
}

/*
 * Creates and returns a contest NULL object.
 * @returns a NULL object. 
 **/
function _LMACWinners_createContestNullObject() {
   
   return (object)[
      'contestId' => 0,
      'title' => '',
      'postUrl' => '',
      'templateImageUrl' => '',
      'winners' => []
   ];
}

/*
 * Submit handler for the back button.
**/
function _LMACWinners_back_submit($form, &$form_state) {
   $contest = (object)$form_state['values'];
   drupal_goto('lmac-winners/' . $contest->contestId);
}

/*
 * Submit handler for the "Add winner" button.
**/
function _LMACWinners_editRound_addWinner_submit($form, &$form_state) {
   $contest = (object)$form_state['values'];

   if (strlen($contest->newWinner['artist']) == 0) {
      drupal_set_message('Missing artist name for new winner entry.', 'error');
      return;
   }

   if ((int)$contest->newWinner['winningPlace'] == 0) {
      drupal_set_message('Missing place number for new winner entry.', 'error');
      return;
   }

   $contest->winners[$contest->newWinner['artist']] = [
      'artist' => $contest->newWinner['artist'],
      'winningPlace' => $contest->newWinner['winningPlace'],
      'imageUrl' => $contest->newWinner['imageUrl'],
      'postUrl' => $contest->newWinner['postUrl']
   ];

   $processedWinners = [];
   foreach($contest->winners as $winner){
   
      unset($winner['delete']);
      $processedWinners[$winner['artist']] = $winner;
   }

   _LMACWinners_saveContest($contest->contestId, $contest->title, $contest->postUrl, $contest->templateImageUrl, $processedWinners);

   drupal_set_message('Added new winner entry.');
}

/*
 * Submit handler for the "Save" button.
**/
function _LMACWinners_editRound_submit($form, &$form_state) {
   $contest = (object)$form_state['values'];

   $processedWinners = [];
   if (isset($contest->winners)) {
      foreach($contest->winners as $winner){
         if ($winner['delete'] !== 0) {
            continue;
         }
      
         unset($winner['delete']);
         $processedWinners[$winner['artist']] = $winner;
      }
   }

   _LMACWinners_saveContest($contest->contestId, $contest->title, $contest->postUrl, $contest->templateImageUrl, $processedWinners, isset($contest->isNew));
   drupal_set_message('Contest saved.');
   drupal_goto('lmac-winners/' . $contest->contestId);
}

/*
 * Loads a specific contest from the database.
 * @param $contestId The Id (round number) of the contest.
 * @return The contest data (object) on success. Otherwise NULL.
**/
function _LMACWinners_loadContest($contestId) {
   $contest = db_query('SELECT * FROM {lmac_contests} WHERE contestId=:id', array(':id' => $contestId))->fetchObject();
   if (! $contest) {
		return NULL;
	}

   $contest->winners = json_decode($contest->winners, True);

   return $contest;
}

/*
 * Saves a contest to the database.
 * @param $contestId The Id (round number) of the contest.
 * @param $title The title of the contest (Post title of the poll post.)
 * @param $postUrl The url of the contest poll post (relative hive url).
 * @param $templateImageUrl The url of the contest template.
 * @param $winners An array of winners.
 * @param $isNew Indicates wether a the entry is a new entry or is an update. TRUE when it is new.
**/
function _LMACWinners_saveContest($contestId, $title, $postUrl, $templateImageUrl, $winners, $isNew = FALSE) {
   if ($isNew) {
      db_insert('lmac_contests')
      ->fields(array(
         'contestId' => (int)$contestId,
         'title' => $title,
         'postUrl' => $postUrl,
         'templateImageUrl' => $templateImageUrl,
         'winners' => json_encode($winners)
      ))
      ->execute();
   }else{
      db_update('lmac_contests')
          ->fields(
            array(
               'title' => $title,
               'postUrl' => $postUrl,
               'templateImageUrl' => $templateImageUrl,
               'winners' => json_encode($winners)
            )
          )
          ->condition('contestId', (int)$contestId)
          ->execute();
   }
}



