/*
 * RelationsTable.cpp
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

#include "RelationsTable.h"
#include "exceptions.h"
#include "support.h"
#include <string>

namespace PhotoLibrary {
namespace DatabaseInterface {

RelationsTable::RelationsTable(SQLiteAdapter::Database& db) noexcept : db(db) {
}

std::vector<int> RelationsTable::getEntries(int collection, const std::array<const std::string,3>& table) const {
	return getVector(collection, table[1], table[2], table[0]);
}

int RelationsTable::getNumberEntries(int collection, const std::array<const std::string,3>& table) const {
	return getNumber(collection, table[1], table[2], table[0]);
}

std::vector<int> RelationsTable::getCollections(int entry, const std::array<const std::string,3>& table) const {
	return getVector(entry, table[2], table[1], table[0]);
}

int RelationsTable::getNumberCollections(int entry, const std::array<const std::string,3>& table) const {
	return getNumber(entry, table[2], table[1], table[0]);
}

void RelationsTable::newRelation(int entry, int collection, const std::array<const std::string,3>& table) {
	std::string sql = "INSERT OR IGNORE INTO " + table[0] + " (" + table[2] + ", " + table[1] +
			") VALUES (" + std::to_string(entry) + ", " + std::to_string(collection) + ");";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i == SQLITE_CONSTRAINT)
		throw(constraint_error(std::string("Constraint Error adding entry to table ") + table[0]));
	else if(i != SQLITE_DONE)
		throw(database_error("Error inserting into " + table[0] + " (error code: " + std::to_string(i) + ")"));
}

void RelationsTable::deleteRelation(int entry, int collection, const std::array<const std::string,3>& table) {
	std::string sql = "DELETE FROM " + table[0] + " WHERE " + table[2] + " = " + std::to_string(entry) +
			" AND " + table[1] + " = " + std::to_string(collection) + ";";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if (querry.nextRow() != SQLITE_DONE)
		throw(database_error("Error deleting relation."));
}

std::vector<int> RelationsTable::getVector(
		int id,
		const std::string& reference_id,
		const std::string& return_id,
		const std::string& table
		) const {
	std::string sql = "SELECT " + return_id + " FROM " + table
			+ " WHERE " + reference_id + " IS '" + std::to_string(id) + "'";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	std::vector<int> ids;
	while (querry.nextRow() == SQLITE_ROW)
		ids.push_back(querry.getColumnInt(0));

	return ids;
}

int RelationsTable::getNumber(
		int id,
		const std::string& reference_id,
		const std::string& return_id,
		const std::string& table
		) const {
	std::string sql = "SELECT COUNT(*) FROM " + table
		+ " WHERE " + reference_id + " IS " + std::to_string(id) + ";";
	SQLiteAdapter::SQLQuerry querry(db, sql.c_str());

	if(int i = querry.nextRow(); i != SQLITE_ROW)
		throw(database_error("Error retrieving number of relations with error code: " + std::to_string(i)));

	return querry.getColumnInt(0);
}

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */
