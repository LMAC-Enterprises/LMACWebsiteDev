<?php

function _LMACGallery_statistics_getAmountOfImages() {

	return  db_query("SELECT COUNT(imageid) FROM {images}")->fetchField();
}

function _LMACGallery_statistics_getAmountOfRatedImages() {

	return db_query("SELECT COUNT(DISTINCT(ratingid)) FROM {lmacg_ratings}")->fetchField();
}

function _LMACGallery_statistics_getAmountOfAuthors() {

	return db_query("SELECT COUNT(DISTINCT(author)) FROM {images}")->fetchField();
}