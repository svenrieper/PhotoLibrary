/*
 * DBInterface.h
 *
 *  Created on: 15 Sep 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_DBINTERFACE_H_
#define SRC_DBINTERFACE_H_

#include <glibmm/ustring.h>
#include "Database.h"
#include "suppport.h"
#include "../Backend/Record/Record.h"
#include "../Backend/InterfaceBase.h"
#include <iostream>

namespace PhotoLibrary {
namespace Adapter {

/**
 * Base class for database interface classes.
 * Currently works for Backend::Record classes based on
 * up to five-tuples.
 *
 * The Backend::Record::getField(int) should return the name of the column
 * for the data returned with Backend::Record::access<int>().
 * Currently supported data types: int, int_least64_t, Glib::ustring, and
 * Backend::RecordOptions::KeywordOptions.
 *
 * @param RType Backend::Record based class used to retrieve,
 * 		save, and update records in the database
 */
template<class RType>
class DBInterface : public Backend::InterfaceBase<RType> {
public:
	using RecordType = RType;

	/**
	 * @param db Handle for the database to use
	 * @param table name of the associated table
	 */
	DBInterface(Database *db, Glib::ustring table);

	virtual ~DBInterface() = default;

	/**
	 * Retrieve a record.
	 *
	 * @param id Id of the record to return
	 * @return complete information for record with id 'id'
	 */
	RecordType getEntry(int id) override;

	/**
	 * Get the children of a entry.
	 *
	 * @param parent id of the entry of which the children should be returned
	 * @return vector of id's of the children of 'parent'
	 */
	std::vector<int> getChildren(int parent) override;

	/**
	 * Add new record.
	 *
	 * @param entry data to be inserted for new record
	 * @return id of the new entry
	 */
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
	 * @param id Id of the record to update
	 * @param newParent New parent id of record 'id'
	 */
	void setParent(int child_id, int new_parent_id) override;

	/**
	 * Get the id to an entry.
	 * @throws throws std::runtime_error if the entry wasn't found
	 *
	 * @param entry the record for which to return the id
	 * @return the id of the record 'entry'
	 */
	int getID(const RType& entry) override;

	/**
	 * Delete record from table.
	 * Deletes a record, all its children, and all information
	 * that depends on it.
	 *
	 * @param id Id of the record to delete
	 */
	void deleteEntry(int id) override;

protected:
	Database* const db;	/**< Database hande used */
	const Glib::ustring table; /**< name of the table associated with the derived interface class */

private:
	void appendSQL(Glib::ustring* sql, Glib::ustring append, bool escape=true) {
		escapeSingleQuotes(&append);
		*sql += (escape?"'":"") + append + (escape?"'":"");
	}

	void appendSQL(Glib::ustring* sql, int append, bool escape=true) {
		*sql += std::to_string(append);
	}

	//append the names of all data fields (table columns)
	void appendFieldNames(RecordType entry, Glib::ustring &sql) {
		int i = entry.size()-1;
		appendSQL(&sql, entry.getField(i), false);
		while(i--) {
			sql += ", ";
			appendSQL(&sql, entry.getField(i), false);
		}
	}

	//append the names of all data fields (table columns) for GET
	void appendFieldNamesReverse(RecordType entry, Glib::ustring &sql) {
		appendSQL(&sql, entry.getField(0), false);
		for(int i=1; i<entry.size(); ++i) {
			sql += ", ";
			appendSQL(&sql, entry.getField(i), false);
		}
	}

	//update 'field' with the retrieved data
	//use overloaded functions to get the right type
	void setValue(SQLQuerry& querry, int column, int_least64_t& field) {
		field = querry.getColumnInt64(column);
	}
	void setValue(SQLQuerry& querry, int column, int& field) {
		field = querry.getColumnInt(column);
	}
	void setValue(SQLQuerry& querry, int column, Backend::RecordOptions::KeywordOptions& field) {
		field = static_cast<Backend::RecordOptions::KeywordOptions>(querry.getColumnInt(column));
	}
	void setValue(SQLQuerry& querry, int column, Glib::ustring& field) {
		field = querry.getColumnText(column);
	}
};



//implementation
template<class RType>
DBInterface<RType>::DBInterface(Database *db, Glib::ustring table) :
		db(db), table(table) {
}

