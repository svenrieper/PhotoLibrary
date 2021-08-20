/*
 * RelationsTable_tests.cpp
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

#include <Database.h>
#include <exceptions.h>
#include <RelationsTable.h>
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace DatabaseInterface {
namespace DatabaseInterface_tests {

TEST_CASE("Tests for RelationsTable", "[DatabaseInterface][RelationsTable]") {
	SQLiteAdapter::Database db { ":memory:" };
	RelationsTable relations { db };

	const char* create_table = "CREATE TABLE Relations( entry INTEGER, col INTEGER, UNIQUE (entry, col));"
			"INSERT INTO Relations (col, entry) VALUES (7, 42);";
	const std::array<const std::string,3> relation_table {"Relations", "col", "entry"};
	db.querry(create_table, nullptr, nullptr);

	CHECK(relations.getNumberEntries(5, relation_table) == 0);
	CHECK(relations.getNumberCollections(15, relation_table) == 0);
	CHECK(relations.getEntries(7, relation_table) == std::vector<int>{42});
	CHECK(relations.getCollections(42, relation_table) == std::vector<int>{7});

	std::vector<std::pair<int,int>> relation_vec {
		{8,13}, {9,25}, {8,25}, {17,25}, {25,25}, {8,225}, {8,97}, {9,16}, {25,16} };

	for(const auto& p : relation_vec) {
		REQUIRE_NOTHROW(relations.newRelation(p.first, p.second, relation_table));
	}
	CHECK(relations.getNumberEntries(13, relation_table) == 1);
	CHECK_THAT(relations.getCollections(8, relation_table), Catch::UnorderedEquals(std::vector<int>{13, 25, 225, 97}));
	CHECK(relations.getNumberEntries(25, relation_table) == 4);

	for(const auto& p : relation_vec) {
		CHECK_NOTHROW(relations.newRelation(p.first, p.second, relation_table));
	}
	CHECK(relations.getNumberEntries(13, relation_table) == 1);
	CHECK(relations.getNumberCollections(8, relation_table) == 4);
	CHECK_THAT(relations.getEntries(25, relation_table), Catch::UnorderedEquals(std::vector<int>{9, 8, 17, 25}));

	CHECK_THAT(relations.getEntries(16, relation_table), Catch::Contains(std::vector<int>{9, 25}));
	CHECK_THAT(relations.getCollections(25, relation_table), Catch::Contains(std::vector<int>{16}));
	CHECK(relations.getNumberEntries(16, relation_table) == 2);
	CHECK(relations.getNumberCollections(25, relation_table) == 2);
	REQUIRE_NOTHROW(relations.deleteRelation(26, 16, relation_table));
	REQUIRE_NOTHROW(relations.deleteRelation(25, 16, relation_table));
	CHECK_THAT(relations.getEntries(16, relation_table), Catch::Contains(std::vector<int>{9}));
	CHECK_THAT(relations.getEntries(16, relation_table), !Catch::Contains(std::vector<int>{25}));
	CHECK_THAT(relations.getCollections(25, relation_table), !Catch::Contains(std::vector<int>{16}));
	CHECK_THAT(relations.getCollections(9, relation_table), Catch::Contains(std::vector<int>{16}));
	CHECK(relations.getNumberEntries(16, relation_table) == 1);
	CHECK(relations.getNumberCollections(25, relation_table) == 1);
	CHECK(relations.getNumberCollections(9, relation_table) == 2);
	REQUIRE_NOTHROW(relations.deleteRelation(25, 16, relation_table));
	REQUIRE_NOTHROW(relations.deleteRelation(9, 16, relation_table));
	CHECK_THAT(relations.getEntries(16, relation_table), !Catch::Contains(std::vector<int>{25}));
	CHECK_THAT(relations.getEntries(16, relation_table), !Catch::Contains(std::vector<int>{9}));
	CHECK_THAT(relations.getCollections(9, relation_table), !Catch::Contains(std::vector<int>{16}));
	CHECK(relations.getNumberEntries(16, relation_table) == 0);
	CHECK(relations.getNumberCollections(25, relation_table) == 1);
	CHECK(relations.getNumberCollections(9, relation_table) == 1);
}

} /* namespace DatabaseInterface_tests */
} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */
