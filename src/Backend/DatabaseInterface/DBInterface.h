/*
 * DBInterface.h
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

#ifndef SRC_DBINTERFACE_H_
#define SRC_DBINTERFACE_H_

#include "../InterfaceBase.h"
#include <support.h>
#include <Database.h>
#include <glibmm/ustring.h>
#include <iostream>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

using PhotoLibrary::DatabaseInterface::appendSQL;
using PhotoLibrary::DatabaseInterface::missing_entry;
using PhotoLibrary::DatabaseInterface::database_error;
using PhotoLibrary::DatabaseInterface::constraint_error;

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
template<class RType>
class DBInterface : public Backend::InterfaceBase<RType> {
public:
	using RecordType = RType;
	static_assert(RType::size() == RType::fields.size()-1);

	/**
	 * @param db Handle for the database to use
	 */
	DBInterface(SQLiteAdapter::Database& db, const Glib::ustring& ={}) noexcept;
	virtual ~DBInterface() = default;

	RecordType getEntry(int id) const override;
	std::vector<int> getChildren(int parent) const override;
	int getNumberChildren(int parent) const override;
	void newEntry(const RecordType& entry) override;

	/**
	 * Updates a record.
	 * Doesn't check that the entry itself or one of its children
	 * isn't set as parent; it's up to the user not to
	 * fiddle-faddle with it.
	 * \todo check for consistency? (refering to self or child* as parent)
	 *
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 * @throws DatabaseInterface::constraint_error If the update failed due to
	 * 		constraint violation
	 * @throws DatabaseInterface::database_error If any other error occurs during update
	 */
	void updateEntry(int id, const RecordType &entry) override;

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
	void setParent(int child_id, int new_parent_id) override;

	int getID(const RType& entry) const override;
	void deleteEntry(int id) override;

private:
	SQLiteAdapter::Database& db;	/**< Database hande used */
	const Glib::ustring& table; /**< name of the table associated with the derived interface class */

	//append the names of all data fields (table columns)
	void appendFieldNames(Glib::ustring &sql) const {
		int i = RecordType::size()-1;
		appendSQL(sql, RecordType::fields[i], false);
		while(i--) {
			sql += ", ";
			appendSQL(sql, RecordType::fields[i], false);
		}
	}

	//append the names of all data fields (table columns) for GET
	void appendFieldNamesReverse(Glib::ustring &sql) const {
		appendSQL(sql, RecordType::fields[0], false);
		for(int i=1; i<RecordType::size(); ++i) {
			sql += ", ";
			appendSQL(sql, RecordType::fields[i], false);
		}
	}
};



//implementation
template<class RType>
DBInterface<RType>::DBInterface(SQLiteAdapter::Database& db, const Glib::ustring&) noexcept :
		db(db), table(RType::table) {
}

//compiletime loop for getEntry
template<int I, typename RecordType>
void getEntryLoop(SQLiteAdapter::SQLQuerry& querry, RecordType& entry) {
	entry.template access<I>() = querry.getColumn(I, entry.template access<I>());
	if constexpr(I!=0)
		getEntryLoop<I-1>(querry, entry);
}

template<class RType>
RType DBInterface<RType>::getEntry(int id) const {
	RecordType entry;

	Glib::ustring sql = "SELECT ";
	appendFieldNamesReverse(sql);
	sql += " FROM " + table + " WHERE id IS " + std::to_string(id);
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(querry.nextRow() != SQLITE_ROW)
		throw(missing_entry(std::string("Error retrieving entry with id ") + std::to_string(id) + " from " + table));

	getEntryLoop<RecordType::size()-1>(querry, entry);

	return entry;
}

template<class RType>
std::vector<int> DBInterface<RType>::getChildren(int parent) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE " + RecordType::fields[0] + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			ids.push_back(i);

	return ids;
}

template<class RType>
int DBInterface<RType>::getNumberChildren(int parent) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE " + RecordType::fields[0] + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	int ids_number = 0;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			++ids_number;

	return ids_number;
}

//compiletime loop for newEntry
template<int I, typename RecordType>
void newEntryLoop(const RecordType& entry, Glib::ustring& sql) {
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += ", ";
		newEntryLoop<I-1>(entry, sql);
	}
}

template<class RType>
void DBInterface<RType>::newEntry(const RecordType& entry) {
	Glib::ustring sql = "INSERT INTO " + table + " (";
	appendFieldNames(sql);
	sql += ") VALUES (";

	newEntryLoop<RecordType::size()-1>(entry, sql);

	sql += Glib::ustring(");");

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error(std::string("Constraint Error adding entry to table ") + table));
	else if(i != SQLITE_DONE)
		throw(database_error("Error inserting into " + table + " (error code: " + std::to_string(i) + ")"));
}

//comiltetime loop for updateEntry
template<int I, typename RecordType>
void updateEntryLoop(const RecordType &entry, Glib::ustring& sql) {
	appendSQL(sql, RecordType::fields[I], false);
	sql += " = ";
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += ", ";
		updateEntryLoop<I-1>(entry, sql);
	}
}

template<class RType>
void DBInterface<RType>::updateEntry(int id, const RecordType &entry) {
	Glib::ustring sql = "UPDATE " + table + " SET ";

	updateEntryLoop<RecordType::size()-1>(entry, sql);

	sql += " WHERE id IS " + std::to_string(id) + "";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error updating " + table));
	else if(i != SQLITE_DONE)
		throw(database_error("Error updating " + table + " (error code: " + std::to_string(i) + ")"));
}

template<class RType>
void DBInterface<RType>::setParent(int child_id, int new_parent_id) {
	Glib::ustring sql = "UPDATE " + table + " SET " + RecordType::fields[0] + " = '" + std::to_string(new_parent_id) +
			"' WHERE id IS '" + std::to_string(child_id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error moving entry."));
	else if (i != SQLITE_DONE)
		throw(database_error("Error moving entry."));
}

//compiletime loop for getID
template<int I, typename RecordType>
void getIDLoop(const RecordType& entry, Glib::ustring& sql) {
	appendSQL(sql, RecordType::fields[I], false);
	sql += " = ";
	appendSQL(sql, entry.template access<I>());
	if constexpr(I!=0) {
		sql += " AND ";
		getIDLoop<I-1>(entry, sql);
	}
}

template<class RType>
int DBInterface<RType>::getID(const RType& entry) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE (";

	getIDLoop<RType::size()-1>(entry, sql);

	sql += ");";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());
	if(int i=querry.nextRow() != SQLITE_ROW)
		throw(missing_entry("Error getting id (error code: " + std::to_string(i)));

	return querry.getColumn<int>(0);
}

template<class RType>
void DBInterface<RType>::deleteEntry(int id) {
	if (!id)
		throw(std::runtime_error("Deleting the root is forbidden."));

	Glib::ustring sql = "DELETE FROM " + table + " WHERE id = " + std::to_string(id);
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(database_error("Error deleting keyword."));
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_DBINTERFACE_H_ */
