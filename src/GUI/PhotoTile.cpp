/*
 * PhotoTile.cpp
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

#include "PhotoTile.h"
#include <filesystem>

namespace PhotoLibrary {
namespace GUI {

using Backend::BackendFactory;
using Backend::DirectoryRecord;

PhotoTile::PhotoTile(Backend::BackendFactory* backend, int photo_id) :
		backend(backend),
		photo_record(backend->getPhotoInterface()->getEntry(photo_id)),
		photo_image(backend->getWindowProperty(BackendFactory::WindowProperties::TILE_WIDTH),
				photo_record.getWidth(), photo_record.getHeight()) {

	pack_start(photo_image);
}

PhotoTile::PhotoTile(PhotoTile&& a) noexcept :
		Gtk::VBox(std::move(a)),
		backend(a.backend),
		photo_record(std::move(a.photo_record)),
		photo_image(std::move(a.photo_image)) {
}

Glib::ustring PhotoTile::getFilename() {
	return getFullPath()+photo_record.getFilename();
}

Glib::ustring PhotoTile::getFullPath() {
	Glib::ustring full_path;
	for(int dir_id = photo_record.getDirectory(); dir_id;) {
		DirectoryRecord dir(backend->getDirectoriesInterface()->getEntry(dir_id));
		full_path = dir.getFullDirectory() + "/" + full_path;
		dir_id = dir.getParent();
	}
	return full_path;
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
