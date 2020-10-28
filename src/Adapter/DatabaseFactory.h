/*
 * DatabaseFactory.h
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

#ifndef SRC_ADAPTER_DATABASEFACTORY_H_
#define SRC_ADAPTER_DATABASEFACTORY_H_

#include "Database.h"
#include "DBInterface.h"
#include "../Backend/Record/DirectoryRecord.h"
#include "../Backend/Record/AlbumRecord.h"
#include "../Backend/Record/KeywordRecord.h"

namespace PhotoLibrary {
namespace Adapter {

/**
 * Factory class to create the database handle and interface.
 */
class DatabaseFactory {
public:
	/**
	 * @param filename filname of the database
	 * @param initialise create new database or open existing?
	 * 		If true filename must not exist.
	 */
	DatabaseFactory(const char* filename, bool initialise=false);
	~DatabaseFactory() = default;

	/**
	 * Get a pointer to the Keyword interface.
	 * @return pointer to the keyword interface
	 */
	DBInterface<Backend::KeywordRecord>* getKeywordInterface();

	/**
	 * Get a pointer to the directories interface.
	 * @return pointer to the directories interface
	 */
	DBInterface<Backend::DirectoryRecord>* getDirectoriesInterface();

	/**
	 * Get a pointer to the album interface.
	 * @return pointer to the album interface
	 */
	DBInterface<Backend::AlbumRecord>* getAlbumInterface();

	//disable copying and copy construction
	DatabaseFactory(const DatabaseFactory &other) = delete;
	DatabaseFactory& operator=(const DatabaseFactory &other) = delete;
	DatabaseFactory(DatabaseFactory &&other) = delete;
	DatabaseFactory& operator=(DatabaseFactory &&other) = delete;

private:
	Database db;
	DBInterface<Backend::KeywordRecord> keyword_interface;
	DBInterface<Backend::DirectoryRecord> directories_interface;
	DBInterface<Backend::AlbumRecord> album_interface;
};

} /* namespace Adapter */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_DATABASEFACTORY_H_ */
