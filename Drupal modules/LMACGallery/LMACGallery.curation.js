jQuery(document).ready(function () { 
	jQuery('#edit-cancel-send-post').click(function() {
		window.location.href = '/admin/lmac/lmac_ratings';
		return false;
	});
	jQuery('#edit-send-post').click(function() {

		hive_keychain.requestHandshake(function () {
			console.log("Handshake received!");
			
 

			//console.debug(JSON.stringify(Drupal.settings.hivePost.customJson));
			//console.debug(JSON.stringify(extensionsArray));
			
			hive_keychain.requestPost(
				Drupal.settings.hivePost.sender,
				Drupal.settings.hivePost.title,
				Drupal.settings.hivePost.postMarkdown,
				'hive-174695',
				'',
				JSON.stringify(Drupal.settings.hivePost.customJson),
				Drupal.settings.hivePost.permlink,
				JSON.stringify({ /* Comment options */
						author: Drupal.settings.hivePost.sender,
						permlink: Drupal.settings.hivePost.permlink,
						max_accepted_payout: '1000000.000 HBD',
						percent_hbd: 10000,
						allow_votes: true,
						allow_curration_rewards: true,
						extensions: [
						[
								0,
								{
									beneficiaries: Drupal.settings.hivePost.beneficiaries,
								}						
							]
						]
					}
				),
				function(response) {
					
					if (response.success) {
						
					}else{
		 
					}
				  console.log("main js response - post");
				  console.log(JSON.stringify(response));
				},
				'https://testnet.openhive.network'
			);
		});
		return false;
	});
});