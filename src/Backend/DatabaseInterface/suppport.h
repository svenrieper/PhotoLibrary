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
 * Thrown to indicate constraint violations.
 * Thrown when INSERT|s and UPDATE|s in the databaes fail due to constraint violations
 * so that they can be discriminated from other runtime_error|s and handled appropriately.
 */
class constraint_error : public std::runtime_error {
public:
	/**
	 * @param error_message the value to be returned by constraint_error::what()
	 */
	constraint_error(const char* error_message) : std::runtime_error(error_message) {}

	/**
	 * \copydoc constraint_error(const char* error_message)
	 */
	constraint_error(const std::string& error_message) : std::runtime_error(error_message) {}
};

/**
 * Thrown if an entry couldn't be retrieved from the database
 */
class missing_entry : public std::runtime_error {
public:
	/**
	 * @param error_message the value to be returned by missing_entry::what()
	 */
	missing_entry(const char* error_message) : std::runtime_error(error_message) {}

	/**
	 * \copydoc missing_entry
	 */
	missing_entry(const std::string& error_message) : std::runtime_error(error_message) {}
};

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */


#endif /* SRC_SUPPPORT_H_ */
