/*
 * suppport.cpp
 *
 *  Created on: 15 Sep 2020
 *      Author: Sven Rieper
 */

#include "suppport.h"
#include <algorithm>

namespace PhotoLibrary {
namespace Adapter {

void escapeSingleQuotes(Glib::ustring* string) {
	for(auto iter = string->begin();
			(iter = std::find(iter, string->end(), '\'')) != string->end();
			++(++iter))
		iter = string->insert(iter, '\'');
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
