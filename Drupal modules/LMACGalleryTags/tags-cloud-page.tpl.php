<div>
   <p><?php print t('All images in the gallery are tagged so that they can be found using the appropriate search terms. The most frequently used tags (keywords) in the image library can be found in this cloud. Click a tag to search for it in the gallery.'); ?></p>
</div>
<div class="tags">
   <canvas id="tagCloudCanvas" width="800" height="300" style="width: 100%">
      <ul>
      <?php foreach(array_keys($tags) as $tag) { ?>
            <li><a data-weight="<?php print $tagWeights[$tag]; ?>" href="/lil-gallery/<?php print $tag; ?>"><?php print $tag; ?> (<?php print $tags[$tag]; ?>)</a></li>
      <?php } ?>
      </ul>
   </canvas>
</div>

<script type="text/javascript">
   jQuery(document).ready(function() {
      if( ! jQuery('#tagCloudCanvas').tagcanvas({
         offsetX: -20,
         offsetY: 20,
         minTags: <?php print $settings['cloud']['minimumTags']; ?>,
         textFont: null,
         splitWidth: 0,
         outlineThickness : 0,
         maxSpeed : <?php print $settings['cloud']['maximumSpeed']; ?>,
         reverse: true,
         depth : 1,
         weight: true,
         initial: [0.01, 0.02],
         padding: 1,
         freezeDecel: true,
         frontSelect: <?php print $settings['cloud']['frontSelect'] ? 'true' : 'false'; ?>,
         weightFrom: 'data-weight',
         weightMode: 'both',
         weightSize: 1,
         weightSizeMin: 10,
         weightSizeMax: 18,
         weightGradient: {
            0.0: '#0000ff',
            0.25: '#850070',
            0.5: '#d60015',
            0.75: '#f63e03',
            1.0: '#fd6600'
         },
         radiusY: 0.5,
         radiusX: 1.25,
         radiusZ: 1,
         dragControl: <?php print $settings['cloud']['dragControl'] ? 'true' : 'false'; ?>,
         dragThreshold: 4,
         bgOutline: null,
         textVAlign: 'middle',
         textAlign: 'centre',
         outlineColour: '#548960',
         outlineMethod: 'colour',
         outlineOffset: 0,
      })) {
         // TagCanvas failed to load
         jQuery('#tags').hide();
      }
      // your other jQuery stuff here...
   });
</script>
