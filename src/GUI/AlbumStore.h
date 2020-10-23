/*
 * AlbumStore.h
 *
 *  Created on: 19 Oct 2020
 *      Author: Sven Rieper
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

private:
	AlbumStore(Backend::InterfaceBase<Backend::AlbumRecord>* db);

	void fillRow(int id, Gtk::TreeModel::Row &row) override;

	/// \todo discriminate between drag'n'drop and expansion/collapsing of a row
	void onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) override;
	bool row_drop_possible_vfunc(const TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const override;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_ALBUMSTORE_H_ */
