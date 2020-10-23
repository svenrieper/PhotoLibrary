/*
 * KeywordsStore.cpp
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
 */

#include "KeywordsStore.h"
#include <iostream>
#include <gtkmm/messagedialog.h>

namespace PhotoLibrary {
namespace GUI {

KeywordsStore::KeywordsStore(Backend::InterfaceBase<Backend::KeywordRecord>* db) : BaseTreeStore(db) {}

Glib::RefPtr<KeywordsStore> KeywordsStore::create(Backend::InterfaceBase<Backend::KeywordRecord>* db) {
	return Glib::RefPtr<KeywordsStore>(new KeywordsStore(db));
}

void KeywordsStore::onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) {
	int parent = iter->parent() ? (*(iter->parent()))[getColumns().id] : 0;
	try {
		getBackend()->setParent((*iter)[getColumns().id], parent);
	} catch (const Adapter::constraint_error& e) {
		/// \todo prepare for internationalisation
		/// \todo improve text
		Gtk::MessageDialog message_dialogue("Keyword '" + (*iter)[getColumns().keyword] + "' could not be moved");
		message_dialogue.set_secondary_text("Keywords in the same group may not be identical.\n"
				"Keywords differing only by capitalisation are not considered different.");
		message_dialogue.run();
	}
}

void KeywordsStore::fillRow(int id, Gtk::TreeModel::Row &row) {
	Backend::KeywordRecord keyword(getBackend()->getEntry(id));
	row[getColumns().id] = id;
	row[getColumns().keyword] = keyword.getKeyword();
	row[getColumns().assigned] = false; /// \todo connect to selected photos
	row[getColumns().inconsistent] = false;
	row[getColumns().expanded] = keyword.getOptions() & Backend::KeywordRecord::Options::ROW_EXPANDED;
}

} /* namespace Backend */
} /* namespace PhotoLibrary */
