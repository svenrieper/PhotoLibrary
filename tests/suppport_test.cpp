/*
 * suppport_test.cpp
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

#include "../src/Backend/DatabaseInterface/suppport.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

TEST_CASE( "escapeSingleQuotes doesn't change a string that doesn't contain singel quotes",
		"[support][ustring][escapeSingleQuotes]" ) {
	Glib::ustring str = "some test string";
	escapeSingleQuotes(&str);
	CHECK(str == "some test string");

	Glib::ustring str2 = "";
	escapeSingleQuotes(&str2);
	CHECK(str2 == "");
}

TEST_CASE( "escapeSingleQuotes escapes singel quotes", "[support][ustring][escapeSingleQuotes]" ) {
	Glib::ustring str = "some 'test' string";
	escapeSingleQuotes(&str);
	CHECK(str == "some ''test'' string");


	Glib::ustring str2 = "'some' ''test'' 'string'''";
	escapeSingleQuotes(&str2);
	CHECK(str2 == "''some'' ''''test'''' ''string''''''");
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
