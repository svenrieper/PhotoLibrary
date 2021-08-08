/*
 * AlbumInterface_test.cpp
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

#include "../src/Backend/DatabaseInterface/DatabaseFactory.h"
#include "../src/Backend/BackendFactory.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

using Backend::AlbumRecord;
using Backend::BackendFactory;
using Backend::InterfaceBase;
using AlbumInterface = InterfaceBase<Backend::AlbumRecord>;

TEMPLATE_TEST_CASE("Test the AlbumInterface of the backend", "[album][interface][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	AlbumInterface* albumIF = db.getAlbumInterface();

	SECTION( "An empty database should contain no albums", "[album][database][getChildren][getNumberChildren]" ) {
		CHECK(albumIF->getChildren(0).size() == 0);
		CHECK(albumIF->getNumberChildren(0) == 0);
	}

	SECTION( "An album needs to have an existing parent", "[album][database]" ) {
		AlbumRecord invalid(5);
		CHECK_THROWS_AS(albumIF->newEntry(invalid), constraint_error);
	}

	SECTION("An album should be in the database after being added (newEntry(AlbumRecord), getID(AlbumRecord))", "[album][database][newEntry][getID]") {
		//root
		std::vector<AlbumRecord> album_vec;
		std::vector<int> album_ids;
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "2014"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::ROW_EXPANDED | AlbumRecord::Options::ALBUM_IS_SET, "2015"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::NONE, "2016"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET | AlbumRecord::Options::ROW_EXPANDED, "2017"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::ROW_EXPANDED | AlbumRecord::Options::ALBUM_IS_SET, "Some album"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::NONE, "2019"));
		album_vec.push_back(AlbumRecord(0, AlbumRecord::Options::NONE, "2020"));

		for(auto album : album_vec)
			albumIF->newEntry(album);

		for(auto album : album_vec)
			REQUIRE_NOTHROW(album_ids.push_back(albumIF->getID(album)));

		//2014
		int id_2014 = album_ids[0];
		std::vector<AlbumRecord> album_2014_children;
		std::vector<int> album_2014_ids;
		album_2014_children.push_back(AlbumRecord(id_2014, AlbumRecord::Options::NONE, "01"));
		album_2014_children.push_back(AlbumRecord(id_2014, AlbumRecord::Options::ALBUM_IS_SET, "05"));
		album_2014_children.push_back(AlbumRecord(id_2014, AlbumRecord::Options::NONE, "06"));
		album_2014_children.push_back(AlbumRecord(id_2014, AlbumRecord::Options::ROW_EXPANDED | AlbumRecord::Options::ALBUM_IS_SET, "07"));
		album_2014_children.push_back(AlbumRecord(id_2014, AlbumRecord::Options::NONE, "some album"));

		for(auto album : album_2014_children)
			albumIF->newEntry(album);

		for(auto album : album_2014_children)
			REQUIRE_NOTHROW(album_2014_ids.push_back(albumIF->getID(album)));

		//2017
		int id_2017 = album_ids[3];
		std::vector<int> album_2017_ids;
		std::vector<AlbumRecord> album_2017_children;
		album_2017_children.push_back(AlbumRecord(id_2017, AlbumRecord::Options::ALBUM_IS_SET, "02"));
		album_2017_children.push_back(AlbumRecord(id_2017, AlbumRecord::Options::ALBUM_IS_SET, "some set"));
		album_2017_children.push_back(AlbumRecord(id_2017, AlbumRecord::Options::NONE, "07"));
		album_2017_children.push_back(AlbumRecord(id_2017, AlbumRecord::Options::NONE, "some album"));
		album_2017_children.push_back(AlbumRecord(id_2017, AlbumRecord::Options::ROW_EXPANDED | AlbumRecord::Options::ALBUM_IS_SET, "12"));

		for(auto album : album_2017_children)
			albumIF->newEntry(album);

		for(auto album : album_2017_children)
			REQUIRE_NOTHROW(album_2017_ids.push_back(albumIF->getID(album)));

		//2017/06
		int id_2017_06 = album_2017_ids[2];
		std::vector<int> album_2017_06_ids;
		std::vector<AlbumRecord> album_2017_06_children;
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "02"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "09"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "13"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "16"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "17"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "22"));
		album_2017_06_children.push_back(AlbumRecord(id_2017_06, AlbumRecord::Options::NONE, "30"));

		for(auto album : album_2017_06_children)
			albumIF->newEntry(album);

		for(auto album : album_2017_06_children)
			REQUIRE_NOTHROW(album_2017_06_ids.push_back(albumIF->getID(album)));


		SECTION("getChildren(int) returns vector of children", "[getChildren][getNumberChildren]") {
			CHECK_THAT(albumIF->getChildren(0), Catch::Matchers::UnorderedEquals(album_ids));
			CHECK_THAT(albumIF->getChildren(id_2014), Catch::Matchers::UnorderedEquals(album_2014_ids));
			CHECK_THAT(albumIF->getChildren(id_2017), Catch::Matchers::UnorderedEquals(album_2017_ids));
			CHECK_THAT(albumIF->getChildren(id_2017_06), Catch::Matchers::UnorderedEquals(album_2017_06_ids));

			CHECK(albumIF->getChildren(0).size() == albumIF->getNumberChildren(0));
			CHECK(albumIF->getChildren(id_2014).size() == albumIF->getNumberChildren(id_2014));
			CHECK(albumIF->getChildren(id_2017).size() == albumIF->getNumberChildren(id_2017));
			CHECK(albumIF->getChildren(id_2017_06).size() == albumIF->getNumberChildren(id_2017_06));
		}


		SECTION("getEntry(int) == entry entered", "[getEntry]") {
			for(unsigned long i=0; i<album_ids.size(); ++i)
				CHECK(album_vec[i] == albumIF->getEntry(album_ids[i]));
			for(unsigned long i=0; i<album_2014_ids.size(); ++i)
				CHECK(album_2014_children[i] == albumIF->getEntry(album_2014_ids[i]));
			for(unsigned long i=0; i<album_2017_ids.size(); ++i)
				CHECK(album_2017_children[i] == albumIF->getEntry(album_2017_ids[i]));
			for(unsigned long i=0; i<album_2017_06_ids.size(); ++i)
				CHECK(album_2017_06_children[i] == albumIF->getEntry(album_2017_06_ids[i]));
		}


		SECTION("An album isn't in the database anymore after it was deleted (deleteEntry(int))", "[deleteEntry]") {
			albumIF->deleteEntry(album_ids[2]);
			CHECK_THROWS_AS(albumIF->getEntry(album_ids[2]), std::runtime_error);

			albumIF->deleteEntry(album_2017_06_ids[4]);
			CHECK_THROWS_AS(albumIF->getEntry(album_2017_06_ids[4]), std::runtime_error);

			albumIF->deleteEntry(album_2014_ids[0]);
			CHECK_THROWS_AS(albumIF->getEntry(album_2014_ids[0]), std::runtime_error);
		}

		SECTION("All children are deleted along with a set", "[deleteEntry]") {
			albumIF->deleteEntry(id_2014);
			CHECK_THROWS_AS(albumIF->getEntry(id_2014), std::runtime_error);

			for(int album_id : album_2014_ids)
				CHECK_THROWS_AS(albumIF->getEntry(album_id), std::runtime_error);

			albumIF->deleteEntry(id_2017);
			CHECK_THROWS_AS(albumIF->getEntry(id_2017), std::runtime_error);

			for(int album_id : album_2017_ids)
				CHECK_THROWS_AS(albumIF->getEntry(album_id), std::runtime_error);

			for(int album_id : album_2017_06_ids)
				CHECK_THROWS_AS(albumIF->getEntry(album_id), std::runtime_error);
		}

		SECTION("After updating an entry the updated entry should be in the database", "[updateEntry]") {
			album_2014_children[0].setAlbumName() = "some other name";
			album_2017_06_children[3].setAlbumName() = "new name";

			REQUIRE_NOTHROW(albumIF->updateEntry(album_2014_ids[0], album_2014_children[0]));
			REQUIRE_NOTHROW(albumIF->updateEntry(album_2017_06_ids[3], album_2017_06_children[3]));

			for(unsigned long i=0; i<album_ids.size(); ++i)
				CHECK(album_vec[i] == albumIF->getEntry(album_ids[i]));
			for(unsigned long i=0; i<album_2014_ids.size(); ++i)
				CHECK(album_2014_children[i] == albumIF->getEntry(album_2014_ids[i]));
			for(unsigned long i=0; i<album_2017_ids.size(); ++i)
				CHECK(album_2017_children[i] == albumIF->getEntry(album_2017_ids[i]));
			for(unsigned long i=0; i<album_2017_06_ids.size(); ++i)
				CHECK(album_2017_06_children[i] == albumIF->getEntry(album_2017_06_ids[i]));
		}

		SECTION("After moving an album, it should be in the new set", "[setParent]") {
			albumIF->setParent(album_2017_06_ids[4], album_ids[4]);
			CHECK_THAT(albumIF->getChildren(album_ids[4]), Catch::Matchers::VectorContains(album_2017_06_ids[4]));
		}
	}
}

TEST_CASE("BackendFactory::getInterface<AlbumRecord>() returns the album interface", "[album][backend]") {
	BackendFactory db {":memory:"};
	CHECK(db.getAlbumInterface() == db.getInterface<AlbumRecord>());
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
