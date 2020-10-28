/*
 * suppport.h
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

#ifndef SRC_SUPPPORT_H_
#define SRC_SUPPPORT_H_

#include <glibmm/ustring.h>
#include "SQLQuerry.h"

namespace PhotoLibrary {
namespace Adapter {

/**
 * Escapes single quotes (') with single quotes for SQLite querries.
 * @param[in, out] string the string to be escaped
 */
void escapeSingleQuotes(Glib::ustring* string);

//namespace Adapter {
/**
 * Thrown to indicate constraint violations.
 * Thrown when INSERT|s and UPDATE|s in the databaes fail due to constraint violations
 * so that they can be discriminated from other runtime_error|s and handled appropriately.
 */
class constraint_error : public std::runtime_error {
public:
	constraint_error(const char* error_message) : std::runtime_error(error_message) {}
	constraint_error(std::string error_message) : std::runtime_error(error_message) {}
};

} /* namespace Adapter */
} /* namespace PhotoLibrary */


#endif /* SRC_SUPPPORT_H_ */
