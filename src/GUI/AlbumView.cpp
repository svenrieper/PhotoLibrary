/*
 * AlbumView.cpp
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

#include "AlbumView.h"
#include "NewAlbumDialogue.h"
#include <gtkmm/messagedialog.h>
#include <iostream>

namespace PhotoLibrary {
namespace GUI {

AlbumView::AlbumView(Backend::BackendFactory* backend) :
		BaseTreeView(*backend) {
	createView();

	fillPopupMenu();
}

void AlbumView::createView() {
	AlbumStore::ModelColumns& columns = getTreeStore()->getColumns();
	//Add the TreeView's view columns:
	append_column("Directory", columns.album_name);
	int col_count = append_column("", columns.photo_count);
	get_column_cell_renderer(col_count-1)->set_alignment(1, .5);
}


void AlbumView::addNewAlbum(Backend::RecordClasses::AlbumRecord::Options options) {
	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();

	int				parent_id			= iter?(*iter)[getTreeStore()->getColumns().id]:0;
	Glib::ustring	parent_album_name	= iter?(*iter)[getTreeStore()->getColumns().album_name]:Glib::ustring();
	Backend::RecordClasses::NewAlbumRecord new_album(parent_id, options);

	NewAlbumDialogue dialogue(&new_album, parent_album_name);
	while(dialogue.run() == Gtk::RESPONSE_OK) {
		try {
			getBackend().newEntry(new_album);
			reloadTreeStore();
			return;
		}
		catch (DatabaseInterface::constraint_error& e) {
			/// \todo prepare for internationalisation
			/// \todo improve text
			Gtk::MessageDialog message_dialogue("Album could not be added");
			message_dialogue.set_secondary_text("Albums in the same set may not have the same name."
//					"\nKeywords differing only by capitalisation are not considered different."
					);
			message_dialogue.run();
		}
	}
}

bool AlbumView::on_button_press_event(GdkEventButton* button_event) {
	//Call base class, to allow normal handling,
	//such as allowing the row to be selected by the right-click:
	bool return_value = TreeView::on_button_press_event(button_event);

	//Then open the menu:
	if (button_event->type == GDK_BUTTON_PRESS && button_event->button == 3) {
		auto refSelection = get_selection();
		Gtk::TreeModel::iterator iter(nullptr);
		if (refSelection)
			iter = refSelection->get_selected();

		//grey out new albums if selection is not an album set
		menu_item_new_album.set_sensitive(!iter || (*iter)[getTreeStore()->getColumns().album_is_set]);
		/// \todo uncomment when smart albums are implemented
//		menu_item_new_smart_album.set_sensitive(!iter || (*iter)[getTreeStore()->getColumns().album_is_set]);
		menu_item_new_album_set.set_sensitive(!iter || (*iter)[getTreeStore()->getColumns().album_is_set]);

		popup_menu.popup_at_pointer((GdkEvent*) button_event);
	}

	return return_value;
}

void AlbumView::fillPopupMenu() {
	addPopupMenuEntry("_New Album", &AlbumView::onMenuAddNewAlbum, &menu_item_new_album);
	addPopupMenuEntry("New S_mart Album", &AlbumView::onMenuAddNewSmartAlbum, &menu_item_new_smart_album);
	/// \todo remove when smart albums are implemented
	menu_item_new_smart_album.set_sensitive(false);
	addPopupMenuEntry("New Album _Set", &AlbumView::onMenuAddNewSet, &menu_item_new_album_set);
	addPopupMenuEntry("_Rename Album", &AlbumView::onMenuRenameAlbum, &menu_item_rename_album);
	addPopupMenuEntry("_Delete Album", &AlbumView::onMenuDeleteAlbum, &menu_item_delete_album);

	popup_menu.accelerate(*this);
	popup_menu.show_all(); //Show all menu items when the menu pops up
}

void AlbumView::addPopupMenuEntry(Glib::ustring text, void(PhotoLibrary::GUI::AlbumView::* callback)(), Gtk::MenuItem* item) {
	*item = Gtk::MenuItem(text, true);
	item->signal_activate().connect(sigc::mem_fun(*this, callback));
	popup_menu.append(*item);
}

void AlbumView::onMenuAddNewAlbum() {
	addNewAlbum(Backend::RecordClasses::AlbumRecord::Options::NONE);
}

void AlbumView::onMenuAddNewSmartAlbum() {
	// \todo implment
}

void AlbumView::onMenuAddNewSet() {
	addNewAlbum(Backend::RecordClasses::AlbumRecord::Options::ALBUM_IS_SET);
}

void AlbumView::onMenuRenameAlbum() {
	using Backend::RecordClasses::AlbumRecord;
	std::cout << "Rename Album Selected.\n";

	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();
	if(!iter)
		return;
	AlbumRecord album(getBackend().getEntry<AlbumRecord>((*iter)[getTreeStore()->getColumns().id]));
	RenameAlbumDialogue dialogue(&album);
	while (dialogue.run() == Gtk::RESPONSE_OK) {
		try {
			getBackend().updateEntry((*iter)[getTreeStore()->getColumns().id], album);
			reloadTreeStore();
			return;
		}
		catch (DatabaseInterface::constraint_error &e) {
			/// \todo prepare for internationalisation
			/// \todo improve text
			Gtk::MessageDialog message_dialogue("Changes could not be saved");
			message_dialogue.set_secondary_text("Albums in the same set may not have the same name.");
			message_dialogue.run();
		}
	}
}

void AlbumView::onMenuDeleteAlbum() {
	auto refSelection = get_selection();
	Gtk::TreeModel::iterator iter(nullptr);
	if (refSelection)
		iter = refSelection->get_selected();
	if(!iter)
		return;

	/// \todo prepare for internationalisation
	/// \todo improve text
	Gtk::MessageDialog delete_dialogue("Delete album '" + (*iter)[getTreeStore()->getColumns().album_name] + "'?",
			false /* use_markup */, Gtk::MESSAGE_QUESTION,
			Gtk::BUTTONS_OK_CANCEL);
	delete_dialogue.set_secondary_text(
			"Are you sure you want to delete the album '" +
			(*iter)[getTreeStore()->getColumns().album_name] +
			"' and all enclosed albums?\n"
			"You cannot undo the action.");
	if(delete_dialogue.run() == Gtk::RESPONSE_OK) {
		getBackend().deleteEntry<Backend::RecordClasses::AlbumRecord>((*iter)[getTreeStore()->getColumns().id]);
		reloadTreeStore();
	}
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
