/*
 * AlbumModelColumns.h
 *
 *  Created on: 19 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_ALBUMMODELCOLUMNS_H_
#define SRC_GUI_ALBUMMODELCOLUMNS_H_

#include <gtkmm/treemodelcolumn.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeModel::ColumnRecord for the album TreeView
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeModelColumnRecord.html
 */
class AlbumModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
	AlbumModelColumns() : Gtk::TreeModel::ColumnRecord() {
		add(id);
		add(album_name);
		add(album_is_set);
		add(expanded);
		add(photo_count);
	}

	~AlbumModelColumns() = default;

	Gtk::TreeModelColumn<int> id;	/**< The id of the album */
	Gtk::TreeModelColumn<Glib::ustring> album_name;	/**< The name of the album itself */
	Gtk::TreeModelColumn<bool> album_is_set;	/**< Whether the album is an album set */
	Gtk::TreeModelColumn<bool> expanded;	/**< Whether the children of the album set are shown in the TreeView */
	Gtk::TreeModelColumn<Glib::ustring> photo_count;	/**< The number of photos in the album */
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_ALBUMMODELCOLUMNS_H_ */
