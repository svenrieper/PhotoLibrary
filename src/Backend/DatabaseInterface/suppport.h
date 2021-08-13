/*
 * suppport.h
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

#include <glibmm/ustring.h>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

/**
 * Escapes single quotes (') with single quotes for SQLite querries.
 * @param[in, out] string the string to be escaped
 */
void escapeSingleQuotes(Glib::ustring& string);

/**
 * Appends string to SQL command
 * Appends a string (append) to another string for generate an SQL command.
 * escapeSingleQuotes will be applied to append.
 *
 * The version using an rvalue reference will leave append in a valid but
 * changed state.
 *
 * @param sql The (partial) SQL command
 * @param append The string or value to append
 * @param escape Whether append should be enclosed by singel quotes
 * 		(for string values)
 */
void appendSQL(Glib::ustring& sql, Glib::ustring&& append, bool escape=true);

/**
 * \copydoc appendSQL
 */
void appendSQL(Glib::ustring& sql, const Glib::ustring& append, bool escape=true);

/**
 * \copydoc appendSQL
 */
void appendSQL(Glib::ustring& sql, int append, bool /*escape*/=false);

/**
 * Thrown to indicate errors executing SQL commands
 */
class database_error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

/**
 * Thrown to indicate constraint violations.
 * Thrown when INSERT|s and UPDATE|s in the databaes fail due to constraint violations
 * so that they can be discriminated from other runtime_error|s and handled appropriately.
 */
class constraint_error : public database_error {
public:
	using database_error::database_error;
};

/**
 * Thrown if an entry couldn't be retrieved from the database
 */
class missing_entry : public database_error {
public:
	using database_error::database_error;
};

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_SUPPPORT_H_ */
