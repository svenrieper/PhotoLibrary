/*
 * DBInterface.h
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

#ifndef SRC_DBINTERFACE_H_
#define SRC_DBINTERFACE_H_

#include <glibmm/ustring.h>
#include "../Record/Record.h"
#include "../InterfaceBase.h"
#include <iostream>
#include "suppport.h"
#include "../../SQLiteAdapter/Database.h"
#include <boost/preprocessor/iteration/local.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

/**
 * Backend interface to the database.
 * Currently works for Backend::Record classes based on
 * up to six-tuples.
 *
 * The RType method Backend::Record::getField(int) should return
 * the name of the column for the data returned with
 * Backend::Record::access<int>().
 * Backend::Record::getField(0) should hold the name of the column
 * containing the parent id or whatever come closest to the parent id.
 * Currently supported data types: int, int_least64_t, Glib::ustring, and
 * Backend::RecordOptions::KeywordOptions.
 *
 * @tparam RType Backend::Record based class used to retrieve,
 * 		save, and update records in the database
 */
template<class RType>
class DBInterface : public Backend::InterfaceBase<RType> {
public:
	using RecordType = RType;

#define MACRO_SIZE_LIMIT 6 /// maximum size of RType

	/**
	 * @param db Handle for the database to use
	 * @param table name of the associated table
	 */
	DBInterface(SQLiteAdapter::Database *db, Glib::ustring table);
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
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @param id Id of the record to update
	 * @param entry Data used to update record with id 'id' with
	 */
	void updateEntry(int id, const RecordType &entry) override;

	/**
	 * Moves a record to a new parent.
	 * Doesn't check that the record itself or one of its children
	 * isn't set as parent; it's up to the user not to
	 * fiddle-faddle with it.
	 * \todo check for consistency? (refering to self or child* as parent)
	 *
	 * @throws constraint_error Thrown if parent 'id' does not exist.
	 * @param child_id Id of the record to update
	 * @param new_parent_id New parent id of record 'id'
	 */
	void setParent(int child_id, int new_parent_id) override;

	int getID(const RType& entry) const override;
	void deleteEntry(int id) override;

protected:
	SQLiteAdapter::Database* const db;	/**< Database hande used */
	const Glib::ustring table; /**< name of the table associated with the derived interface class */

private:
	void appendSQL(Glib::ustring* sql, Glib::ustring append, bool escape=true) const {
		escapeSingleQuotes(&append);
		*sql += (escape?"'":"") + append + (escape?"'":"");
	}

	void appendSQL(Glib::ustring* sql, int append, bool /*escape*/=true) const {
		*sql += std::to_string(append);
	}

	//append the names of all data fields (table columns)
	void appendFieldNames(Glib::ustring &sql) const {
		int i = RecordType::size()-1;
		appendSQL(&sql, RecordType::getField(i), false);
		while(i--) {
			sql += ", ";
			appendSQL(&sql, RecordType::getField(i), false);
		}
	}

	//append the names of all data fields (table columns) for GET
	void appendFieldNamesReverse(Glib::ustring &sql) const {
		appendSQL(&sql, RecordType::getField(0), false);
		for(int i=1; i<RecordType::size(); ++i) {
			sql += ", ";
			appendSQL(&sql, RecordType::getField(i), false);
		}
	}

	//update 'field' with the retrieved data
	//use overloaded functions to get the right type
	void setValue(SQLiteAdapter::SQLQuerry& querry, int column, int_least64_t& field) const {
		field = querry.getColumnInt64(column);
	}
	void setValue(SQLiteAdapter::SQLQuerry& querry, int column, int& field) const {
		field = querry.getColumnInt(column);
	}
	void setValue(SQLiteAdapter::SQLQuerry& querry, int column, Backend::RecordOptions::Options& field) const {
		field = static_cast<Backend::RecordOptions::Options>(querry.getColumnInt(column));
	}
	void setValue(SQLiteAdapter::SQLQuerry& querry, int column, Glib::ustring& field) const {
		field = querry.getColumnText(column);
	}
};



//implementation
template<class RType>
DBInterface<RType>::DBInterface(SQLiteAdapter::Database *db, Glib::ustring table) :
		db(db), table(table) {
}

template<class RType>
RType DBInterface<RType>::getEntry(int id) const {
	RecordType entry;

	Glib::ustring sql = "SELECT ";
	appendFieldNamesReverse(sql);
	sql += " FROM " + table + " WHERE id IS " + std::to_string(id) + "";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(querry.nextRow() != SQLITE_ROW)
			throw(std::runtime_error(std::string("Error retrieving entry with id ") + std::to_string(id) + " from " + table));

	switch (entry.size()) {
#define BOOST_PP_LOCAL_LIMITS (0, MACRO_SIZE_LIMIT-1)

#define BOOST_PP_LOCAL_MACRO(n)									\
	case (MACRO_SIZE_LIMIT - (n)):								\
		{														\
			constexpr int i = MACRO_SIZE_LIMIT - (n) -1;		\
			setValue(querry, i, entry.template access<i>());	\
		}														\
		[[fallthrough]];

#include BOOST_PP_LOCAL_ITERATE()
	}

	return entry;
}

