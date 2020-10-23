/*
 * suppport_test.cpp
 *
 *  Created on: 15 Sep 2020
 *      Author: Sven Rieper
 */

#include "../src/Adapter/suppport.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Adapter {

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

} /* namespace Adapter */
} /* namespace PhotoLibrary */
