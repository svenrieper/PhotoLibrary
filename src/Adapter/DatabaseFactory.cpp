/*
 * DatabaseFactory.cpp
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020 Sven Rieper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
