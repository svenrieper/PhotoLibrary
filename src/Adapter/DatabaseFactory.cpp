/*
 * DatabaseFactory.cpp
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
 */

#include "DatabaseFactory.h"

namespace PhotoLibrary {
namespace Adapter {

DatabaseFactory::DatabaseFactory(const char* filename, bool initialise) :
		db(filename, initialise),
		keyword_interface(&db, "Keywords"),
		directories_interface(&db, "Directories"),
		album_interface(&db, "Albums"),
		photo_interface(&db, "Photos") {
}

DBInterface<Backend::KeywordRecord>* DatabaseFactory::getKeywordInterface() {
	return &keyword_interface;
}

const DBInterface<Backend::KeywordRecord>* DatabaseFactory::getKeywordInterface() const {
	return &keyword_interface;
}

DBInterface<Backend::DirectoryRecord>* DatabaseFactory::getDirectoriesInterface() {
	return &directories_interface;
}

const DBInterface<Backend::DirectoryRecord>* DatabaseFactory::getDirectoriesInterface() const {
	return &directories_interface;
}

DBInterface<Backend::AlbumRecord>* DatabaseFactory::getAlbumInterface() {
	return &album_interface;
}

const DBInterface<Backend::AlbumRecord>* DatabaseFactory::getAlbumInterface() const {
	return &album_interface;
}

DBInterface<Backend::PhotoRecord>* DatabaseFactory::getPhotoInterface() {
	return &photo_interface;
}

const DBInterface<Backend::PhotoRecord>* DatabaseFactory::getPhotoInterface() const {
	return &photo_interface;
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
