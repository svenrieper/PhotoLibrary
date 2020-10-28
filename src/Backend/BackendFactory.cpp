/*
 * BackendFactory.cpp
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
 */

#include "BackendFactory.h"

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

	try {
		db = new Adapter::DatabaseFactory(":memory:");
	}
	catch (...) {
		delete db;
		throw;
	}
}

BackendFactory::~BackendFactory() {
	delete db;
}

KeywordInterface* BackendFactory::getKeywordInterface() {
	return db->getKeywordInterface();
}

DirectoryInterface* BackendFactory::getDirectoriesInterface() {
	return db->getDirectoriesInterface();
}

AlbumInterface* BackendFactory::getAlbumInterface() {
	return db->getAlbumInterface();
}

PhotoInterface* BackendFactory::getPhotoInterface() {
	return db->getPhotoInterface();
}

int BackendFactory::getWindowProperty(WindowProperties property) {
	return window_properties.at(property);
}

void BackendFactory::setWindowProperty(WindowProperties property, int value) {
	/// \todo save value
	window_properties[property] = value;
}

int BackendFactory::getCentreWidth() {
	return window_properties[WindowProperties::WINDOW_WIDTH] -
			window_properties[WindowProperties::RIGHT_PANE_WIDTH] -
			window_properties[WindowProperties::RIGHT_PANE_WIDTH] - 4;
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
