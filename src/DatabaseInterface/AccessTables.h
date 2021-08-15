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
#include <iostream>

namespace PhotoLibrary {
namespace DatabaseInterface {

using SQLiteAdapter::String_type;

/**
 * Backend interface to the database.
 *
 * static RType::fields[i] should hold the name of the database column
 * for the data returned with RType::access<i>().
 *
 * RType::fields[RType::size()] should hold the name of the name of the
 * database associated to RType.
 *
 * RType::fields[0] should hold the name of the column containing the
 * parent id or whatever come closest to the parent id.
 *
 * Currently supported data types: integral types, strings, and enum|s.
 *
 * @tparam RType Backend::RecordClasses::Record based class used to retrieve,
 * 		save, and update records in the database
 */
template<String_type String=std::string>
class AccessTables {
public:
	/**
	 * @param db Handle for the database to use
	 */
	AccessTables(SQLiteAdapter::Database& db) noexcept : db(db) {}
	virtual ~AccessTables() = default;

	template<typename RecordType>
	RecordType getEntry(int id) const;

	template<typename RecordType>
	std::vector<int> getChildren(int parent) const;

	template<typename RecordType>
	int getNumberChildren(int parent) const;

	template<typename RecordType>
	void newEntry(const RecordType& entry);

	/**
	 * Updates a record.
	 * Doesn't check that the entry itself or one of its children
	 * isn't set as parent; it's up to the user not to
	 * fiddle-faddle with it.
	 * If no entry with an ID 'id' exists this function doesn't do anything.
	 * \todo check for consistency? (refering to self or child* as parent)
	 *
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 * @throws DatabaseInterface::constraint_error If the update failed due to
	 * 		constraint violation
	 * @throws DatabaseInterface::database_error If any other error occurs during update
	 */
	template<typename RecordType>
	void updateEntry(int id, const RecordType &entry);

	/**
	 * Moves a record to a new parent.
	 * Doesn't check that the record itself or one of its children
	 * isn't set as parent; it's up to the user not to
	 * fiddle-faddle with it.
	 * \todo check for consistency? (refering to self or child* as parent)
	 *
	 * @param child_id Id of the record to update
	 * @param new_parent_id New parent id of record 'id'
	 * @throws DatabaseInterface::constraint_error Thrown if parent 'id' does not exist.
	 * @throws DatabaseInterface::database_error If any other error occurs during updating
	 * 		the database
	 */
	template<typename RecordType>
	void setParent(int child_id, int new_parent_id);

	template<typename RecordType>
	int getID(const RecordType& entry) const;

	template<typename RecordType>
	void deleteEntry(int id);

private:
	SQLiteAdapter::Database& db;	/**< Database hande used */
};


//compiletime loop for getEntry
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

//compiletime loop for newEntry
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

//comiltetime loop for updateEntry
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

//compiletime loop for getID
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
		throw(missing_entry("Error deleting keyword."));
}

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* SRC_ACCESSTABLES_H_ */
