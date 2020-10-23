/*
 * suppport.h
 *
 *  Created on: 15 Sep 2020
 *      Author: Sven Rieper
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
