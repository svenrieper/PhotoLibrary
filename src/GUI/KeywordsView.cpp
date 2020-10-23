/*
 * KeywordsView.cpp
 *
 *  Created on: 28 Sep 2020
 *      Author: Sven Rieper
 */

#include "KeywordsView.h"
#include "../Backend/Record/KeywordRecord.h"
#include "../Adapter/suppport.h"
#include <map>
#include <vector>
#include <iostream>
#include <gtkmm/messagedialog.h>

#include "NewKeywordDialogue.h"

namespace PhotoLibrary {
namespace GUI {

KeywordsView::KeywordsView(Backend::BackendFactory* backend) :
		BaseTreeView(backend->getKeywordInterface()) {
	createView();
//	getTreeStore()->signalExpandRow().connect(sigc::mem_fun(*this, &KeywordsView::onSignalExpandRow));
//	getTreeStore()->initialise();

//	connectOnRowExpandedOrCollapsed();
	fillPopupMenu();
}

void KeywordsView::createView() {
	KeywordsStore::ModelColumns& columns = getTreeStore()->getColumns();
	//Add the TreeView's view columns:
	auto cell = Gtk::make_managed<Gtk::CellRendererToggle>();
//	cell->set_activatable(true);
	int cols_count = append_column("", *cell);
	auto pColumn = get_column(cols_count - 1);
	if (pColumn) {
		pColumn->add_attribute(cell->property_active(), columns.assigned);
		pColumn->add_attribute(cell->property_inconsistent(), columns.inconsistent);
	}
	append_column("Keyword", columns.keyword);
}


bool KeywordsView::on_button_press_event(GdkEventButton* button_event) {
	//Call base class, to allow normal handling,
	//such as allowing the row to be selected by the right-click:
	bool return_value = TreeView::on_button_press_event(button_event);

	//Then open the menu:
	if (button_event->type == GDK_BUTTON_PRESS && button_event->button == 3)
		popup_menu.popup_at_pointer((GdkEvent*) button_event);

	return return_value;
}

void KeywordsView::fillPopupMenu() {
	using std::make_pair;

	std::vector<std::pair<Glib::ustring,void(PhotoLibrary::GUI::KeywordsView::*)()>> menu_entries;
	/// \todo prepare for internationalisation
	menu_entries.push_back(make_pair("_Edit Keyword", &KeywordsView::onMenuEditKeyword));
	menu_entries.push_back(make_pair("_Add New Keyword", &KeywordsView::onMenuAddNewKeyword));
	menu_entries.push_back(make_pair("_Delete Keyword", &KeywordsView::onMenuDeleteKeyword));

//	popup_menu.fillMenu(menu_entries);
	for (auto menu_entry : menu_entries) {
		auto item = Gtk::make_managed<Gtk::MenuItem>(menu_entry.first, true);
		item->signal_activate().connect(sigc::mem_fun(*this, menu_entry.second));
		popup_menu.append(*item);
	}

	popup_menu.accelerate(*this);
	popup_menu.show_all(); //Show all menu items when the menu pops up
}

void KeywordsView::onMenuAddNewKeyword() {
	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();

	int				parent_id	= iter?(*iter)[getTreeStore()->getColumns().id]:0;
	Glib::ustring	parent_key	= iter?(*iter)[getTreeStore()->getColumns().keyword]:Glib::ustring();
	Backend::NewKeywordRecord new_keyword(parent_id, (Backend::KeywordRecord::Options::INCLUDE_ON_EXPORT|Backend::KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT));

	NewKeywordDialogue dialogue(&new_keyword, parent_key);
	while(dialogue.run() == Gtk::RESPONSE_OK) {
		try {
			getDBInterface()->newEntry(new_keyword);
			reloadTreeStore();
			return;
		}
		catch (Adapter::constraint_error& e) {
			/// \todo prepare for internationalisation
			/// \todo improve text
			Gtk::MessageDialog message_dialogue("Keyword could not be added");
			message_dialogue.set_secondary_text("Keywords in the same group may not be identical.\n"
					"Keywords differing only by capitalisation are not considered different.");
			message_dialogue.run();
		}
	}
}

void KeywordsView::onMenuEditKeyword() {
	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();
	if(!iter)
		return;
	Backend::KeywordRecord keyword(getDBInterface()->getEntry((*iter)[getTreeStore()->getColumns().id]));
	EditKeywordDialogue dialogue(&keyword);
	while (dialogue.run() == Gtk::RESPONSE_OK) {
		try {
			getDBInterface()->updateEntry((*iter)[getTreeStore()->getColumns().id], keyword);
			reloadTreeStore();
			return;
		}
		catch (Adapter::constraint_error &e) {
			/// \todo prepare for internationalisation
			/// \todo improve text
			Gtk::MessageDialog message_dialogue("Changes could not be saved");
			message_dialogue.set_secondary_text("Keywords in the same group may not be identical.\n"
					"Keywords differing only by capitalisation are not considered different.");
			message_dialogue.run();
		}
	}
}

void KeywordsView::onMenuDeleteKeyword() {
	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();
	if(!iter)
		return;

	/// \todo prepare for internationalisation
	/// \todo improve text
	Gtk::MessageDialog delete_dialogue("Delete keyword'" + (*iter)[getTreeStore()->getColumns().keyword] + "'?",
			false /* use_markup */, Gtk::MESSAGE_QUESTION,
			Gtk::BUTTONS_OK_CANCEL);
	delete_dialogue.set_secondary_text(
			"Are you sure you want to delete the keyword '" +
			(*iter)[getTreeStore()->getColumns().keyword] +
			"' and all enclosed keywords?\n"
			"You cannot undo the action.");
	if(delete_dialogue.run() == Gtk::RESPONSE_OK) {
		getDBInterface()->deleteEntry((*iter)[getTreeStore()->getColumns().id]);
		reloadTreeStore();
	}
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
