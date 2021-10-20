/*
 * simpleGallery v0.9.4
 * https://github.com/w3tl/simpleGallery
 *
 * Copyright 2013 Vitaly Dostovalov (dostovalovvs@gmail.com)
 * Released under the MIT license.
 *
 * Date: 2013.12.23
 */
(function ($) {
    $.fn.gallery = function () {
        return this.each(function() {
            var gallery = $(this),
                curr_image;
				all_images = $(".gallery img");
            
            gallery.prepend(
                '<div class="view"><span class="prev control"></span><span class="close control"></span><span class="next control"></span><img><div class="load"></div></div>'
            );

            var view = $(".view", gallery),
                view_image = view.children("img"),
                loading = $(".load", view);

            
            $(document).keyup(function(event){
                if (event.keyCode == 27) {
                    view.hide();
                    view_image.attr('src', '');
                }
            });
            function next() {
				var imageIndex = all_images.index(curr_image);
                return all_images.length + 1 > imageIndex ? all_images.eq(imageIndex + 1) : all_images.eq(0); 
            }
            function prev() {
				var imageIndex = all_images.index(curr_image);
                return imageIndex > 0 ? all_images.eq(imageIndex - 1) : all_images.eq(all_images.length - 1); 
            }
        
            function viewImage(image) {
                curr_image = image.data("full") ? image : next();
                full_image = curr_image.data("full");
                loading.show();
                view_image.attr('src', '').hide().attr('src', full_image).load(function() {
                    loading.hide();
                    view_image.show();
                });
                view_image.error(function() {
                    loading.hide();
                });
            }
        
            gallery.click(function(e) {
                var clicked = $(e.target)[0].tagName === "IMG" ? $(e.target) : null ;
                if ( clicked ) {
                    viewImage(clicked); 
                    view.show();
                }
            });
        
            $( ".close", view ).click(function () { 
                view.hide();
                view_image.attr('src', '');
            });
            $( ".prev", view ).click(function () {
                curr_image = prev();
                viewImage(curr_image);
            });
            $( ".next", view ).click(function () {
                curr_image = next();
                viewImage(curr_image);
            });
        });
    }        
})(jQuery);
