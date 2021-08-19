/*
 * AccessTables_tests.cpp
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

#include "AccessTables_tests.h"
#include <Database.h>
#include <functional>
#include <support.h>
#include <AccessTables.h>
#include <Record.h>
#include <catch2/catch.hpp>
#include <stdexcept>
#include <limits>

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
		"[DatabaseInterface][AccessTables]",
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
	int invalid_index {1};

	for(T& p : vec) {
		T entry {p};
		REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.newEntry(entry));

		int id {};
		REQUIRE_NOTHROW(id = interface.getID(entry));
		ret_vec.push_back({id, entry.template access<0>(), entry.template access<1>()});

		if(invalid_index <= id) invalid_index = id+1;
	}

	int id = ret_vec[2][0];
	for(T& p : vec) {
		T entry {id, p.template access<1>()};
		REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.newEntry(entry));

		int new_id {};
		REQUIRE_NOTHROW(new_id = interface.getID(entry));
		ret_vec.push_back({new_id, id, entry.template access<1>()});

		if(invalid_index <= new_id) invalid_index = new_id+1;
	}

	id = ret_vec[5][0];
	int value { 17986 };
	T entry { id, value };
	REQUIRE_THROWS_AS(interface.getID(entry), missing_entry);
	REQUIRE_NOTHROW(interface.newEntry(entry));

	int new_id {};
	REQUIRE_NOTHROW(new_id = interface.getID(entry));
	ret_vec.push_back({new_id, id, value});

	if(invalid_index <= new_id) invalid_index = new_id+1;
	REQUIRE_THROWS_AS(interface.newEntry(T{invalid_index, 200}), constraint_error);
	REQUIRE_THROWS_AS(interface.newEntry(T{invalid_index+1, 200}), constraint_error);

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


// Test Glib::ustring as base and table of four strings
template<SQLiteAdapter::String_type T>
std::vector<int> fout_strings_new_and_get(AccessTables<T>& interface);

template<SQLiteAdapter::String_type T>
void four_strings_update(AccessTables<T>& interface, const std::vector<int>& ids);

template<SQLiteAdapter::String_type T>
void four_strings_delete(AccessTables<T>& interface, const std::vector<int>& ids);

TEMPLATE_TEST_CASE(
		"Glib::ustring as base and table of four strings;"
		"AccessTables's methods work independent of the string type used in Record",
		"[DatabaseInterface][AccessTables]",
		Glib::ustring, std::string) {
	SQLiteAdapter::Database db { ":memory:" };
	AccessTables<TestType> interface { db };

	REQUIRE_NOTHROW(db.querry(FourStrings<TestType>::create, nullptr, nullptr));

	std::vector<int> ids = fout_strings_new_and_get<TestType>(interface);

	four_strings_update<TestType>(interface, ids);

	four_strings_delete<TestType>(interface, ids);
}

template<SQLiteAdapter::String_type T>
std::vector<int> fout_strings_new_and_get(AccessTables<T>& interface) {
	std::vector<std::array<T,4>> entries;
	entries.emplace_back(std::array<T,4>{"some", "word", "1", "42"});
	entries.emplace_back(std::array<T,4>{"some", "other word", "1", "42"});
	entries.emplace_back(std::array<T,4>{"some", "WORD", "99", "text"});
	entries.emplace_back(std::array<T,4>{"onother string", "word", "1", "42"});

	std::vector<int> entry_ids;

	for(const auto& a : entries) {
		FourStrings<T> entry {a[0], a[1], a[2], a[3]};
		REQUIRE_NOTHROW(interface.newEntry(entry));
		REQUIRE_THROWS_AS(interface.newEntry(entry), constraint_error);
		REQUIRE_NOTHROW(entry_ids.push_back(interface.getID(entry)));
	}

	REQUIRE_THROWS_AS(interface.newEntry(FourStrings<T>{"some", "word", "some", "word"}), constraint_error);

	REQUIRE(entries.size() == entry_ids.size());

	for(int i {0}; i < entries.size(); ++i) {
		FourStrings<T> entry {entries[i][0], entries[i][1], entries[i][2], entries[i][3]};
		FourStrings<T> entry2 {};
		REQUIRE_NOTHROW(entry2 = interface.template getEntry<FourStrings<T>>(entry_ids[i]));

		CHECK(entry == entry2);
	}

	return entry_ids;
}

template<SQLiteAdapter::String_type T>
void four_strings_update(AccessTables<T>& interface, const std::vector<int>& ids) {
	FourStrings<T> entry;
	REQUIRE(ids.size() >= 4);

	REQUIRE_NOTHROW(entry = interface.template getEntry<FourStrings<T>>(ids[2]));
	entry.template access<0>() += " some more";
	REQUIRE_NOTHROW(interface.updateEntry(ids[2], entry));
	CHECK(entry == interface.template getEntry<FourStrings<T>>(ids[2]));

	REQUIRE_NOTHROW(entry = interface.template getEntry<FourStrings<T>>(ids[0]));
	entry.template access<1>() += "something";
	REQUIRE_NOTHROW(interface.updateEntry(ids[0], entry));
	CHECK(entry == interface.template getEntry<FourStrings<T>>(ids[0]));

	REQUIRE_NOTHROW(entry = interface.template getEntry<FourStrings<T>>(ids[1]));
	entry.template access<2>() += " 97";
	REQUIRE_NOTHROW(interface.updateEntry(ids[1], entry));
	CHECK(entry == interface.template getEntry<FourStrings<T>>(ids[1]));

	REQUIRE_NOTHROW(entry = interface.template getEntry<FourStrings<T>>(ids[3]));
	entry.template access<3>() += " 97";
	REQUIRE_NOTHROW(interface.updateEntry(ids[3], entry));
	CHECK(entry == interface.template getEntry<FourStrings<T>>(ids[3]));
}

template<SQLiteAdapter::String_type T>
void four_strings_delete(AccessTables<T>& interface, const std::vector<int>& ids) {
	for(int a : ids) {
		REQUIRE_NOTHROW(interface.template getEntry<FourStrings<T>>(a));
		REQUIRE_NOTHROW(interface.template deleteEntry<FourStrings<T>>(a));
		REQUIRE_NOTHROW(interface.template deleteEntry<FourStrings<T>>(a));
		REQUIRE_THROWS_AS(interface.template getEntry<FourStrings<T>>(a), missing_entry);
	}
}


//Test two tables and different types
namespace TwoTables {
template<typename T>
void get_children(AccessTables<Glib::ustring>& interface, const std::vector<int> children_expected, int parent);

TEST_CASE("Test two tables and differend integral types", "[DatabaseInterface][AccessTables]") {
	SQLiteAdapter::Database db { ":memory:" };
	AccessTables<Glib::ustring> interface { db };

	REQUIRE_NOTHROW(db.querry(twoTablesCreate, nullptr, nullptr));

	// fill tables and retrieve 
	// fill Table1
	std::vector<Glib::ustring> t1_values {"one", "2", "word", "elephant", "five", "06", "another string"};
	std::vector<int> t1_ids;

	for(const auto& v : t1_values) {
		TwoTables1 entry { 0, v };
		REQUIRE_NOTHROW(interface.newEntry(entry));
		REQUIRE_THROWS_AS(interface.newEntry(entry), constraint_error);

		int new_id;
		REQUIRE_NOTHROW(new_id = interface.getID(entry));
		t1_ids.push_back(new_id);
	}

	for(const auto& v : t1_values) {
		TwoTables1 entry { t1_ids[2], v };
		REQUIRE_NOTHROW(interface.newEntry(entry));
		REQUIRE_THROWS_AS(interface.newEntry(entry), constraint_error);

		int new_id;
		REQUIRE_NOTHROW(new_id = interface.getID(entry));
		t1_ids.push_back(new_id);
	}

	//  retrieve entries from Table1
	REQUIRE(t1_ids.size() == 2*t1_values.size());
	for(int i {}; i<t1_ids.size(); ++i) {
		TwoTables1 entry {
			i/t1_values.size()?t1_ids[2]:0,
			t1_values[i%t1_values.size()]
		};

		TwoTables1 entry2;
		REQUIRE_NOTHROW(entry2 = interface.getEntry<TwoTables1>(t1_ids[i]));
		CHECK(entry == entry2);
	}

	// fill Table2
	REQUIRE(t1_ids.size() > 1);

	std::vector<int> t2_ids;
	std::vector<TwoTables2> t2_entries;
	t2_entries.emplace_back(t1_ids[0], 7, 3, 77, 'r', E2, 65);
	t2_entries.emplace_back(t1_ids[0], -155, 75, 0, '1', E2, 355);
	t2_entries.emplace_back(t1_ids[0], 7, 3, 77, 'r', E5, 65);
	t2_entries.emplace_back(t1_ids[0], -3698, 3, 77, 'r', E5, 65);
	t2_entries.emplace_back(t1_ids.back(), 7, 3, 77, 'r', E2, 65);
	t2_entries.emplace_back(
			t1_ids.back(),
			std::numeric_limits<int64_t>::max(),
			std::numeric_limits<int64_t>::min(),
			-129,
			32,
			E1,
			std::numeric_limits<uint64_t>::max()
			);

	for(const auto& e : t2_entries) {
		REQUIRE_NOTHROW(interface.newEntry(e));
		REQUIRE_THROWS_AS(interface.newEntry(e), constraint_error);

		int id;
		REQUIRE_NOTHROW(id = interface.getID(e));
		t2_ids.push_back(id);
	}

	// retrieve entries from Table2
	for(int i{}; i<t2_entries.size(); ++i) {
		TwoTables2 entry;
		REQUIRE_NOTHROW(entry = interface.getEntry<TwoTables2>(t2_ids[i]));
		CHECK(entry == t2_entries[i]);
	}


	// getChildren and getNumberChildren
	// Table1
	std::vector<int> t1_children_expected(t1_ids.size()/2);
	std::copy(t1_ids.begin(), t1_ids.begin()+t1_ids.size()/2, t1_children_expected.begin());
	get_children<TwoTables1>(interface, t1_children_expected, 0);

	std::copy(t1_ids.begin()+t1_ids.size()/2, t1_ids.end(), t1_children_expected.begin());
	get_children<TwoTables1>(interface, t1_children_expected, t1_ids[2]);
	
	for(int i {}; i<t1_ids.size(); ++i) {
		if(i!=2)
			get_children<TwoTables1>(interface, std::vector<int>{}, t1_ids[i]);
	}

	// Table2
	REQUIRE(t2_ids.size() == t2_entries.size());
	std::unordered_map<int,std::vector<int>> t2_children_exprected;
	for(int i{}; i<t2_ids.size(); ++i) {
		t2_children_exprected[t2_entries[i].access<0>()].push_back(t2_ids[i]);
	}

	for(int id : t1_ids) {
		get_children<TwoTables2>(interface, t2_children_exprected[id], id);
	}

	for(auto& a : t2_children_exprected) {
		get_children<TwoTables2>(interface, a.second, a.first);
	}

	// updateEntry
	// Table1
	REQUIRE(t1_ids.size() >= t1_values.size()+2);
	for(int i{}; i<t1_values.size(); ++i) {
		TwoTables1 entry{ 0, t1_values[i]+" "+std::to_string(i) };
		CHECK_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.updateEntry(t1_ids[i], entry));

		TwoTables1 entry_got;
		REQUIRE_NOTHROW(entry_got = interface.getEntry<TwoTables1>(t1_ids[i]));

		CHECK(entry == entry_got);
	}

	{
		int i = t1_values.size();
		TwoTables1 entry = { t1_ids[i-1], t1_values[0] };
		TwoTables1 entry_got;

		CHECK_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.updateEntry(t1_ids[i], entry));
		REQUIRE_NOTHROW(entry_got = interface.getEntry<TwoTables1>(t1_ids[i]));
		CHECK(entry == entry_got);

		++i;
		entry = { t1_ids[i-1], "value" };

		CHECK_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.updateEntry(t1_ids[i], entry));
		REQUIRE_NOTHROW(entry_got = interface.getEntry<TwoTables1>(t1_ids[i]));
		CHECK(entry == entry_got);
	}

	// Table2
	{
		TwoTables2 entry = t2_entries[1];
		entry.access<0>() = t1_ids[t1_values.size()];
		entry.access<1>() += 17;
		entry.access<2>() /= 5;
		entry.access<3>() -= 42;
		entry.access<4>() *= 2;
		entry.access<5>() = E6;
		entry.access<6>() = 15;
		TwoTables2 entry_got;

		CHECK_THROWS_AS(interface.getID(entry), missing_entry);
		REQUIRE_NOTHROW(interface.updateEntry(t2_ids[1], entry));
		REQUIRE_NOTHROW(entry_got = interface.getEntry<TwoTables2>(t2_ids[1]));
		CHECK(entry == entry_got);
	}


	// setParent
	// Table1
	REQUIRE(t1_ids.size() >= t1_values.size()+3);
	CHECK(0 == interface.getNumberChildren<TwoTables1>(t1_ids[t1_values.size()+2]));
	REQUIRE_NOTHROW(interface.setParent<TwoTables1>(t1_ids[t1_values.size()+3] , t1_ids[t1_values.size()+2]));
	CHECK(1 == interface.getNumberChildren<TwoTables1>(t1_ids[t1_values.size()+2]));
	CHECK_THAT(
			interface.getChildren<TwoTables1>(t1_ids[t1_values.size()+2]),
			Catch::Contains(std::vector<int> (1, t1_ids[t1_values.size()+3]))
			);

	// Table2
	REQUIRE(t2_ids.size() == t2_entries.size());
	CHECK(0 == interface.getNumberChildren<TwoTables2>(t1_ids[t1_values.size()+2]));
	for(int i=2; i<t2_ids.size(); ++i) {
		REQUIRE_NOTHROW(interface.setParent<TwoTables2>(t2_ids[i], t1_ids[t1_values.size()+2]));
		t2_entries[i].access<0>()=t1_ids[t1_values.size()+2];
	}
	CHECK(t2_ids.size()-2 == interface.getNumberChildren<TwoTables2>(t1_ids[t1_values.size()+2]));
	CHECK_THAT(
			interface.getChildren<TwoTables2>(t1_ids[t1_values.size()+2]),
			Catch::Contains(std::vector<int> (t2_ids.begin()+2, t2_ids.end()))
			);


	//deleteEntry
	std::unordered_map<int,std::vector<int>> t1_children_recursive, t2_children_recursive;

	std::function<void(int)> fill_child_rec1 = [&](int parent_id){
			std::vector<int> children;
			REQUIRE_NOTHROW(children = interface.getChildren<TwoTables1>(parent_id));
			auto& parent_vec = t1_children_recursive[parent_id];
			for(auto id : children) {
				fill_child_rec1(id);
				auto& children_vec = t1_children_recursive[id];
				parent_vec.push_back(id);
				parent_vec.insert(parent_vec.end(), children_vec.begin(), children_vec.end());
			}
		};
	fill_child_rec1(0);

	// test fill_child_rec1(int)
	REQUIRE_THAT(t1_ids, Catch::UnorderedEquals(t1_children_recursive[0]));

	REQUIRE(t2_ids.size() == t2_entries.size());
	for(int i{}; i<t2_ids.size(); ++i) {
		int parent_id =t2_entries[i].access<0>();
		t2_children_recursive[parent_id].push_back(t2_ids[i]);
		for(auto id : t1_ids) {
			const auto& child_vec = t1_children_recursive[id];
			if(std::find(child_vec.begin(), child_vec.end(), parent_id) != child_vec.end())
				t2_children_recursive[id].push_back(t2_ids[i]);
		}
	}
	CHECK(t2_children_recursive[t1_ids[2]].size() == t2_ids.size()-2); //==4
	CHECK(t2_children_recursive[t1_ids[2]].size() == t2_children_recursive[t1_ids[t1_values.size()+2]].size()); //==4

	SECTION("Delete empty leaves from Table1 and entries from Table2") {
		CHECK(t1_children_recursive[t1_ids[1]].size() == 0);
		CHECK(t1_children_recursive[t1_ids[4]].size() == 0);
		CHECK(t2_children_recursive[t1_ids[1]].size() == 0);
		CHECK(t2_children_recursive[t1_ids[4]].size() == 0);
		CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[1]));
		CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[4]));
		REQUIRE_NOTHROW(interface.deleteEntry<TwoTables1>(t1_ids[1]));
		REQUIRE_NOTHROW(interface.deleteEntry<TwoTables1>(t1_ids[4]));
		CHECK_THROWS_AS(interface.getEntry<TwoTables1>(t1_ids[1]), missing_entry);
		CHECK_THROWS_AS(interface.getEntry<TwoTables1>(t1_ids[4]), missing_entry);

		for(int i{}; i<t1_ids.size(); ++i)
			if(i != 1 && i != 4)
				CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[i]));

		for(auto id : t2_ids)
			CHECK_NOTHROW(interface.getEntry<TwoTables2>(id));

		REQUIRE_NOTHROW(interface.deleteEntry<TwoTables2>(t2_ids[0]));
		REQUIRE_NOTHROW(interface.deleteEntry<TwoTables2>(t2_ids[2]));

		for(int i{}; i<t1_ids.size(); ++i)
			if(i != 1 && i != 4)
				CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[i]));

		for(int i{}; i<t2_ids.size(); ++i)
			if(i != 0 && i != 2)
				CHECK_NOTHROW(interface.getEntry<TwoTables2>(t2_ids[i]));
	}

	SECTION("Delete recursively") {
		for(int i{}; i<t1_ids.size(); ++i)
			CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[i]));
		for(int i{}; i<t2_ids.size(); ++i)
			CHECK_NOTHROW(interface.getEntry<TwoTables2>(t2_ids[i]));

		REQUIRE_NOTHROW(interface.deleteEntry<TwoTables1>(t1_ids[2]));

		for(int i{}; i<t1_ids.size(); ++i) {
			if(i == 2 || i >= t1_values.size()+2)
				CHECK_THROWS_AS(interface.getEntry<TwoTables1>(t1_ids[i]), missing_entry);
			else
				CHECK_NOTHROW(interface.getEntry<TwoTables1>(t1_ids[i]));
		}
		for(int i{}; i<t2_ids.size(); ++i) {
			if(i < 2)
				CHECK_NOTHROW(interface.getEntry<TwoTables2>(t2_ids[i]));
			else
				CHECK_THROWS_AS(interface.getEntry<TwoTables2>(t2_ids[i]), missing_entry);
		}
	}
}

template<typename T>
void get_children(AccessTables<Glib::ustring>& interface, const std::vector<int> children_expected, int parent) {
	std::vector<int> children_got;
	int num_children_got;

	REQUIRE_NOTHROW(num_children_got = interface.getNumberChildren<T>(parent));
	REQUIRE_NOTHROW(children_got = interface.getChildren<T>(parent));

	CHECK(num_children_got == children_expected.size());
	CHECK_THAT(children_expected, Catch::UnorderedEquals(children_got));
}

} /* namespace TwoTables */

// other test cases:
// enum class|es

} /* namespace DatabaseInterface_tests */
} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */
