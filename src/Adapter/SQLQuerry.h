/*
 * SQLQuerry.h
 *
 *  Created on: 20 Sep 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_SQLQUERRY_H_
#define SRC_SQLQUERRY_H_

#include <string>
#include <sqlite3.h>
#include <cstdint>

namespace PhotoLibrary {
namespace Adapter {

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
	 * @throws std::runtime_error if preparing the first SQL statement fails.
	 * @param db handle for the database to send the querry to
	 * @param querry semicolon seperated list of SQL querries
	 */
	SQLQuerry(Database* db, const char* querry);
	~SQLQuerry();

	/**
	 * Evalueate the SQL statement.
	 * Must be called at least once for every SQL querry.
	 * @see https://sqlite.org/c3ref/step.html
	 *
	 * @return returns the return value of sqlite3_step(sqlite3_stmt*)
	 */
	int nextRow();

	/**
	 * Get the number of columns in the result set.
	 * @see https://sqlite.org/c3ref/column_count.html
	 *
	 * @return number of columns in the result set.
	 */
	int columnCount();

	/**
	 * Get the content of a column as a string.
	 * Returns the content of column 'colNum' as a  c-string.
	 * @see https://sqlite.org/c3ref/column_blob.html
	 *
	 * @param colNum number of the column from the result to return
	 * @return content of 'colNum' converted to a c-string
	 */
	const char* getColumnText(int colNum);

	/**
	 * Get the content of a column as an int.
	 * @see https://sqlite.org/c3ref/column_blob.html
	 *
	 * @param colNum number of the column from the result to return
	 * @return value of 'colNum'
	 */
	int getColumnInt(int colNum);

	/**
	 * Get the content of a column as an int_least64_t.
	 * @see https://sqlite.org/c3ref/column_blob.html
	 *
	 * @param colNum number of the column from the result to return
	 * @return value of 'colNum'
	 */
	int_least64_t getColumnInt64(int colNum);

	/**
	 * Prepare the next SQL querry.
	 * Prepare the next SQL querry from the list handed to the constructor.
	 * @throws std::runtime_error if preparing the SQL statement fails.
	 */
	void nextStatement();

private:
	Database* db;
	sqlite3_stmt* sqlStmt;
	const char* nextStmt;

	void prepareStmt();
	friend Database;	//is there another solution to make (private) prepareStmt a friend of Database?
};

} /* namespace Adapter */
} /* namespace PhotoLibrary */

#endif /* SRC_SQLQUERRY_H_ */
