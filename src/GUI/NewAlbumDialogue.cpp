/*
 * NewAlbumDialogue.cpp
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

#include "NewAlbumDialogue.h"

namespace PhotoLibrary {
namespace GUI {

using Backend::RecordClasses::NewAlbumRecord;
using Backend::RecordClasses::AlbumRecord;

NewAlbumDialogue::NewAlbumDialogue(NewAlbumRecord* album, const Glib::ustring& parent_album_name) :
		RenameAlbumDialogue(album),
		check_button_is_set("New album set") {
	//Fill the window with widgets
	Gtk::Box* content_box = get_content_area();

	check_button_is_set.set_active(album->getOptions()&AlbumRecord::Options::ALBUM_IS_SET);
	content_box->add(check_button_is_set);
	if(album->new_parent_id_backup) {
		check_button_add_as_child.set_label("Add in '" + parent_album_name + "'");
		check_button_add_as_child.set_active(album->getParent());
		content_box->add(check_button_add_as_child);
	}
	content_box->show_all_children(true);

	check_button_is_set.signal_clicked().connect(sigc::mem_fun(*this, &NewAlbumDialogue::onIsSetClicked));
	check_button_add_as_child.signal_clicked().connect(sigc::mem_fun(*this, &NewAlbumDialogue::onAddAsChildClicked));
}

void NewAlbumDialogue::onIsSetClicked() {
	album->setOptions() ^= AlbumRecord::Options::ALBUM_IS_SET;
}

void NewAlbumDialogue::onAddAsChildClicked() {
	album->setParent() = check_button_add_as_child.get_active()?static_cast<NewAlbumRecord*>(album)->new_parent_id_backup:0;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
