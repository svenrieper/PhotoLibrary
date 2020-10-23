/*
 * DirectoryStore.h
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
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
class DirectoryStore: public BaseTreeStore<DirectoryModelColumns,Backend::DirectoryRecord> {
public:
	virtual ~DirectoryStore() = default;

	/**
	 * Create an object.
	 * Creates an objetct of DirectoryStore and returns a RefPtr to it.
	 *
	 * @param db keyword interface to the database
	 * @return Glib::RefPtr to the newly created TreeStore
	 */
	static Glib::RefPtr<DirectoryStore> create(Backend::InterfaceBase<Backend::DirectoryRecord>* db);

private:
	DirectoryStore(Backend::InterfaceBase<Backend::DirectoryRecord>* db);
	void fillRow(int id, Gtk::TreeModel::Row &row) override;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_DIRECTORYSTORE_H_ */
