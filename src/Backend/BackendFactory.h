/*
 * BackendFactory.h
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

#ifndef SRC_BACKEND_BACKENDFACTORY_H_
#define SRC_BACKEND_BACKENDFACTORY_H_

#include <AccessTables.h>
#include <RelationsTable.h>
#include <glibmm/ustring.h>
#include <unordered_map>
#include <memory>

namespace PhotoLibrary {
namespace Backend {

/**
 * Interface to the backend.
 */
class BackendFactory {
public:
	/**
	 * Properties of the main window.
	 * Properties of the main window that can be loaded or saved with
	 * getWindowProperty(WindowProperties) and setWindowProperty(WindowProperties, int).
	 */
	enum class WindowProperties {
		WINDOW_WIDTH,	/**< Width of the main window. */
		WINDOW_HEIGHT,	/**< Height of the main window. */
		LEFT_PANE_WIDTH,	/**< Width of the left pane. */
		RIGHT_PANE_WIDTH,	/**< Width of the right pane. */
		TILE_WIDTH,	/**< The width of a tile in the centre frame */
//		TILE_HEIGHT,	/**< The height of a tile in the centre frame */
		N_THREADS,	/**< Number of threads to use */
	};

	/**
	 * Template parameters for the relations tables
	 */
	enum class Relations {
		PHOTOS_ALBUMS=0,    /**< Photos albums relations */
		PHOTOS_KEYWORDS=1   /**< Photos keywords relations */
	};

	/**
	 * @param filename Filename and path of the database to use
	 */
	BackendFactory(const char* filename = nullptr);

	/**
	 * Retrieve a record.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param id Id of the record to return
	 * @return RecordType containing the values of the row with row id 'id'
	 *
	 * @throws missing_entry if no entry with id 'id' was found.
	 */
	template<typename RecordType>
	RecordType getEntry(int id);

	/**
	 * Get the children of a entry.
	 * Returns the ids of all children of parent id 'parent'.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param parent id of the entry of which the children should be returned
	 * @return vector of id's of the children of 'parent'
	 */
	template<typename RecordType>
	std::vector<int> getChildren(int parent);

	/**
	 * Get the number children of a entry.
	 * Returns the number of children of parent id 'parent'.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param parent id of the entry of which the number children should be
	 * 		returned
	 * @return Number of children of 'parent'
	 */
	template<typename RecordType>
	int getNumberChildren(int parent);

	/**
	 * Add new record.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param entry data to be inserted for new record
	 *
	 * @throws constraint_error If insertion into the database fails due to
	 * 		constraint violation
	 * @throws database_error If any other error occurs during insertion
	 */
	template<typename RecordType>
	void newEntry(const RecordType& entry);

	/**
	 * Updates a record.
	 *
	 * If no entry with an ID 'id' exists this function doesn't do anything.
	 *
	 * Doesn't check for loops; it's up to the user not to fiddle-faddle with it.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 *
	 * @throws constraint_error If the update failed due to constraint violation
	 * @throws database_error If any other error occurs during update
	 */
	template<typename RecordType>
	void updateEntry(int id, const RecordType& entry);

	/**
	 * Moves a record to a new parent.
	 *
	 * Doesn't check for loops; it's up to the user not to fiddle-faddle with it.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param child_id Id of the record to update
	 * @param new_parent_id New parent id of record 'id'
	 *
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @throws database_error If any other error occurs during updating
	 * 		the database
	 */
	template<typename RecordType>
	void setParent(int child_id, int new_parent_id);

	/**
	 * Get the id to an entry.
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param entry the record for which to return the id
	 *
	 * @throws missing_entry If the entry wasn't found (or any other error
	 * 		occurs in the database)
	 */
	template<typename RecordType>
	int getID(const RecordType& entry);

	/**
	 * Delete record from table.
	 *
	 * Deleting an invalid ID has no effect.
	 *
	 *
	 * @tparam RecordType Record based class for the table (see
	 * 		AccessTables' class documentation for more information)
	 * @param id Id of the record to delete
	 *
	 * @throws database_error If the database returns an error.
	 * @throws std::runtime_error If id == 0
	 */
	template<typename RecordType>
	void deleteEntry(int id);

	/**
	 * Get a vector of photos in a collection.
	 *
	 * @tparam relation Enumerator for the relations table
	 * @param collection Id of the 'collection' (e.g. keyword or album)
	 * 		for which the photos should be returned.
	 * @return Vector of ids of all photos in the collection
	 */
	template<Relations relation>
	std::vector<int> getEntries(int collection);

	/**
	 * Get the number of photos in a 'collection'.
	 *
	 * @tparam relation Enumerator for the relations table
	 * @param collection Id of the 'collection' (e.g. keyword or album)
	 * 		for which the number of photos should be returned.
	 * @return Number of photos in the collection
	 */
	template<Relations relation>
	int getNumberEntries(int collection);

	/**
	 * Get a vector of 'collections' containig a photo
	 *
	 * @tparam relation Enumerator for the relations table
	 * @photo Id of the photo for which the 'collections'
	 * 		(e.g. keyword or album) containig it should be returned.
	 * @return Vector of ids of all 'collections' containing photo
	 */
	template<Relations relation>
	std::vector<int> getCollections(int photo);