template<class RType>
RType DBInterface<RType>::getEntry(int id) {
	RecordType entry;

	Glib::ustring sql = "SELECT ";
	appendFieldNamesReverse(entry, sql);
	sql += " FROM " + table + " WHERE id IS " + std::to_string(id) + "";
	SQLQuerry querry(db, sql.c_str());

	if(querry.nextRow() != SQLITE_ROW)
			throw(std::runtime_error(std::string("Error retrieving entry with id ") + std::to_string(id) + " from " + table));

	switch (entry.size()) {
	case 5:
		{
		constexpr int i = 4;
		setValue(querry, i, entry.template access<i>());
	}
		/* no break */
	case 4:
		{
		constexpr int i = 3;
		setValue(querry, i, entry.template access<i>());
	}
		/* no break */
	case 3:
		{
		constexpr int i = 2;
		setValue(querry, i, entry.template access<i>());
	}
		/* no break */
	case 2:
		{
		constexpr int i = 1;
		setValue(querry, i, entry.template access<i>());
	}
		/* no break */
	case 1:
		{
		constexpr int i =0;
		setValue(querry, i, entry.template access<i>());
	}
		/* no break */
	}

	return entry;
}

template<class RType>
std::vector<int> DBInterface<RType>::getChildren(int parent) {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE parent IS '" + std::to_string(parent) + "'";
	SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		if(int i = querry.getColumnInt(0))
			ids.push_back(i);

	return ids;
}

template<class RType>
void DBInterface<RType>::newEntry(const RecordType& entry) {
	Glib::ustring sql = "INSERT INTO " + table + " (";
	appendFieldNames(entry, sql);
	sql += ") VALUES (";
	switch (entry.size()) {
	case 5:
		{
		constexpr int i = 4;
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 4:
		{
		constexpr int i = 3;
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 3:
		{
		constexpr int i = 2;
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 2:
		{
		constexpr int i = 1;
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 1:
		{
		constexpr int i = 0;
		appendSQL(&sql, entry.template access<i>());
	}
		/* no break */
	}
	sql += Glib::ustring(");");

	SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error(std::string("Constraint Error adding entry to table ") + table));
	else if(i != SQLITE_DONE)
		throw(std::runtime_error("Error inserting into " + table + " (error code: " + std::to_string(i) + ")"));
}

template<class RType>
void DBInterface<RType>::updateEntry(int id, const RecordType &entry) {
	Glib::ustring sql = "UPDATE " + table + " SET ";
	switch (entry.size()) {
	case 5:
		{
		constexpr int i = 4;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 4:
		{
		constexpr int i = 3;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 3:
		{
		constexpr int i = 2;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 2:
		{
		constexpr int i = 1;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += ", ";
	}
		/* no break */
	case 1:
		{
		constexpr int i = 0;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
	}
		/* no break */
	}
	sql += " WHERE id IS " + std::to_string(id) + "";
//		std::cout << sql << std::endl;

	SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error updating " + table));
	else if(i != SQLITE_DONE)
		throw(std::runtime_error("Error updating " + table + " (error code: " + std::to_string(i) + ")"));
}

template<class RType>
void DBInterface<RType>::setParent(int child_id, int new_parent_id) {
	Glib::ustring sql = "UPDATE " + table + " SET parent = '" + std::to_string(new_parent_id) +
			"' WHERE id IS '" + std::to_string(child_id) + "'";
	SQLQuerry querry(db, sql.c_str());

	if (int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error("Error moving entry."));
	else if (i != SQLITE_DONE)
		throw(std::runtime_error("Error moving entry."));
}

template<class RType>
int DBInterface<RType>::getID(const RType& entry) {
	Glib::ustring sql = "SELECT id FROM " + table + " WHERE (";
	switch (entry.size()) {
	case 5:
		{
		constexpr int i = 4;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += " AND ";
	}
		/* no break */
	case 4:
		{
		constexpr int i = 3;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += " AND ";
	}
		/* no break */
	case 3:
		{
		constexpr int i = 2;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += " AND ";
	}
		/* no break */
	case 2:
		{
		constexpr int i = 1;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
		sql += " AND ";
	}
		/* no break */
	case 1:
		{
		constexpr int i = 0;
		appendSQL(&sql, entry.getField(i), false);
		sql += " = ";
		appendSQL(&sql, entry.template access<i>());
	}
		/* no break */
	}
	sql += ");";

	SQLQuerry querry(db, sql.c_str());
	if(int i=querry.nextRow() != SQLITE_ROW)
		throw(std::runtime_error("Error getting id (error code: " + std::to_string(i)));

	return querry.getColumnInt(0);
}

template<class RType>
void DBInterface<RType>::deleteEntry(int id) {
	if (!id)
		throw(std::runtime_error("Deleting the root is forbidden."));

	Glib::ustring sql = "DELETE FROM " + table + " WHERE id = " + std::to_string(id);
	SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(std::runtime_error("Error deleting keyword."));
}


} /* namespace Adapter */
} /* namespace PhotoLibrary */

#endif /* SRC_DBINTERFACE_H_ */
