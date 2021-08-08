/*
 * DatabaseFactory.cpp
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020-2021 Sven Rieper
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
namespace Backend {
namespace DatabaseInterface {

DatabaseFactory::DatabaseFactory(const char* filename, bool initialise) :
		db(/*filename*/ ":memory:", /*initialise*/ true),
		keyword_interface(db, "Keywords"),
		directories_interface(db, "Directories"),
		album_interface(db, "Albums"),
		photo_interface(db, "Photos"),
		photos_albums_relations(&db, "PhotosAlbumsRelations", "photoId", "albumId"),
		photos_keywords_relations(&db, "PhotosKeywordsRelations", "photoId", "keywordId") {
	initialise=true;
	if (!initialise);
		//testConsistency(); /// \todo Add test for db
	else
		createTables();
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

RelationsDBInterface* DatabaseFactory::getPhotosAlbumsRelationsInterface() {
	return &photos_albums_relations;
}

const RelationsDBInterface* DatabaseFactory::getPhotosAlbumsRelationsInterface() const {
	return &photos_albums_relations;
}

RelationsDBInterface* DatabaseFactory::getPhotosKeywordsRelationsInterface() {
	return &photos_keywords_relations;
}

const RelationsDBInterface* DatabaseFactory::getPhotosKeywordsRelationsInterface() const {
	return &photos_keywords_relations;
}

void DatabaseFactory::createTables() {
	/// \todo Add database structure.
	const char* tables =
			"PRAGMA foreign_keys = ON;"
		//Keywords table
			"CREATE TABLE Keywords("
			"  id				INTEGER	PRIMARY KEY AUTOINCREMENT"	//AUTOINCREMENT?
			", parent			INTEGER	NOT NULL"
			", keyword	TEXT	NOT NULL"
			", lkeyword	TEXT	AS (lower(keyword))"	//VIRTUAL or STORED?
			", synonyms	TEXT	DEFAULT ''"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, lkeyword)"
			", FOREIGN KEY		(parent) REFERENCES Keywords ON DELETE CASCADE"
			");"
			//Create index for parent
			"CREATE INDEX keyParentIndex ON Keywords(parent);"
			"INSERT INTO Keywords (id, keyword, parent) VALUES (1, 'internal root', 1);"
			"INSERT INTO Keywords (id, keyword, parent) VALUES (0, 'root', 0);"
			//Reserve keyword ids 1 through 50 for internal use
			"INSERT INTO Keywords (id, keyword, parent) VALUES (50, 'temp', 0);"
			"DELETE FROM Keywords WHERE id = 50;"
		//Directories table
			"CREATE TABLE Directories("
			"  id				INTEGER	PRIMARY KEY"	//AUTOINCREMENT?
			", parent			INTEGER	NOT NULL"
			", name				TEXT	NOT NULL"
			", fullname			TEXT	NOT NULL"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, fullname)"
			", FOREIGN KEY		(parent) REFERENCES Directories ON DELETE CASCADE"
			");"
			//Create index for parent
			"CREATE INDEX dirParentIndex ON Directories(parent);"
			"INSERT INTO Directories (id, name, fullname, parent) VALUES (0, 'root', '/', 0);"
		//Albums table
			"CREATE TABLE Albums("
			"  id				INTEGER	PRIMARY KEY"
			", parent			INTEGER	NOT NULL"
			", name				TEXT	NOT NULL"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, name)"
			", FOREIGN KEY		(parent) REFERENCES Albums ON DELETE CASCADE"
			");"
			"CREATE INDEX albumsParentIndex ON Albums(parent);"
			"INSERT INTO Albums (id, name, parent) VALUES (0, 'root', 0);"
		//Photos table
			"CREATE TABLE Photos("
			"  id				INTEGER	PRIMARY KEY"
			", directory		INTEGER	NOT NULL"
			", filename			TEXT	NOT NULL"
			", rating			INTEGER	DEFAULT 0"
			", datetime			INTEGER"
			", width			INTEGER"
			", height			INTEGER"
			/// \todo add other attributes
			//Constraints
			", UNIQUE			(directory, filename)"
			", FOREIGN KEY		(directory) REFERENCES Directories ON DELETE CASCADE"
			");"
			//Create index for directory
			"CREATE INDEX photosDirIndex ON Photos(directory);"
		//Photos-Albums relations table
			"CREATE TABLE PhotosAlbumsRelations("
			"  photoId			INTEGER"
			", albumId			INTEGER"
			//Constraints
			", UNIQUE			(photoId, albumId)"
			", FOREIGN KEY		(photoId) REFERENCES Photos ON DELETE CASCADE"
			", FOREIGN KEY		(albumId) REFERENCES Albums ON DELETE CASCADE"
			");"
			//Create indeces for id and albumId
			"CREATE INDEX photosAlbumsRelationsIdIndex ON PhotosAlbumsRelations(photoId);"
			"CREATE INDEX photosAlbumsRelationsAlbumIdIndex ON PhotosAlbumsRelations(albumId);"
		//Photos-Albums relations table
			"CREATE TABLE PhotosKeywordsRelations("
			"  photoId			INTEGER"
			", keywordId		INTEGER"
			//Constraints
			", UNIQUE			(photoId, keywordId)"
			", FOREIGN KEY		(photoId) REFERENCES Photos ON DELETE CASCADE"
			", FOREIGN KEY		(keywordId) REFERENCES Keywords ON DELETE CASCADE"
			");"
			//Create indeces for id and albumId
			"CREATE INDEX photosKeywordsRelationsIdIndex ON PhotosKeywordsRelations(photoId);"
			"CREATE INDEX photosKeywordsRelationsKeywordIdIndex ON PhotosKeywordsRelations(keywordId);"
			;

	std::string error_msg;
	if(db.querryNoThrow(tables, nullptr, nullptr, error_msg))
		throw(std::runtime_error("Error creating tables: " + error_msg));
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
