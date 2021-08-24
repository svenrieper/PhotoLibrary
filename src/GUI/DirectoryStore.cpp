/*
 * DirectoryStore.cpp
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

#include "DirectoryStore.h"
#include "Record/DirectoryRecord.h"
#include "Record/PhotoRecord.h"
#include <gtkmm/messagedialog.h>

namespace PhotoLibrary {
namespace GUI {

using Backend::RecordClasses::DirectoryRecord;

DirectoryStore::DirectoryStore(Backend::BackendFactory* db) :
		BaseTreeStore(*db) {
}

Glib::RefPtr<DirectoryStore> DirectoryStore::create(Backend::BackendFactory* db) {
	return Glib::RefPtr<DirectoryStore>(new DirectoryStore(db));
}

//Fill the TreeRow
void DirectoryStore::fillRow(int id, Gtk::TreeModel::Row &row) {
	using Backend::RecordClasses::DirectoryRecord;
	DirectoryRecord directory(getBackend().getEntry<DirectoryRecord>(id));
	row[getColumns().id] = id;
	row[getColumns().name] = directory.getDirectory();
	row[getColumns().expanded] = directory.getOptions() & DirectoryRecord::Options::ROW_EXPANDED;
	row[getColumns().photo_count] = getBackend().getNumberChildren<Backend::RecordClasses::PhotoRecord>(id);
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
