/*
 * Database.h
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

#ifndef SRC_SQLITEADAPTER_DATABASE_H_
#define SRC_SQLITEADAPTER_DATABASE_H_

#include <sqlite3.h>
#include "SQLQuerry.h"

namespace PhotoLibrary {
namespace SQLiteAdapter {

/**
 * SQLite database handle. It holds the connection to the SQLite
 * database, prepares new databases, and performs rudimentary
 * consistency tests on existing databases.
 */
class Database {
public:
	/**
	 * @param filename filename of the SQLite database
	 * @param initialise create new database or open existing?
	 * 		If true filename must not exist.
	 */
	Database(const char* filename, bool create);
	~Database();

	//prevent copy-construction and copying
	Database(Database&&) = delete;
	Database& operator=(Database&&) = delete;
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;

	/**
	 * Passes an SQL querry to the database.
	 *
	 * For details see the SQLite C/C++ API documentation (sqlite3_exec).
	 * @see https://sqlite.org/c3ref/exec.html
	 *
	 * @throw Throws an std::runtime_error if it encounters any problems.
	 */
	void querry(const char* sql, int (*callback)(void*,int,char**,char**), void* data);

	/**
	 * Passes an SQL querry to the database.
	 *
	 * For details see the SQLite C/C++ API documentation (sqlite3_exec).
	 * @see https://sqlite.org/c3ref/exec.html
	 *
	 * @param error_msg String into which the error message will be written if an error occurs during execution
	 * @return Return the SQLite Error code.
	 */
	int querryNoThrow(const char* sql, int (*callback)(void*,int,char**,char**), void* data, std::string& error_msg);

private:
	sqlite3* db;

	friend void SQLQuerry::prepareStmt();
};

} /* namespace SQLiteAdapter */
} /* namespace PhotoLibrary */

#endif /* SRC_SQLITEADAPTER_DATABASE_H_ */
