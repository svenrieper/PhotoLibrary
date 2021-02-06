/*
 * AlbumStore.h
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

#ifndef SRC_GUI_ALBUMSTORE_H_
#define SRC_GUI_ALBUMSTORE_H_

#include "BaseTreeStore.h"
#include "AlbumModelColumns.h"
#include <gtkmm/selectiondata.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeStore for the album TreeView
 */
class AlbumStore: public BaseTreeStore<AlbumModelColumns,Backend::AlbumRecord> {
public:
	~AlbumStore() = default;

	/**
	 * Create an object.
	 * Creates an objetct of AlbumStore and returns a RefPtr to it.
	 *
	 * @param db album interface to the database
	 * @return Glib::RefPtr to the newly created TreeStore
	 */
	static Glib::RefPtr<AlbumStore> create(Backend::InterfaceBase<Backend::AlbumRecord>* db);
//	static Glib::RefPtr<AlbumStore> create(Backend::BackendFactory* db);

private:
//	Backend::RelationsInterfaceBase* relations;

	AlbumStore(Backend::InterfaceBase<Backend::AlbumRecord>* db);
//	AlbumStore(Backend::BackendFactory* db);

	void fillRow(int id, Gtk::TreeModel::Row &row) override;

	/// \todo discriminate between drag'n'drop and expansion/collapsing of a row
	void onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) override;
	bool row_drop_possible_vfunc(const TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const override;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_ALBUMSTORE_H_ */
