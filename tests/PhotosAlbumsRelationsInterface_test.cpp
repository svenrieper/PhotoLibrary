/*
 * PhotosAlbumsRelationsInterface_test.cpp
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

#include "BackendFactory.h"
#include "Record/AlbumRecord.h"
#include "Record/PhotoRecord.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace Tests {

using RecordClasses::AlbumRecord;
using RecordClasses::PhotoRecord;
using PhotoLibrary::DatabaseInterface::constraint_error;
using Relations = BackendFactory::Relations;

TEST_CASE(
		"Test the PhotosAlbumsRelations interface of the Adapter and the Backend"
		, "[relations][PhotosAlbumsRelations][adapter][backend]"
		) {
	BackendFactory db { ":memory:" };

	SECTION("getEntries should return an empty vector for an empty database", "[getEntries][getNumberEntries]") {
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(0).size() == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(0)== 0);
	}

	SECTION(
			"getCollections should return an empty vector for an empty database",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(0).size() == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(0) == 0);
	}

	AlbumRecord album_record(0, AlbumRecord::Options::ALBUM_IS_SET, "Holiday");
	db.newEntry(album_record);
	int a_holiday = db.getID(album_record);

	album_record = AlbumRecord(0, AlbumRecord::Options::NONE, "Zoo");
	db.newEntry(album_record);
	int a_zoo = db.getID(album_record);

	album_record = AlbumRecord(a_holiday, AlbumRecord::Options::ALBUM_IS_SET, "City Trips");
	db.newEntry(album_record);
	int a_city_trips = db.getID(album_record);

	album_record = AlbumRecord(a_city_trips, AlbumRecord::Options::NONE, "Venice");
	db.newEntry(album_record);
	int a_venice = db.getID(album_record);

	SECTION("An empty album should not contain any photos.", "[getEntries][getNumberEntries]") {
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_holiday).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_zoo).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice).size() == 0);

		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_holiday) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_zoo) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_city_trips) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_venice) == 0);
	}

	PhotoRecord photo_record(0, "1.jpg", 1, 1604149700, 1920, 1080);
	db.newEntry(photo_record);
	int photo_1 = db.getID(photo_record);

	photo_record = PhotoRecord(0, "2.jpg", 1, 1604149700, 1920, 1080);
	db.newEntry(photo_record);
	int photo_2 = db.getID(photo_record);

	photo_record = PhotoRecord(0, "3.jpg", 3, 1604149700, 1920, 1080);
	db.newEntry(photo_record);
	int photo_3 = db.getID(photo_record);

	photo_record = PhotoRecord(0, "4.jpg", 1, 1604150500, 1920, 1080);
	db.newEntry(photo_record);
	int photo_4 = db.getID(photo_record);

	photo_record = PhotoRecord(0, "5.jpg", 5, 1604150650, 1920, 1080);
	db.newEntry(photo_record);
	int photo_5 = db.getID(photo_record);

	SECTION(
			"A photo should not be in any albums if it's not asigned to any.",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_2).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_4).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_5).size() == 0);

		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_1) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_2) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_3) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_4) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_5) == 0);
	}

	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_holiday);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_2, a_holiday);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_4, a_holiday);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_5, a_holiday);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_3, a_holiday);

	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_3, a_city_trips);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_4, a_city_trips);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_city_trips);

	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_zoo);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_4, a_zoo);

	SECTION(
			"The number of photos in an album should be equal to the number of photos added.",
			"[getEntries][getNumberEntries]"
			) {
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_holiday).size() == 5);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_zoo).size() == 2);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips).size() == 3);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice).size() == 0);

		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_holiday) == 5);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_zoo) == 2);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_city_trips) == 3);
		CHECK(db.getNumberEntries<Relations::PHOTOS_ALBUMS>(a_venice) == 0);
	}

	SECTION(
			"getEntries(int) should return a vector that contains all the photos that were added to it.",
			"[getEntries]"
			) {
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_ALBUMS>(a_holiday),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_2, photo_3, photo_4, photo_5})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_3, photo_4, photo_1})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_ALBUMS>(a_zoo),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_4})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice),
				Catch::Matchers::UnorderedEquals(std::vector<int>{})
				);
	}

	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_venice);
	db.newRelation<Relations::PHOTOS_ALBUMS>(photo_4, a_venice);

	SECTION(
			"A photo should be assigned to as many albums as it was assigned to.",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size() == 4);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_2).size() == 1);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3).size() == 2);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_4).size() == 4);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_5).size() == 1);

		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_1) == 4);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_2) == 1);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_3) == 2);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_4) == 4);
		CHECK(db.getNumberCollections<Relations::PHOTOS_ALBUMS>(photo_5) == 1);
	}

	SECTION(
			"getCollections(int) should return a vector that contains all the albums a photo was assigned to.",
			"[getCollections]"
			) {
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1),
				Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips, a_venice, a_zoo})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_ALBUMS>(photo_2),
				Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3),
				Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_ALBUMS>(photo_4),
				Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips, a_venice, a_zoo})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_ALBUMS>(photo_5),
				Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday})
				);
	}

	SECTION(
			"After deleting a relation the photo shouldn't be assigned to the album and vice versa."
			"Other relations should not be affected.",
			"[deleteRelation]"
			) {
		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1), Catch::Matchers::VectorContains(a_venice));
		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3), Catch::Matchers::VectorContains(a_city_trips));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice), Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips), Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3), Catch::Matchers::VectorContains(a_holiday));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice), Catch::Matchers::VectorContains(photo_4));

		db.deleteRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_venice);
		db.deleteRelation<Relations::PHOTOS_ALBUMS>(photo_3, a_city_trips);

		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1), !Catch::Matchers::VectorContains(a_venice));
		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3), !Catch::Matchers::VectorContains(a_city_trips));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice), !Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips), !Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_3), Catch::Matchers::VectorContains(a_holiday));
		CHECK_THAT(db.getEntries<Relations::PHOTOS_ALBUMS>(a_venice), Catch::Matchers::VectorContains(photo_4));
	}

	SECTION("Adding a photo twice to the same album should not have any effect.", "[newRelation]") {
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_holiday));
		auto n_1 = db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size();
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, a_holiday));
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size() == n_1);

		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_ALBUMS>(photo_3, a_zoo));
		auto n_3 = db.getEntries<Relations::PHOTOS_ALBUMS>(a_zoo).size();
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_ALBUMS>(photo_3, a_zoo));
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_zoo).size() == n_3);
	}

	SECTION("Adding a photo to a non-existing album should return a constraint_error", "[newRelation]") {
		int i = 1;
		while(i == a_holiday || i == a_zoo || i == a_city_trips || i == a_venice)
			++i;

		CHECK_THROWS_AS(db.newRelation<Relations::PHOTOS_ALBUMS>(photo_1, i), constraint_error);
	}

	SECTION("Adding a non-photo to an album should return a constraint_error", "[newRelation]") {
		int i = 1;
		while(i == photo_1 || i == photo_2 || i == photo_3 || i == photo_4 || i == photo_5)
			++i;

		CHECK_THROWS_AS(db.newRelation<Relations::PHOTOS_ALBUMS>(i, a_city_trips), constraint_error);
	}

	SECTION("After deleting a photo all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size() != 0);
		db.template deleteEntry<PhotoRecord>(photo_1);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_1).size() == 0);

		CHECK_NOFAIL(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_4).size() != 0);
		db.template deleteEntry<PhotoRecord>(photo_4);
		CHECK(db.getCollections<Relations::PHOTOS_ALBUMS>(photo_4).size() == 0);
	}

	SECTION("After deleting an album all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(db.getEntries<Relations::PHOTOS_ALBUMS>(a_holiday).size() != 0);
		db.template deleteEntry<AlbumRecord>(a_holiday);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_holiday).size() == 0);

		CHECK_NOFAIL(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips).size() != 0);
		db.template deleteEntry<AlbumRecord>(a_city_trips);
		CHECK(db.getEntries<Relations::PHOTOS_ALBUMS>(a_city_trips).size() == 0);
	}
}

} /* namespace Test */
} /* namespace Backend */
} /* namespace PhotoLibrary */
