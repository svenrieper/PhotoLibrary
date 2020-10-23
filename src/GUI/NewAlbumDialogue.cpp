/*
 * NewAlbumDialogue.cpp
 *
 *  Created on: 20 Oct 2020
 *      Author: Sven Rieper
 */

#include "NewAlbumDialogue.h"

namespace PhotoLibrary {
namespace GUI {

NewAlbumDialogue::NewAlbumDialogue(Backend::NewAlbumRecord* album, const Glib::ustring& parent_album_name) :
		RenameAlbumDialogue(album),
		check_button_is_set("New album set") {
	//Fill the window with widgets
	Gtk::Box* content_box = get_content_area();

	check_button_is_set.set_active(album->getOptions()&Backend::AlbumRecord::Options::ALBUM_IS_SET);
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
	album->setOptions() ^= Backend::AlbumRecord::Options::ALBUM_IS_SET;
}

void NewAlbumDialogue::onAddAsChildClicked() {
	album->setParent() = check_button_add_as_child.get_active()?static_cast<Backend::NewAlbumRecord*>(album)->new_parent_id_backup:0;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
