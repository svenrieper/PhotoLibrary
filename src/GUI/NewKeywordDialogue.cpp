/*
 * NewKeywordDialogue.cpp
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

#include "NewKeywordDialogue.h"

//#include <iostream>

namespace PhotoLibrary {
namespace GUI {

NewKeywordDialogue::NewKeywordDialogue(Backend::NewKeywordRecord* keyword, const Glib::ustring& parent_key) :
		EditKeywordDialogue(keyword) {

	//Fill the window with widgets
	Gtk::Box* content_box = get_content_area();

	if(keyword->new_parent_id_backup) {
		check_button_add_as_child.set_label("Add in '" + parent_key + "'");
		check_button_add_as_child.set_active(keyword->getParent());
		content_box->add(check_button_add_as_child);
	}
	content_box->show_all_children(true);

	//Connect the signal handlers
	check_button_add_as_child.signal_clicked().connect(sigc::mem_fun(*this, &NewKeywordDialogue::onAddAsChildClicked));
}

void NewKeywordDialogue::onAddAsChildClicked() {
	keyword->setParent() = check_button_add_as_child.get_active()?static_cast<Backend::NewKeywordRecord*>(keyword)->new_parent_id_backup:0;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
