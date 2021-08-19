/*
 * AccessTables.h
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

#ifndef SRC_ACCESSTABLES_H_
#define SRC_ACCESSTABLES_H_

#include "support.h"
#include <Database.h>
#include <Concepts.h>
#include <vector>

namespace PhotoLibrary {
namespace DatabaseInterface {

using SQLiteAdapter::String_type;

/**
 * Class to access tables in a database.
 *
 * This class only works on tables containing a UNIQUE integral
 * column named 'id'.
 *
 * The template parameter RecordType of the methods to access
 * tables needs to be a class derived from Record.
 * The derived class needs a static string field named 'table'
 * containig the table name and a static array of strings named
 * 'fields' containing the names of the of the columns.
 * Both need to be accessible by AccessTables.
 * Record's template parameter 'Tuple' needs to hold the types
 * of the columns in the same order as 'fields'.
 *
 * Supported data types: integral types, enum|s, and String_type|s.
 * The String_type|s used by RecordType (in the Tuple parameter as
 * well as for 'table' and 'fields') should be the same as the one
 * used as a template parameter 'String' for AccessTables.
 *
 * The methods refering to children or parents (i.e. getChildern,
 * getNumberChildren, and setParent) assume the the column
 * RecordType::fields[0] refers to an integral parent of an entry.
 *
 * Currently only ids of type 'int' are supported by this class
 * (for the id of an entry as well as the id of its parent).
 *
 * \todo add support for enum class|es
 * \todo add support for 64 bit integer ids
 * \todo more consistent behaviour regarding id == 0
 * \todo make non-modifying methods const
 *
 * @tparam String String_type to use internally (see above for
 * 		significance)
 */
template<String_type String=std::string>
class AccessTables {
public:
	/**
	 * @param db Handle for the database to use
	 */
	AccessTables(SQLiteAdapter::Database& db) noexcept : db(db) {}

	/**
	 * Retrieve a record.
	 *
	 * @param id Id of the record to return
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @return RecordType containing the values of the row with row id 'id'
	 * @throws missing_entry if no entry with id 'id' was found.
	 */
	template<typename RecordType>
	RecordType getEntry(int id) const;

	/**
	 * Get the children of a entry.
	 * Returns the ids of all entries where the column RecordType::fileds[0]
	 * equals the value handed to the method.
	 *
	 * @param parent id of the entry of which the children should be returned
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @return vector of id's of the children of 'parent'
	 */
	template<typename RecordType>
	std::vector<int> getChildren(int parent) const;

	/**
	 * Get the number children of a entry.
	 * Returns the number of entries where the column RecordType::fileds[0]
	 * equals the value handed to the method.
	 *
	 * @param parent id of the entry of which the number children should be
	 * 		returned
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @return Number of children of 'parent'
	 */
	template<typename RecordType>
	int getNumberChildren(int parent) const;

	/**
	 * Add new record.
	 *
	 * @param entry data to be inserted for new record
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
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
	 * Doesn't check that the entry itself or one of its children
	 * isn't set as parent; it's up to the user not to fiddle-faddle with it.
	 *
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @throws constraint_error If the update failed due to constraint violation
	 * @throws database_error If any other error occurs during update
	 */
	template<typename RecordType>
	void updateEntry(int id, const RecordType &entry);

	/**
	 * Moves a record to a new parent.
	 *
	 * Doesn't check that the record itself or one of its children
	 * isn't set as parent; it's up to the user not to fiddle-faddle with it.
	 *
	 * @param child_id Id of the record to update
	 * @param new_parent_id New parent id of record 'id'
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @throws database_error If any other error occurs during updating
	 * 		the database
	 */
	template<typename RecordType>
	void setParent(int child_id, int new_parent_id);

	/**
	 * Get the id to an entry.
	 *
	 * @param entry the record for which to return the id
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @throws missing_entry If the entry wasn't found (or any other error
	 * 		occurs in the database)
	 */
	template<typename RecordType>
	int getID(const RecordType& entry) const;

	/**
	 * Delete record from table.
	 *
	 * If according constraints are defined all children and other entries relying
	 * on the deleted entry are deleted recursively as well.
	 *
	 * Deleting an invalid ID has no effect.
	 *
	 * @param id Id of the record to delete
	 * @tparam RecordType Record based class for the table (see AccessTables'
	 * 		class documentation for more information)
	 * @throws database_error If the database returns an error.
	 * @throws std::runtime_error If id == 0
	 * \todo Is it correct to forbid deleting the entry with id == 0?
	 */
	template<typename RecordType>
	void deleteEntry(int id);

private:
	SQLiteAdapter::Database& db;	/**< Database hande used */
};


/**
 * Template loop used by AccessTables::getEntry
 */
