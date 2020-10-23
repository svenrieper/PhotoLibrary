/*
 * DatabaseFactory.h
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
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
