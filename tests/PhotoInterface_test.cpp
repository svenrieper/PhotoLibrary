/*
 * PhotoInterface_test.cpp
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

using Backend::RecordClasses::PhotoRecord;
using Backend::RecordClasses::DirectoryRecord;
using Backend::BackendFactory;
using Backend::InterfaceBase;
using PhotoInterface = InterfaceBase<Backend::RecordClasses::PhotoRecord>;
using DirectoryInterface = InterfaceBase<Backend::RecordClasses::DirectoryRecord>;

TEMPLATE_TEST_CASE("Test the PhotoInterface of the backend", "[photos][interface][backend]", DatabaseFactory, BackendFactory) {
	TestType db { ":memory:" };
	PhotoInterface* photoIF = db.getPhotoInterface();
	DirectoryInterface* dirIF = db.getDirectoriesInterface();

	SECTION( "A photo needs to have an existing parent", "[photos][database][getEntry][newEntry]" ) {
		REQUIRE_THROWS_AS(dirIF->getEntry(5), std::runtime_error);
		PhotoRecord invalid(5);
		CHECK_THROWS_AS(photoIF->newEntry(invalid), constraint_error);
	}

	SECTION("A photo should be in the database after being added (newEntry(PhotoRecord), getID(PhotoRecord))", "[photos][database][newEntry][getID]") {
		DirectoryRecord dir_here(0, DirectoryRecord::Options::ROW_EXPANDED, "here", ".");
		dirIF->newEntry(dir_here);
		int here = dirIF->getID(dir_here);
		DirectoryRecord dir_numbers(here, DirectoryRecord::Options::NONE, "numbers", "numbers");
		dirIF->newEntry(dir_numbers);
		int numbers = dirIF->getID(dir_numbers);
		DirectoryRecord dir_letters(here, DirectoryRecord::Options::NONE, "letters", "letters");
		dirIF->newEntry(dir_letters);
		int letters = dirIF->getID(dir_letters);
		DirectoryRecord dir_examples(here, DirectoryRecord::Options::NONE, "example pictures", "example pictures");
		dirIF->newEntry(dir_examples);
		int examples = dirIF->getID(dir_examples);

		std::vector<PhotoRecord> numbers_vec;
		std::vector<int> numbers_ids;
		numbers_vec.push_back(PhotoRecord(numbers, "0.jpg", 4, 1604149700, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "1.jpg", 1, 1604149700, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "2.jpg", 5, 1604149000, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "3.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "4.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "5.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "6.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "7.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "8.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "9.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "10.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "11.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "12.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "13.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "14.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "15.jpg", 0, 1604150001, 1920, 1080));
		numbers_vec.push_back(PhotoRecord(numbers, "16.jpg", 0, 1604150001, 1920, 1080));

		for(auto photo : numbers_vec)
			photoIF->newEntry(photo);
		for(auto photo : numbers_vec)
			REQUIRE_NOTHROW(numbers_ids.push_back(photoIF->getID(photo)));

		std::vector<PhotoRecord> letters_vec;
		std::vector<int> letters_ids;
		letters_vec.push_back(PhotoRecord(letters, "a.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "b.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "c.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "d.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "e.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "f.jpg", 0, 1604150001, 1080, 1920));
		letters_vec.push_back(PhotoRecord(letters, "g.jpg", 0, 1604150001, 1080, 1920));

		for(auto photo : letters_vec)
			photoIF->newEntry(photo);
		for(auto photo : letters_vec)
			REQUIRE_NOTHROW(letters_ids.push_back(photoIF->getID(photo)));

		SECTION("getChildren(int) returns vector of children", "[getChildren][getNumberChildren]") {
			CHECK_THAT(photoIF->getChildren(numbers), Catch::Matchers::UnorderedEquals(numbers_ids));
			CHECK_THAT(photoIF->getChildren(letters), Catch::Matchers::UnorderedEquals(letters_ids));

			CHECK(photoIF->getChildren(numbers).size() == photoIF->getNumberChildren(numbers));
			CHECK(photoIF->getChildren(letters).size() == photoIF->getNumberChildren(letters));
		}

		SECTION("getEntry(int) == entry entered", "[getEntry]") {
			for(unsigned long i=0; i<numbers_ids.size(); ++i)
				CHECK(numbers_vec[i] == photoIF->getEntry(numbers_ids[i]));
			for(unsigned long i=0; i<letters_ids.size(); ++i)
				CHECK(letters_vec[i] == photoIF->getEntry(letters_ids[i]));
		}

		SECTION("A photo isn't in the database anymore after it was deleted (deleteEntry(int))", "[deleteEntry]") {
			photoIF->deleteEntry(numbers_ids[2]);
			CHECK_THROWS_AS(photoIF->getEntry(numbers_ids[2]), std::runtime_error);

			photoIF->deleteEntry(numbers_ids[4]);
			CHECK_THROWS_AS(photoIF->getEntry(numbers_ids[4]), std::runtime_error);

			photoIF->deleteEntry(letters_ids[0]);
			CHECK_THROWS_AS(photoIF->getEntry(letters_ids[0]), std::runtime_error);
		}

		SECTION("All photos are deleted along their directory", "[deleteEntry]") {
			dirIF->deleteEntry(numbers);
			CHECK_THROWS_AS(dirIF->getEntry(numbers), std::runtime_error);

			for(int photo_id : numbers_ids)
				CHECK_THROWS_AS(photoIF->getEntry(photo_id), std::runtime_error);

			dirIF->deleteEntry(letters);
			CHECK_THROWS_AS(dirIF->getEntry(letters), std::runtime_error);

			for(int photo_id : letters_ids)
				CHECK_THROWS_AS(photoIF->getEntry(photo_id), std::runtime_error);
		}

		SECTION("After updating an entry the updated entry should be in the database", "[updateEntry]") {
			numbers_vec[5] = PhotoRecord(numbers, "05.jpg", 2, 1604170001, 1080, 1920);
			letters_vec[2] = PhotoRecord(letters, "1c.jpg", 5, 1604165001, 20, 150);

			REQUIRE_NOTHROW(photoIF->updateEntry(numbers_ids[5], numbers_vec[5]));
			REQUIRE_NOTHROW(photoIF->updateEntry(letters_ids[2], letters_vec[2]));

			for(unsigned long i=0; i<numbers_ids.size(); ++i)
				CHECK(numbers_vec[i] == photoIF->getEntry(numbers_ids[i]));
			for(unsigned long i=0; i<letters_ids.size(); ++i)
				CHECK(letters_vec[i] == photoIF->getEntry(letters_ids[i]));
		}

		SECTION("After moving a photo, it should be in the new directory", "[setParent]") {
			for(int photo_id : numbers_ids)
				photoIF->setParent(photo_id, examples);

			CHECK_THAT(photoIF->getChildren(examples), Catch::Matchers::UnorderedEquals(numbers_ids));
		}
	}
}

TEST_CASE("BackendFactory::getInterface<AlbumRecord>() returns the photo interface", "[photo][backend]") {
	BackendFactory db {":memory:"};
	CHECK(db.getPhotoInterface() == db.getInterface<PhotoRecord>());
}

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */
