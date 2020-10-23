/*
 * AlbumView.h
 *
 *  Created on: 19 Oct 2020
 *      Author: Sven Rieper
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
class AlbumView: public BaseTreeView<AlbumStore, Backend::AlbumRecord> {
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
	void addNewAlbum(Backend::AlbumRecord::Options options=Backend::AlbumRecord::Options::NONE);

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