template<class RType>
std::vector<int> DBInterface<RType>::getChildren(int parent) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE " + RecordType::getField(0) + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			ids.push_back(i);

	return ids;
}

template<class RType>
int DBInterface<RType>::getNumberChildren(int parent) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE " + RecordType::getField(0) + " IS '" + std::to_string(parent) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	int ids_number = 0;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			++ids_number;

	return ids_number;
}

template<class RType>
void DBInterface<RType>::newEntry(const RecordType& entry) {
	Glib::ustring sql = "INSERT INTO " + table + " (";
	appendFieldNames(sql);
	sql += ") VALUES (";

	switch (entry.size()) {
#define BOOST_PP_LOCAL_LIMITS (0, MACRO_SIZE_LIMIT-1)

#define BOOST_PP_LOCAL_MACRO(n)								\
	case (MACRO_SIZE_LIMIT - (n)):							\
		{													\
			constexpr int i = MACRO_SIZE_LIMIT-(n)-1;		\
			appendSQL(&sql, entry.template access<i>());	\
			if(i)											\
				sql += ", ";								\
		}													\
		[[fallthrough]];

#include BOOST_PP_LOCAL_ITERATE()
	}

	sql += Glib::ustring(");");

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error(std::string("Constraint Error adding entry to table ") + table));
	else if(i != SQLITE_DONE)
		throw(std::runtime_error("Error inserting into " + table + " (error code: " + std::to_string(i) + ")"));
}

template<class RType>
void DBInterface<RType>::updateEntry(int id, const RecordType &entry) {
	Glib::ustring sql = "UPDATE " + table + " SET ";

	switch (entry.size()) {
#define BOOST_PP_LOCAL_LIMITS (0, MACRO_SIZE_LIMIT-1)

#define BOOST_PP_LOCAL_MACRO(n)									\
	case (MACRO_SIZE_LIMIT - (n)):								\
		{														\
			constexpr int i = MACRO_SIZE_LIMIT-(n)-1;			\
			appendSQL(&sql, RecordType::getField(i), false);	\
			sql += " = ";										\
			appendSQL(&sql, entry.template access<i>());		\
			if(i)												\
				sql += ", ";									\
		}														\
		[[fallthrough]];

#include BOOST_PP_LOCAL_ITERATE()
	}

	sql += " WHERE id IS " + std::to_string(id) + "";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error updating " + table));
	else if(i != SQLITE_DONE)
		throw(std::runtime_error("Error updating " + table + " (error code: " + std::to_string(i) + ")"));
}

template<class RType>
void DBInterface<RType>::setParent(int child_id, int new_parent_id) {
	Glib::ustring sql = "UPDATE " + table + " SET " + RecordType::getField(0) + " = '" + std::to_string(new_parent_id) +
			"' WHERE id IS '" + std::to_string(child_id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error moving entry."));
	else if (i != SQLITE_DONE)
		throw(std::runtime_error("Error moving entry."));
}

template<class RType>
int DBInterface<RType>::getID(const RType& entry) const {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE (";

	switch (entry.size()) {
#define BOOST_PP_LOCAL_LIMITS (0, MACRO_SIZE_LIMIT-1)

#define BOOST_PP_LOCAL_MACRO(n)									\
	case (MACRO_SIZE_LIMIT - (n)):								\
		{														\
			constexpr int i = MACRO_SIZE_LIMIT-(n)-1;			\
			appendSQL(&sql, RecordType::getField(i), false);	\
			sql += " = ";										\
			appendSQL(&sql, entry.template access<i>());		\
			if(i)												\
				sql += " AND ";									\
		}														\
		[[fallthrough]];

#include BOOST_PP_LOCAL_ITERATE()
	}

	sql += ");";

	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());
	if(int i=querry.nextRow() != SQLITE_ROW)
		throw(std::runtime_error("Error getting id (error code: " + std::to_string(i)));

	return querry.getColumnInt(0);
}

template<class RType>
void DBInterface<RType>::deleteEntry(int id) {
	if (!id)
		throw(std::runtime_error("Deleting the root is forbidden."));

	Glib::ustring sql = "DELETE FROM " + table + " WHERE id = " + std::to_string(id);
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(std::runtime_error("Error deleting keyword."));
}

#undef MACRO_SIZE_LIMIT


} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_DBINTERFACE_H_ */
