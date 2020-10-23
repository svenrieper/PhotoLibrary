/*
 * DirectoryModelColumns.h
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_DIRECTORYMODELCOLUMNS_H_
#define SRC_GUI_DIRECTORYMODELCOLUMNS_H_

#include <gtkmm/treemodel.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeModel::ColumnRecord for the directory TreeView
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeModelColumnRecord.html
 *
 * \todo add number of photos
 */
class DirectoryModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
	DirectoryModelColumns() : Gtk::TreeModel::ColumnRecord() {
		add(id);
		add(name);
		add(expanded);
	}

	~DirectoryModelColumns() = default;

	Gtk::TreeModelColumn<int> id;	/**< The id of the directory */
	Gtk::TreeModelColumn<Glib::ustring> name;	/**< The (short) name of the directory */
	Gtk::TreeModelColumn<bool> expanded;	/**< Whether subdierectories should be shown in the TreeView */
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_DIRECTORYMODELCOLUMNS_H_ */
