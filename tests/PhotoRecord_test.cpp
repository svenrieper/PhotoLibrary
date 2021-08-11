/*
 * PhotoRecord_test.cpp
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

#include "../src/Backend/Record/PhotoRecord.h"
#include <glibmm/ustring.h>
#include <catch2/catch.hpp>
#include <vector>

namespace PhotoLibrary {
namespace Tests {

using Photo = Backend::PhotoRecord;
using namespace Backend;

TEST_CASE( "test operator== and operator!= for class PhotoRecord", "[photos][record]" ) {
	Photo default_photo;
	Photo default_photo2;
	CHECK(default_photo == default_photo);
	CHECK(default_photo == default_photo2);

	/// \todo add more tests
}

TEST_CASE("test constructors for class PhotoRecord", "[photos][record]") {
	Glib::ustring filename{"some_name"};

	SECTION("Provide all arguments") {
		Photo photo_copying_filename{0, filename, 0, 2255656663, 2563, 1560};
		Photo photo_moving_fileanem{0, std::move(filename), 0, 2255656663, 2563, 1560};

		CHECK(photo_copying_filename == photo_moving_fileanem);
	}

	filename = "";

	SECTION("Default constructor") {
		std::vector<Photo> photo_vec;

		photo_vec.push_back({});
		photo_vec.push_back({0});
		photo_vec.push_back({0, filename});
		photo_vec.push_back({0, Glib::ustring(filename)});
		photo_vec.push_back({0, filename, 0});
		photo_vec.push_back({0, Glib::ustring(filename), 0});
		photo_vec.push_back({0, filename, 0, 0});
		photo_vec.push_back({0, Glib::ustring(filename), 0, 0});
		photo_vec.push_back({0, filename, 0, 0, 0});
		photo_vec.push_back({0, Glib::ustring(filename), 0, 0,0 });
		photo_vec.push_back({0, filename, 0, 0, 0, 0});
		photo_vec.push_back({0, Glib::ustring(filename), 0, 0, 0, 0});

		for(auto& photo : photo_vec) {
			CHECK(photo == photo_vec[0]);
		}
	}
}

} /* namespace Tests */
} /* namespace PhotoLibrary */
