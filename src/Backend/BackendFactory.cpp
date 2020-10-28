/*
 * BackendFactory.cpp
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

#include "BackendFactory.h"

namespace PhotoLibrary {
namespace Backend {

BackendFactory::BackendFactory(const char* filename) : db(nullptr) {
	/// \todo load values
	window_properties[WindowProperties::WINDOW_WIDTH] = 1800;
	window_properties[WindowProperties::WINDOW_HEIGHT] = 1200;
	window_properties[WindowProperties::LEFT_PANE_WIDTH] = 250;
	window_properties[WindowProperties::RIGHT_PANE_WIDTH] = 250;

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
