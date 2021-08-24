/*
 * RelationsTable.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2021 Sven Rieper
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

#ifndef SRC_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_
#define SRC_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_

#include <Database.h>
#include <array>
#include <vector>

namespace PhotoLibrary {
namespace DatabaseInterface {

/**
 * Interface to access relations tables for the SQLite database.
 *
 * Used to get, add, and delete relations for many-to-many relationships
 * with integer keys.
 *
 * The naming of the interface assumes that there is some kind of
 * collections-entries relationship.
 *
 * The methods to access a relations table need an
 * std::array<const std::string,3> containing the name of the relations
 * table, and the names of the columns holding the collection id and the
 * entry id (in that order).
 *
 * \todo Allow 64 bis integer ids
 */
class RelationsTable {
public:
	/**
	 * @param db Handle for the database to use
	 */
	RelationsTable(SQLiteAdapter::Database& db) noexcept;

	/**
	 * Get a vector of 'entries' associated with 'collection' id.
	 *
	 * @param collection Id of the 'collection' for which the 'entries'
	 * 		should be returned.
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 * @return Vector of ids of the 'entries' associated to collection
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 */
	std::vector<int> getEntries(int collection, const std::array<const std::string,3>& table) const;

	/**
	 * Get the number of 'entries' associated with 'collection' id.
	 *
	 * @param collection Id of the 'collection' for which the number
	 * 		of 'entries' should be returned.
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 * @return Number of the 'entries' associated with collection
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 * @throws database_error if the database returns an error trying to
	 * 		get the number of entries
	 */
	int getNumberEntries(int collection, const std::array<const std::string,3>& table) const;

	/**
	 * Get a vector of 'collections' associated with 'entry' id.
	 *
	 * @param entry Id of the 'entry' for which the 'collections'
	 * 		containig it should be returned.
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 * @return Vector of ids of the 'collections' associated with entry
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 */
	std::vector<int> getCollections(int entry, const std::array<const std::string,3>& table) const;

	/**
	 * Get the number of 'collections' associated with 'entry' id.
	 *
	 * @param entry Id of the 'entry' for which the number of
	 * 		'collections' containig it should be returned.
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 * @return Number of the 'collections' associated with entry
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 * @throws database_error if the database returns an error trying to
	 * 		get the number of collections
	 */
	int getNumberCollections(int entry, const std::array<const std::string,3>& table) const;

	/**
	 * Add a new relation.
	 *
	 * Adding an existing relation has no effenct.
	 *
	 * @param entry Id of the 'entry'
	 * @param collection Id of the 'collection'
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 * @throws constraint_error if adding the new relation fails due to
	 * 		a constraint violation
	 * @throws database_error if any error occurs in the database
	 */
	void newRelation(int entry, int collection, const std::array<const std::string,3>& table);

	/**
	 * Remove a relation.
	 *
	 * Deleting a non-existing relation has no effect.
	 *
	 * @param entry Id of the 'entry'
	 * @param collection Id of the 'collection'
	 * @param table Name of the relations table and the columns (see
	 * 		RelationsTable class' description)
	 *
	 * @throws std::runtime_error if doesn't hold the correct strings
	 * 		for a table
	 * @throws database_error if any error occurs in the database
	 */
	void deleteRelation(int entry, int collection, const std::array<const std::string,3>& table);

private:
	SQLiteAdapter::Database& db;

	std::vector<int> getVector(
			int id,
			const std::string& reference_id,
			const std::string& return_id,
			const std::string& table
			) const;
	int getNumber(
			int id,
			const std::string& reference_id,
			const std::string& return_id, 
			const std::string& table
			) const;
};

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* SRC_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_ */
