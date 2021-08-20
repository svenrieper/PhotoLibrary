/*
 * DirectoryInterface_test.cpp
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
#include "Record/DirectoryRecord.h"
#include "exceptions.h"
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace Tests {

using RecordClasses::DirectoryRecord;
using PhotoLibrary::DatabaseInterface::constraint_error;


TEST_CASE(
		"Test the DirectoryInterface of the Adapter and the Backend"
		, "[directory][adapter][backend]"
		) {
	BackendFactory db { ":memory:" };

	SECTION(
			"An empty database should contain no directories",
			"[directory][database][getChildren][getNumberChildren]"
			) {
		CHECK(db.template getChildren<DirectoryRecord>(0).size() == 0);
		CHECK(db.template getNumberChildren<DirectoryRecord>(0) == 0);
	}

	SECTION( "A directory needs to have a parent or 0", "[directory][database][newEntry]" ) {
		DirectoryRecord invalid(5);
		CHECK_THROWS_AS(db.template newEntry<DirectoryRecord>(invalid), constraint_error);
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
			db.template newEntry<DirectoryRecord>(dir);

		SECTION("getID()") {
			for(auto dir : dir_vec)
				REQUIRE_NOTHROW(db.template getID<DirectoryRecord>(dir));
		}

		for(auto dir : dir_vec)
			dir_ids.push_back(db.template getID<DirectoryRecord>(dir));

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
			db.template newEntry<DirectoryRecord>(dir);

		SECTION("getID() children of 2014") {
			for (auto dir : dir_2014_children)
				REQUIRE_NOTHROW(db.template getID<DirectoryRecord>(dir));
		}

		for(auto dir : dir_2014_children)
			dir_2014_ids.push_back(db.template getID<DirectoryRecord>(dir));

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
			db.template newEntry<DirectoryRecord>(dir);

		SECTION("getID() children of 2017") {
			for (auto dir : dir_2017_children)
				REQUIRE_NOTHROW(db.template getID<DirectoryRecord>(dir));
		}

		for(auto dir : dir_2017_children)
			dir_2017_ids.push_back(db.template getID<DirectoryRecord>(dir));

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
			db.template newEntry<DirectoryRecord>(dir);

		SECTION("getID() children on 2017/06") {
			for (auto dir : dir_2017_06_children)
				REQUIRE_NOTHROW(db.template getID<DirectoryRecord>(dir));
		}

		for(auto dir : dir_2017_06_children)
			dir_2017_06_ids.push_back(db.template getID<DirectoryRecord>(dir));


		SECTION("getChildren(int) returns vector of children", "[getChildren][getNumberChildren]") {
			CHECK(db.template getChildren<DirectoryRecord>(0).size() == dir_vec.size());
			CHECK(db.template getChildren<DirectoryRecord>(id_2014).size() == dir_2014_children.size());
			CHECK(db.template getChildren<DirectoryRecord>(id_2017).size() == dir_2017_children.size());
			CHECK(db.template getChildren<DirectoryRecord>(id_2017_06).size() == dir_2017_06_children.size());

			CHECK(db.template getNumberChildren<DirectoryRecord>(0) == dir_vec.size());
			CHECK(db.template getNumberChildren<DirectoryRecord>(id_2014) == dir_2014_children.size());
			CHECK(db.template getNumberChildren<DirectoryRecord>(id_2017) == dir_2017_children.size());
			CHECK(db.template getNumberChildren<DirectoryRecord>(id_2017_06) == dir_2017_06_children.size());
		}


		SECTION("getEntry() == entry entered", "[getEntry]") {
			for(int i=0; i<dir_ids.size(); ++i)
				CHECK(dir_vec[i] == db.template getEntry<DirectoryRecord>(dir_ids[i]));
			for(int i=0; i<dir_2014_ids.size(); ++i)
				CHECK(dir_2014_children[i] == db.template getEntry<DirectoryRecord>(dir_2014_ids[i]));
			for(int i=0; i<dir_2017_ids.size(); ++i)
				CHECK(dir_2017_children[i] == db.template getEntry<DirectoryRecord>(dir_2017_ids[i]));
			for(int i=0; i<dir_2017_06_ids.size(); ++i)
				CHECK(dir_2017_06_children[i] == db.template getEntry<DirectoryRecord>(dir_2017_06_ids[i]));
		}

		SECTION(
				"A directory isn't in the database anymore after it was deleted (deleteEntry(int))",
				"[deleteEntry]"
				) {
			db.template deleteEntry<DirectoryRecord>(dir_ids[2]);
			CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_ids[2]), std::runtime_error);

			db.template deleteEntry<DirectoryRecord>(dir_2017_06_ids[4]);
			CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_2017_06_ids[4]), std::runtime_error);

			db.template deleteEntry<DirectoryRecord>(dir_2014_ids[0]);
			CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_2014_ids[0]), std::runtime_error);
		}

		SECTION("All children are deleted along with a directory", "[deleteEntry]") {
			db.template deleteEntry<DirectoryRecord>(id_2014);
			CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(id_2014), std::runtime_error);

			for(int dir_id : dir_2014_ids)
				CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_id), std::runtime_error);

			db.template deleteEntry<DirectoryRecord>(id_2017);
			CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(id_2017), std::runtime_error);

			for(int dir_id : dir_2017_ids)
				CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_id), std::runtime_error);

			for(int dir_id : dir_2017_06_ids)
				CHECK_THROWS_AS(db.template getEntry<DirectoryRecord>(dir_id), std::runtime_error);
		}

		/// \todo change other values
		SECTION("After updating an entry the updated entry should be in the database", "[updateEntry]") {
			dir_2014_children[0].setDirectory() = "some other name";
			dir_2017_06_children[3].setDirectory() = "new name";

			REQUIRE_NOTHROW(db.template updateEntry(dir_2014_ids[0], dir_2014_children[0]));
			REQUIRE_NOTHROW(db.template updateEntry(dir_2017_06_ids[3], dir_2017_06_children[3]));

			for(unsigned long i=0; i<dir_ids.size(); ++i)
				CHECK(dir_vec[i] == db.template getEntry<DirectoryRecord>(dir_ids[i]));
			for(unsigned long i=0; i<dir_2014_ids.size(); ++i)
				CHECK(dir_2014_children[i] == db.template getEntry<DirectoryRecord>(dir_2014_ids[i]));
			for(unsigned long i=0; i<dir_2017_ids.size(); ++i)
				CHECK(dir_2017_children[i] == db.template getEntry<DirectoryRecord>(dir_2017_ids[i]));
			for(unsigned long i=0; i<dir_2017_06_ids.size(); ++i)
				CHECK(dir_2017_06_children[i] == db.template getEntry<DirectoryRecord>(dir_2017_06_ids[i]));
		}

		SECTION("After moving a dirctory, it should be in the parent directory", "[setParent]") {
			db.template setParent<DirectoryRecord>(dir_2017_06_ids[4], dir_ids[4]);
			CHECK_THAT(db.template getChildren<DirectoryRecord>(dir_ids[4]), Catch::Matchers::VectorContains(dir_2017_06_ids[4]));
		}
	}
}

} /* namespace Tests */
} /* namespace Backend */
} /* namespace PhotoLibrary */
