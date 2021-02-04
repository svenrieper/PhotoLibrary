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
#include "../Backend/FactoryBase.h"
#include "RelationsDBInterface.h"

namespace PhotoLibrary {
namespace Adapter {

/**
 * Factory class to create the database handle and interface.
 */
class DatabaseFactory : public Backend::FactoryBase {
public:
	/**
	 * @param filename filname of the database
	 * @param initialise create new database or open existing?
	 * 		If true filename must not exist.
	 */
	DatabaseFactory(const char* filename, bool initialise=false);
	~DatabaseFactory() = default;

	DBInterface<Backend::KeywordRecord>* getKeywordInterface() override;
	const DBInterface<Backend::KeywordRecord>* getKeywordInterface() const override;
	DBInterface<Backend::DirectoryRecord>* getDirectoriesInterface() override;
	const DBInterface<Backend::DirectoryRecord>* getDirectoriesInterface() const override;
	DBInterface<Backend::AlbumRecord>* getAlbumInterface() override;
	const DBInterface<Backend::AlbumRecord>* getAlbumInterface() const override;
	DBInterface<Backend::PhotoRecord>* getPhotoInterface() override;
	const DBInterface<Backend::PhotoRecord>* getPhotoInterface() const override;
	RelationsDBInterface* getPhotosAlbumsRelationsInterface() override;
	const RelationsDBInterface* getPhotosAlbumsRelationsInterface() const override;

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
	DBInterface<Backend::PhotoRecord> photo_interface;
	RelationsDBInterface photos_albums_relations;
};

} /* namespace Adapter */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_DATABASEFACTORY_H_ */
