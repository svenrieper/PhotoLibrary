/*
 * RenameAlbumDialogue.h
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

#ifndef SRC_GUI_RENAMEALBUMDIALOGUE_H_
#define SRC_GUI_RENAMEALBUMDIALOGUE_H_

#include "../Backend/Record/AlbumRecord.h"
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The Dialogue to rename albums and album sets.
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1Dialog.html
 */
class RenameAlbumDialogue: public Gtk::Dialog {
public:
	/**
	 * @param[in,out] album Pointer to the Backend::AlbumRecord object to edit
	 */
	RenameAlbumDialogue(Backend::RecordClasses::AlbumRecord* album);
	~RenameAlbumDialogue() = default;

protected:
	Backend::RecordClasses::AlbumRecord* album;	/**< Pointer to the AlbumRecord edited in the dialogue */

private:
	Gtk::Label album_name_label;
	Gtk::Entry album_name_entry;

	void updateAlbumName();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_RENAMEALBUMDIALOGUE_H_ */
