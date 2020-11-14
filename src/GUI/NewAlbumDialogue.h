/*
 * NewAlbumDialogue.h
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

#ifndef SRC_GUI_NEWALBUMDIALOGUE_H_
#define SRC_GUI_NEWALBUMDIALOGUE_H_

#include "RenameAlbumDialogue.h"
#include <gtkmm/checkbutton.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The Dialogue to add new albums and album sets
 */
class NewAlbumDialogue: public RenameAlbumDialogue {
public:
	/**
	 * @param[out] album Pointer to the Backend::NewAlbumRecord object to save the to
	 * @param[in] parent_key The parent album set
	 */
	NewAlbumDialogue(Backend::NewAlbumRecord* album, const Glib::ustring& parent_album_name="");
	~NewAlbumDialogue() = default;

private:
	Gtk::CheckButton check_button_is_set;
	Gtk::CheckButton check_button_add_as_child;

	void onIsSetClicked();
	void onAddAsChildClicked();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_NEWALBUMDIALOGUE_H_ */
