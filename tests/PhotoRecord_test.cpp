/*
 * Keyword_test.cpp
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

namespace PhotoLibrary {

using Photo = Backend::PhotoRecord;
using namespace Backend;

TEST_CASE( "test operator== and operator!= for class PhotoRecord", "[photos][record]" ) {
	Photo default_photo;
	Photo default_photo2;
	CHECK(default_photo == default_photo);
	CHECK(default_photo == default_photo2);

	/// \todo add more tests
}

TEST_CASE("test constructors using Glib::ustring&& and const Glib::ustring& for class PhotoRecord", "[photos][record]") {
	Glib::ustring filename{"some_name"};
	Photo photo_copying_filename{0, filename};
	Photo photo_moving_fileanem{0, std::move(filename)};

	CHECK(photo_copying_filename == photo_moving_fileanem);
}

} /* namespace PhotoLibrary */
