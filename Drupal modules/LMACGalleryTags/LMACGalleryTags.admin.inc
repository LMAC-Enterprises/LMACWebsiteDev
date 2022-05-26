<?php

function LMACGalleryTags_admin_form($form, &$form_state) {

   $settings = _LMACGalleryTags_getSettings();
 
   $form['page_welcome'] = array(
      '#type' => 'item',
      '#markup' => t('In this gallery you will find images that Hive users have diligently contributed to the LMAC Image Library.  Some images probably could not be read by the bot. If you can\'t find your images here, check the respective Hive posts for errors with the LIL Checker tool.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>'
    );

    $form['minimumTags'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum tags:'),
      '#description' => t('The minimum amount of tags in the cloud.'),
      '#size' => 10,
      '#default_value' => $settings['cloud']['minimumTags']
    );
 
    $form['maximumSpeed'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum speed:'),
      '#description' => t('The maximum rotation speed of the cloud.'),
      '#size' => 10,
      '#default_value' => $settings['cloud']['maximumSpeed']
    );

    $form['frontSelect'] = array(
      '#type' => 'checkbox',
      '#title' => t('Front select:'),
      '#description' => t('Check to prevent selection of tags at back of the cloud.'),
      '#default_value' => $settings['cloud']['frontSelect']
    );

    $form['dragControl'] = array(
      '#type' => 'checkbox',
      '#title' => t('Drag control:'),
      '#description' => t('When enabled, cloud moves when dragged instead of based on mouse position.'),
      '#default_value' => $settings['cloud']['dragControl']
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );

   return $form;
}

function LMACGalleryTags_admin_form_submit(&$form, &$form_state) {
   $values = $form_state['values'];

   _LMACGalleryTags_setSettings('cloud', array(
         'minimumTags' => (int)$values['minimumTags'],
         'maximumSpeed' => (float)$values['maximumSpeed'],
         'frontSelect' => (bool)$values['frontSelect'],
         'dragControl' => (bool)$values['dragControl'],
      )
   );

   drupal_set_message(t('All settings successfully saved!'));
}

