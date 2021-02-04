/*
 * RelationsDBInterface_test.cpp
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

#include "../src/Adapter/DatabaseFactory.h"
#include "../src/Backend/BackendFactory.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {

using PhotoLibrary::Adapter::DatabaseFactory;
using PhotoLibrary::Backend::BackendFactory;
using Backend::RelationsInterfaceBase;
using Backend::AlbumRecord;
using Backend::PhotoRecord;
using AlbumInterface = Backend::InterfaceBase<Backend::AlbumRecord>;
using PhotoInterface = Backend::InterfaceBase<Backend::PhotoRecord>;

TEMPLATE_TEST_CASE("Test the PhotosAlbumsRelations interface of the Adapter and the Backend", "[relations][PhotosAlbumsRelations][adapter][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	RelationsInterfaceBase* photos_albums_realtions_if = db.getPhotosAlbumsRelationsInterface();
	AlbumInterface* album_if = db.getAlbumInterface();
	PhotoInterface* photo_if = db.getPhotoInterface();

	SECTION("getEntries should return an empty vector for an empty database", "") {
		CHECK(photos_albums_realtions_if->getEntries(0).size() == 0);
	}

	SECTION("getCollections should return an empty vector for an empty database", "") {
		CHECK(photos_albums_realtions_if->getCollections(0).size() == 0);
	}

	AlbumRecord album_record(0, AlbumRecord::Options::ALBUM_IS_SET, "Holiday");
	album_if->newEntry(album_record);
	int a_holiday = album_if->getID(album_record);

	album_record = AlbumRecord(0, AlbumRecord::Options::NONE, "Zoo");
	album_if->newEntry(album_record);
	int a_zoo = album_if->getID(album_record);

	album_record = AlbumRecord(a_holiday, AlbumRecord::Options::ALBUM_IS_SET, "City Trips");
	album_if->newEntry(album_record);
	int a_city_trips = album_if->getID(album_record);

	album_record = AlbumRecord(a_city_trips, AlbumRecord::Options::NONE, "Venice");
	album_if->newEntry(album_record);
	int a_venice = album_if->getID(album_record);

	SECTION("An empty album should not contain any photos.", "") {
		CHECK(photos_albums_realtions_if->getEntries(a_holiday).size() == 0);
		CHECK(photos_albums_realtions_if->getEntries(a_zoo).size() == 0);
		CHECK(photos_albums_realtions_if->getEntries(a_city_trips).size() == 0);
		CHECK(photos_albums_realtions_if->getEntries(a_venice).size() == 0);
	}

	PhotoRecord photo_record(0, "1.jpg", 1, 1604149700, 1920, 1080);
	photo_if->newEntry(photo_record);
	int photo_1 = photo_if->getID(photo_record);

	photo_record = PhotoRecord(0, "2.jpg", 1, 1604149700, 1920, 1080);
	photo_if->newEntry(photo_record);
	int photo_2 = photo_if->getID(photo_record);

	photo_record = PhotoRecord(0, "3.jpg", 3, 1604149700, 1920, 1080);
	photo_if->newEntry(photo_record);
	int photo_3 = photo_if->getID(photo_record);

	photo_record = PhotoRecord(0, "4.jpg", 1, 1604150500, 1920, 1080);
	photo_if->newEntry(photo_record);
	int photo_4 = photo_if->getID(photo_record);

	photo_record = PhotoRecord(0, "5.jpg", 5, 1604150650, 1920, 1080);
	photo_if->newEntry(photo_record);
	int photo_5 = photo_if->getID(photo_record);

	SECTION("A photo should not be in any albums if it's not asigned to any.", "") {
		CHECK(photos_albums_realtions_if->getCollections(photo_1).size() == 0);
		CHECK(photos_albums_realtions_if->getCollections(photo_2).size() == 0);
		CHECK(photos_albums_realtions_if->getCollections(photo_3).size() == 0);
		CHECK(photos_albums_realtions_if->getCollections(photo_4).size() == 0);
		CHECK(photos_albums_realtions_if->getCollections(photo_5).size() == 0);
	}

	photos_albums_realtions_if->newRelation(photo_1, a_holiday);
	photos_albums_realtions_if->newRelation(photo_2, a_holiday);
	photos_albums_realtions_if->newRelation(photo_4, a_holiday);
	photos_albums_realtions_if->newRelation(photo_5, a_holiday);
	photos_albums_realtions_if->newRelation(photo_3, a_holiday);

	photos_albums_realtions_if->newRelation(photo_3, a_city_trips);
	photos_albums_realtions_if->newRelation(photo_4, a_city_trips);
	photos_albums_realtions_if->newRelation(photo_1, a_city_trips);

	photos_albums_realtions_if->newRelation(photo_1, a_zoo);
	photos_albums_realtions_if->newRelation(photo_4, a_zoo);

	SECTION("The number of photos in an album should be equal to the number of photos added.", "") {
		CHECK(photos_albums_realtions_if->getEntries(a_holiday).size() == 5);
		CHECK(photos_albums_realtions_if->getEntries(a_zoo).size() == 2);
		CHECK(photos_albums_realtions_if->getEntries(a_city_trips).size() == 3);
		CHECK(photos_albums_realtions_if->getEntries(a_venice).size() == 0);
	}

	SECTION("getEntries(int) should return a vector that contains all the photos that were added to it.", "") {
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_holiday), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_2, photo_3, photo_4, photo_5}));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_city_trips), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_3, photo_4, photo_1}));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_zoo), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_4}));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_venice), Catch::Matchers::UnorderedEquals(std::vector<int>{}));
	}

	photos_albums_realtions_if->newRelation(photo_1, a_venice);
	photos_albums_realtions_if->newRelation(photo_4, a_venice);

	SECTION("A photo should be assigned to as many albums as it was assigned to.", "") {
		CHECK(photos_albums_realtions_if->getCollections(photo_1).size() == 4);
		CHECK(photos_albums_realtions_if->getCollections(photo_2).size() == 1);
		CHECK(photos_albums_realtions_if->getCollections(photo_3).size() == 2);
		CHECK(photos_albums_realtions_if->getCollections(photo_4).size() == 4);
		CHECK(photos_albums_realtions_if->getCollections(photo_5).size() == 1);
	}

	SECTION("getCollections(int) should return a vector that contains all the albums a photo was assigned to.", "") {
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_1), Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips, a_venice, a_zoo}));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_2), Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday}));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_3), Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips}));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_4), Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday, a_city_trips, a_venice, a_zoo}));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_5), Catch::Matchers::UnorderedEquals(std::vector<int>{a_holiday}));
	}

	SECTION("After deleting a relation the photo shouldn't be assigned to the album and vice versa. Other relations should not be affected.", "") {
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_1), Catch::Matchers::VectorContains(a_venice));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(a_city_trips));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_venice), Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_city_trips), Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(a_holiday));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_venice), Catch::Matchers::VectorContains(photo_4));

		photos_albums_realtions_if->deleteRelation(photo_1, a_venice);
		photos_albums_realtions_if->deleteRelation(photo_3, a_city_trips);

		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_1), !Catch::Matchers::VectorContains(a_venice));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_3), !Catch::Matchers::VectorContains(a_city_trips));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_venice), !Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_city_trips), !Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(photos_albums_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(a_holiday));
		CHECK_THAT(photos_albums_realtions_if->getEntries(a_venice), Catch::Matchers::VectorContains(photo_4));
	}

	SECTION("Adding a photo twice to the same album should not have any effect.", "") {
		CHECK_NOTHROW(photos_albums_realtions_if->newRelation(photo_1, a_holiday));
		auto n_1 = photos_albums_realtions_if->getCollections(photo_1).size();
		CHECK_NOTHROW(photos_albums_realtions_if->newRelation(photo_1, a_holiday));
		CHECK(photos_albums_realtions_if->getCollections(photo_1).size() == n_1);

		CHECK_NOTHROW(photos_albums_realtions_if->newRelation(photo_3, a_zoo));
		auto n_3 = photos_albums_realtions_if->getEntries(a_zoo).size();
		CHECK_NOTHROW(photos_albums_realtions_if->newRelation(photo_3, a_zoo));
		CHECK(photos_albums_realtions_if->getEntries(a_zoo).size() == n_3);
	}

	SECTION("Adding a photo to a non-existing album should return a constraint_error", "") {
		int i = 1;
		while(i == a_holiday || i == a_zoo || i == a_city_trips || i == a_venice)
			++i;

		CHECK_THROWS_AS(photos_albums_realtions_if->newRelation(photo_1, i), Adapter::constraint_error);
	}

	SECTION("Adding a non-photo to an album should return a constraint_error", "") {
		int i = 1;
		while(i == photo_1 || i == photo_2 || i == photo_3 || i == photo_4 || i == photo_5)
			++i;

		CHECK_THROWS_AS(photos_albums_realtions_if->newRelation(i, a_city_trips), Adapter::constraint_error);
	}

	SECTION("After deleting a photo all its relations should be removed as well.", "") {
		CHECK_NOFAIL(photos_albums_realtions_if->getCollections(photo_1).size() != 0);
		photo_if->deleteEntry(photo_1);
		CHECK(photos_albums_realtions_if->getCollections(photo_1).size() == 0);

		CHECK_NOFAIL(photos_albums_realtions_if->getCollections(photo_4).size() != 0);
		photo_if->deleteEntry(photo_4);
		CHECK(photos_albums_realtions_if->getCollections(photo_4).size() == 0);
	}

	SECTION("After deleting an album all its relations should be removed as well.", "") {
		CHECK_NOFAIL(photos_albums_realtions_if->getEntries(a_holiday).size() != 0);
		album_if->deleteEntry(a_holiday);
		CHECK(photos_albums_realtions_if->getEntries(a_holiday).size() == 0);

		CHECK_NOFAIL(photos_albums_realtions_if->getEntries(a_city_trips).size() != 0);
		album_if->deleteEntry(a_city_trips);
		CHECK(photos_albums_realtions_if->getEntries(a_city_trips).size() == 0);
	}
}

} /* namespace PhotoLibrary */
