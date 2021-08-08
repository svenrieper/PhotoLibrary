/*
 * SQLQerry.cpp
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

#include "SQLQuerry.h"
#include "Database.h"
#include <stdexcept>
#include <string>

namespace PhotoLibrary {
namespace SQLiteAdapter {

SQLQuerry::SQLQuerry(Database& db, const char* querry) : db(db), sqlStmt(nullptr), nextStmt(querry) {
	try {
		prepareStmt();
	}
	catch (...) {
		sqlite3_finalize(sqlStmt);
		sqlStmt = nullptr;
		throw;
	}
}

SQLQuerry::~SQLQuerry() noexcept {
	sqlite3_finalize(sqlStmt);
}

int SQLQuerry::nextRow() noexcept {
	return sqlite3_step(sqlStmt);
}

int SQLQuerry::columnCount() noexcept {
	return sqlite3_column_count(sqlStmt);
}

void SQLQuerry::nextStatement() {
	sqlite3_finalize(sqlStmt);
	sqlStmt = nullptr;

	prepareStmt();
}

void SQLQuerry::prepareStmt() {
	if (int i = sqlite3_prepare_v2(db.db, nextStmt, -1, &sqlStmt, &nextStmt); i != SQLITE_OK) {
		throw(std::runtime_error("Error preparing SQL statement. (Error Code " + std::to_string(i) + ")"));
	}
}

} /* namespace SQLiteAdapter */
} /* namespace PhotoLibrary */
