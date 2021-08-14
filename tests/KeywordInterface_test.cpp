/*
 * KeywordInterface_test.cpp
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
#include <support.h>
#include <catch2/catch.hpp>

using namespace PhotoLibrary;
using PhotoLibrary::Backend::DatabaseInterface::DatabaseFactory;
using PhotoLibrary::Backend::BackendFactory;
using PhotoLibrary::Backend::DatabaseInterface::constraint_error;
using Keyword = PhotoLibrary::Backend::RecordClasses::KeywordRecord;
using PhotoLibrary::Backend::RecordClasses::KeywordRecord;
using KeywordInterface = Backend::InterfaceBase<KeywordRecord>;

TEMPLATE_TEST_CASE("Test the Keyword interface of the Adapter and the Backend", "[keywords][adapter][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	KeywordInterface *keyIF = db.getKeywordInterface();

	SECTION( "The 'id' of a keyword should not be 0", "[keywords][database]" ) {
		KeywordRecord some_key(0, KeywordRecord::Options::NONE, "some key");

		REQUIRE_NOTHROW(keyIF->newEntry(some_key));
		CHECK(keyIF->getID(some_key) != 0);
	}

	SECTION( "Keywords should be in database afer being added", "[keywords][database][newEntry][getEntry][getID]" ) {
		Glib::ustring keyword1 = "some other key";
		KeywordRecord key1(0, KeywordRecord::Options::NONE, keyword1);
		int keyId;
		REQUIRE_NOTHROW(keyIF->newEntry(key1));
		REQUIRE_NOTHROW(keyId = keyIF->getID(key1));
		CHECK( keyIF->getEntry(keyId) == key1 );

		Glib::ustring keyword2 = "yet another key";
		KeywordRecord key2(0, KeywordRecord::Options::NONE, keyword2);
		int keyId2;
		REQUIRE_NOTHROW(keyIF->newEntry(key2));
		REQUIRE_NOTHROW(keyId2 = keyIF->getID(key2));
		CHECK( keyIF->getEntry(keyId2) == key2 );
		CHECK( keyIF->getEntry(keyId2).getKeyword() == keyword2 );

		Glib::ustring keyword3 = "åöß";
		KeywordRecord key3(0, KeywordRecord::Options::NONE, keyword3);
		int keyId3;
		REQUIRE_NOTHROW(keyIF->newEntry(key3));
		REQUIRE_NOTHROW(keyId3 = keyIF->getID(key3));
		CHECK( keyIF->getEntry(keyId3) == key3 );
		CHECK( keyIF->getEntry(keyId3).getKeyword() == keyword3 );

		Keyword animal(0, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
		int animal_id;
		db.getKeywordInterface()->newEntry(animal);
		REQUIRE_NOTHROW(animal_id = db.getKeywordInterface()->getID(animal));
		CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);

		Keyword elephant(animal_id, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
		int elephant_id;
		db.getKeywordInterface()->newEntry(elephant);
		REQUIRE_NOTHROW(elephant_id = db.getKeywordInterface()->getID(elephant));
		CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);

		Keyword asian_elephant(elephant_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
		int asian_elephant_id;
		db.getKeywordInterface()->newEntry(asian_elephant);
		REQUIRE_NOTHROW( asian_elephant_id = db.getKeywordInterface()->getID(asian_elephant));
		CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);

		Keyword african_elephant(elephant_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "African Elephant", "Elephant");
		int african_elephant_id;
		db.getKeywordInterface()->newEntry(african_elephant);
		REQUIRE_NOTHROW( african_elephant_id = db.getKeywordInterface()->getID(african_elephant));
		CHECK(db.getKeywordInterface()->getEntry(african_elephant_id) == african_elephant);

		Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
		int giraffe_id;
		db.getKeywordInterface()->newEntry(giraffe);
		REQUIRE_NOTHROW(giraffe_id = db.getKeywordInterface()->getID(giraffe));
		CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);

		Keyword empty(0, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "");
		int empty_id;
		db.getKeywordInterface()->newEntry(empty);
		REQUIRE_NOTHROW(empty_id = db.getKeywordInterface()->getID(empty));
		CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);

		SECTION( "After updating a keyword it should contain the new information (using the Keyword struct)", "[keywords][database][updateEntry]" ) {
			Keyword animal(0, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "");
			db.getKeywordInterface()->updateEntry(animal_id, animal);
			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);


			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
			elephant = Keyword(animal_id, Keyword::Options::NONE, "Elephant", "Elephant");
			db.getKeywordInterface()->updateEntry(elephant_id, elephant);
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);

			Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
			db.getKeywordInterface()->updateEntry(giraffe_id, giraffe);
			CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);

			Keyword empty(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED, "Squirrel");
			db.getKeywordInterface()->updateEntry(empty_id, empty);
			CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);
		}

		SECTION( "Moving or renaming a keyword s.t. two identical keywords (modulo capitalisation) have the same parent should throw PhotoLibrary::constraint_error (using the Keyword struct)", "[keywords][database][updateEntry]" ) {
			Keyword elephant(animal_id, Keyword::Options::NONE, "giraffe", "Elephant");
			CHECK_THROWS_AS(db.getKeywordInterface()->updateEntry(elephant_id, elephant), constraint_error);

			Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant");
			CHECK_THROWS_AS(db.getKeywordInterface()->updateEntry(giraffe_id, giraffe), constraint_error);

			Keyword empty(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED, "Elephant");
			CHECK_THROWS_AS(db.getKeywordInterface()->updateEntry(empty_id, empty), constraint_error);
		}

//		//No such checks for now
//		TEST_CASE( "Setting child or self as parent should throw runtime_error (using the Keyword struct)", "[keywords][database][updateEntry]" ) {
//			Keyword animal(giraffe_id, Keyword::INCLUDE_ON_EXPORT | Keyword::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "");
//			CHECK_THROWS_AS(db.getKeywordInterface()->updateKeyword(animal_id, animal), std::runtime_error);
//	//		CHECK(db.getKeywordInterface()->getKeyword(animal_id) == animal);
//
//	//		Keyword elephant(animal_id, Keyword::NONE, "Elephant", "Elephant");
//	//		db.getKeywordInterface()->updateKeyword(elephant_id, elephant);
//	//		CHECK(db.getKeywordInterface()->getKeyword(elephant_id) == elephant);
//	//
//	//		Keyword giraffe(animal_id, Keyword::INCLUDE_ON_EXPORT | Keyword::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
//	//		db.getKeywordInterface()->updateKeyword(giraffe_id, giraffe);
//	//		CHECK(db.getKeywordInterface()->getKeyword(giraffe_id) == giraffe);
//
//			Keyword empty(empty_id, Keyword::INCLUDE_ON_EXPORT | Keyword::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::ROW_EXPANDED, "Elephant");
//			CHECK_THROWS_AS(db.getKeywordInterface()->updateKeyword(empty_id, empty), std::runtime_error);
//		}

		//delete
		SECTION("Delete leaves on defferent levels.", "[deleteEntry]") {
			db.getKeywordInterface()->deleteEntry(empty_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(empty_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
			CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);
			CHECK(db.getKeywordInterface()->getEntry(african_elephant_id) == african_elephant);
			CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);

			db.getKeywordInterface()->deleteEntry(african_elephant_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(african_elephant_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
			CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);
			CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);

			db.getKeywordInterface()->deleteEntry(giraffe_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(giraffe_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
			CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);

			db.getKeywordInterface()->deleteEntry(asian_elephant_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(asian_elephant_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);

			db.getKeywordInterface()->deleteEntry(elephant_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(elephant_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);

			db.getKeywordInterface()->deleteEntry(animal_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(animal_id), std::runtime_error);
		}

		SECTION("Children should be deleted as well (test 1).", "[deleteEntry]") {
			db.getKeywordInterface()->deleteEntry(elephant_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(asian_elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(african_elephant_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
			CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);
			CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);

			db.getKeywordInterface()->deleteEntry(animal_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(animal_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(giraffe_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);
		}

		SECTION("Children should be deleted as well (test 2).", "[deleteEntry]") {
			db.getKeywordInterface()->deleteEntry(animal_id);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(animal_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(giraffe_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(asian_elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.getKeywordInterface()->getEntry(african_elephant_id), std::runtime_error);

			CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);
		}
	}

	SECTION( "Inserting a Keyword twice (modulo capitalisation) should throw PhotoLibrary::constraint_error", "[keywords][database][newEntry]" ) {
		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		REQUIRE_NOTHROW(keyIF->newEntry(key1));
		CHECK_THROWS_AS(keyIF->newEntry(key1), constraint_error);

		KeywordRecord key2(0, KeywordRecord::Options::NONE, "Some Key");
		CHECK_THROWS_AS(keyIF->newEntry(key2), constraint_error);
	}

	SECTION( "Reqesting keyword from empty database or wrong id should throw runtime_error (excluding internal keywords)", "[keywords][database][getEntry]" ) {
		CHECK_THROWS_AS(keyIF->getEntry(51), std::runtime_error);

		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		int keyId;
		REQUIRE_NOTHROW(keyIF->newEntry(key1));
		REQUIRE_NOTHROW(keyId = keyIF->getID(key1));
		CHECK_THROWS_AS(keyIF->getEntry(keyId+5), std::runtime_error);
	}

	SECTION( "Keyword should have the parent it was given", "[keywords][database][newEntry][getEntry][getID]" ) {
		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		int keyId;
		REQUIRE_NOTHROW(keyIF->newEntry(key1));
		REQUIRE_NOTHROW(keyId = keyIF->getID(key1));
		REQUIRE( keyIF->getEntry(keyId).getParent() == 0 );

		KeywordRecord key2(keyId, KeywordRecord::Options::NONE, "some other key");
		int keyId2;
		REQUIRE_NOTHROW(keyIF->newEntry(key2));
		REQUIRE_NOTHROW(keyId2 = keyIF->getID(key2));
		REQUIRE( keyIF->getEntry(keyId2).getParent() == keyId );

		KeywordRecord key3(keyId, KeywordRecord::Options::NONE, "yet another key");
		int keyId3;
		REQUIRE_NOTHROW(keyIF->newEntry(key3));
		REQUIRE_NOTHROW(keyId3 = keyIF->getID(key3));
		REQUIRE( keyIF->getEntry(keyId3).getParent() == keyId );

		SECTION( "Keyword should have the new parent after is was changed", "[keywords][database][setParent]" ) {
			keyIF->setParent(keyId3, keyId2);
			CHECK(keyIF->getEntry(keyId3).getParent() == keyId2 );
		}
	}

	SECTION( "Vector of children should contain the children and only the children", "[keywords][database][getChildren][getNumberChildren]" ) {
		using Catch::Matchers::VectorContains;
		using Catch::Matchers::Equals;

		KeywordRecord key1(0, KeywordRecord::Options::NONE, "bear");
		int keyId1;
		REQUIRE_NOTHROW(keyIF->newEntry(key1));
		REQUIRE_NOTHROW(keyId1 = keyIF->getID(key1));

		KeywordRecord key2(0, KeywordRecord::Options::NONE, "spider");
		int keyId2;
		REQUIRE_NOTHROW(keyIF->newEntry(key2));
		REQUIRE_NOTHROW(keyId2 = keyIF->getID(key2));

		KeywordRecord key3(0, KeywordRecord::Options::NONE, "Bee");
		int keyId3;
		REQUIRE_NOTHROW(keyIF->newEntry(key3));
		REQUIRE_NOTHROW(keyId3 = keyIF->getID(key3));

		KeywordRecord key4(keyId3, KeywordRecord::Options::NONE, "Bumblebee");
		int keyId4;
		REQUIRE_NOTHROW(keyIF->newEntry(key4));
		REQUIRE_NOTHROW(keyId4 = keyIF->getID(key4));

		std::vector<int> childrenOfZero = keyIF->getChildren(0);
		std::vector<int> childrenOfBee = keyIF->getChildren(keyId3);

		CHECK(childrenOfZero.size() == keyIF->getNumberChildren(0));
		CHECK(childrenOfBee.size() == keyIF->getNumberChildren(keyId3));

		CHECK_THAT( childrenOfZero, VectorContains( keyId1 ) );
		CHECK_THAT( childrenOfZero, VectorContains( keyId2 ) );
		CHECK_THAT( childrenOfZero, VectorContains( keyId3 ) );
		CHECK_THAT( childrenOfBee, VectorContains( keyId4 ) );

		CHECK_THAT( childrenOfBee, !VectorContains( keyId1 ) );
		CHECK_THAT( childrenOfBee, !VectorContains( keyId2 ) );
		CHECK_THAT( childrenOfBee, !VectorContains( keyId3 ) );
		CHECK_THAT( childrenOfZero, !VectorContains( keyId4 ) );

		CHECK_THAT( keyIF->getChildren(keyId1), Equals( std::vector<int>(0) ) );
		CHECK_THAT( keyIF->getChildren(keyId2), Equals( std::vector<int>(0) ) );
		CHECK_THAT( keyIF->getChildren(keyId4), Equals( std::vector<int>(0) ) );
	}

	SECTION( "Don't hand unchecked userinput to database", "[keywords][database][newEntry]" ) {
		using Catch::Matchers::VectorContains;

		SECTION("Using 'newKeyword(Keyword)' and Keyword::keyword") {
			const char *keywordWithSQL_string = "keyword', '0', '0');"
					"INSERT INTO Keywords (keyword, parent) VALUES"
					"('some other key', '0');";
			KeywordRecord keywordWithSQL(0, KeywordRecord::Options::NONE, keywordWithSQL_string);
			int keyId = 0;
			CHECK_NOTHROW(keyIF->newEntry(keywordWithSQL));
			CHECK_NOTHROW(keyId = keyIF->getID(keywordWithSQL));

			std::vector<int> childrenOfZero = keyIF->getChildren(0);

			CHECK(childrenOfZero.size() == 1);
			CHECK(keyIF->getEntry(keyId) == keywordWithSQL);

			const char *deleteTable_string = "keyword2', '0', '', 0);"
					"DROP TABLE Keywords;";
			KeywordRecord deleteTable(0, KeywordRecord::Options::NONE, deleteTable_string);
			int keyId2 = 0;
			CHECK_NOTHROW(keyIF->newEntry(deleteTable));
			CHECK_NOTHROW(keyId2 = keyIF->getID(deleteTable));

			REQUIRE_NOTHROW(keyIF->getChildren(0));
			CHECK(keyIF->getEntry(keyId2) == deleteTable);
		}

		SECTION("Using 'newKeyword(Keyword)' and Keyword::synonyms") {
			const char *keywordWithSQL_string = "keyword', '0');"
					"INSERT INTO Keywords (keyword, parent) VALUES"
					"('some other key', '0');";
			KeywordRecord keywordWithSQL(0, KeywordRecord::Options::NONE, "keyword", keywordWithSQL_string);
			int keyId = 0;
			CHECK_NOTHROW(keyIF->newEntry(keywordWithSQL));
			CHECK_NOTHROW(keyId = keyIF->getID(keywordWithSQL));

			std::vector<int> childrenOfZero = keyIF->getChildren(0);

			CHECK(childrenOfZero.size() == 1);
			CHECK(keyIF->getEntry(keyId) == keywordWithSQL);

			const char *deleteTable_string = "keyword2', 0);"
					"DROP TABLE Keywords;";
			KeywordRecord deleteTable(0, KeywordRecord::Options::NONE, "keyword2", deleteTable_string);
			int keyId2 = 0;
			CHECK_NOTHROW(keyIF->newEntry(deleteTable));
			CHECK_NOTHROW(keyId2 = keyIF->getID(deleteTable));

			REQUIRE_NOTHROW(keyIF->getChildren(0));
			CHECK(keyIF->getEntry(keyId2) == deleteTable);
		}

		SECTION("Using 'updateKeyword(Keyword)'") {
			//Initilaise
			KeywordRecord animal(0, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
			int animal_id;
			db.getKeywordInterface()->newEntry(animal);
			REQUIRE_NOTHROW(animal_id = db.getKeywordInterface()->getID(animal));
			CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);

			KeywordRecord elephant(animal_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
			int elephant_id;
			db.getKeywordInterface()->newEntry(elephant);
			REQUIRE_NOTHROW(elephant_id = db.getKeywordInterface()->getID(elephant));
			CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);

			KeywordRecord asian_elephant(elephant_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
			int asian_elephant_id;
			db.getKeywordInterface()->newEntry(asian_elephant);
			REQUIRE_NOTHROW(asian_elephant_id = db.getKeywordInterface()->getID(asian_elephant));
			CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);

			KeywordRecord african_elephant(elephant_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "African Elephant", "Elephant");
			int african_elephant_id;
			db.getKeywordInterface()->newEntry(african_elephant);
			REQUIRE_NOTHROW(african_elephant_id = db.getKeywordInterface()->getID(african_elephant));
			CHECK(db.getKeywordInterface()->getEntry(african_elephant_id) == african_elephant);

			KeywordRecord giraffe(animal_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
			int giraffe_id;
			db.getKeywordInterface()->newEntry(giraffe);
			REQUIRE_NOTHROW(giraffe_id = db.getKeywordInterface()->getID(giraffe));
			CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);

			KeywordRecord empty(0, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "");
			int empty_id;
			db.getKeywordInterface()->newEntry(empty);
			REQUIRE_NOTHROW(empty_id = db.getKeywordInterface()->getID(empty));
			CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);

			SECTION("and using Keyword::keyword") {
				animal.setKeyword() = "' || id;";
				CHECK_NOTHROW(db.getKeywordInterface()->updateEntry(animal_id, animal));

				CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
				CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
				CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);
				CHECK(db.getKeywordInterface()->getEntry(african_elephant_id) == african_elephant);
				CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);
				CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);
			}

			SECTION("and using Keyword::synonyms") {
				animal.setSynonyms() = "' || id, keyword = id;";
				CHECK_NOTHROW(db.getKeywordInterface()->updateEntry(animal_id, animal));

				CHECK(db.getKeywordInterface()->getEntry(animal_id) == animal);
				CHECK(db.getKeywordInterface()->getEntry(elephant_id) == elephant);
				CHECK(db.getKeywordInterface()->getEntry(asian_elephant_id) == asian_elephant);
				CHECK(db.getKeywordInterface()->getEntry(african_elephant_id) == african_elephant);
				CHECK(db.getKeywordInterface()->getEntry(giraffe_id) == giraffe);
				CHECK(db.getKeywordInterface()->getEntry(empty_id) == empty);
			}
		}
	}

	SECTION( "root keyword can't be delted", "[keywords][database][deleteEntry]" ) {
		CHECK_THROWS_AS(db.getKeywordInterface()->deleteEntry(0), std::runtime_error);
	}
}

//TODO parent_id should always exist (after creation, after moving, after update, after deleting parent)

TEST_CASE("BackendFactory::getInterface<KeywordRecord>() returns the keyword interface", "[keywords][backend]") {
	BackendFactory db {":memory:"};
	CHECK(db.getKeywordInterface() == db.getInterface<KeywordRecord>());
}








