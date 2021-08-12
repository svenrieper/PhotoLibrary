/*
 * InterfaceBase.h
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

#ifndef SRC_BACKEND_INTERFACEBASE_H_
#define SRC_BACKEND_INTERFACEBASE_H_

#include <vector>

namespace PhotoLibrary {
namespace Backend {

/**
 * Abstract base class for the backend interface.
 *
 * @tparam RType Backend::Record based class used to retrieve,
 * 		save, and update records in the database
 */
template<class RType>
class InterfaceBase {
public:
	using RecordType = RType;

	/**
	 * Retrieve a record.
	 *
	 * @throws DatabaseInterface::missing_entry if no entry with id 'id' was found.
	 * @param id Id of the record to return
	 * @return complete information for record with id 'id'
	 */
	virtual RecordType getEntry(int id) const = 0;

	/**
	 * Get the children of a entry.
	 * Returns the ids of all entries where the column RType::getField(0)
	 * equals the value handed to the method.
	 *
	 * @param parent id of the entry of which the children should be returned
	 * @return vector of id's of the children of 'parent'
	 */
	virtual std::vector<int> getChildren(int parent) const = 0;

	/**
	 * Get the number children of a entry.
	 * Returns the number of entries where the column RType::getField(0)
	 * equals the value handed to the method.
	 *
	 * @param Parent id of the entry of which the number children should be returned
	 * @return Number of children of 'parent'
	 */
	virtual int getNumberChildren(int parent) const = 0;

	/**
	 * Add new record.
	 *
	 * @param entry data to be inserted for new record
	 * @return id of the new entry
	 * @throws DatabaseInterface::constraint_error If insertion into the database fails
	 * 		due to constraint violation
	 * @throws DatabaseInterface::database_error If any other error occurs during insertion
	 */
	virtual void newEntry(const RecordType& entry) = 0;

	/**
	 * Updates a record.
	 *
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 * @throws DatabaseInterface::constraint_error If the update failed due to
	 * 		constraint violation
	 * @throws DatabaseInterface::database_error If any other error occurs during update
	 */
	virtual void updateEntry(int id, const RecordType &entry) = 0;

	/**
	 * Moves a record to a new parent.
	 *
	 * @param child_id Id of the record to update
	 * @param new_parent_id New parent id of record 'id'
	 * @throws DatabaseInterface::constraint_error Thrown if parent 'id' does not exist.
	 * @throws DatabaseInterface::database_error If any other error occurs during updating
	 * 		the database
	 */
	virtual void setParent(int child_id, int new_parent_id) = 0;

	/**
	 * Get the id to an entry.
	 *
	 * @param entry the record for which to return the id
	 * @return the id of the record 'entry'
	 * @throws DatabaseInterface::missing_entry If the entry wasn't found
	 * 		(or any other error occurs in the database)
	 */
	virtual int getID(const RType& entry) const = 0;

	/**
	 * Delete record from table.
	 * Deletes a record, all its children, and all information
	 * that depends on it.
	 *
	 * @param id Id of the record to delete
	 * @throws std::runtime_error If id == 0
	 * @throws DatabaseInterface::missing_entry If the entry wasn't found
	 * 		(or any other error occurs in the database)
	 */
	virtual void deleteEntry(int id) = 0;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_INTERFACEBASE_H_ */
