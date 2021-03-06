/*
 * suppport.cpp
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

#include "suppport.h"
#include <algorithm>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

void escapeSingleQuotes(Glib::ustring* string) {
	for(auto iter = string->begin();
			(iter = std::find(iter, string->end(), '\'')) != string->end();
			++(++iter))
		iter = string->insert(iter, '\'');
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
