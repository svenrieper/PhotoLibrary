/*
 * InterfaceBase.h
 *
 *  Created on: 19 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_BACKEND_INTERFACEBASE_H_
#define SRC_BACKEND_INTERFACEBASE_H_

#include <vector>

namespace PhotoLibrary {
namespace Backend {

/**
 * Abstract base class for the backend interface.
 *
 * @param RType Backend::Record based class used to retrieve,
 * 		save, and update records in the database
 */
template<class RType>
class InterfaceBase {
public:
	using RecordType = RType;

	InterfaceBase() = default;

	virtual ~InterfaceBase() = default;

	/**
	 * Retrieve a record.
	 *
	 * @param id Id of the record to return
	 * @return complete information for record with id 'id'
	 */
	virtual RecordType getEntry(int id) const = 0;

	/**
	 * Get the children of a entry.
	 *
	 * @param parent id of the entry of which the children should be returned
	 * @return vector of id's of the children of 'parent'
	 */
	virtual std::vector<int> getChildren(int parent) const = 0;

	/**
	 * Add new record.
	 *
	 * @param entry data to be inserted for new record
	 * @return id of the new entry
	 */
	virtual void newEntry(const RecordType& entry) = 0;

	/**
	 * Updates a record.
	 *
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 */
	virtual void updateEntry(int id, const RecordType &entry) = 0;

	/**
	 * Moves a record to a new parent.
	 *
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @param id Id of the record to update
	 * @param newParent New parent id of record 'id'
	 */
	virtual void setParent(int child_id, int new_parent_id) = 0;

	/**
	 * Get the id to an entry.
	 * @throws throws std::runtime_error if the entry wasn't found
	 *
	 * @param entry the record for which to return the id
	 * @return the id of the record 'entry'
	 */
	virtual int getID(const RType& entry) const = 0;

	/**
	 * Delete record from table.
	 * Deletes a record, all its children, and all information
	 * that depends on it.
	 *
	 * @param id Id of the record to delete
	 */
	virtual void deleteEntry(int id) = 0;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_INTERFACEBASE_H_ */
