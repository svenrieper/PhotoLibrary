/*
 * examples.cpp
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

#include "Backend/BackendFactory.h"

namespace PhotoLibrary {

//Fill the database with some examples during development
void exampleKeywords(PhotoLibrary::Backend::BackendFactory& db);
void exampleDirectories(PhotoLibrary::Backend::BackendFactory& db);
void exampleAlbums(PhotoLibrary::Backend::BackendFactory& db);
void examplePictures(PhotoLibrary::Backend::BackendFactory& db);
template<class TRecord>
int addEntry(const TRecord& entry, PhotoLibrary::Backend::BackendFactory& backend);

void examples(PhotoLibrary::Backend::BackendFactory* db) {
	exampleKeywords(*db);
	exampleDirectories(*db);
	exampleAlbums(*db);
	examplePictures(*db);
}

void exampleKeywords(PhotoLibrary::Backend::BackendFactory& db) {
	using PhotoLibrary::Backend::RecordClasses::KeywordRecord;

	[[maybe_unused]] int animal = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Animal"), db);
	[[maybe_unused]] int giraffe = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Giraffe"), db);
	[[maybe_unused]] int elepant = addEntry(KeywordRecord(animal, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), db);
	[[maybe_unused]] int spider = addEntry(KeywordRecord(animal, KeywordRecord::Options::NONE, "Spider"), db);
	[[maybe_unused]] int bird_eating_spider = addEntry(KeywordRecord(spider, KeywordRecord::Options::NONE, "Bird-Eating Spider"), db);
	[[maybe_unused]] int building = addEntry(KeywordRecord(0, KeywordRecord::Options::NONE, "Building"), db);
	[[maybe_unused]] int theatre = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Theatre"), db);
	[[maybe_unused]] int bridge = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Bridge"), db);
	[[maybe_unused]] int tower_bridge = addEntry(KeywordRecord(bridge, KeywordRecord::Options::ROW_EXPANDED, "Tower Bridge"), db);
	[[maybe_unused]] int museum = addEntry(KeywordRecord(building, KeywordRecord::Options::ROW_EXPANDED, "Museum"), db);
	[[maybe_unused]] int art_galerie = addEntry(KeywordRecord(museum, KeywordRecord::Options::ROW_EXPANDED, "Art Galerie"), db);
	[[maybe_unused]] int elepant2 = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Elephant"), db);

	[[maybe_unused]] int plant = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "Plant"), db);
	[[maybe_unused]] int tree = addEntry(KeywordRecord(plant, KeywordRecord::Options::ROW_EXPANDED, "Tree"), db);
	[[maybe_unused]] int oak = addEntry(KeywordRecord(tree, KeywordRecord::Options::NONE, "Oak"), db);
}

void exampleDirectories(PhotoLibrary::Backend::BackendFactory& db) {
	using PhotoLibrary::Backend::RecordClasses::DirectoryRecord;

	[[maybe_unused]] int d2017 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2017", "/home/user/Photos/2017"), db);
	[[maybe_unused]] int d2018 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2018", "/home/user/Photos/2018"), db);
	[[maybe_unused]] int d2019 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2019", "/home/user/Photos/2019"), db);
	[[maybe_unused]] int d2020 = addEntry(DirectoryRecord(0, DirectoryRecord::Options::NONE, "2020", "/home/user/Photos/2020"), db);

	[[maybe_unused]] int d2017_01 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "01", "01"), db);
	[[maybe_unused]] int d2017_04 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::ROW_EXPANDED, "04", "04"), db);
	[[maybe_unused]] int d2017_06 = addEntry(DirectoryRecord(d2017, DirectoryRecord::Options::NONE, "06", "06"), db);

	[[maybe_unused]] int d2018_01 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "01", "01"), db);
	[[maybe_unused]] int d2018_04 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "04", "04"), db);
	[[maybe_unused]] int d2018_06 = addEntry(DirectoryRecord(d2018, DirectoryRecord::Options::NONE, "06", "06"), db);

	[[maybe_unused]] int d2017_04_01 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "01", "01"), db);
	[[maybe_unused]] int d2017_04_04 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "04", "04"), db);
	[[maybe_unused]] int d2017_04_13 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "13", "13"), db);
	[[maybe_unused]] int d2017_04_23 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "23", "23"), db);
	[[maybe_unused]] int d2017_04_27 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "27", "27"), db);
	[[maybe_unused]] int d2017_04_30 = addEntry(DirectoryRecord(d2017_04, DirectoryRecord::Options::NONE, "30", "30"), db);
}

void exampleAlbums(PhotoLibrary::Backend::BackendFactory& db) {
	using PhotoLibrary::Backend::RecordClasses::AlbumRecord;

	[[maybe_unused]] int holiday = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Holiday"), db);
	[[maybe_unused]] int zoo = addEntry(AlbumRecord(0, AlbumRecord::Options::NONE, "Zoo"), db);
	[[maybe_unused]] int architecture = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "Architecture"), db);

	[[maybe_unused]] int city_trips = addEntry(AlbumRecord(holiday, AlbumRecord::Options::ALBUM_IS_SET, "City Trips"), db);
	[[maybe_unused]] int beach = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Beach"), db);
	[[maybe_unused]] int venice2 = addEntry(AlbumRecord(holiday, AlbumRecord::Options::NONE, "Venice"), db);

	[[maybe_unused]] int venice = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Venice"), db);
	[[maybe_unused]] int tokyo = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Tokyo"), db);
	[[maybe_unused]] int moscow = addEntry(AlbumRecord(city_trips, AlbumRecord::Options::NONE, "Moscow"), db);
}

void examplePictures(PhotoLibrary::Backend::BackendFactory& db) {
	using PhotoLibrary::Backend::RecordClasses::DirectoryRecord;
	using PhotoLibrary::Backend::RecordClasses::PhotoRecord;
	using PhotoLibrary::Backend::RecordClasses::AlbumRecord;
	using PhotoLibrary::Backend::RecordClasses::KeywordRecord;

	PhotoLibrary::Backend::RelationsInterfaceBase* photos_albums_interface = db.getPhotosAlbumsRelationsInterface();
	PhotoLibrary::Backend::RelationsInterfaceBase* photos_keywords_interface = db.getPhotosKeywordsRelationsInterface();

	[[maybe_unused]] int here = addEntry(DirectoryRecord(0, DirectoryRecord::Options::ROW_EXPANDED, "here", "."), db);
	[[maybe_unused]] int examples = addEntry(DirectoryRecord(here, DirectoryRecord::Options::NONE, "example pictures", "example pictures"), db);

	[[maybe_unused]] int photo_0 = addEntry(PhotoRecord(examples, "0.jpg", 4, 1604149700, 1920, 1080), db);
	[[maybe_unused]] int photo_1 = addEntry(PhotoRecord(examples, "1.jpg", 1, 1604149700, 1920, 1080), db);
	[[maybe_unused]] int photo_2 = addEntry(PhotoRecord(examples, "2.jpg", 5, 1604149000, 1920, 1080), db);
	[[maybe_unused]] int photo_3 = addEntry(PhotoRecord(examples, "3.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_4 = addEntry(PhotoRecord(examples, "4.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_5 = addEntry(PhotoRecord(examples, "5.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_6 = addEntry(PhotoRecord(examples, "6.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_7 = addEntry(PhotoRecord(examples, "7.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_8 = addEntry(PhotoRecord(examples, "8.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_9 = addEntry(PhotoRecord(examples, "9.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_10 = addEntry(PhotoRecord(examples, "10.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_11 = addEntry(PhotoRecord(examples, "11.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_12 = addEntry(PhotoRecord(examples, "12.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_13 = addEntry(PhotoRecord(examples, "13.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_14 = addEntry(PhotoRecord(examples, "14.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_15 = addEntry(PhotoRecord(examples, "15.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_16 = addEntry(PhotoRecord(examples, "16.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_17 = addEntry(PhotoRecord(examples, "17.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_18 = addEntry(PhotoRecord(examples, "18.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_19 = addEntry(PhotoRecord(examples, "19.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_20 = addEntry(PhotoRecord(examples, "20.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_21 = addEntry(PhotoRecord(examples, "21.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_22 = addEntry(PhotoRecord(examples, "22.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_23 = addEntry(PhotoRecord(examples, "23.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_24 = addEntry(PhotoRecord(examples, "24.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_25 = addEntry(PhotoRecord(examples, "25.jpg", 0, 1604150001, 1920, 1080), db);
	[[maybe_unused]] int photo_26 = addEntry(PhotoRecord(examples, "26.jpg", 0, 1604150001, 1920, 1080), db);

	[[maybe_unused]] int photo_a = addEntry(PhotoRecord(examples, "a.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_b = addEntry(PhotoRecord(examples, "b.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_c = addEntry(PhotoRecord(examples, "c.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_d = addEntry(PhotoRecord(examples, "d.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_e = addEntry(PhotoRecord(examples, "e.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_f = addEntry(PhotoRecord(examples, "f.jpg", 0, 1604150001, 1080, 1920), db);
	[[maybe_unused]] int photo_g = addEntry(PhotoRecord(examples, "g.jpg", 0, 1604150001, 1080, 1920), db);

	[[maybe_unused]] int a_examples = addEntry(AlbumRecord(0, AlbumRecord::Options::ALBUM_IS_SET, "examples"), db);
	[[maybe_unused]] int a_hex_dig = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Hex digits"), db);
	[[maybe_unused]] int a_dec_dig = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Dec digits"), db);
	[[maybe_unused]] int a_dec_num = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Decimal numbers"), db);
	[[maybe_unused]] int a_letters = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Letters"), db);
	[[maybe_unused]] int a_oct_num = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Octal numbers"), db);
	[[maybe_unused]] int a_oct_dig = addEntry(AlbumRecord(a_examples, AlbumRecord::Options::NONE, "Octal digits"), db);

	std::vector<int> v_hex_dig {photo_0, photo_1, photo_2, photo_3, photo_4, photo_5, photo_6, photo_7, photo_8, photo_9,
				photo_a, photo_b, photo_c, photo_d, photo_e, photo_f};
	std::vector<int> v_dec_dig {photo_0, photo_1, photo_2, photo_3, photo_4, photo_5, photo_6, photo_7, photo_8, photo_9};
	std::vector<int> v_dec_num {photo_0, photo_1, photo_2, photo_3, photo_4, photo_5, photo_6, photo_7, photo_8, photo_9,
				photo_10, photo_11, photo_12, photo_13, photo_14, photo_15, photo_16, photo_17, photo_18, photo_19,
				photo_20, photo_21, photo_22, photo_23, photo_24, photo_25, photo_26};
	std::vector<int> v_oct_dig {photo_0, photo_1, photo_2, photo_3, photo_4, photo_5, photo_6, photo_7};
	std::vector<int> v_oct_num {photo_0, photo_1, photo_2, photo_3, photo_4, photo_5, photo_6, photo_7,
				photo_10, photo_11, photo_12, photo_13, photo_14, photo_15, photo_16, photo_17,
				photo_20, photo_21, photo_22, photo_23, photo_24, photo_25, photo_26};
	std::vector<int> v_letters {photo_a, photo_b, photo_c, photo_d, photo_e, photo_f, photo_g};

	for(int a : v_hex_dig)
		photos_albums_interface->newRelation(a, a_hex_dig);
	for(int a : v_dec_dig)
		photos_albums_interface->newRelation(a, a_dec_dig);
	for(int a : v_dec_num)
		photos_albums_interface->newRelation(a, a_dec_num);
	for(int a : v_oct_dig)
		photos_albums_interface->newRelation(a, a_oct_dig);
	for(int a : v_oct_num)
		photos_albums_interface->newRelation(a, a_oct_num);
	for(int a : v_letters)
		photos_albums_interface->newRelation(a, a_letters);

	[[maybe_unused]] int k_examples = addEntry(KeywordRecord(0, KeywordRecord::Options::ROW_EXPANDED, "examples"), db);
	[[maybe_unused]] int k_hex_dig = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Hex digits"), db);
	[[maybe_unused]] int k_dec_dig = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Dec digits"), db);
	[[maybe_unused]] int k_dec_num = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Decimal numbers"), db);
	[[maybe_unused]] int k_letters = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Letters"), db);
	[[maybe_unused]] int k_oct_num = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Octal numbers"), db);
	[[maybe_unused]] int k_oct_dig = addEntry(KeywordRecord(k_examples, KeywordRecord::Options::NONE, "Octal digits"), db);

	for(int a : v_hex_dig)
		photos_keywords_interface->newRelation(a, k_hex_dig);
	for(int a : v_dec_dig)
		photos_keywords_interface->newRelation(a, k_dec_dig);
	for(int a : v_dec_num)
		photos_keywords_interface->newRelation(a, k_dec_num);
	for(int a : v_oct_dig)
		photos_keywords_interface->newRelation(a, k_oct_dig);
	for(int a : v_oct_num)
		photos_keywords_interface->newRelation(a, k_oct_num);
	for(int a : v_letters)
		photos_keywords_interface->newRelation(a, k_letters);
}

template<class TRecord>
int addEntry(const TRecord& entry, PhotoLibrary::Backend::BackendFactory& backend) {
	backend.newEntry(entry);
	return backend.getID(entry);
}

} /* namespace PhotoLibrary */


