/*
 * KeywordRecord_test.cpp
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

#include "../src/Backend/Record/KeywordRecord.h"
#include <catch2/catch.hpp>
#include <vector>

namespace PhotoLibrary {
namespace Tests {

using Keyword = Backend::RecordClasses::KeywordRecord;
using namespace Backend;

TEST_CASE( "test operator== and operator!= for class Keyword", "[keyword][record]" ) {
	Keyword default_key;
	Keyword default_key2;
	CHECK(default_key == default_key);
	CHECK(default_key == default_key2);

	Keyword animal(0, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
	Keyword animal2(0, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
	Keyword animal_parent(2, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
	CHECK(animal == animal);
	CHECK(animal == animal2);
	CHECK(animal != animal_parent);

	Keyword elephant(1, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
	Keyword elephant2(1, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
	Keyword elephant_options1(1, Keyword::Options::INCLUDE_ON_EXPORT, "Elephant", "Elephant");
	Keyword elephant_options2(1,
			Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED | Keyword::Options::PRIVATE,
			"Elephant", "Elephant");
	Keyword elephant_options3(1, Keyword::Options::NONE, "Elephant", "Elephant");
	Keyword elephant_options4(1, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED, "Elephant", "Elephant");
	CHECK(elephant == elephant);
	CHECK(elephant == elephant2);
	CHECK(elephant != elephant_options1);
	CHECK(elephant != elephant_options2);
	CHECK(elephant != elephant_options3);
	CHECK(elephant != elephant_options4);
	CHECK(elephant_options1 != elephant_options2);
	CHECK(elephant_options1 != elephant_options3);
	CHECK(elephant_options1 != elephant_options4);
	CHECK(elephant_options2 != elephant_options3);
	CHECK(elephant_options2 != elephant_options4);
	CHECK(elephant_options3 != elephant_options4);


	Keyword asian_elephant(5, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
	Keyword asian_elephant2(5, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
	Keyword african_elephant(5, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "African Elephant", "Elephant");
	CHECK(asian_elephant == asian_elephant2);
	CHECK(asian_elephant2 != african_elephant);

	Keyword giraffe(7, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
	Keyword giraffe_synonyms(7, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe", "Giraffe");
	Keyword giraffe_synonyms2(7, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe", "Elephant");
	CHECK(giraffe != giraffe_synonyms);
	CHECK(giraffe != giraffe_synonyms2);
	CHECK(giraffe_synonyms2 != giraffe_synonyms);

	CHECK(default_key != animal);
	CHECK(default_key != elephant);
	CHECK(default_key != elephant_options4);
	CHECK(default_key != asian_elephant);
}

TEST_CASE("test constructors for class KeywordRecord", "[keywords][record]") {
    Glib::ustring name{"some_name"};
	Glib::ustring fullname{"another name"};

	SECTION("Provide all arguments") {
		Keyword keyword1 {0, Keyword::Options::NONE, name, fullname};
		Keyword keyword2 {0, Keyword::Options::NONE, Glib::ustring(name), fullname};
		Keyword keyword3 {0, Keyword::Options::NONE, name, Glib::ustring(fullname)};
		Keyword keyword4 {0, Keyword::Options::NONE, Glib::ustring(name), Glib::ustring(fullname)};

		CHECK(keyword1 == keyword2);
		CHECK(keyword1 == keyword3);
		CHECK(keyword1 == keyword4);
	}

	SECTION("Default synonyms") {
		Keyword keyword_copying_name{0, Keyword::Options::NONE, name};
		Keyword keyword_moving_name{0, Keyword::Options::NONE, std::move(name)};

		CHECK(keyword_copying_name == keyword_moving_name);
	}

	name = "";
	fullname = "";

	SECTION("Default constructor") {
		std::vector<Keyword> key_vec;

		key_vec.push_back({});
		key_vec.push_back({0});
		key_vec.push_back({0, Keyword::Options::NONE});
		key_vec.push_back({0, Keyword::Options::NONE, name});
		key_vec.push_back({0, Keyword::Options::NONE, Glib::ustring(name)});
		key_vec.push_back({0, Keyword::Options::NONE, name, fullname});
		key_vec.push_back({0, Keyword::Options::NONE, Glib::ustring(name), fullname});
		key_vec.push_back({0, Keyword::Options::NONE, name, Glib::ustring(fullname)});
		key_vec.push_back({0, Keyword::Options::NONE, Glib::ustring(name), Glib::ustring(fullname)});

		for(auto& key : key_vec) {
			CHECK(key == key_vec[0]);
		}
	}
}

} /* namespace Tests */
} /* namespace PhotoLibrary */
