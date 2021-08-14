/*
 * SQLQuerry.h
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

#ifndef SRC_SQLQUERRY_H_
#define SRC_SQLQUERRY_H_

#include "Concepts.h"
#include <sqlite3.h>
#include <string>
#include <cstdint>

namespace PhotoLibrary {
namespace SQLiteAdapter {

//catch2 doesn't like it if Database.h is included here (why?)
class Database;

/**
 * Wrapper to handle SQL Querries.
 * SQLQuerry is mainly a cenvenience wrapper for SQL querries
 * to prevent memory leaks. It performs minimal conversions and
 * checking. Unless otherwise stated assume that input is not
 * checked.
 *
 * Run nextRow() at least once for every SQL querry.
 *
 * \todo add nextStatement() that returns the SQLite db return code
 * and maybe a newQuerry(const char*) method
 */
class SQLQuerry {
public:
	/**
	 * @param db Handle for the database to send the querry to
	 * @param querry Semicolon seperated list of SQL querries
	 * @throws std::runtime_error if preparing the first SQL statement fails.
	 */
	SQLQuerry(Database& db, const char* querry);

	~SQLQuerry() noexcept;

	/**
	 * Evalueate the SQL statement.
	 * Must be called at least once for every SQL querry.
	 * @see https://sqlite.org/c3ref/step.html
	 *
	 * @return returns the return value of sqlite3_step(sqlite3_stmt*)
	 */
	int nextRow() noexcept;

	/**
	 * Get the number of columns in the result set.
	 * @see https://sqlite.org/c3ref/column_count.html
	 *
	 * @return number of columns in the result set.
	 */
	int columnCount() noexcept;

	/**
	 * Get the content of a column as a string.
	 * Returns the content of column 'colNum' as a  utf8 string.
	 *
	 * The access is unchecked:
	 * The behaviour is undefined if colNum not in [0;columnCount()),
	 * or the last call to nextRow() didn't return 100(SQLITE_ROW) or
	 * nextRow() wasn't called since construction ot the last call of nextStatement().
	 * @see https://sqlite.org/c3ref/column_blob.html
	 *
	 * @tparam S Support::String_type that should be returned
	 * @param colNum number of the column from the result to return
	 * @return Text representation of 'colNum', empty string if content is NULL
	 *
	 * @throws Anything thrown by the constructor of S (S() or S(const char*))
	 */
	template<String_type S=std::string>
	S getColumnText(int colNum);

	/**
	 * \copydoc getColumnText
	 */
	template<String_type S>
	S getColumn(int colNum, const S& ={});

	/**
	 * Get the content of a column as an int.
	 * @see https://sqlite.org/c3ref/column_blob.html
	 * @see getColumnText(int)
	 *
	 * @tparam I Integral type to be returned
	 * @param colNum number of the column from the result to return
	 * @return value of 'colNum'
	 */
	template<std::integral I=int>
	I getColumnInt(int colNum) noexcept;

	/**
	 * \copydoc getColumnInt
	 */
	template<Integral_or_enum I>
	I getColumn(int colNum, I ={}) noexcept;

	/**
	 * Prepare the next SQL querry.
	 * Prepare the next SQL querry from the list handed to the constructor.
	 * @throws std::runtime_error if preparing the SQL statement fails.
	 */
	void nextStatement();

private:
	Database& db;
	sqlite3_stmt* sqlStmt;
	const char* nextStmt;

	void prepareStmt();
	friend Database;	//is there another solution to make (private) prepareStmt a friend of Database?
};

//implementation
template<Integral_or_enum I>
I SQLQuerry::getColumn(int colNum, I) noexcept {
	return static_cast<I>(sqlite3_column_int64(sqlStmt, colNum));
}

template<String_type S>
S SQLQuerry::getColumn(int colNum, const S&) {
	const unsigned char* value = sqlite3_column_text(sqlStmt, colNum);
	return value?S{reinterpret_cast<const char*>(value)}:S{};
}

template<String_type S>
S SQLQuerry::getColumnText(int colNum) {
	const unsigned char* value = sqlite3_column_text(sqlStmt, colNum);
	return value?S{reinterpret_cast<const char*>(value)}:S{};
}

template<std::integral I>
I SQLQuerry::getColumnInt(int colNum) noexcept {
	return sqlite3_column_int64(sqlStmt, colNum);
}

} /* namespace SQLiteAdapter */
} /* namespace PhotoLibrary */

#endif /* SRC_SQLQUERRY_H_ */
