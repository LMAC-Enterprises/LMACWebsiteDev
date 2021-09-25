<?php
/**
 * @file
 * Controls the LMAC gallery administration.
 * @author
 * QuantumG
 **/
 
 
 function LMACGallery_admin_configuration_form($form, &$form_state) {

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
 
 	$form['settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Settings'),
		'#collapsible' => False,
		'#collapsed' => False,  
	); 
 
    $form['settings']['dapp'] = array(
       '#type' => 'select',
       '#title' => t('In which Dapp Hive links should open?'),
       '#options' => array(
			'https://peakd.com/' => 'PeakD',
			'https://hive.blog/' => 'Hive Blog',
			'https://ecency.com/' => 'Ecency'
       ),
       '#default_value' => variable_get('LMACGallery_dapp', 'https://hive.blog/')
   );
	
    $form['submit'] = array(
        '#type' 	=> 'submit',
        '#value' 	=> t('Save'),
		'#submit' 	=> array('LMACGallery_admin_configuration_form_submit'),
    );
		
	$form['#rebuild'] = True;
	
	return $form;
 }
 
 function _LMACGallery_getHideList($makeItemsLinks = False) {

	$hideList = variable_get('LMACGallery_hideList', array());
 
	if ($makeItemsLinks) {
		$listToChange = $hideList;
		$hideList = [];
		foreach($listToChange as $hideListItem) {
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
	
	variable_set('LMACGallery_dapp', $form_state['values']['dapp']);
}
 
