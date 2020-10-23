/*
 * DirectoryStore.cpp
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
 */

#include "DirectoryStore.h"
#include <gtkmm/messagedialog.h>

namespace PhotoLibrary {
namespace GUI {

DirectoryStore::DirectoryStore(Backend::InterfaceBase<Backend::DirectoryRecord>* db) : BaseTreeStore(db) {}

Glib::RefPtr<DirectoryStore> DirectoryStore::create(Backend::InterfaceBase<Backend::DirectoryRecord>* db) {
	return Glib::RefPtr<DirectoryStore>(new DirectoryStore(db));
}

//Fill the TreeRow
void DirectoryStore::fillRow(int id, Gtk::TreeModel::Row &row) {
	Backend::DirectoryRecord directory(getBackend()->getEntry(id));
	row[getColumns().id] = id;
	row[getColumns().name] = directory.getDirectory();
	row[getColumns().expanded] = directory.getOptions() & Backend::DirectoryRecord::Options::ROW_EXPANDED;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
