/*
 * Keyword_test.cpp
 *
 *  Created on: 14 Sep 2020
 *      Author: Sven Rieper
 */

#include <catch2/catch.hpp>
#include "../src/Backend/Record/KeywordRecord.h"

namespace PhotoLibrary {

using Keyword = Backend::KeywordRecord;
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

} /* namespace PhotoLibrary */
