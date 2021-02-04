/*
 * SQLQerry.cpp
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

#include <stdexcept>
#include "SQLQuerry.h"
#include "Database.h"
#include <string>

namespace PhotoLibrary {
namespace Adapter {

SQLQuerry::SQLQuerry(Database* db, const char* querry) : db(db), sqlStmt(nullptr), nextStmt(querry) {
	try {
		prepareStmt();
	}
	catch (...) {
		sqlite3_finalize(sqlStmt);
		sqlStmt = nullptr;
		throw;
	}
}

SQLQuerry::~SQLQuerry() {
	sqlite3_finalize(sqlStmt);
}

int SQLQuerry::nextRow() {
	return sqlite3_step(sqlStmt);
}

int SQLQuerry::columnCount() {
	return sqlite3_column_count(sqlStmt);
}

const char* SQLQuerry::getColumnText(int colNum) {
	return reinterpret_cast<const char*>(sqlite3_column_text(sqlStmt, colNum));
}

int SQLQuerry::getColumnInt(int colNum) {
	return sqlite3_column_int(sqlStmt, colNum);
}

int_least64_t SQLQuerry::getColumnInt64(int colNum) {
	return sqlite3_column_int64(sqlStmt, colNum);
}

void SQLQuerry::nextStatement() {
	sqlite3_finalize(sqlStmt);
	sqlStmt = nullptr;

	prepareStmt();
}

void SQLQuerry::prepareStmt() {
	if (int i = sqlite3_prepare_v2(db->db, nextStmt, -1, &sqlStmt, &nextStmt); i != SQLITE_OK) {
		throw(std::runtime_error("Error preparing SQL statement. (Error Code " + std::to_string(i) + ")"));
	}
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
