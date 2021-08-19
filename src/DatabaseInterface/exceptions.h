/*
 * exceptions.h
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

#ifndef SRC_DATABASEINTERFACE_EXCEPTIONS_H_
#define SRC_DATABASEINTERFACE_EXCEPTIONS_H_

#include <stdexcept>

namespace PhotoLibrary {
namespace DatabaseInterface {

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


} /* DatabaseInterface */
} /* PhotoLibrary */

#endif /* SRC_DATABASEINTERFACE_EXCEPTIONS_H_*/

