/*
 * support.h
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

#ifndef SRC_SUPPPORT_H_
#define SRC_SUPPPORT_H_

#include <Concepts.h>
#include <stdexcept>

namespace PhotoLibrary {
namespace DatabaseInterface {

/**
 * Escapes single quotes (') with single quotes for SQLite querries.
 * @tparam String type of argument string
 * @param[in, out] string the string to be escaped
 */
template<SQLiteAdapter::String_type String =std::string>
void escapeSingleQuotes(String& string) {
	for(auto iter = string.begin();
			(iter = std::find(iter, string.end(), '\'')) != string.end();
			++(++iter))
		iter = string.insert(iter, '\'');
}

/**
 * Appends string to SQL command
 * Appends a string (append) to another string for generate an SQL command.
 * escapeSingleQuotes will be applied to append.
 *
 * The version using an rvalue reference will leave append in a valid but
 * changed state.
 *
 * @tparam String type of argument sql and string argument append
 * @param sql The (partial) SQL command
 * @param append The string or value to append
 * @param escape Whether append should be enclosed by singel quotes
 * 		(for string values)
 */
template<SQLiteAdapter::String_type String =std::string>
void appendSQL(String& sql, String&& append, bool escape=true) {
	escapeSingleQuotes(append);
	sql += (escape?"'":"") + append + (escape?"'":"");
}

/**
 * \copydoc appendSQL
 */
template<SQLiteAdapter::String_type String =std::string>
void appendSQL(String& sql, const String& append, bool escape=true) {
	appendSQL(sql, String(append), escape);
}

/**
 * \copydoc appendSQL
 * \todo Add version for enum (classe)s
 */
template<SQLiteAdapter::String_type String =std::string>
void appendSQL(String& sql, int64_t append, bool /*escape*/=false) {
	sql += std::to_string(append);
}

/**
 * Appends the names of all columns to string starting whit the last.
 *
 * Appends the names of all columns (seperated by commas) for the
 * given table to the given string starting with the last columns
 * form RecordType::fields.
 *
 * @tparam RecordType Record based class for the table for which the column
 * 		names should be appended
 * @tparam String Type of argument sql
 * @param[in,out] sql String to which the column names are appended
 */
template<typename RecordType, SQLiteAdapter::String_type String>
void appendFieldNames(String &sql) {
	int i = RecordType::size()-1;
	appendSQL(sql, RecordType::fields[i], false);
	while(i--) {
		sql += ", ";
		appendSQL(sql, RecordType::fields[i], false);
	}
}

/**
 * Appends the names of all columns to string starting with the first.
 *
 * Appends the names of all columns (seperated by commas) for the
 * given table to the given string starting with the first columns
 * form RecordType::fields.
 *
 * @tparam RecordType Record based class for the table for which the column
 * 		names should be appended
 * @tparam String Type of argument sql
 * @param[in,out] sql String to which the column names are appended
 */
//append the names of all data fields (table columns) for GET
template<typename RecordType, SQLiteAdapter::String_type String>
void appendFieldNamesReverse(String &sql) {
	appendSQL(sql, RecordType::fields[0], false);
	for(int i=1; i<RecordType::size(); ++i) {
		sql += ", ";
		appendSQL(sql, RecordType::fields[i], false);
	}
}

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* SRC_SUPPPORT_H_ */
