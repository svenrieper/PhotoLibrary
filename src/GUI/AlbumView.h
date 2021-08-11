/*
 * AlbumView.h
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

#ifndef SRC_GUI_ALBUMVIEW_H_
#define SRC_GUI_ALBUMVIEW_H_

#include "BaseTreeView.h"
#include "AlbumStore.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * TreeView for the albums.
 * BaseTreeView based TreeView for the albums for the side pane.
 *
 * \todo optical discrimination between albums, album sets, and smart albums
 */
class AlbumView: public BaseTreeView<AlbumStore, Backend::RecordClasses::AlbumRecord> {
public:
	/**
	 * @param backend the backend interface factory object
	 */
	AlbumView(Backend::BackendFactory* backend);
	~AlbumView() = default;

private:
	void createView() override;
	void fillPopupMenu();
	void addPopupMenuEntry(Glib::ustring text, void(PhotoLibrary::GUI::AlbumView::* callback)(), Gtk::MenuItem* item);
	void addNewAlbum(Backend::RecordClasses::AlbumRecord::Options options=Backend::RecordClasses::AlbumRecord::Options::NONE);

	bool on_button_press_event(GdkEventButton* button_event) override;
	void onMenuAddNewAlbum();
	/// \todo implement smart albums
	void onMenuAddNewSmartAlbum();
	void onMenuAddNewSet();
	void onMenuRenameAlbum();
	void onMenuDeleteAlbum();

	Gtk::Menu popup_menu;
	Gtk::MenuItem menu_item_new_album;
	Gtk::MenuItem menu_item_new_smart_album;
	Gtk::MenuItem menu_item_new_album_set;
	Gtk::MenuItem menu_item_rename_album;
	Gtk::MenuItem menu_item_delete_album;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_ALBUMVIEW_H_ */
