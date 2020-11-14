/*
 * DirectoryModelColumns.h
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
