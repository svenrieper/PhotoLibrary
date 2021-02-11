/*
 * PhotosKeywordsRelationsInterface_test.cpp
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

#include "../src/Backend/DatabaseInterface/DatabaseFactory.h"
#include "../src/Backend/BackendFactory.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {

using PhotoLibrary::Backend::DatabaseInterface::DatabaseFactory;
using PhotoLibrary::Backend::BackendFactory;
using Backend::RelationsInterfaceBase;
using Backend::KeywordRecord;
using Backend::PhotoRecord;
using KeywordInterface = Backend::InterfaceBase<Backend::KeywordRecord>;
using PhotoInterface = Backend::InterfaceBase<Backend::PhotoRecord>;

TEMPLATE_TEST_CASE("Test the PhotosKeywordsRelations interface of the Adapter and the Backend", "[relations][PhotosKeywordsRelations][adapter][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	RelationsInterfaceBase* photos_keywords_realtions_if = db.getPhotosKeywordsRelationsInterface();
	KeywordInterface* keyword_if = db.getKeywordInterface();
	PhotoInterface* photo_if = db.getPhotoInterface();

	SECTION("getEntries should return an empty vector for an empty database", "[getEntries][getNumberEntries]") {
		CHECK(photos_keywords_realtions_if->getEntries(0).size() == 0);
		CHECK(photos_keywords_realtions_if->getNumberEntries(0)== 0);
	}

	SECTION("getCollections should return an empty vector for an empty database", "[getCollections][getNumberCollections]") {
		CHECK(photos_keywords_realtions_if->getCollections(0).size() == 0);
		CHECK(photos_keywords_realtions_if->getNumberCollections(0) == 0);
	}

	KeywordRecord keyword_record(0, KeywordRecord::Options::PRIVATE, "Holiday");
	keyword_if->newEntry(keyword_record);
	int k_holiday = keyword_if->getID(keyword_record);

	keyword_record = KeywordRecord(0, KeywordRecord::Options::NONE, "Zoo");
	keyword_if->newEntry(keyword_record);
	int k_zoo = keyword_if->getID(keyword_record);

	keyword_record = KeywordRecord(k_holiday, KeywordRecord::Options::INCLUDE_ON_EXPORT, "City Trips");
	keyword_if->newEntry(keyword_record);
	int k_city_trips = keyword_if->getID(keyword_record);

	keyword_record = KeywordRecord(k_city_trips, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Venice");
	keyword_if->newEntry(keyword_record);
	int k_venice = keyword_if->getID(keyword_record);

	SECTION("A keyword with no photos assinged to it should not have any photos assigned to it.", "[getEntries][getNumberEntries]") {
		CHECK(photos_keywords_realtions_if->getEntries(k_holiday).size() == 0);
		CHECK(photos_keywords_realtions_if->getEntries(k_zoo).size() == 0);
		CHECK(photos_keywords_realtions_if->getEntries(k_city_trips).size() == 0);
		CHECK(photos_keywords_realtions_if->getEntries(k_venice).size() == 0);

		CHECK(photos_keywords_realtions_if->getNumberEntries(k_holiday) == 0);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_zoo) == 0);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_city_trips) == 0);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_venice) == 0);
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

	SECTION("A photo should not be assigned to any keywords if it's not asigned to any.", "[getCollections][getNumberCollections]") {
		CHECK(photos_keywords_realtions_if->getCollections(photo_1).size() == 0);
		CHECK(photos_keywords_realtions_if->getCollections(photo_2).size() == 0);
		CHECK(photos_keywords_realtions_if->getCollections(photo_3).size() == 0);
		CHECK(photos_keywords_realtions_if->getCollections(photo_4).size() == 0);
		CHECK(photos_keywords_realtions_if->getCollections(photo_5).size() == 0);

		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_1) == 0);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_2) == 0);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_3) == 0);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_4) == 0);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_5) == 0);
	}

	photos_keywords_realtions_if->newRelation(photo_1, k_holiday);
	photos_keywords_realtions_if->newRelation(photo_2, k_holiday);
	photos_keywords_realtions_if->newRelation(photo_4, k_holiday);
	photos_keywords_realtions_if->newRelation(photo_5, k_holiday);
	photos_keywords_realtions_if->newRelation(photo_3, k_holiday);

	photos_keywords_realtions_if->newRelation(photo_3, k_city_trips);
	photos_keywords_realtions_if->newRelation(photo_4, k_city_trips);
	photos_keywords_realtions_if->newRelation(photo_1, k_city_trips);

	photos_keywords_realtions_if->newRelation(photo_1, k_zoo);
	photos_keywords_realtions_if->newRelation(photo_4, k_zoo);

	SECTION("The number of photos assigned to a keyword should be equal to the number of photos added.", "[getEntries][getNumberEntries]") {
		CHECK(photos_keywords_realtions_if->getEntries(k_holiday).size() == 5);
		CHECK(photos_keywords_realtions_if->getEntries(k_zoo).size() == 2);
		CHECK(photos_keywords_realtions_if->getEntries(k_city_trips).size() == 3);
		CHECK(photos_keywords_realtions_if->getEntries(k_venice).size() == 0);

		CHECK(photos_keywords_realtions_if->getNumberEntries(k_holiday) == 5);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_zoo) == 2);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_city_trips) == 3);
		CHECK(photos_keywords_realtions_if->getNumberEntries(k_venice) == 0);
	}

	SECTION("getEntries(int) should return a vector that contains all the photos that were added to it.", "[getEntries]") {
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_holiday), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_2, photo_3, photo_4, photo_5}));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_city_trips), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_3, photo_4, photo_1}));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_zoo), Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_4}));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_venice), Catch::Matchers::UnorderedEquals(std::vector<int>{}));
	}

	photos_keywords_realtions_if->newRelation(photo_1, k_venice);
	photos_keywords_realtions_if->newRelation(photo_4, k_venice);

	SECTION("A photo should be assigned to as many keywords as it was assigned to.", "[getCollections][getNumberCollections]") {
		CHECK(photos_keywords_realtions_if->getCollections(photo_1).size() == 4);
		CHECK(photos_keywords_realtions_if->getCollections(photo_2).size() == 1);
		CHECK(photos_keywords_realtions_if->getCollections(photo_3).size() == 2);
		CHECK(photos_keywords_realtions_if->getCollections(photo_4).size() == 4);
		CHECK(photos_keywords_realtions_if->getCollections(photo_5).size() == 1);

		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_1) == 4);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_2) == 1);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_3) == 2);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_4) == 4);
		CHECK(photos_keywords_realtions_if->getNumberCollections(photo_5) == 1);
	}

	SECTION("getCollections(int) should return a vector that contains all the keywords a photo was assigned to.", "[getCollections]") {
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_1), Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips, k_venice, k_zoo}));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_2), Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday}));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_3), Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips}));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_4), Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips, k_venice, k_zoo}));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_5), Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday}));
	}

	SECTION("After deleting a relation the photo shouldn't be assigned to the keyword and vice versa. Other relations should not be affected.", "[deleteRelation]") {
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_1), Catch::Matchers::VectorContains(k_venice));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(k_city_trips));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_venice), Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_city_trips), Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(k_holiday));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_venice), Catch::Matchers::VectorContains(photo_4));

		photos_keywords_realtions_if->deleteRelation(photo_1, k_venice);
		photos_keywords_realtions_if->deleteRelation(photo_3, k_city_trips);

		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_1), !Catch::Matchers::VectorContains(k_venice));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_3), !Catch::Matchers::VectorContains(k_city_trips));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_venice), !Catch::Matchers::VectorContains(photo_1));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_city_trips), !Catch::Matchers::VectorContains(photo_3));
		CHECK_THAT(photos_keywords_realtions_if->getCollections(photo_3), Catch::Matchers::VectorContains(k_holiday));
		CHECK_THAT(photos_keywords_realtions_if->getEntries(k_venice), Catch::Matchers::VectorContains(photo_4));
	}

	SECTION("Adding a photo twice to the same keyword should not have any effect.", "[newRelation]") {
		CHECK_NOTHROW(photos_keywords_realtions_if->newRelation(photo_1, k_holiday));
		auto n_1 = photos_keywords_realtions_if->getCollections(photo_1).size();
		CHECK_NOTHROW(photos_keywords_realtions_if->newRelation(photo_1, k_holiday));
		CHECK(photos_keywords_realtions_if->getCollections(photo_1).size() == n_1);

		CHECK_NOTHROW(photos_keywords_realtions_if->newRelation(photo_3, k_zoo));
		auto n_3 = photos_keywords_realtions_if->getEntries(k_zoo).size();
		CHECK_NOTHROW(photos_keywords_realtions_if->newRelation(photo_3, k_zoo));
		CHECK(photos_keywords_realtions_if->getEntries(k_zoo).size() == n_3);
	}

	SECTION("Assigning a photo to a non-existing keyword should return a constraint_error", "[newRelation]") {
		int i = 51;
		while(i == k_holiday || i == k_zoo || i == k_city_trips || i == k_venice)
			++i;

		CHECK_THROWS_AS(photos_keywords_realtions_if->newRelation(photo_1, i), Backend::DatabaseInterface::constraint_error);
	}

	SECTION("Adding a non-existing photo to a keyword should return a constraint_error", "[newRelation]") {
		int i = 51;
		while(i == photo_1 || i == photo_2 || i == photo_3 || i == photo_4 || i == photo_5)
			++i;

		CHECK_THROWS_AS(photos_keywords_realtions_if->newRelation(i, k_city_trips), Backend::DatabaseInterface::constraint_error);
	}

	SECTION("After deleting a photo all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(photos_keywords_realtions_if->getCollections(photo_1).size() != 0);
		photo_if->deleteEntry(photo_1);
		CHECK(photos_keywords_realtions_if->getCollections(photo_1).size() == 0);

		CHECK_NOFAIL(photos_keywords_realtions_if->getCollections(photo_4).size() != 0);
		photo_if->deleteEntry(photo_4);
		CHECK(photos_keywords_realtions_if->getCollections(photo_4).size() == 0);
	}

	SECTION("After deleting a keyword all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(photos_keywords_realtions_if->getEntries(k_holiday).size() != 0);
		keyword_if->deleteEntry(k_holiday);
		CHECK(photos_keywords_realtions_if->getEntries(k_holiday).size() == 0);

		CHECK_NOFAIL(photos_keywords_realtions_if->getEntries(k_city_trips).size() != 0);
		keyword_if->deleteEntry(k_city_trips);
		CHECK(photos_keywords_realtions_if->getEntries(k_city_trips).size() == 0);
	}
}

} /* namespace PhotoLibrary */
