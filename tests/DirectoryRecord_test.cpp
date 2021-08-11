/*
 * DirectoryRecord_test.cpp
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

#include "../src/Backend/Record/DirectoryRecord.h"
#include <catch2/catch.hpp>
#include <vector>

namespace PhotoLibrary {
namespace Tests {

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

TEST_CASE("test constructors for class DirectoryRecord", "[directories][record]") {
    Glib::ustring name{"some_name"};
	Glib::ustring fullname{"another name"};

	SECTION("Provide all arguments") {
		DirectoryRecord directory1 {0, DirectoryRecord::Options::NONE, name, fullname};
		DirectoryRecord directory2 {0, DirectoryRecord::Options::NONE, Glib::ustring(name), fullname};
		DirectoryRecord directory3 {0, DirectoryRecord::Options::NONE, name, Glib::ustring(fullname)};
		DirectoryRecord directory4 {0, DirectoryRecord::Options::NONE, Glib::ustring(name), Glib::ustring(fullname)};

		CHECK(directory1 == directory2);
		CHECK(directory1 == directory3);
		CHECK(directory1 == directory4);
	}

	SECTION("Default fullname") {
		DirectoryRecord directory1 {0, DirectoryRecord::Options::NONE, name};
		DirectoryRecord directory2 {0, DirectoryRecord::Options::NONE, Glib::ustring(name)};

		CHECK(directory1 == directory2);
	}

	name = "";
	fullname = "";

	SECTION("Default constructor") {
		std::vector<DirectoryRecord> dir_vec;

		dir_vec.push_back({});
		dir_vec.push_back({0});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, name});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, Glib::ustring(name)});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, name, fullname});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, Glib::ustring(name), fullname});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, name, Glib::ustring(fullname)});
		dir_vec.push_back({0, DirectoryRecord::Options::NONE, Glib::ustring(name), Glib::ustring(fullname)});

		for(auto& dir : dir_vec) {
			CHECK(dir == dir_vec[0]);
		}
	}

}

} /* namespcae Tests */
} /* namespace PhotoLibrary */
