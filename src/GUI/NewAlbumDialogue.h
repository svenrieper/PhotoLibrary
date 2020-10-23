/*
 * NewAlbumDialogue.h
 *
 *  Created on: 20 Oct 2020
 *      Author: Sven Rieper
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
