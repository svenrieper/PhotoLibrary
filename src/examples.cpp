/*
 * examples.cpp
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

#include "Backend/BackendFactory.h"

namespace PhotoLibrary {

//Fill the database with some examples during development
void exampleKeywords(PhotoLibrary::Backend::BackendFactory* db);
void exampleDirectories(PhotoLibrary::Backend::BackendFactory* db);
void exampleAlbums(PhotoLibrary::Backend::BackendFactory* db);
void examplePictures(PhotoLibrary::Backend::BackendFactory* db);
template<class TRecord>
int addEntry(TRecord, PhotoLibrary::Backend::InterfaceBase<TRecord>*);

void examples(PhotoLibrary::Backend::BackendFactory* db) {
	exampleKeywords(db);
	exampleDirectories(db);
	exampleAlbums(db);
	examplePictures(db);
}

void exampleKeywords(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::KeywordRecord;

	PhotoLibrary::Backend::InterfaceBase<KeywordRecord>* keywordDBIF = db->getKeywordInterface();
	[[maybe_unused]] int animal = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Animal"), keywordDBIF);
	[[maybe_unused]] int giraffe = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Giraffe"), keywordDBIF);
	[[maybe_unused]] int elepant = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), keywordDBIF);
	[[maybe_unused]] int spider = addEntry(KeywordRecord(animal, KeywordRecord::Options::NONE, "Spider"), keywordDBIF);
	[[maybe_unused]] int bird_eating_spider = addEntry(KeywordRecord(spider, KeywordRecord::Options::NONE, "Bird-Eating Spider"), keywordDBIF);
	[[maybe_unused]] int building = addEntry(KeywordRecord(0, KeywordRecord::Options::NONE, "Building"), keywordDBIF);
	[[maybe_unused]] int theatre = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Theatre"), keywordDBIF);
	[[maybe_unused]] int bridge = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Bridge"), keywordDBIF);
	[[maybe_unused]] int tower_bridge = addEntry(KeywordRecord(bridge, KeywordRecord::Options::ROW_EXPANDED, "Tower Bridge"), keywordDBIF);
	[[maybe_unused]] int museum = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Museum"), keywordDBIF);
	[[maybe_unused]] int art_galerie = addEntry(KeywordRecord(museum, KeywordRecord::Options::ROW_EXPANDED, "Art Galerie"), keywordDBIF);
	[[maybe_unused]] int elepant2 = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), keywordDBIF);

	[[maybe_unused]] int plant = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Plant"), keywordDBIF);
	[[maybe_unused]] int tree = addEntry(KeywordRecord(plant, KeywordRecord::Options::ROW_EXPANDED, "Tree"), keywordDBIF);
	[[maybe_unused]] int oak = addEntry(KeywordRecord(tree, KeywordRecord::Options::NONE, "Oak"), keywordDBIF);
}

void exampleDirectories(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::DirectoryRecord;

	PhotoLibrary::Backend::InterfaceBase<DirectoryRecord>* directory_interface = db->getDirectoriesInterface();
	[[maybe_unused]] int d2017 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017"), directory_interface);
	[[maybe_unused]] int d2018 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2018", "/home/user/Photos/2018"), directory_interface);
	[[maybe_unused]] int d2019 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2019", "/home/user/Photos/2019"), directory_interface);
	[[maybe_unused]] int d2020 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2020", "/home/user/Photos/2020"), directory_interface);

	[[maybe_unused]] int d2017_01 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	[[maybe_unused]] int d2017_04 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::ROW_EXPANDED, "04", "04"), directory_interface);
	[[maybe_unused]] int d2017_06 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "06", "06"), directory_interface);

	[[maybe_unused]] int d2018_01 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	[[maybe_unused]] int d2018_04 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "04", "04"), directory_interface);
	[[maybe_unused]] int d2018_06 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "06", "06"), directory_interface);

	[[maybe_unused]] int d2017_04_01 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	[[maybe_unused]] int d2017_04_04 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "04", "04"), directory_interface);
	[[maybe_unused]] int d2017_04_13 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "13", "13"), directory_interface);
	[[maybe_unused]] int d2017_04_23 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "23", "23"), directory_interface);
	[[maybe_unused]] int d2017_04_27 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "27", "27"), directory_interface);
	[[maybe_unused]] int d2017_04_30 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "30", "30"), directory_interface);
}

void exampleAlbums(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::AlbumRecord;

	PhotoLibrary::Backend::InterfaceBase<AlbumRecord>* album_interface = db->getAlbumInterface();
	[[maybe_unused]] int holiday = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Holiday"), album_interface);
	[[maybe_unused]] int zoo = addEntry(AlbumRecord(0, AlbumRecord::Options::NONE, "Zoo"), album_interface);
	[[maybe_unused]] int architecture = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Architecture"), album_interface);

	[[maybe_unused]] int city_trips = addEntry(AlbumRecord(holiday, AlbumRecord::Options::ALBUM_IS_SET, "City Trips"), album_interface);
	[[maybe_unused]] int beach = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Beach"), album_interface);
	[[maybe_unused]] int venice2 = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Venice"), album_interface);

	[[maybe_unused]] int venice = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Venice"), album_interface);
	[[maybe_unused]] int tokyo = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Tokyo"), album_interface);
	[[maybe_unused]] int moscow = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Moscow"), album_interface);
}

void examplePictures(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::DirectoryRecord;
	using PhotoLibrary::Backend::PhotoRecord;

	PhotoLibrary::Backend::InterfaceBase<DirectoryRecord>* directory_interface = db->getDirectoriesInterface();
	PhotoLibrary::Backend::InterfaceBase<PhotoRecord>* photo_interface = db->getPhotoInterface();

	[[maybe_unused]] int here = addEntry(DirectoryRecord(0, DirectoryRecord::Options::ROW_EXPANDED, "here", "."), directory_interface);
	[[maybe_unused]] int examples = addEntry(DirectoryRecord(here, DirectoryRecord::Options::NONE, "example pictures", "example pictures"), directory_interface);

	[[maybe_unused]] int photo_1 = addEntry(PhotoRecord(examples, "1.jpg", 1, 1604149700, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_2 = addEntry(PhotoRecord(examples, "2.jpg", 5, 1604149000, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_3 = addEntry(PhotoRecord(examples, "3.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_4 = addEntry(PhotoRecord(examples, "4.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_5 = addEntry(PhotoRecord(examples, "5.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_6 = addEntry(PhotoRecord(examples, "6.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_7 = addEntry(PhotoRecord(examples, "7.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_8 = addEntry(PhotoRecord(examples, "8.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_9 = addEntry(PhotoRecord(examples, "9.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_10 = addEntry(PhotoRecord(examples, "10.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_11 = addEntry(PhotoRecord(examples, "11.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_12 = addEntry(PhotoRecord(examples, "12.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_13 = addEntry(PhotoRecord(examples, "13.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_14 = addEntry(PhotoRecord(examples, "14.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_15 = addEntry(PhotoRecord(examples, "15.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_16 = addEntry(PhotoRecord(examples, "16.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_17 = addEntry(PhotoRecord(examples, "17.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_18 = addEntry(PhotoRecord(examples, "18.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_19 = addEntry(PhotoRecord(examples, "19.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_20 = addEntry(PhotoRecord(examples, "20.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_21 = addEntry(PhotoRecord(examples, "21.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_22 = addEntry(PhotoRecord(examples, "22.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_23 = addEntry(PhotoRecord(examples, "23.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_24 = addEntry(PhotoRecord(examples, "24.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_25 = addEntry(PhotoRecord(examples, "25.jpg", 0, 1604150001, 1920, 1080), photo_interface);
	[[maybe_unused]] int photo_26 = addEntry(PhotoRecord(examples, "26.jpg", 0, 1604150001, 1920, 1080), photo_interface);

	[[maybe_unused]] int photo_a = addEntry(PhotoRecord(examples, "a.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_b = addEntry(PhotoRecord(examples, "b.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_c = addEntry(PhotoRecord(examples, "c.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_d = addEntry(PhotoRecord(examples, "d.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_e = addEntry(PhotoRecord(examples, "e.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_f = addEntry(PhotoRecord(examples, "f.jpg", 0, 1604150001, 1080, 1920), photo_interface);
	[[maybe_unused]] int photo_g = addEntry(PhotoRecord(examples, "g.jpg", 0, 1604150001, 1080, 1920), photo_interface);
}

template<class TRecord>
int addEntry(TRecord entry, PhotoLibrary::Backend::InterfaceBase<TRecord>* db) {
	db->newEntry(entry);
	return db->getID(entry);
}

} /* namespace PhotoLibrary */


