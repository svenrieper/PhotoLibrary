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
		album_interface(&db, "Albums") {
}

DBInterface<Backend::KeywordRecord>* DatabaseFactory::getKeywordInterface() {
	return &keyword_interface;
}

DBInterface<Backend::DirectoryRecord>* DatabaseFactory::getDirectoriesInterface() {
	return &directories_interface;
}

DBInterface<Backend::AlbumRecord>* DatabaseFactory::getAlbumInterface() {
	return &album_interface;
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
