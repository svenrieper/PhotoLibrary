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
#include "../src/Backend/Record/PhotoRecord.h"

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

} /* namespace PhotoLibrary */
