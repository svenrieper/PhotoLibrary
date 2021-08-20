/*
 * KeywordsStore.cpp
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

#include "KeywordsStore.h"
#include "BackendFactory.h"
#include "Record/KeywordRecord.h"
#include <iostream>
#include <gtkmm/messagedialog.h>

namespace PhotoLibrary {
namespace GUI {

using Backend::RecordClasses::KeywordRecord;

KeywordsStore::KeywordsStore(Backend::BackendFactory& backend) : BaseTreeStore(backend) {}

Glib::RefPtr<KeywordsStore> KeywordsStore::create(Backend::BackendFactory* db) {
	return Glib::RefPtr<KeywordsStore>(new KeywordsStore(*db));
}

void KeywordsStore::onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) {
	int parent = iter->parent() ? (*(iter->parent()))[getColumns().id] : 0;
	try {
		getBackend().setParent<KeywordRecord>((*iter)[getColumns().id], parent);
	} catch (const DatabaseInterface::constraint_error& e) {
		/// \todo prepare for internationalisation
		/// \todo improve text
		Gtk::MessageDialog message_dialogue("Keyword '" + (*iter)[getColumns().keyword] + "' could not be moved");
		message_dialogue.set_secondary_text("Keywords in the same group may not be identical.\n"
				"Keywords differing only by capitalisation are not considered different.");
		message_dialogue.run();
	}
}

void KeywordsStore::fillRow(int id, Gtk::TreeModel::Row &row) {
	KeywordRecord keyword(getBackend().getEntry<KeywordRecord>(id));
	row[getColumns().id] = id;
	row[getColumns().keyword] = keyword.getKeyword();
	row[getColumns().assigned] = false; /// \todo connect to selected photos
	row[getColumns().inconsistent] = false;
	row[getColumns().expanded] = keyword.getOptions() & KeywordRecord::Options::ROW_EXPANDED;
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
