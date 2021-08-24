/*
 * BackendFactory.cpp
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

#include "BackendFactory.h"
#include "Database.h"
#include <thread>

namespace PhotoLibrary {
namespace Backend {

BackendFactory::BackendFactory(const char* filename) : db(nullptr) {
	/// \todo load values
	window_properties[WindowProperties::WINDOW_WIDTH] = 1800;
	window_properties[WindowProperties::WINDOW_HEIGHT] = 1200;
	window_properties[WindowProperties::LEFT_PANE_WIDTH] = 250;
	window_properties[WindowProperties::RIGHT_PANE_WIDTH] = 250;
	window_properties[WindowProperties::TILE_WIDTH] = 250;
//	window_properties[WindowProperties::TILE_HEIGHT] = 250;
	window_properties[WindowProperties::N_THREADS] =
		std::thread::hardware_concurrency()?std::thread::hardware_concurrency():1;

	db                  = std::make_unique<SQLiteAdapter::Database>(":memory:", true);
	tables_interface    = std::make_unique<PhotoLibrary::DatabaseInterface::AccessTables<Glib::ustring>>(*db);
	relations_interface = std::make_unique<PhotoLibrary::DatabaseInterface::RelationsTable>(*db);

	bool initialise {true};
	if(initialise)
		createTables();
	else
		;
}

int BackendFactory::getWindowProperty(WindowProperties property) const {
	return window_properties.at(property);
}

void BackendFactory::setWindowProperty(WindowProperties property, int value) {
	/// \todo save value
	window_properties[property] = value;
}

int BackendFactory::getCentreWidth() const {
	return window_properties.at(WindowProperties::WINDOW_WIDTH) -
			window_properties.at(WindowProperties::RIGHT_PANE_WIDTH) -
			window_properties.at(WindowProperties::RIGHT_PANE_WIDTH) - 4;
}

void BackendFactory::createTables() {
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
	if(db->querryNoThrow(tables, nullptr, nullptr, error_msg))
		throw(std::runtime_error("Error creating tables: " + error_msg));
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
