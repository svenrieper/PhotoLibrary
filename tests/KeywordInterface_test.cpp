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

#include "BackendFactory.h"
#include "Record/KeywordRecord.h"
#include "exceptions.h"
#include <support.h>
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace Backend {
namespace Tests {

using PhotoLibrary::DatabaseInterface::constraint_error;
using Keyword = RecordClasses::KeywordRecord;
using RecordClasses::KeywordRecord;

TEST_CASE(
		"Test the Keyword interface of the Adapter and the Backend",
		"[keywords][adapter][backend]"
		) {
	BackendFactory db { ":memory:" };

	SECTION( "The 'id' of a keyword should not be 0", "[keywords][database]" ) {
		KeywordRecord some_key(0, KeywordRecord::Options::NONE, "some key");

		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(some_key));
		CHECK(db.template getID<KeywordRecord>(some_key) != 0);
	}

	SECTION( "Keywords should be in database afer being added", "[keywords][database][newEntry][getEntry][getID]" ) {
		Glib::ustring keyword1 = "some other key";
		KeywordRecord key1(0, KeywordRecord::Options::NONE, keyword1);
		int keyId;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key1));
		REQUIRE_NOTHROW(keyId = db.template getID<KeywordRecord>(key1));
		CHECK( db.template getEntry<KeywordRecord>(keyId) == key1 );

		Glib::ustring keyword2 = "yet another key";
		KeywordRecord key2(0, KeywordRecord::Options::NONE, keyword2);
		int keyId2;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key2));
		REQUIRE_NOTHROW(keyId2 = db.template getID<KeywordRecord>(key2));
		CHECK( db.template getEntry<KeywordRecord>(keyId2) == key2 );
		CHECK( db.template getEntry<KeywordRecord>(keyId2).getKeyword() == keyword2 );

		Glib::ustring keyword3 = "åöß";
		KeywordRecord key3(0, KeywordRecord::Options::NONE, keyword3);
		int keyId3;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key3));
		REQUIRE_NOTHROW(keyId3 = db.template getID<KeywordRecord>(key3));
		CHECK( db.template getEntry<KeywordRecord>(keyId3) == key3 );
		CHECK( db.template getEntry<KeywordRecord>(keyId3).getKeyword() == keyword3 );

		Keyword animal(0, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
		int animal_id;
		db.template newEntry<KeywordRecord>(animal);
		REQUIRE_NOTHROW(animal_id = db.template getID<KeywordRecord>(animal));
		CHECK(db.template getEntry<Keyword>(animal_id) == animal);

		Keyword elephant(animal_id, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
		int elephant_id;
		db.template newEntry<KeywordRecord>(elephant);
		REQUIRE_NOTHROW(elephant_id = db.template getID<KeywordRecord>(elephant));
		CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);

		Keyword asian_elephant(elephant_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
		int asian_elephant_id;
		db.template newEntry<KeywordRecord>(asian_elephant);
		REQUIRE_NOTHROW( asian_elephant_id = db.template getID<KeywordRecord>(asian_elephant));
		CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);

		Keyword african_elephant(elephant_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "African Elephant", "Elephant");
		int african_elephant_id;
		db.template newEntry<KeywordRecord>(african_elephant);
		REQUIRE_NOTHROW( african_elephant_id = db.template getID<KeywordRecord>(african_elephant));
		CHECK(db.template getEntry<Keyword>(african_elephant_id) == african_elephant);

		Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
		int giraffe_id;
		db.template newEntry<KeywordRecord>(giraffe);
		REQUIRE_NOTHROW(giraffe_id = db.template getID<KeywordRecord>(giraffe));
		CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);

		Keyword empty(0, Keyword::Options::INCLUDE_ON_EXPORT|Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "");
		int empty_id;
		db.template newEntry<KeywordRecord>(empty);
		REQUIRE_NOTHROW(empty_id = db.template getID<KeywordRecord>(empty));
		CHECK(db.template getEntry<Keyword>(empty_id) == empty);

		SECTION(
				"After updating a keyword it should contain the new information (using the Keyword struct)",
				"[keywords][database][updateEntry]"
				) {
			Keyword animal(0, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "");
			db.template updateEntry<KeywordRecord>(animal_id, animal);
			CHECK(db.template getEntry<Keyword>(animal_id) == animal);


			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
			elephant = Keyword(animal_id, Keyword::Options::NONE, "Elephant", "Elephant");
			db.template updateEntry<KeywordRecord>(elephant_id, elephant);
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);

			Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
			db.template updateEntry<KeywordRecord>(giraffe_id, giraffe);
			CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);

			Keyword empty(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED, "Squirrel");
			db.template updateEntry<KeywordRecord>(empty_id, empty);
			CHECK(db.template getEntry<Keyword>(empty_id) == empty);
		}

		SECTION( "Moving or renaming a keyword s.t. two identical keywords (modulo capitalisation) have the same parent should throw PhotoLibrary::constraint_error (using the Keyword struct)", "[keywords][database][updateEntry]" ) {
			Keyword elephant(animal_id, Keyword::Options::NONE, "giraffe", "Elephant");
			CHECK_THROWS_AS(db.template updateEntry<KeywordRecord>(elephant_id, elephant), constraint_error);

			Keyword giraffe(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant");
			CHECK_THROWS_AS(db.template updateEntry<KeywordRecord>(giraffe_id, giraffe), constraint_error);

			Keyword empty(animal_id, Keyword::Options::INCLUDE_ON_EXPORT | Keyword::Options::INCLUDE_SYNONYMS_ON_EXPORT | Keyword::Options::ROW_EXPANDED, "Elephant");
			CHECK_THROWS_AS(db.template updateEntry<KeywordRecord>(empty_id, empty), constraint_error);
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
		SECTION("Delete leaves on different levels.", "[deleteEntry]") {
			REQUIRE_NOTHROW(db.template deleteEntry<KeywordRecord>(empty_id));
			CHECK_THROWS_AS(db.template getEntry<Keyword>(empty_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
			CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);
			CHECK(db.template getEntry<Keyword>(african_elephant_id) == african_elephant);
			CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);

			db.template deleteEntry<KeywordRecord>(african_elephant_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(african_elephant_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
			CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);
			CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);

			db.template deleteEntry<KeywordRecord>(giraffe_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(giraffe_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
			CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);

			db.template deleteEntry<KeywordRecord>(asian_elephant_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(asian_elephant_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);

			db.template deleteEntry<KeywordRecord>(elephant_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(elephant_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);

			db.template deleteEntry<KeywordRecord>(animal_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(animal_id), std::runtime_error);
		}

		SECTION("Children should be deleted as well (test 1).", "[deleteEntry]") {
			db.template deleteEntry<KeywordRecord>(elephant_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(asian_elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(african_elephant_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(animal_id) == animal);
			CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);
			CHECK(db.template getEntry<Keyword>(empty_id) == empty);

			db.template deleteEntry<KeywordRecord>(animal_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(animal_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(giraffe_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(empty_id) == empty);
		}

		SECTION("Children should be deleted as well (test 2).", "[deleteEntry]") {
			db.template deleteEntry<KeywordRecord>(animal_id);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(animal_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(giraffe_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(asian_elephant_id), std::runtime_error);
			CHECK_THROWS_AS(db.template getEntry<Keyword>(african_elephant_id), std::runtime_error);

			CHECK(db.template getEntry<Keyword>(empty_id) == empty);
		}
	}

	SECTION(
			"Inserting a Keyword twice (modulo capitalisation) should throw PhotoLibrary::constraint_error",
			"[keywords][database][newEntry]"
			) {
		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key1));
		CHECK_THROWS_AS(db.template newEntry<KeywordRecord>(key1), constraint_error);

		KeywordRecord key2(0, KeywordRecord::Options::NONE, "Some Key");
		CHECK_THROWS_AS(db.template newEntry<KeywordRecord>(key2), constraint_error);
	}

	SECTION( "Reqesting keyword from empty database or wrong id should throw runtime_error (excluding internal keywords)", "[keywords][database][getEntry]" ) {
		CHECK_THROWS_AS(db.template getEntry<KeywordRecord>(51), std::runtime_error);

		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		int keyId;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key1));
		REQUIRE_NOTHROW(keyId = db.template getID<KeywordRecord>(key1));
		CHECK_THROWS_AS(db.template getEntry<KeywordRecord>(keyId+5), std::runtime_error);
	}

	SECTION( "Keyword should have the parent it was given", "[keywords][database][newEntry][getEntry][getID]" ) {
		KeywordRecord key1(0, KeywordRecord::Options::NONE, "some key");
		int keyId;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key1));
		REQUIRE_NOTHROW(keyId = db.template getID<KeywordRecord>(key1));
		REQUIRE( db.template getEntry<KeywordRecord>(keyId).getParent() == 0 );

		KeywordRecord key2(keyId, KeywordRecord::Options::NONE, "some other key");
		int keyId2;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key2));
		REQUIRE_NOTHROW(keyId2 = db.template getID<KeywordRecord>(key2));
		REQUIRE( db.template getEntry<KeywordRecord>(keyId2).getParent() == keyId );

		KeywordRecord key3(keyId, KeywordRecord::Options::NONE, "yet another key");
		int keyId3;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key3));
		REQUIRE_NOTHROW(keyId3 = db.template getID<KeywordRecord>(key3));
		REQUIRE( db.template getEntry<KeywordRecord>(keyId3).getParent() == keyId );

		SECTION( "Keyword should have the new parent after is was changed", "[keywords][database][setParent]" ) {
			db.template setParent<KeywordRecord>(keyId3, keyId2);
			CHECK(db.template getEntry<KeywordRecord>(keyId3).getParent() == keyId2 );
		}
	}

	SECTION( "Vector of children should contain the children and only the children", "[keywords][database][getChildren][getNumberChildren]" ) {
		using Catch::Matchers::VectorContains;
		using Catch::Matchers::Equals;

		KeywordRecord key1(0, KeywordRecord::Options::NONE, "bear");
		int keyId1;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key1));
		REQUIRE_NOTHROW(keyId1 = db.template getID<KeywordRecord>(key1));

		KeywordRecord key2(0, KeywordRecord::Options::NONE, "spider");
		int keyId2;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key2));
		REQUIRE_NOTHROW(keyId2 = db.template getID<KeywordRecord>(key2));

		KeywordRecord key3(0, KeywordRecord::Options::NONE, "Bee");
		int keyId3;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key3));
		REQUIRE_NOTHROW(keyId3 = db.template getID<KeywordRecord>(key3));

		KeywordRecord key4(keyId3, KeywordRecord::Options::NONE, "Bumblebee");
		int keyId4;
		REQUIRE_NOTHROW(db.template newEntry<KeywordRecord>(key4));
		REQUIRE_NOTHROW(keyId4 = db.template getID<KeywordRecord>(key4));

		std::vector<int> childrenOfZero = db.template getChildren<KeywordRecord>(0);
		std::vector<int> childrenOfBee = db.template getChildren<KeywordRecord>(keyId3);

		CHECK(childrenOfZero.size() == db.template getNumberChildren<KeywordRecord>(0));
		CHECK(childrenOfBee.size() == db.template getNumberChildren<KeywordRecord>(keyId3));

		CHECK_THAT( childrenOfZero, VectorContains( keyId1 ) );
		CHECK_THAT( childrenOfZero, VectorContains( keyId2 ) );
		CHECK_THAT( childrenOfZero, VectorContains( keyId3 ) );
		CHECK_THAT( childrenOfBee, VectorContains( keyId4 ) );

		CHECK_THAT( childrenOfBee, !VectorContains( keyId1 ) );
		CHECK_THAT( childrenOfBee, !VectorContains( keyId2 ) );
		CHECK_THAT( childrenOfBee, !VectorContains( keyId3 ) );
		CHECK_THAT( childrenOfZero, !VectorContains( keyId4 ) );

		CHECK_THAT( db.template getChildren<KeywordRecord>(keyId1), Equals( std::vector<int>(0) ) );
		CHECK_THAT( db.template getChildren<KeywordRecord>(keyId2), Equals( std::vector<int>(0) ) );
		CHECK_THAT( db.template getChildren<KeywordRecord>(keyId4), Equals( std::vector<int>(0) ) );
	}

	SECTION( "Don't hand unchecked userinput to database", "[keywords][database][newEntry]" ) {
		using Catch::Matchers::VectorContains;

		SECTION("Using 'newKeyword(Keyword)' and Keyword::keyword") {
			const char *keywordWithSQL_string = "keyword', '0', '0');"
					"INSERT INTO Keywords (keyword, parent) VALUES"
					"('some other key', '0');";
			KeywordRecord keywordWithSQL(0, KeywordRecord::Options::NONE, keywordWithSQL_string);
			int keyId = 0;
			CHECK_NOTHROW(db.template newEntry<KeywordRecord>(keywordWithSQL));
			CHECK_NOTHROW(keyId = db.template getID<KeywordRecord>(keywordWithSQL));

			std::vector<int> childrenOfZero = db.template getChildren<KeywordRecord>(0);

			CHECK(childrenOfZero.size() == 1);
			CHECK(db.template getEntry<KeywordRecord>(keyId) == keywordWithSQL);

			const char *deleteTable_string = "keyword2', '0', '', 0);"
					"DROP TABLE Keywords;";
			KeywordRecord deleteTable(0, KeywordRecord::Options::NONE, deleteTable_string);
			int keyId2 = 0;
			CHECK_NOTHROW(db.template newEntry<KeywordRecord>(deleteTable));
			CHECK_NOTHROW(keyId2 = db.template getID<KeywordRecord>(deleteTable));

			REQUIRE_NOTHROW(db.template getChildren<KeywordRecord>(0));
			CHECK(db.template getEntry<KeywordRecord>(keyId2) == deleteTable);
		}

		SECTION("Using 'newKeyword(Keyword)' and Keyword::synonyms") {
			const char *keywordWithSQL_string = "keyword', '0');"
					"INSERT INTO Keywords (keyword, parent) VALUES"
					"('some other key', '0');";
			KeywordRecord keywordWithSQL(0, KeywordRecord::Options::NONE, "keyword", keywordWithSQL_string);
			int keyId = 0;
			CHECK_NOTHROW(db.template newEntry<KeywordRecord>(keywordWithSQL));
			CHECK_NOTHROW(keyId = db.template getID<KeywordRecord>(keywordWithSQL));

			std::vector<int> childrenOfZero = db.template getChildren<KeywordRecord>(0);

			CHECK(childrenOfZero.size() == 1);
			CHECK(db.template getEntry<KeywordRecord>(keyId) == keywordWithSQL);

			const char *deleteTable_string = "keyword2', 0);"
					"DROP TABLE Keywords;";
			KeywordRecord deleteTable(0, KeywordRecord::Options::NONE, "keyword2", deleteTable_string);
			int keyId2 = 0;
			CHECK_NOTHROW(db.template newEntry<KeywordRecord>(deleteTable));
			CHECK_NOTHROW(keyId2 = db.template getID<KeywordRecord>(deleteTable));

			REQUIRE_NOTHROW(db.template getChildren<KeywordRecord>(0));
			CHECK(db.template getEntry<KeywordRecord>(keyId2) == deleteTable);
		}

		SECTION("Using 'updateKeyword(Keyword)'") {
			//Initilaise
			KeywordRecord animal(0, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Animal", "Animals");
			int animal_id;
			db.template newEntry<KeywordRecord>(animal);
			REQUIRE_NOTHROW(animal_id = db.template getID<KeywordRecord>(animal));
			CHECK(db.template getEntry<Keyword>(animal_id) == animal);

			KeywordRecord elephant(animal_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Elephant", "Elephant");
			int elephant_id;
			db.template newEntry<KeywordRecord>(elephant);
			REQUIRE_NOTHROW(elephant_id = db.template getID<KeywordRecord>(elephant));
			CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);

			KeywordRecord asian_elephant(elephant_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Asian Elephant", "Elephant");
			int asian_elephant_id;
			db.template newEntry<KeywordRecord>(asian_elephant);
			REQUIRE_NOTHROW(asian_elephant_id = db.template getID<KeywordRecord>(asian_elephant));
			CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);

			KeywordRecord african_elephant(elephant_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "African Elephant", "Elephant");
			int african_elephant_id;
			db.template newEntry<KeywordRecord>(african_elephant);
			REQUIRE_NOTHROW(african_elephant_id = db.template getID<KeywordRecord>(african_elephant));
			CHECK(db.template getEntry<Keyword>(african_elephant_id) == african_elephant);

			KeywordRecord giraffe(animal_id, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "Giraffe");
			int giraffe_id;
			db.template newEntry<KeywordRecord>(giraffe);
			REQUIRE_NOTHROW(giraffe_id = db.template getID<KeywordRecord>(giraffe));
			CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);

			KeywordRecord empty(0, KeywordRecord::Options::INCLUDE_ON_EXPORT | KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT, "");
			int empty_id;
			db.template newEntry<KeywordRecord>(empty);
			REQUIRE_NOTHROW(empty_id = db.template getID<KeywordRecord>(empty));
			CHECK(db.template getEntry<Keyword>(empty_id) == empty);

			SECTION("and using Keyword::keyword") {
				animal.setKeyword() = "' || id;";
				CHECK_NOTHROW(db.template updateEntry<KeywordRecord>(animal_id, animal));

				CHECK(db.template getEntry<Keyword>(animal_id) == animal);
				CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
				CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);
				CHECK(db.template getEntry<Keyword>(african_elephant_id) == african_elephant);
				CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);
				CHECK(db.template getEntry<Keyword>(empty_id) == empty);
			}

			SECTION("and using Keyword::synonyms") {
				animal.setSynonyms() = "' || id, keyword = id;";
				CHECK_NOTHROW(db.template updateEntry<KeywordRecord>(animal_id, animal));

				CHECK(db.template getEntry<Keyword>(animal_id) == animal);
				CHECK(db.template getEntry<Keyword>(elephant_id) == elephant);
				CHECK(db.template getEntry<Keyword>(asian_elephant_id) == asian_elephant);
				CHECK(db.template getEntry<Keyword>(african_elephant_id) == african_elephant);
				CHECK(db.template getEntry<Keyword>(giraffe_id) == giraffe);
				CHECK(db.template getEntry<Keyword>(empty_id) == empty);
			}
		}
	}

	SECTION("root keywords can't be deleted", "[keywords][database][deleteEntry]") {
		CHECK_THROWS_AS(db.template deleteEntry<KeywordRecord>(0), constraint_error);
		CHECK_THROWS_AS(db.template deleteEntry<KeywordRecord>(1), constraint_error);
	}

	SECTION("root keywords can't be altered", "[keywords][database][updateEntry][setParent]") {
		CHECK_THROWS_AS(db.updateEntry(0, KeywordRecord(1)), constraint_error);
		CHECK_THROWS_AS(db.updateEntry(1, KeywordRecord(0)), constraint_error);
		CHECK_THROWS_AS(db.template setParent<KeywordRecord>(0, 1), constraint_error);
		CHECK_THROWS_AS(db.template setParent<KeywordRecord>(1, 0), constraint_error);
	}
}

//TODO parent_id should always exist (after creation, after moving, after update, after deleting parent)

//TEST_CASE("BackendFactory::getInterface<KeywordRecord>() returns the keyword interface", "[keywords][backend]") {
//	BackendFactory db {":memory:"};
//	CHECK(db.getKeywordInterface() == db.getInterface<KeywordRecord>());
//}


} /* Tests */
} /* Backend */
} /* PhotoLibrary */
