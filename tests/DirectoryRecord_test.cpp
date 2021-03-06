/*
 * Keyword_test.cpp
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

#include <catch2/catch.hpp>
#include "../src/Backend/Record/DirectoryRecord.h"

namespace PhotoLibrary {
namespace Adapter {

using Backend::DirectoryRecord;

TEST_CASE( "test operator== and operator!= for class DirectoryRecord", "[directory][record]" ) {
	DirectoryRecord empty;
	DirectoryRecord empty2;
	CHECK(empty == empty);
	CHECK(empty == empty2);

	DirectoryRecord d2017(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017");
	DirectoryRecord d2017_2(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017");
	DirectoryRecord d2017_options(0, DirectoryRecord::Options::ROW_EXPANDED, "2017", "/home/user/Photos/2017");
	CHECK(d2017 == d2017_2);
	CHECK(d2017 != d2017_options);
	CHECK(d2017_2 != empty);

	DirectoryRecord name(5, DirectoryRecord::Options::ROW_EXPANDED, "Name", "Name");
	DirectoryRecord name_root(7, DirectoryRecord::Options::ROW_EXPANDED, "Name", "Name");
	DirectoryRecord name_name(5, DirectoryRecord::Options::ROW_EXPANDED, "name", "Name");
	DirectoryRecord name_fullname(5, DirectoryRecord::Options::ROW_EXPANDED, "Name", "name");
	CHECK(name == DirectoryRecord(5, DirectoryRecord::Options::ROW_EXPANDED, "Name", "Name"));
	CHECK(name != name_root);
	CHECK(name != name_name);
	CHECK(name != name_fullname);
	CHECK(name_root != name_name);
	CHECK(name_root != name_fullname);
	CHECK(name_name != name_fullname);
}

} /* namespace Adapter */
} /* namespace PhotoLibrary */
