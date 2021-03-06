/*
 * KeywordModelColumns.h
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

#ifndef SRC_GUI_KEYWORDMODELCOLUMNS_H_
#define SRC_GUI_KEYWORDMODELCOLUMNS_H_

#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodel.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeModel::ColumnRecord for the keyword TreeView
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeModelColumnRecord.html
 *
 * \todo add number of photos
 */
class KeywordModelColumns: public Gtk::TreeModel::ColumnRecord {
public:
	KeywordModelColumns() : Gtk::TreeModel::ColumnRecord() {
		add(id);
		add(keyword);
		add(assigned);
		add(inconsistent);
		add(expanded);
	}

	~KeywordModelColumns() = default;

	Gtk::TreeModelColumn<int> id;	/**< The id of the keyword */
	Gtk::TreeModelColumn<Glib::ustring> keyword;	/**< The keyword itself */
	Gtk::TreeModelColumn<bool> assigned;	/**< Whether the keyword is assigned to the selected photos */
	Gtk::TreeModelColumn<bool> inconsistent;	/**< Whether the keyword is assinged to some (but not all) of the selected photos */
	Gtk::TreeModelColumn<bool> expanded;	/**< Whether the children of the keyword are shown in the TreeView */
};

}
}



#endif /* SRC_GUI_KEYWORDMODELCOLUMNS_H_ */
