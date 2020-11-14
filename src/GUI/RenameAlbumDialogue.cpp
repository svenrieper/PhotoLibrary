/*
 * RenameAlbumDialogue.cpp
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

#include "RenameAlbumDialogue.h"

namespace PhotoLibrary {
namespace GUI {

RenameAlbumDialogue::RenameAlbumDialogue(Backend::AlbumRecord* album) :
		album(album),
		album_name_label("Album name:") {
	//Set the window properties
	set_title("Rename album");
	set_resizable(false);
	set_default_size(550, -1);
	set_border_width(4);

	//Set the values of the widgets
	album_name_entry.set_text(album->getAlbumName());

	//Fill the window with widgets
	add_button("_OK", Gtk::RESPONSE_OK);
	add_button("_Cancel", Gtk::RESPONSE_CANCEL);

	Gtk::Widget* ok_button = get_widget_for_response(Gtk::RESPONSE_OK);
	if(ok_button) {
		ok_button->set_can_default();
		ok_button->grab_default();
	}

	album_name_label.set_xalign(0);
	album_name_entry.set_activates_default();

	Gtk::Box *content_box = get_content_area();

	content_box->add(album_name_label);
	content_box->add(album_name_entry);
	content_box->show_all_children(true);

	//Connect the signal handlers
	album_name_entry.signal_changed().connect(sigc::mem_fun(*this, &RenameAlbumDialogue::updateAlbumName));
}

void RenameAlbumDialogue::updateAlbumName() {
	album->setAlbumName() = album_name_entry.get_text();
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