template<int I, typename RecordType>
void getEntryLoop(SQLiteAdapter::SQLQuerry& querry, RecordType& entry) {
	entry.template access<I>() = querry.getColumn(I, entry.template access<I>());
	if constexpr(I!=0)
		getEntryLoop<I-1>(querry, entry);
}

template<String_type String>
template<typename RecordType>
RecordType AccessTables<String>::getEntry(int id) const {
	const String& table = RecordType::table;
	RecordType entry;

	String sql = "SELECT ";
	appendFieldNamesReverse<RecordType>(sql);
	sql += " FROM " + table + " WHERE id IS " + std::to_string(id);
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(querry.nextRow() != SQLITE_ROW)
		throw(missing_entry(std::string("Error retrieving entry with id ") + std::to_string(id) + " from " + table));

	getEntryLoop<RecordType::size()-1>(querry, entry);

	return entry;
}

template<String_type String>
template<typename RecordType>
std::vector<int> AccessTables<String>::getChildren(int parent) const {
	const String& table = RecordType::table;

	String sql = "SELECT id FROM " + table + " WHERE " + RecordType::fields[0] + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			ids.push_back(i);

	return ids;
}

template<String_type String>
template<typename RecordType>
int AccessTables<String>::getNumberChildren(int parent) const {
	const String& table = RecordType::table;

	String sql = "SELECT id FROM " + table + " WHERE " + RecordType::fields[0] + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	int ids_number = 0;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			++ids_number;

	return ids_number;
}

/**
 * Template loop used by AccessTables::newEntry
 */
template<int I, typename RecordType, String_type String>
void newEntryLoop(const RecordType& entry, String& sql) {
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += ", ";
		newEntryLoop<I-1>(entry, sql);
	}
}

template<String_type String>
template<typename RecordType>
void AccessTables<String>::newEntry(const RecordType& entry) {
	const String& table = RecordType::table;

	String sql = "INSERT INTO " + table + " (";
	appendFieldNames<RecordType>(sql);
	sql += ") VALUES (";

	newEntryLoop<RecordType::size()-1>(entry, sql);

	sql += String(");");

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error(std::string("Constraint Error adding entry to table ") + table));
	else if(i != SQLITE_DONE)
		throw(database_error("Error inserting into " + table + " (error code: " + std::to_string(i) + ")"));
}

/**
 * Template loop used by AccessTables::updateEntry
 */
template<int I, typename RecordType, String_type String>
void updateEntryLoop(const RecordType &entry, String& sql) {
	appendSQL(sql, RecordType::fields[I], false);
	sql += " = ";
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += ", ";
		updateEntryLoop<I-1>(entry, sql);
	}
}

template<String_type String>
template<typename RecordType>
void AccessTables<String>::updateEntry(int id, const RecordType &entry) {
	const String& table = RecordType::table;

	String sql = "UPDATE " + table + " SET ";

	updateEntryLoop<RecordType::size()-1>(entry, sql);

	sql += " WHERE id IS " + std::to_string(id) + "";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error updating " + table));
	else if(i != SQLITE_DONE)
		throw(database_error("Error updating " + table + " (error code: " + std::to_string(i) + ")"));
}

template<String_type String>
template<typename RecordType>
void AccessTables<String>::setParent(int child_id, int new_parent_id) {
	const String& table = RecordType::table;

	String sql = "UPDATE " + table + " SET " + RecordType::fields[0] + " = '" + std::to_string(new_parent_id) +
			"' WHERE id IS '" + std::to_string(child_id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error moving entry."));
	else if (i != SQLITE_DONE)
		throw(database_error("Error moving entry."));
}

/**
 * Template loop used by AccessTables::getID
 */
template<int I, typename RecordType, String_type String>
void getIDLoop(const RecordType& entry, String& sql) {
	appendSQL(sql, RecordType::fields[I], false);
	sql += " = ";
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += " AND ";
		getIDLoop<I-1>(entry, sql);
	}
}

template<String_type String>
template<typename RecordType>
int AccessTables<String>::getID(const RecordType& entry) const {
	const String& table = RecordType::table;

	String sql = "SELECT id FROM " + table + " WHERE (";

	getIDLoop<RecordType::size()-1>(entry, sql);

	sql += ");";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());
	if(int i=querry.nextRow() != SQLITE_ROW)
		throw(missing_entry("Error getting id (error code: " + std::to_string(i)));

	return querry.getColumn<int>(0);
}

template<String_type String>
template<typename RecordType>
void AccessTables<String>::deleteEntry(int id) {
	const String& table = RecordType::table;

	if (!id)
		throw(std::runtime_error("Deleting the root is forbidden."));

	String sql = "DELETE FROM " + table + " WHERE id = " + std::to_string(id);
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(database_error("Error deleting keyword."));
}

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* SRC_ACCESSTABLES_H_ */