	/**
	 * Get the number of 'collections' containing a photo
	 *
	 * @tparam relation Enumerator for the relations table
	 * @photo Id of the photo for which the number of 'collections'
	 * 		(e.g. keyword or album) containig it should be returned.
	 * @return Number of 'collections' containing photo
	 */
	template<Relations relation>
	int getNumberCollections(int photo);

	/**
	 * Add a new relation.
	 *
	 * Adding an existing relation has no effenct.
	 *
	 * @tparam relation Enumerator for the relations table
	 * @param photo Id of the photo
	 * @param collection Id of the 'collection' (e.g. keyword or album)
	 *
	 * @throws constraint_error if adding the new relation fails due to
	 * 		a constraint violation
	 * @throws database_error if any error occurs in the database
	 */
	template<Relations relation>
	void newRelation(int photo, int collection);

	/**
	 * Remove a relation.
	 *
	 * Deleting a non-existing relation has no effect.
	 *
	 * @tparam relation Enumerator for the relations table
	 * @param photo Id of the photo
	 * @param collection Id of the 'collection' (e.g. keyword or album)
	 *
	 * @throws database_error if any error occurs in the database
	 */
	template<Relations relation>
	void deleteRelation(int photo, int collection);

	/**
	 * Retrieve the value of a main window property.
	 *
	 * @param property Property for which the value should be returned
	 * @return Value of 'property'
	 */
	int getWindowProperty(WindowProperties property) const;

	/**
	 * Save the value of a main window property.
	 *
	 * @param property Property for which the value should be saved
	 * @param value New value of 'property'
	 */
	void setWindowProperty(WindowProperties property, int value);

	/**
	 * Get the width of the central pane.
	 *
	 * @return Width of the central pane
	 */
	int getCentreWidth() const;

private:
	std::unique_ptr<SQLiteAdapter::Database> db;
	std::unique_ptr<PhotoLibrary::DatabaseInterface::AccessTables<Glib::ustring>> tables_interface;
	std::unique_ptr<PhotoLibrary::DatabaseInterface::RelationsTable> relations_interface;
	std::unordered_map<WindowProperties,int> window_properties;
	static inline const std::array<const std::array<const std::string,3>,2> relations_tables {
		std::array<const std::string,3>{"PhotosAlbumsRelations", "albumId", "photoId"},
		{"PhotosKeywordsRelations", "keywordId", "photoId"}
	};

	void createTables();

	//prevent copying and copy construction
	BackendFactory(const BackendFactory &other) = delete;
	BackendFactory(BackendFactory &&other) = delete;
	BackendFactory& operator=(const BackendFactory &other) = delete;
	BackendFactory& operator=(BackendFactory &&other) = delete;
};

template<typename RecordType>
RecordType BackendFactory::getEntry(int id) {
	return tables_interface->getEntry<RecordType>(id);
}

template<typename RecordType>
std::vector<int> BackendFactory::getChildren(int parent) {
	return tables_interface->getChildren<RecordType>(parent);
}

template<typename RecordType>
int BackendFactory::getNumberChildren(int parent) {
	return tables_interface->getNumberChildren<RecordType>(parent);
}

template<typename RecordType>
void BackendFactory::newEntry(const RecordType& entry) {
	return tables_interface->newEntry<RecordType>(entry);
}

template<typename RecordType>
void BackendFactory::updateEntry(int id, const RecordType& entry) {
	return tables_interface->updateEntry<RecordType>(id, entry);
}

template<typename RecordType>
void BackendFactory::setParent(int child_id, int new_parent_id) {
	return tables_interface->setParent<RecordType>(child_id, new_parent_id);
}

template<typename RecordType>
int BackendFactory::getID(const RecordType& entry) {
	return tables_interface->getID<RecordType>(entry);
}

template<typename RecordType>
void BackendFactory::deleteEntry(int id) {
	return tables_interface->deleteEntry<RecordType>(id);
}

template<BackendFactory::Relations relation>
std::vector<int> BackendFactory::getEntries(int collection) {
	return relations_interface->getEntries(
			collection,
			relations_tables[static_cast<int>(relation)]
			);
}

template<BackendFactory::Relations relation>
int BackendFactory::getNumberEntries(int collection) {
	return relations_interface->getNumberEntries(
			collection,
			relations_tables[static_cast<int>(relation)]
			);
}

template<BackendFactory::Relations relation>
std::vector<int> BackendFactory::getCollections(int entry) {
	return relations_interface->getCollections(
			entry,
			relations_tables[static_cast<int>(relation)]
			);
}

template<BackendFactory::Relations relation>
int BackendFactory::getNumberCollections(int entry) {
	return relations_interface->getNumberCollections(
			entry,
			relations_tables[static_cast<int>(relation)]
			);
}

template<BackendFactory::Relations relation>
void BackendFactory::newRelation(int entry, int collection) {
	return relations_interface->newRelation(
			entry,
			collection,
			relations_tables[static_cast<int>(relation)]
			);
}

template<BackendFactory::Relations relation>
void BackendFactory::deleteRelation(int entry, int collection) {
	return relations_interface->deleteRelation(
			entry,
			collection,
			relations_tables[static_cast<int>(relation)]
			);
}

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_BACKENDFACTORY_H_ */
