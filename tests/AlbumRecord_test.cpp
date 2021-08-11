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
namespace Tests {

using Album = Backend::RecordClasses::AlbumRecord;
using NewAlbum = Backend::RecordClasses::NewAlbumRecord;
using namespace Backend;

TEST_CASE( "test operator== and operator!= for class AlbumRecord", "[albums][record]" ) {
	Album default_album;
	Album default_album2;
	CHECK(default_album == default_album);
	CHECK(default_album == default_album2);

	/// \todo add more tests
}

TEST_CASE( "test operator== and operator!= for class NewAlbumRecord", "[albums][record]" ) {
	NewAlbum default_album;
	NewAlbum default_album2;
	CHECK(default_album == default_album);
	CHECK(default_album == default_album2);
}

TEST_CASE("test constructors for class AlbumRecord", "[albums][record]") {
	Glib::ustring name{"some_name"};
	SECTION("DirectoryRecord constructors") {
		Album album_copying_name{0, Album::Options::NONE, name};
		Album album_moving_name{0, Album::Options::NONE, std::move(name)};

		CHECK(album_copying_name == album_moving_name);
	}

	SECTION("NewDirectoryRecord constructors") {
		NewAlbum album_copying_name{0, Album::Options::NONE, name};
		NewAlbum album_moving_name{0, Album::Options::NONE, std::move(name)};

		CHECK(album_copying_name == album_moving_name);
	}

	name = "";

	SECTION("DirectoryRecord default constructors") {
		Album album_copying_name{0, Album::Options::NONE, name};
		Album album_moving_name{0, Album::Options::NONE, std::move(name)};
		Album album_default_constucted{};

		CHECK(album_copying_name == album_default_constucted);
		CHECK(album_moving_name == album_default_constucted);
	}

	SECTION("NewDirectoryRecord default constructors") {
		NewAlbum album_copying_name{0, Album::Options::NONE, name};
		NewAlbum album_moving_name{0, Album::Options::NONE, std::move(name)};
		NewAlbum album_default_constucted{};

		CHECK(album_copying_name == album_default_constucted);
		CHECK(album_moving_name == album_default_constucted);
	}
}

} /* namespace Tests */
} /* namespace PhotoLibrary */
