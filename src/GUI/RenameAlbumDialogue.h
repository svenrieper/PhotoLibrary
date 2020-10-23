/*
 * RenameAlbumDialogue.h
 *
 *  Created on: 20 Oct 2020
 *      Author: Sven Rieper
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
	 * @param[in,out] keyword Pointer to the Backend::AlbumRecord object to edit
	 */
	RenameAlbumDialogue(Backend::AlbumRecord* album);
	~RenameAlbumDialogue() = default;

protected:
	Backend::AlbumRecord* album;

private:
	Gtk::Label album_name_label;
	Gtk::Entry album_name_entry;

	void updateAlbumName();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_RENAMEALBUMDIALOGUE_H_ */
