/*
 * DatabaseInterface_tests.cpp
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

#include "DatabaseInterface_tests.h"
#include <Database.h>
#include <support.h>
#include <AccessTables.h>
#include <Record.h>
#include <catch2/catch.hpp>
#include <stdexcept>

namespace PhotoLibrary {
namespace DatabaseInterface {
namespace DatabaseInterface_tests {

// Tests for tuple, pair, and array
template<typename T>
std::vector<T> two_ints_make_vector();

template<typename T>
std::vector<std::array<int,3>> two_ints_insert(AccessTables<>& interface);

template<typename T>
void two_ints_get_entry(AccessTables<>& interface, const std::vector<std::array<int,3>>& reference_vec);

template<typename T>
void two_ints_get_children(AccessTables<>& interface, const std::vector<std::array<int,3>>& reference_vec);

template<typename T>
void two_ints_update_entry(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec);

template<typename T>
void two_ints_set_parent(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec);

template<typename T>
void two_ints_delete1(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec);

template<typename T>
void two_ints_delete2(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec);

TEMPLATE_TEST_CASE(
		"Test pair, array, and tuple and their interchangability",
		"[DatabaseInterface]",
		TwoIntsTuple, TwoIntsArray, TwoIntsPair) {
	SQLiteAdapter::Database db { ":memory:" };
	AccessTables interface { db };

	REQUIRE_NOTHROW(db.querry(TwoIntsPair::create, nullptr, nullptr));

	//Test insert() and getID()
	std::vector<std::array<int,3>> db_content = two_ints_insert<TestType>(interface);

	//Test getEntry()
	two_ints_get_entry<TwoIntsTuple>(interface, db_content);
	two_ints_get_entry<TwoIntsArray>(interface, db_content);
	two_ints_get_entry<TwoIntsPair>(interface, db_content);

	//Test getChildren() and getNumberChildren()
	two_ints_get_children<TwoIntsTuple>(interface, db_content);
	two_ints_get_children<TwoIntsArray>(interface, db_content);
	two_ints_get_children<TwoIntsPair>(interface, db_content);

	//Test updateEntry
	SECTION("Update entry with tuple") {
		two_ints_update_entry<TwoIntsTuple>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}
	SECTION("Update entry with array") {
		two_ints_update_entry<TwoIntsArray>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}
	SECTION("Update entry with pair") {
		two_ints_update_entry<TwoIntsPair>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}

	//Test setParent
	SECTION("setParent with tuple") {
		two_ints_set_parent<TwoIntsTuple>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}
	SECTION("setParent with array") {
		two_ints_set_parent<TwoIntsArray>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}
	SECTION("setParent with pair") {
		two_ints_set_parent<TwoIntsPair>(interface, db_content);
		two_ints_get_entry<TestType>(interface, db_content);
	}

	//Test deleteEntry individually
	SECTION("deleteEntry individually with tuple") {
		two_ints_delete1<TwoIntsTuple>(interface, db_content);
	}
	SECTION("deleteEntry individually with array") {
		two_ints_delete1<TwoIntsArray>(interface, db_content);
	}
	SECTION("deleteEntry individually with pair") {
		two_ints_delete1<TwoIntsPair>(interface, db_content);
	}

	//Test deleteEntry recursively
	SECTION("deleteEntry recursively with tuple") {
		two_ints_delete2<TwoIntsTuple>(interface, db_content);
	}
	SECTION("deleteEntry recursively with array") {
		two_ints_delete2<TwoIntsArray>(interface, db_content);
	}
	SECTION("deleteEntry recursively with pair") {
		two_ints_delete2<TwoIntsPair>(interface, db_content);
	}
}

template<typename T>
std::vector<T> two_ints_make_vector() {
	std::vector<T> vec;
	vec.emplace_back(0,1);
	vec.emplace_back(0,19);
	vec.emplace_back(0,255);
	vec.emplace_back(0,34);

	return vec;
}

template<typename T>
std::vector<std::array<int,3>> two_ints_insert(AccessTables<>& interface) {
	std::vector<T> vec { two_ints_make_vector<T>() };
	std::vector<std::array<int,3>> ret_vec;

	for(T& p : vec) {
		T entry {p};
		REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.newEntry(entry));

		int id {};
		REQUIRE_NOTHROW(id = interface.getID(entry));
		ret_vec.push_back({id, entry.template access<0>(), entry.template access<1>()});
	}

	int id = ret_vec[2][0];
	for(T& p : vec) {
		T entry {id, p.template access<1>()};
		REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.newEntry(entry));

		int new_id {};
		REQUIRE_NOTHROW(new_id = interface.getID(entry));
		ret_vec.push_back({new_id, id, entry.template access<1>()});
	}

	id = ret_vec[5][0];
	int value { 17986 };
	T entry { id, value };
	REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
	REQUIRE_NOTHROW(interface.newEntry(entry));

	int new_id {};
	REQUIRE_NOTHROW(new_id = interface.getID(entry));
	ret_vec.push_back({new_id, id, value});

	return ret_vec;
}

template<typename T>
void two_ints_get_entry(AccessTables<>& interface, const std::vector<std::array<int,3>>& reference_vec) {
	int index { 1 };
	for(auto& a : reference_vec) {
		T entry;
		REQUIRE_NOTHROW(entry = interface.getEntry<T>(a[0]));
		CHECK(a[1] == entry.template access<0>());
		CHECK(a[2] == entry.template access<1>());

		while(a[0] == index) ++index;
	}
	REQUIRE_THROWS_AS(interface.getEntry<T>(index), missing_entry);
}

template<typename T>
void two_ints_get_children(AccessTables<>& interface, const std::vector<std::array<int,3>>& reference_vec) {
	std::unordered_map<int,std::vector<int>> children;
	int invalid_index {1};
	for(auto& a : reference_vec) {
		children[a[0]];
		children[a[1]].push_back(a[0]);
		while(invalid_index == a[0]) ++invalid_index;
	}

	REQUIRE(children[0].size() != 0);
	for(auto iter=children.begin(); iter!=children.end(); ++iter) {
		std::vector<int> child_vec;
		int number_children {};
		REQUIRE_NOTHROW(child_vec = interface.getChildren<T>(iter->first));
		REQUIRE_NOTHROW(number_children = interface.getNumberChildren<T>(iter->first));
		CHECK(child_vec.size() == iter->second.size());
		CHECK(number_children == child_vec.size());

		CHECK_THAT(child_vec, Catch::UnorderedEquals(iter->second));
	}

	//Test invalid id
	int number_children {};
	std::vector<int> child_vec;
	REQUIRE_NOTHROW(child_vec = interface.getChildren<T>(invalid_index));
	REQUIRE_NOTHROW(number_children = interface.getNumberChildren<T>(invalid_index));
	CHECK(number_children == 0);
	CHECK(child_vec.size() == 0);
}

template<typename T>
void two_ints_update_entry(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec) {
	int next_parent {};
	int invalid_index {1};
	for(int i=0; i!=reference_vec.size(); ++i) {
		reference_vec[i][1] = next_parent;
		(reference_vec[i][2] *= 2) += 1;
		T entry {reference_vec[i][1], reference_vec[i][2]};
		REQUIRE_NOTHROW(interface.updateEntry(reference_vec[i][0], entry));

		if(!next_parent) next_parent = reference_vec[i][0];
		if(!(i%3)) next_parent = 0;

		if(invalid_index<=reference_vec[i][0]) invalid_index = reference_vec[i][0]+1;
	}

	REQUIRE_NOTHROW(interface.updateEntry(invalid_index, T{0,0}));
	REQUIRE_THROWS_AS(interface.getEntry<T>(invalid_index), missing_entry);
	REQUIRE_THROWS_AS(interface.updateEntry(reference_vec[2][0], T{invalid_index, 5}), constraint_error);
}

template<typename T>
void two_ints_set_parent(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec) {
	int next_parent {};
	int invalid_index {1};
	for(int i=0; i!=reference_vec.size(); ++i) {
		reference_vec[i][1] = next_parent;
		REQUIRE_NOTHROW(interface.setParent<T>(reference_vec[i][0], next_parent));

		if(!next_parent) next_parent = reference_vec[i][0];
		if(!(i%3)) next_parent = 0;

		if(invalid_index<=reference_vec[i][0]) invalid_index = reference_vec[i][0]+1;
	}

	REQUIRE_THROWS_AS(interface.setParent<T>(reference_vec[2][0], invalid_index), constraint_error);
	REQUIRE_NOTHROW(interface.setParent<T>(invalid_index, 0));
	REQUIRE_THROWS_AS(interface.getEntry<T>(invalid_index), missing_entry);
}

template<typename T>
void two_ints_delete1(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec) {
	while(reference_vec.size()) {
		REQUIRE_NOTHROW(interface.deleteEntry<T>(reference_vec.back()[0]));
		reference_vec.pop_back();

		two_ints_get_entry<T>(interface, reference_vec);
	}

	REQUIRE_THROWS_AS(interface.deleteEntry<T>(0), std::runtime_error);
}

template<typename T>
void two_ints_delete2(AccessTables<>& interface, std::vector<std::array<int,3>>& reference_vec) {
	//std::vector<int> parent_children, other;
	std::unordered_map<int,bool> parent_children;
	for(const auto& a : reference_vec) {
		parent_children[a[0]]=false;
	}
	parent_children[reference_vec[2][0]]=true;
	int number_children {};
	for(const auto& a : reference_vec) {
		if(parent_children[a[0]] || parent_children[a[1]]) {
			parent_children[a[0]]=true;
			++number_children;
		}
	}
	CHECK(number_children);

	REQUIRE_NOTHROW(interface.deleteEntry<T>(reference_vec[2][0]));
	for(const auto& a : reference_vec) {
		if(parent_children[a[0]])
			CHECK_THROWS_AS(interface.getEntry<T>(a[0]), missing_entry);
		else
			CHECK_NOTHROW(interface.getEntry<T>(a[0]));
	}
}


//
TEST_CASE("", "[DatabaseInterface]") {
	SQLiteAdapter::Database db { ":memory:" };
	AccessTables interface { db };

	SECTION("") {
		REQUIRE_NOTHROW(db.querry(FourInts::create, nullptr, nullptr));
	}
}

} /* namespace DatabaseInterface_tests */
} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */
