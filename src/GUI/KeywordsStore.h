/*
 * KeywordsStore.h
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

#ifndef SRC_GUI_KEYWORDSSTORE_H_
#define SRC_GUI_KEYWORDSSTORE_H_

#include <gtkmm/treestore.h>
#include "../Backend/BackendFactory.h"
#include "KeywordModelColumns.h"
#include "BaseTreeStore.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The TreeStore for the keywords TreeView
 */
class KeywordsStore : public BaseTreeStore<KeywordModelColumns,Backend::RecordClasses::KeywordRecord> {
public:
	~KeywordsStore() = default;

	/**
	 * Create an object.
	 * Creates an objetct of KeywordStore and returns a RefPtr to it.
	 *
	 * @param db Pointer to the BackendFactory
	 * @return Glib::RefPtr to the newly created TreeStore
	 */
	static Glib::RefPtr<KeywordsStore> create(Backend::BackendFactory* db);

	//no copying or moving
	KeywordsStore(const KeywordsStore &other) = delete;
	KeywordsStore(KeywordsStore &&other) = delete;
	KeywordsStore& operator=(const KeywordsStore &other) = delete;
	KeywordsStore& operator=(KeywordsStore &&other) = delete;

private:
	KeywordsStore(Backend::BackendFactory& backend);

	void fillRow(int id, Gtk::TreeModel::Row &row) override;

	/// \todo discriminate between drag'n'drop and expansion/collapsing of a row
	void onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) override;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_KEYWORDSSTORE_H_ */
