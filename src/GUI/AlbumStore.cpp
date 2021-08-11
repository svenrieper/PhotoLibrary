/*
 * AlbumStore.cpp
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

#include "AlbumStore.h"
#include "gtkmm/messagedialog.h"

namespace PhotoLibrary {
namespace GUI {

AlbumStore::AlbumStore(Backend::BackendFactory* db) :
		BaseTreeStore(db->getAlbumInterface()),
		relations(db->getPhotosAlbumsRelationsInterface()) {
}

Glib::RefPtr<AlbumStore> AlbumStore::create(Backend::BackendFactory* db) {
	return Glib::RefPtr<AlbumStore>(new AlbumStore(db));
}

void AlbumStore::onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) {
	int parent = iter->parent() ? (*(iter->parent()))[getColumns().id] : 0;
	try {
		getBackend()->setParent((*iter)[getColumns().id], parent);
	} catch (const Backend::DatabaseInterface::constraint_error& e) {
		/// \todo prepare for internationalisation
		/// \todo improve text
		Gtk::MessageDialog message_dialogue("Album '" + (*iter)[getColumns().album_name] + "' could not be moved");
		message_dialogue.set_secondary_text("Albums in the same album set may not have the same name.");
		message_dialogue.run();
	}
}

void AlbumStore::fillRow(int id, Gtk::TreeModel::Row& row) {
	using Backend::RecordClasses::AlbumRecord;

	AlbumRecord album(getBackend()->getEntry(id));
	/// \todo implement photo_count
	int photo_count = relations->getNumberEntries(id);
	row[getColumns().id] = id;
	row[getColumns().album_name]   = album.getAlbumName();
	row[getColumns().album_is_set] = album.getOptions() & AlbumRecord::Options::ALBUM_IS_SET;
	row[getColumns().expanded]     = album.getOptions() & AlbumRecord::Options::ROW_EXPANDED;
	row[getColumns().photo_count]  = (album.getOptions() & AlbumRecord::Options::ALBUM_IS_SET)?"":std::to_string(photo_count);
}

bool AlbumStore::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const {
	Gtk::TreeModel::Path dest_parent = dest_path;
	//if the drop destination is not the top level and not a set return false
	//otherwise let Gtk::TreeView decide to prevent loops
	if (dest_parent.up() && !dest_parent.empty()) {
		auto unconstThis = const_cast<AlbumStore*>(this);
		if (const_iterator iter_dest_parent = unconstThis->get_iter(dest_parent))
			return (*iter_dest_parent)[unconstThis->getColumns().album_is_set]
						&& Gtk::TreeStore::row_drop_possible_vfunc(dest_path, selection_data);
	}

	return Gtk::TreeStore::row_drop_possible_vfunc(dest_path, selection_data);
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
