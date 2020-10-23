/*
 * SQLQerry.cpp
 *
 *  Created on: 20 Sep 2020
 *      Author: Sven Rieper
 */

#include <stdexcept>
#include "SQLQuerry.h"
#include "Database.h"
#include <string>
#include <iostream>

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
