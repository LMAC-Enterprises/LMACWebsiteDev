<?php
/**
 * @file
 * Controls the LMAC gallery administration.
 * @author
 * QuantumG
 **/
 
 
 function LMACGallery_admin_configuration_form($form, &$form_state) {

	global $user;
 
	$form['#attached']['css'][] = drupal_get_path('module', 'LMACGallery').'/LMACGallery.admin.css';
 
	$form['hidegroup'] = array(
		'#type' => 'fieldset',
		'#title' => t('Hide list'),
		'#description' => t('Check an Url and press the "Save" button to delete an Url from the list. The images added to this list will no more apear in the gallery (Make sure to flash the cache).'),
		'#collapsible' => False,
		'#collapsed' => False,  
	);

	$hideList = _LMACGallery_getHideList(True);
	
	$form['hidegroup']['hidelist_items'] = array(
	  '#type' => 'checkboxes',
	  '#options' => $hideList,
	);
	
	if (empty($hideList)) {
		$form['hidegroup']['empty_hidelist'] = array(
			'#type' 	=> 'markup',
			'#markup' 	=> t('Hide List is empty.'),
			'#prefix'	=> '<p>',
			'#suffix'	=> '</p>'
		);
	}
 
    $form['hidegroup']['url_to_hide'] = array(
        '#type' => 'textfield',
        '#title' => t('Hide image Url'),
		'#description' => t('Hide an image by adding the image Url to the Hide List.'),
        '#size' => 50,
        '#maxlength' => 200,
        '#required' => False
    );
	
    $form['submit'] = array(
        '#type' 	=> 'submit',
        '#value' 	=> t('Save'),
		'#submit' 	=> array('LMACGallery_admin_configuration_form_submit'),
    );
	
 
	$form['ratings_group'] = array(
		'#type'  => 'fieldset',
		'#title' => t('Ratings'),
		'#description' => t('Never hit the "DELETE ALL RATINGS" button unless you want to delete ALL ratings from ALL images in the gallery.'),
		'#access' => user_access('delete lmac_gallery ratings', $user)
	);
	
	$form['ratings_group']['confirm_delete_ratings_password'] = array(
		'#type'  => 'password',
		'#title' => t('Confirm by entering your password.'),
		'#size' => 32,
		'#access' => user_access('delete lmac_gallery ratings', $user)
	);
	
	$form['ratings_group']['ratings'] = array(
		'#type'  => 'submit',
		'#value' => t('Delete all ratings'),
		'#submit' => array('LMACGallery_admin_deleteRatings_submit'),
		'#attributes' => array('onclick'=>'return confirm("Are you sure? Press \'yes\' if you want to delete all ratings.");'),
		'#access' => user_access('delete lmac_gallery ratings', $user)
	);
 
	
		
	$form['#rebuild'] = True;
	
	return $form;
 }
 
 function LMACGallery_admin_deleteRatings_submit($form, &$form_state) {
	
	global $user;
	
	require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
	 
	if (! user_check_password($form_state['input']['confirm_delete_ratings_password'], $user)) {
		drupal_set_message('Error. The password you have entered doesn\'t match.', 'error');
		return;
	}
	
	
	db_delete(' lmacg_ratings')->execute();
	cache_clear_all();
	
	drupal_set_message('All ratings deleted.');
 }

function _LMACGallery_admin_startsWith ($string, $startString)
{
    $len = strlen($startString);
    return (substr($string, 0, $len) === $startString);
}
 
 function _LMACGallery_getHideList($makeItemsLinks = False) {

	$hideList = variable_get('LMACGallery_hideList', array());
 
	if ($makeItemsLinks) {
		$listToChange = $hideList;
		$hideList = [];
		foreach($listToChange as $hideListItem) {

			if (_LMACGallery_admin_startsWith($hideListItem, 'https://images.hive.blog/0x0/')) {
				$hideListItem = str_replace('https://images.hive.blog/0x0/', 'https://images.hive.blog/120x0/', $hideListItem);
			}else{
				$hideListItem = 'https://images.hive.blog/120x0/' . $hideListItem;
			}

			$hideList[] = "<a href=\"$hideListItem\" target=\"_blank\"><img src=\"$hideListItem\" width=\"120\"/></a>";
		}
	}
 
	return $hideList;
 }
 
 function _LMACGallery_deleteItemsFromHideList($optionItems) {

	$hideList = variable_get('LMACGallery_hideList', array());
 
	foreach($optionItems as $optionItem) {
		
		if ($optionItem === 0) {

			continue;
		}
		
		unset($hideList[$optionItem]);
	}
	
	variable_set('LMACGallery_hideList', $hideList);
 }
 
 function _LMACGallery_addUrlToHideList($Url) {
	 
	$hideList = variable_get('LMACGallery_hideList', array());
	$hideList[] = $Url;
	variable_set('LMACGallery_hideList', $hideList);	 
 }
 
 function LMACGallery_admin_configuration_form_submit($form, &$form_state){
	if (isset($form_state['values']['url_to_hide']) && !empty($form_state['values']['url_to_hide'])) {
		_LMACGallery_addUrlToHideList($form_state['values']['url_to_hide']);
	}
	if (isset($form_state['values']['hidelist_items']) && !empty($form_state['values']['hidelist_items'])) {
		_LMACGallery_deleteItemsFromHideList($form_state['values']['hidelist_items']);		
	}

	drupal_set_message('All changes have been saved.');
}
 
