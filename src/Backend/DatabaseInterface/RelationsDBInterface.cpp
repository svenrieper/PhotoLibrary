/*
 * RelationsDBInterface.cpp
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

#include "RelationsDBInterface.h"
#include <exceptions.h>
#include <support.h>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

RelationsDBInterface::RelationsDBInterface(
		SQLiteAdapter::Database& db,
		const Glib::ustring& table,
		const Glib::ustring& entry_name,
		const Glib::ustring& collection_name) :
				db(db),
				table(table),
				entry_name(entry_name),
				collection_name(collection_name) {
}

std::vector<int> RelationsDBInterface::getEntries(int collection) const {
	return getVector(collection, collection_name, entry_name);
}

int RelationsDBInterface::getNumberEntries(int collection) const {
	return getNumber(collection, collection_name, entry_name);
}

std::vector<int> RelationsDBInterface::getCollections(int entry) const {
	return getVector(entry, entry_name, collection_name);
}

int RelationsDBInterface::getNumberCollections(int entry) const {
	return getNumber(entry, entry_name, collection_name);
}

void RelationsDBInterface::newRelation(int entry, int collection) {
	Glib::ustring sql = "INSERT OR IGNORE INTO " + table + " (" + entry_name + ", " + collection_name +
			") VALUES (" + std::to_string(entry) + ", " + std::to_string(collection) + ");";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(PhotoLibrary::DatabaseInterface::constraint_error(std::string("Constraint Error adding entry to table ") + table));
	else if(i != SQLITE_DONE)
		throw(std::runtime_error("Error inserting into " + table + " (error code: " + std::to_string(i) + ")"));
}

void RelationsDBInterface::deleteRelation(int entry, int collection) {
	Glib::ustring sql = "DELETE FROM " + table + " WHERE " + entry_name + " = " + std::to_string(entry) +
			" AND " + collection_name + " = " + std::to_string(collection) + ";";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(std::runtime_error("Error deleting relation."));
}

std::vector<int> RelationsDBInterface::getVector(int id, const Glib::ustring& reference_id, const Glib::ustring& return_id) const {
	Glib::ustring sql = "SELECT " + return_id + " FROM " + table + " WHERE " + reference_id + " IS '" + std::to_string(id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		ids.push_back(querry.getColumnInt(0));

	return ids;
}

int RelationsDBInterface::getNumber(int id, const Glib::ustring& reference_id, const Glib::ustring& return_id) const {
	Glib::ustring sql = "SELECT " + return_id + " FROM " + table + " WHERE " + reference_id + " IS '" + std::to_string(id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	int number_ids = 0;
	while (querry.nextRow() == SQLITE_ROW)
		++number_ids;

	return number_ids;
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
