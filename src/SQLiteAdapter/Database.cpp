/*
 * Database.cpp
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

#include "Database.h"
#include <stdexcept>
#include <string>

namespace PhotoLibrary {
namespace SQLiteAdapter {

Database::Database(const char*, bool) :
	db(nullptr) {
	/// \todo Add support for persistence
	const char* filename = ":memory:";

	if(int rc = sqlite3_open(filename, &db)) { /// \todo add more expressive error messages
		sqlite3_close(db);
		throw(std::runtime_error(std::string("Database ") + filename +
				" couldn't be opened: error code " + std::to_string(rc))); /// \todo prepare for internationalisation
	}
}

Database::~Database() noexcept {
	sqlite3_close(db);
}

void Database::querry(const char* sql, int (*callback)(void*,int,char**,char**), void* data) {
	char* zErrMsg = nullptr;
	if(sqlite3_exec(db, sql, callback, data, &zErrMsg)) {
		std::string error_msg = "Error executing SQL command: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw(std::runtime_error(error_msg));
	}
}

int Database::querryNoThrow(const char* sql, int (*callback)(void*,int,char**,char**), void* data, std::string& error_msg) {
	char* zErrMsg = nullptr;
	int return_value = sqlite3_exec(db, sql, callback, data, &zErrMsg);
	if(return_value) {
		error_msg = *zErrMsg;
		sqlite3_free(zErrMsg);
	}
	return return_value;
}

} /* namespace SQLiteAdapter */
} /* namespace PhotoLibrary */
