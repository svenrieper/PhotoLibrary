/*
 * BackendFactory.cpp
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
#include <thread>

namespace PhotoLibrary {
namespace Backend {

BackendFactory::BackendFactory(const char* filename) : db(nullptr) {
	/// \todo load values
	window_properties[WindowProperties::WINDOW_WIDTH] = 1800;
	window_properties[WindowProperties::WINDOW_HEIGHT] = 1200;
	window_properties[WindowProperties::LEFT_PANE_WIDTH] = 250;
	window_properties[WindowProperties::RIGHT_PANE_WIDTH] = 250;
	window_properties[WindowProperties::TILE_WIDTH] = 250;
//	window_properties[WindowProperties::TILE_HEIGHT] = 250;
	window_properties[WindowProperties::N_THREADS] = std::thread::hardware_concurrency()?std::thread::hardware_concurrency():1;

	db = std::make_unique<Backend::DatabaseInterface::DatabaseFactory>(":memory:", true);
}

KeywordInterface* BackendFactory::getKeywordInterface() {
	return db->getKeywordInterface();
}

const KeywordInterface* BackendFactory::getKeywordInterface() const {
	return db->getKeywordInterface();
}

DirectoryInterface* BackendFactory::getDirectoriesInterface() {
	return db->getDirectoriesInterface();
}

const DirectoryInterface* BackendFactory::getDirectoriesInterface() const {
	return db->getDirectoriesInterface();
}

AlbumInterface* BackendFactory::getAlbumInterface() {
	return db->getAlbumInterface();
}

const AlbumInterface* BackendFactory::getAlbumInterface() const {
	return db->getAlbumInterface();
}

PhotoInterface* BackendFactory::getPhotoInterface() {
	return db->getPhotoInterface();
}

const PhotoInterface* BackendFactory::getPhotoInterface() const {
	return db->getPhotoInterface();
}

RelationsInterfaceBase* BackendFactory::getPhotosAlbumsRelationsInterface() {
	return db->getPhotosAlbumsRelationsInterface();
}

const RelationsInterfaceBase* BackendFactory::getPhotosAlbumsRelationsInterface() const {
	return db->getPhotosAlbumsRelationsInterface();
}

RelationsInterfaceBase* BackendFactory::getPhotosKeywordsRelationsInterface() {
	return db->getPhotosKeywordsRelationsInterface();
}

const RelationsInterfaceBase* BackendFactory::getPhotosKeywordsRelationsInterface() const {
	return db->getPhotosKeywordsRelationsInterface();
}

template<>
InterfaceBase<KeywordRecord>* BackendFactory::getInterface<KeywordRecord>() {
	return getKeywordInterface();
}

template<>
const InterfaceBase<KeywordRecord>* BackendFactory::getInterface<KeywordRecord>() const {
	return getKeywordInterface();
}

template<>
InterfaceBase<DirectoryRecord>* BackendFactory::getInterface<DirectoryRecord>() {
	return getDirectoriesInterface();
}

template<>
const InterfaceBase<DirectoryRecord>* BackendFactory::getInterface<DirectoryRecord>() const {
	return getDirectoriesInterface();
}

template<>
InterfaceBase<AlbumRecord>* BackendFactory::getInterface<AlbumRecord>() {
	return getAlbumInterface();
}

template<>
const InterfaceBase<AlbumRecord>* BackendFactory::getInterface<AlbumRecord>() const {
	return getAlbumInterface();
}

template<>
InterfaceBase<PhotoRecord>* BackendFactory::getInterface<PhotoRecord>() {
	return getPhotoInterface();
}

template<>
const InterfaceBase<PhotoRecord>* BackendFactory::getInterface<PhotoRecord>() const {
	return getPhotoInterface();
}

int BackendFactory::getWindowProperty(WindowProperties property) const {
	return window_properties.at(property);
}

void BackendFactory::setWindowProperty(WindowProperties property, int value) {
	/// \todo save value
	window_properties[property] = value;
}

int BackendFactory::getCentreWidth() const {
	return window_properties.at(WindowProperties::WINDOW_WIDTH) -
			window_properties.at(WindowProperties::RIGHT_PANE_WIDTH) -
			window_properties.at(WindowProperties::RIGHT_PANE_WIDTH) - 4;
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
