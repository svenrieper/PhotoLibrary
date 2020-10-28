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
template<class TRecord>
int addEntry(TRecord, PhotoLibrary::Backend::InterfaceBase<TRecord>*);

void examples(PhotoLibrary::Backend::BackendFactory* db) {
	exampleKeywords(db);
	exampleDirectories(db);
	exampleAlbums(db);
}

void exampleKeywords(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::KeywordRecord;

	PhotoLibrary::Backend::InterfaceBase<KeywordRecord>* keywordDBIF = db->getKeywordInterface();
	int animal = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Animal"), keywordDBIF);
	int giraffe = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Giraffe"), keywordDBIF);
	int elepant = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), keywordDBIF);
	int spider = addEntry(KeywordRecord(animal, KeywordRecord::Options::NONE, "Spider"), keywordDBIF);
	int bird_eating_spider = addEntry(KeywordRecord(spider, KeywordRecord::Options::NONE, "Bird-Eating Spider"), keywordDBIF);
	int building = addEntry(KeywordRecord(0, KeywordRecord::Options::NONE, "Building"), keywordDBIF);
	int theatre = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Theatre"), keywordDBIF);
	int bridge = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Bridge"), keywordDBIF);
	int tower_bridge = addEntry(KeywordRecord(bridge, KeywordRecord::Options::ROW_EXPANDED, "Tower Bridge"), keywordDBIF);
	int museum = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Museum"), keywordDBIF);
	int art_galerie = addEntry(KeywordRecord(museum, KeywordRecord::Options::ROW_EXPANDED, "Art Galerie"), keywordDBIF);
	int elepant2 = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), keywordDBIF);

	int plant = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Plant"), keywordDBIF);
	int tree = addEntry(KeywordRecord(plant, KeywordRecord::Options::ROW_EXPANDED, "Tree"), keywordDBIF);
	int oak = addEntry(KeywordRecord(tree, KeywordRecord::Options::NONE, "Oak"), keywordDBIF);
}

void exampleDirectories(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::DirectoryRecord;

	PhotoLibrary::Backend::InterfaceBase<DirectoryRecord>* directory_interface = db->getDirectoriesInterface();
	int d2017 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017"), directory_interface);
	int d2018 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2018", "/home/user/Photos/2018"), directory_interface);
	int d2019 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2019", "/home/user/Photos/2019"), directory_interface);
	int d2020 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2020", "/home/user/Photos/2020"), directory_interface);

	int d2017_01 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	int d2017_04 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::ROW_EXPANDED, "04", "04"), directory_interface);
	int d2017_06 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "06", "06"), directory_interface);

	int d2018_01 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	int d2018_04 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "04", "04"), directory_interface);
	int d2018_06 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "06", "06"), directory_interface);

	int d2017_04_01 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "01", "01"), directory_interface);
	int d2017_04_04 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "04", "04"), directory_interface);
	int d2017_04_13 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "13", "13"), directory_interface);
	int d2017_04_23 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "23", "23"), directory_interface);
	int d2017_04_27 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "27", "27"), directory_interface);
	int d2017_04_30 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "30", "30"), directory_interface);
}

void exampleAlbums(PhotoLibrary::Backend::BackendFactory* db) {
	using PhotoLibrary::Backend::AlbumRecord;

	PhotoLibrary::Backend::InterfaceBase<AlbumRecord>* album_interface = db->getAlbumInterface();
	int holiday = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Holiday"), album_interface);
	int zoo = addEntry(AlbumRecord(0, AlbumRecord::Options::NONE, "Zoo"), album_interface);
	int architecture = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Architecture"), album_interface);

	int city_trips = addEntry(AlbumRecord(holiday, AlbumRecord::Options::ALBUM_IS_SET, "City Trips"), album_interface);
	int beach = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Beach"), album_interface);
	int venice2 = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Venice"), album_interface);

	int venice = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Venice"), album_interface);
	int tokyo = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Tokyo"), album_interface);
	int moscow = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Moscow"), album_interface);
}

template<class TRecord>
int addEntry(TRecord entry, PhotoLibrary::Backend::InterfaceBase<TRecord>* db) {
	db->newEntry(entry);
	return db->getID(entry);
}

} /* namespace PhotoLibrary */


