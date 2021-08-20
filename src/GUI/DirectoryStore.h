/*
 * DirectoryStore.h
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

#ifndef SRC_GUI_DIRECTORYSTORE_H_
#define SRC_GUI_DIRECTORYSTORE_H_

#include "BaseTreeStore.h"
#include "DirectoryModelColumns.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeStore for the directory TreeView
 */
class DirectoryStore: public BaseTreeStore<DirectoryModelColumns,Backend::RecordClasses::DirectoryRecord> {
public:
	virtual ~DirectoryStore() = default;

	/**
	 * Create an object.
	 * Creates an objetct of DirectoryStore and returns a RefPtr to it.
	 *
	 * @param db Pointer to the BackendFactory object
	 * @return Glib::RefPtr to the newly created TreeStore
	 */
	static Glib::RefPtr<DirectoryStore> create(Backend::BackendFactory* db);

private:
	DirectoryStore(Backend::BackendFactory* db);
	void fillRow(int id, Gtk::TreeModel::Row &row) override;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_DIRECTORYSTORE_H_ */
