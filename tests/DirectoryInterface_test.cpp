/*
 * DirectoryInterface_test.cpp
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

#include "../src/Backend/DatabaseInterface/DatabaseFactory.h"
#include "../src/Backend/BackendFactory.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

using Backend::DirectoryRecord;
using Backend::BackendFactory;
using Backend::InterfaceBase;
using DirectoryInterface = InterfaceBase<Backend::DirectoryRecord>;


TEMPLATE_TEST_CASE("Test the DirectoryInterface of the Adapter and the Backend", "[directory][adapter][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	DirectoryInterface* dirIF = db.getDirectoriesInterface();

	SECTION( "An empty database should contain no directories", "[directory][database][getChildren][getNumberChildren]" ) {
//		std::vector<int> children_of_root = dirIF->getChildren(0);
		CHECK(dirIF->getChildren(0).size() == 0);
		CHECK(dirIF->getNumberChildren(0) == 0);
	}

	SECTION( "A directory needs to have a parent or 0", "[directory][database][newEntry]" ) {
		DirectoryRecord invalid(5);
		CHECK_THROWS_AS(dirIF->newEntry(invalid), constraint_error);
	}

	SECTION("A directory should be in the database after being entered", "[directory][database]") {
		//root
		std::vector<DirectoryRecord> dir_vec;
		std::vector<int> dir_ids;
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2014", "/home/user/Photos/2014"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::ROW_EXPANDED, "2015", "/home/user/Photos/2015"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2016", "/home/user/Photos/2016"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::ROW_EXPANDED, "2018", "/home/user/Photos/2018"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2019", "/home/user/Photos/2019"));
		dir_vec.push_back(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2020", "/home/user/Photos/2020"));

		for(auto dir : dir_vec)
			dirIF->newEntry(dir);

		SECTION("getID()") {
			for(auto dir : dir_vec)
				REQUIRE_NOTHROW(dirIF->getID(dir));
		}

		for(auto dir : dir_vec)
			dir_ids.push_back(dirIF->getID(dir));

		//2014
		int id_2014 = dir_ids[0];
		std::vector<DirectoryRecord> dir_2014_children;
		std::vector<int> dir_2014_ids;
		dir_2014_children.push_back(DirectoryRecord(id_2014, DirectoryRecord::Options::NONE, "01", "01"));
		dir_2014_children.push_back(DirectoryRecord(id_2014, DirectoryRecord::Options::NONE, "05", "05"));
		dir_2014_children.push_back(DirectoryRecord(id_2014, DirectoryRecord::Options::NONE, "06", "06"));
		dir_2014_children.push_back(DirectoryRecord(id_2014, DirectoryRecord::Options::NONE, "07", "07"));
		dir_2014_children.push_back(DirectoryRecord(id_2014, DirectoryRecord::Options::NONE, "10", "10"));

		for (auto dir : dir_2014_children)
			dirIF->newEntry(dir);

		SECTION("getID() children of 2014") {
			for (auto dir : dir_2014_children)
				REQUIRE_NOTHROW(dirIF->getID(dir));
		}

		for(auto dir : dir_2014_children)
			dir_2014_ids.push_back(dirIF->getID(dir));

		//2017
		int id_2017 = dir_ids[3];
		std::vector<int> dir_2017_ids;
		std::vector<DirectoryRecord> dir_2017_children;
		dir_2017_children.push_back(DirectoryRecord(id_2017, DirectoryRecord::Options::NONE, "02", "02"));
		dir_2017_children.push_back(DirectoryRecord(id_2017, DirectoryRecord::Options::NONE, "06", "06"));
		dir_2017_children.push_back(DirectoryRecord(id_2017, DirectoryRecord::Options::NONE, "07", "07"));
		dir_2017_children.push_back(DirectoryRecord(id_2017, DirectoryRecord::Options::NONE, "09", "09"));
		dir_2017_children.push_back(DirectoryRecord(id_2017, DirectoryRecord::Options::NONE, "12", "12"));

		for (auto dir : dir_2017_children)
			dirIF->newEntry(dir);

		SECTION("getID() children of 2017") {
			for (auto dir : dir_2017_children)
				REQUIRE_NOTHROW(dirIF->getID(dir));
		}

		for(auto dir : dir_2017_children)
			dir_2017_ids.push_back(dirIF->getID(dir));

		//2017/06
		int id_2017_06 = dir_2017_ids[2];
		std::vector<int> dir_2017_06_ids;
		std::vector<DirectoryRecord> dir_2017_06_children;
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "02", "02"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "09", "09"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "13", "13"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "16", "16"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "17", "17"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "22", "22"));
		dir_2017_06_children.push_back(DirectoryRecord(id_2017_06, DirectoryRecord::Options::NONE, "30", "30"));

		for (auto dir : dir_2017_06_children)
			dirIF->newEntry(dir);

		SECTION("getID() children on 2017/06") {
			for (auto dir : dir_2017_06_children)
				REQUIRE_NOTHROW(dirIF->getID(dir));
		}

		for(auto dir : dir_2017_06_children)
			dir_2017_06_ids.push_back(dirIF->getID(dir));


		SECTION("getChildren(int) returns vector of children", "[getChildren][getNumberChildren]") {
			CHECK(dirIF->getChildren(0).size() == dir_vec.size());
			CHECK(dirIF->getChildren(id_2014).size() == dir_2014_children.size());
			CHECK(dirIF->getChildren(id_2017).size() == dir_2017_children.size());
			CHECK(dirIF->getChildren(id_2017_06).size() == dir_2017_06_children.size());

			CHECK(dirIF->getNumberChildren(0) == dir_vec.size());
			CHECK(dirIF->getNumberChildren(id_2014) == dir_2014_children.size());
			CHECK(dirIF->getNumberChildren(id_2017) == dir_2017_children.size());
			CHECK(dirIF->getNumberChildren(id_2017_06) == dir_2017_06_children.size());
		}


		SECTION("getEntry() == entry entered", "[getEntry]") {
			for(int i=0; i<dir_ids.size(); ++i)
				CHECK(dir_vec[i] == dirIF->getEntry(dir_ids[i]));
			for(int i=0; i<dir_2014_ids.size(); ++i)
				CHECK(dir_2014_children[i] == dirIF->getEntry(dir_2014_ids[i]));
			for(int i=0; i<dir_2017_ids.size(); ++i)
				CHECK(dir_2017_children[i] == dirIF->getEntry(dir_2017_ids[i]));
			for(int i=0; i<dir_2017_06_ids.size(); ++i)
				CHECK(dir_2017_06_children[i] == dirIF->getEntry(dir_2017_06_ids[i]));
		}

		SECTION("A directory isn't in the database anymore after it was deleted (deleteEntry(int))", "[deleteEntry]") {
			dirIF->deleteEntry(dir_ids[2]);
			CHECK_THROWS_AS(dirIF->getEntry(dir_ids[2]), std::runtime_error);

			dirIF->deleteEntry(dir_2017_06_ids[4]);
			CHECK_THROWS_AS(dirIF->getEntry(dir_2017_06_ids[4]), std::runtime_error);

			dirIF->deleteEntry(dir_2014_ids[0]);
			CHECK_THROWS_AS(dirIF->getEntry(dir_2014_ids[0]), std::runtime_error);
		}

		SECTION("All children are deleted along with a directory", "[deleteEntry]") {
			dirIF->deleteEntry(id_2014);
			CHECK_THROWS_AS(dirIF->getEntry(id_2014), std::runtime_error);

			for(int dir_id : dir_2014_ids)
				CHECK_THROWS_AS(dirIF->getEntry(dir_id), std::runtime_error);

			dirIF->deleteEntry(id_2017);
			CHECK_THROWS_AS(dirIF->getEntry(id_2017), std::runtime_error);

			for(int dir_id : dir_2017_ids)
				CHECK_THROWS_AS(dirIF->getEntry(dir_id), std::runtime_error);

			for(int dir_id : dir_2017_06_ids)
				CHECK_THROWS_AS(dirIF->getEntry(dir_id), std::runtime_error);
		}

		/// \todo change other values
		SECTION("After updating an entry the updated entry should be in the database", "[updateEntry]") {
			dir_2014_children[0].setDirectory() = "some other name";
			dir_2017_06_children[3].setDirectory() = "new name";

			REQUIRE_NOTHROW(dirIF->updateEntry(dir_2014_ids[0], dir_2014_children[0]));
			REQUIRE_NOTHROW(dirIF->updateEntry(dir_2017_06_ids[3], dir_2017_06_children[3]));

			for(unsigned long i=0; i<dir_ids.size(); ++i)
				CHECK(dir_vec[i] == dirIF->getEntry(dir_ids[i]));
			for(unsigned long i=0; i<dir_2014_ids.size(); ++i)
				CHECK(dir_2014_children[i] == dirIF->getEntry(dir_2014_ids[i]));
			for(unsigned long i=0; i<dir_2017_ids.size(); ++i)
				CHECK(dir_2017_children[i] == dirIF->getEntry(dir_2017_ids[i]));
			for(unsigned long i=0; i<dir_2017_06_ids.size(); ++i)
				CHECK(dir_2017_06_children[i] == dirIF->getEntry(dir_2017_06_ids[i]));
		}

		SECTION("After moving a dirctory, it should be in the parent directory", "[setParent]") {
			dirIF->setParent(dir_2017_06_ids[4], dir_ids[4]);
			CHECK_THAT(dirIF->getChildren(dir_ids[4]), Catch::Matchers::VectorContains(dir_2017_06_ids[4]));
		}
	}
}

TEST_CASE("BackendFactory::getInterface<DirectoryRecord>() returns the directory interface", "[directory][backend]") {
	BackendFactory db {":memory:"};
	CHECK(db.getDirectoriesInterface() == db.getInterface<DirectoryRecord>());
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
