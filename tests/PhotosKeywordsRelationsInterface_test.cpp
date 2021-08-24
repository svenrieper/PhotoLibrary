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

#include "BackendFactory.h"
#include "Record/KeywordRecord.h"
#include "Record/PhotoRecord.h"
#include "exceptions.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace Tests {

using RecordClasses::KeywordRecord;
using RecordClasses::PhotoRecord;
using PhotoLibrary::DatabaseInterface::constraint_error;
using Relations = BackendFactory::Relations;

TEST_CASE(
		"Test the PhotosKeywordsRelations interface of the Adapter and the Backend"
		, "[relations][PhotosKeywordsRelations][adapter][backend]"
		) {
	BackendFactory db { ":memory:" };

	SECTION("getEntries should return an empty vector for an empty database", "[getEntries][getNumberEntries]") {
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(0).size() == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(0)== 0);
	}

	SECTION(
			"getCollections should return an empty vector for an empty database",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(0).size() == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(0) == 0);
	}

	KeywordRecord keyword_record(0, KeywordRecord::Options::PRIVATE, "Holiday");
	db.template newEntry(keyword_record);
	int k_holiday = db.template getID<KeywordRecord>(keyword_record);

	keyword_record = KeywordRecord(0, KeywordRecord::Options::NONE, "Zoo");
	db.template newEntry(keyword_record);
	int k_zoo = db.template getID<KeywordRecord>(keyword_record);

	keyword_record = KeywordRecord(k_holiday, KeywordRecord::Options::INCLUDE_ON_EXPORT, "City Trips");
	db.template newEntry(keyword_record);
	int k_city_trips = db.template getID<KeywordRecord>(keyword_record);

	keyword_record = KeywordRecord(
			k_city_trips,
			KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT,
			"Venice"
			);
	db.template newEntry(keyword_record);
	int k_venice = db.template getID<KeywordRecord>(keyword_record);

	SECTION(
			"A keyword with no photos assinged to it should not have any photos assigned to it.",
			"[getEntries][getNumberEntries]"
			) {
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_holiday).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_zoo).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips).size() == 0);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice).size() == 0);

		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_holiday) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_zoo) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips) == 0);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_venice) == 0);
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
			"A photo should not be assigned to any keywords if it's not asigned to any.",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_2).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_4).size() == 0);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_5).size() == 0);

		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_1) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_2) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_3) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_4) == 0);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_5) == 0);
	}

	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_holiday);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_2, k_holiday);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_4, k_holiday);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_5, k_holiday);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_3, k_holiday);

	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_3, k_city_trips);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_4, k_city_trips);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_city_trips);

	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_zoo);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_4, k_zoo);

	SECTION(
			"The number of photos assigned to a keyword should be equal to the number of photos added.",
			"[getEntries][getNumberEntries]"
			) {
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_holiday).size() == 5);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_zoo).size() == 2);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips).size() == 3);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice).size() == 0);

		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_holiday) == 5);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_zoo) == 2);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips) == 3);
		CHECK(db.getNumberEntries<Relations::PHOTOS_KEYWORDS>(k_venice) == 0);
	}

	SECTION(
			"getEntries(int) should return a vector that contains all the photos that were added to it.",
			"[getEntries]"
			) {
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_holiday),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_2, photo_3, photo_4, photo_5})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_3, photo_4, photo_1})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_zoo),
				Catch::Matchers::UnorderedEquals(std::vector<int>{photo_1, photo_4})
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice),
				Catch::Matchers::UnorderedEquals(std::vector<int>{})
				);
	}

	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_venice);
	db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_4, k_venice);

	SECTION(
			"A photo should be assigned to as many keywords as it was assigned to.",
			"[getCollections][getNumberCollections]"
			) {
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size() == 4);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_2).size() == 1);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3).size() == 2);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_4).size() == 4);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_5).size() == 1);

		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_1) == 4);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_2) == 1);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_3) == 2);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_4) == 4);
		CHECK(db.getNumberCollections<Relations::PHOTOS_KEYWORDS>(photo_5) == 1);
	}

	SECTION(
			"getCollections(int) should return a vector that contains all the keywords a photo was assigned to.",
			"[getCollections]"
			) {
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1),
				Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips, k_venice, k_zoo})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_2),
				Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3),
				Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_4),
				Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday, k_city_trips, k_venice, k_zoo})
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_5),
				Catch::Matchers::UnorderedEquals(std::vector<int>{k_holiday})
				);
	}

	SECTION(
			"After deleting a relation the photo shouldn't be assigned to the keyword and vice versa."
			"Other relations should not be affected.",
			"[deleteRelation]"
			) {
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1),
				Catch::Matchers::VectorContains(k_venice)
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3),
				Catch::Matchers::VectorContains(k_city_trips)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice),
				Catch::Matchers::VectorContains(photo_1)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips),
				Catch::Matchers::VectorContains(photo_3)
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3),
				Catch::Matchers::VectorContains(k_holiday)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice),
				Catch::Matchers::VectorContains(photo_4)
				);

		db.deleteRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_venice);
		db.deleteRelation<Relations::PHOTOS_KEYWORDS>(photo_3, k_city_trips);

		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1),
				!Catch::Matchers::VectorContains(k_venice)
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3),
				!Catch::Matchers::VectorContains(k_city_trips)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice),
				!Catch::Matchers::VectorContains(photo_1)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips),
				!Catch::Matchers::VectorContains(photo_3)
				);
		CHECK_THAT(
				db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_3),
				Catch::Matchers::VectorContains(k_holiday)
				);
		CHECK_THAT(
				db.getEntries<Relations::PHOTOS_KEYWORDS>(k_venice),
				Catch::Matchers::VectorContains(photo_4)
				);
	}

	SECTION("Adding a photo twice to the same keyword should not have any effect.", "[newRelation]") {
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_holiday));
		auto n_1 = db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size();
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, k_holiday));
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size() == n_1);

		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_3, k_zoo));
		auto n_3 = db.getEntries<Relations::PHOTOS_KEYWORDS>(k_zoo).size();
		CHECK_NOTHROW(db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_3, k_zoo));
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_zoo).size() == n_3);
	}

	SECTION("Assigning a photo to a non-existing keyword should return a constraint_error", "[newRelation]") {
		int i = 51;
		while(i == k_holiday || i == k_zoo || i == k_city_trips || i == k_venice)
			++i;

		CHECK_THROWS_AS(db.newRelation<Relations::PHOTOS_KEYWORDS>(photo_1, i), constraint_error);
	}

	SECTION("Adding a non-existing photo to a keyword should return a constraint_error", "[newRelation]") {
		int i = 51;
		while(i == photo_1 || i == photo_2 || i == photo_3 || i == photo_4 || i == photo_5)
			++i;

		CHECK_THROWS_AS(db.newRelation<Relations::PHOTOS_KEYWORDS>(i, k_city_trips), constraint_error);
	}

	SECTION("After deleting a photo all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size() != 0);
		db.template deleteEntry<PhotoRecord>(photo_1);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_1).size() == 0);

		CHECK_NOFAIL(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_4).size() != 0);
		db.template deleteEntry<PhotoRecord>(photo_4);
		CHECK(db.getCollections<Relations::PHOTOS_KEYWORDS>(photo_4).size() == 0);
	}

	SECTION("After deleting a keyword all its relations should be removed as well.", "[deleteEntry]") {
		CHECK_NOFAIL(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_holiday).size() != 0);
		db.template deleteEntry<KeywordRecord>(k_holiday);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_holiday).size() == 0);

		CHECK_NOFAIL(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips).size() != 0);
		db.template deleteEntry<KeywordRecord>(k_city_trips);
		CHECK(db.getEntries<Relations::PHOTOS_KEYWORDS>(k_city_trips).size() == 0);
	}
}


} /* namespace Tests */
} /* namespace Backend */
} /* namespace PhotoLibrary */
