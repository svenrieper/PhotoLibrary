/*
 * AlbumRecord_test.cpp
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2021 Sven Rieper
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

#include "../src/Backend/Record/AlbumRecord.h"
#include <glibmm/ustring.h>
#include <catch2/catch.hpp>

namespace PhotoLibrary {

using Album = Backend::AlbumRecord;
using namespace Backend;

TEST_CASE( "test operator== and operator!= for class AlbumRecord", "[albums][record]" ) {
	Album default_album;
	Album default_album2;
	CHECK(default_album == default_album);
	CHECK(default_album == default_album2);

	/// \todo add more tests
}

TEST_CASE("test constructors using Glib::ustring&& and const Glib::ustring& for class AlbumRecord", "[albums][record]") {
	Glib::ustring name{"some_name"};
	Album album_copying_name{0, AlbumRecord::Options::NONE, name};
	Album album_moving_name{0, AlbumRecord::Options::NONE, std::move(name)};

	CHECK(album_copying_name == album_moving_name);
}

} /* namespace PhotoLibrary */
