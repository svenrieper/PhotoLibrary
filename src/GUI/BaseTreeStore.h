/*
 * BaseTreeStore.h
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

#ifndef SRC_GUI_BASETREESTORE_H_
#define SRC_GUI_BASETREESTORE_H_

#include <gtkmm/treestore.h>

#include "../Backend/BackendFactory.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * Abstract base TreeStore class.
 * TModelColumns must contain a field named 'expanded' of type
 * bool (or a type that can be implicitly converted to bool). It should
 * contain information on whether the a row should be expanded.
 *
 * When used with a BaseTreeStore based class the TModelColumns must also
 * contain a field 'id' with the id of the entry.
 *
 * \see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeStore.html
 *
 * @param TModelColumns Gtk::TreeModel::ColumnRecord based class
 * 		used by the Gtk::TreeStore
 * @param RecordType the Backend::Record type used by the TreeStore;
 * 		the Backend interface must be derived from
 * 		Backend::InterfaceBase<RecordType>
 */
template<class TModelColumns, class RecordType>
class BaseTreeStore : public Gtk::TreeStore {
public:
	using ModelColumns = TModelColumns;

	virtual ~BaseTreeStore() = default;

	/**
	 * Returns reference to the ModelColumns used by the TreeStore.
	 *
	 * @return reference to the ModelColumns used by the TreeStore
	 */
	inline ModelColumns& getColumns();

	/**
	 * Initialise the TreeStore.
	 * Should be called after construction to fill the TreeStore.
	 */
	inline void initialise();

	/**
	 * Reload the TreeStore.
	 * Reload the TreeStore to apply changes to the rows made elsewhere
	 * in the program.
	 */
	inline void reload();

	/**
	 * Signal emitted when a row should be expanded.
	 * Signal emitted during initialise()ation or reload()ing if a row on the
	 * first level shoud be expanded. It is called for a Row after all its
	 * children have been added so the TreeView can handle the expansion of
	 * child Row|s appropriately.
	 *
	 * @return sigc::signal; use connect() to connect a signal handler
	 * @see https://developer.gnome.org/libsigc++/stable/group__signal.html
	 *
	 * \par Prototype
	 * bool onSignalExpandRow(const TreeModel::Path& path, bool open_all)
	 * @param path TreeModel::Path to the row that should be expanded
	 * @param open_all is always false
	 * @return the return value is ignored
	 */
	inline sigc::signal<bool, const TreeModel::Path&,bool> signalExpandRow();

	//No copying or moving
	BaseTreeStore(const BaseTreeStore &other) = delete;
	BaseTreeStore(BaseTreeStore &&other) = delete;
	BaseTreeStore& operator=(const BaseTreeStore &other) = delete;
	BaseTreeStore& operator=(BaseTreeStore &&other) = delete;

protected:
	/**
	 * @param backend the backend interface used by the TreeStore
	 */
	inline BaseTreeStore(Backend::InterfaceBase<RecordType>* backend);

	/**
	 * Returns the backend interface.
	 *
	 * @return a pointer to the backend interface used by the TreeStore
	 */
	inline Backend::InterfaceBase<RecordType>* getBackend();

	/**
	 * Fills a Row of the TreeStore.
	 * The method is called by initialise() and reload() for every record
	 * retrieved from the backend.
	 * It needs to be implemented by derived classes.
	 *
	 * @param[in] id id of the record
	 * @param[out] row reference to the TreeModel::Row to be filled
	 */
	virtual void fillRow(int id, Gtk::TreeModel::Row& row) = 0;

	/**
	 * Connected to Gtk::TreeStore::signal_row_changed().
	 * Is called everytime a row changes.
	 *
	 * \attention Should be overridden - unless drag and drop is deactivated -
	 * to handle drops appropriately.
	 *
	 * \see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeModel.html#a68bcb8a4563a498b504a4e5f0fb2a731
	 */
	virtual void onRowChanged(const TreeModel::Path& path, const TreeModel::iterator& iter) {};

private:
	Backend::InterfaceBase<RecordType>* backend;
	ModelColumns columns;
	sigc::connection row_changed_connection;
	sigc::signal<bool, const Gtk::TreeModel::Path&,bool> signal_expand_row;
	using SignalExpandRow = sigc::signal<bool, const TreeModel::Path&, bool>;

	void fillStore(int parent=0, Gtk::TreeModel::Row* parentRow=nullptr);
	inline void connectRowSignalChanged();
	inline void disconnectRowSignalChanged();
};


//implementation
template<class TModelColumns, class RecordType>
BaseTreeStore<TModelColumns,RecordType>::BaseTreeStore(Backend::InterfaceBase<RecordType>* backend) : backend(backend) {
	set_column_types(columns);
}

template<class TModelColumns, class RecordType>
TModelColumns& BaseTreeStore<TModelColumns,RecordType>::getColumns() {
	return columns;
}

template<class TModelColumns, class RecordType>
Backend::InterfaceBase<RecordType>* BaseTreeStore<TModelColumns,RecordType>::getBackend() {
	return backend;
}

template<class TModelColumns, class RecordType>
void BaseTreeStore<TModelColumns,RecordType>::initialise() {
	fillStore();
	connectRowSignalChanged();
}

template<class TModelColumns, class RecordType>
void BaseTreeStore<TModelColumns,RecordType>::reload() {
	disconnectRowSignalChanged();
	clear();
	initialise();
}

template<class TModelColumns, class RecordType>
typename BaseTreeStore<TModelColumns,RecordType>::SignalExpandRow BaseTreeStore<TModelColumns,RecordType>::signalExpandRow() {
	return signal_expand_row;
}

template<class TModelColumns, class RecordType>
void BaseTreeStore<TModelColumns,RecordType>::connectRowSignalChanged() {
	row_changed_connection = signal_row_changed().connect(sigc::mem_fun(*this, &BaseTreeStore::onRowChanged));
}

template<class TModelColumns, class RecordType>
void BaseTreeStore<TModelColumns,RecordType>::disconnectRowSignalChanged() {
	row_changed_connection.disconnect();
}

template<class TModelColumns, class RecordType>
void BaseTreeStore<TModelColumns,RecordType>::fillStore(int parent, Gtk::TreeModel::Row* parentRow) {
	for(int child_id : backend->getChildren(parent)){
		RecordType record(backend->getEntry(child_id));
		Gtk::TreeModel::Row row = parentRow?*(append(parentRow->children())):*(append());
		fillRow(child_id, row);

		fillStore(child_id, &row);
		if(row[getColumns().expanded] && !parentRow) {
			signalExpandRow().emit(get_path(row), false);
		}
	}
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_BASETREESTORE_H_ */
