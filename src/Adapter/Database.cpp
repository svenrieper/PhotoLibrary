/*
 * Database.cpp
 *
 *  Created on: 14 Sep 2020
 *      Author: Sven Rieper
 */

#include "Database.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace PhotoLibrary {
namespace Adapter {

Database::Database(const char*, bool) :
	db(nullptr) {
	/// \todo Add support for persistence
	const char* filename = ":memory:";
	bool initialise = true; // file_exists(filename)

	int rc;
	rc = sqlite3_open(filename, &db);

	if(rc) {
		sqlite3_close(db);
		throw(std::runtime_error(std::string("Database ") + filename +
				" couldn't be opened: ")); // + sqlite3_errmsg(db)
	}

	try {
		if (!initialise);
			//testConsistency(); /// \todo Add test for db
		else
			createTables();
	} catch (...) {
		sqlite3_close(db);
		throw;
	}
}

Database::~Database() {
	sqlite3_close(db);
}

void Database::querry(const char* sql, int (*callback)(void*,int,char**,char**), void* data) {
	char* zErrMsg = nullptr;
	if(sqlite3_exec(db, sql, callback, data, &zErrMsg))
		throw(std::runtime_error(std::string("Error executing SQL command: ") + *zErrMsg));
}

void Database::createTables() {
	char* zErrMsg=nullptr;

	const char* enable_foreign_key_support = "PRAGMA foreign_keys = ON;";

	if (sqlite3_exec(db, enable_foreign_key_support, nullptr, nullptr, &zErrMsg))
		throw(std::runtime_error(std::string("Table \"keyword\": ") + sqlite3_errmsg(db)));

	/// \todo Add database structure.
	const char* tables =
		//Keywords table
			"CREATE TABLE Keywords("
			"  id				INTEGER	PRIMARY KEY AUTOINCREMENT"	//AUTOINCREMENT?
			", parent			INTEGER	NOT NULL"
			", keyword	TEXT	NOT NULL"
			", lkeyword	TEXT	AS (lower(keyword))"	//VIRTUAL or STORED?
			", synonyms	TEXT	DEFAULT ''"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, lkeyword)"
			", FOREIGN KEY		(parent) REFERENCES Keywords ON DELETE CASCADE"
			");"
			//Create index for parent
			"CREATE INDEX keyParentIndex ON Keywords(parent);"
			"INSERT INTO Keywords (id, keyword, parent) VALUES (1, 'internal root', 1);"
			"INSERT INTO Keywords (id, keyword, parent) VALUES (0, 'root', 0);"
			//Reserve keyword ids 1 through 50 for internal use
			"INSERT INTO Keywords (id, keyword, parent) VALUES (50, 'temp', 0);"
			"DELETE FROM Keywords WHERE id = 50;"
		//Directories table
			"CREATE TABLE Directories("
			"  id				INTEGER	PRIMARY KEY"	//AUTOINCREMENT?
			", parent			INTEGER	NOT NULL"
			", name				TEXT	NOT NULL"
			", fullname			TEXT	NOT NULL"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, fullname)"
			", FOREIGN KEY		(parent) REFERENCES Directories ON DELETE CASCADE"
			");"
			//Create index for parent
			"CREATE INDEX dirParentIndex ON Directories(parent);"
			"INSERT INTO Directories (id, name, fullname, parent) VALUES (0, 'root', '/', 0);"
		//Albums table
			"CREATE TABLE Albums("
			"  id				INTEGER	PRIMARY KEY"
			", parent			INTEGER	NOT NULL"
			", name				TEXT	NOT NULL"
			", attributes		INTEGER	DEFAULT 0"
			//Constraints
			", UNIQUE			(parent, name)"
			", FOREIGN KEY		(parent) REFERENCES Albums ON DELETE CASCADE"
			");"
			"CREATE INDEX albumsParentIndex ON Albums(parent);"
			"INSERT INTO Albums (id, name, parent) VALUES (0, 'root', 0);"
			;

	if(sqlite3_exec(db, tables, nullptr, nullptr, &zErrMsg))
		throw(std::runtime_error(std::string("Table \"keyword\": ") + sqlite3_errmsg(db)));
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
