/*
 * BaseTreeView.h
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

#ifndef SRC_GUI_BASETREEVIEW_H_
#define SRC_GUI_BASETREEVIEW_H_

#include <gtkmm/treeview.h>
#include "../Backend/BackendFactory.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * Abstract base class for the TreeView|s in the left and right pane
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1TreeView.html
 *
 * @param TStore BaseTreeStore based class to display in the TreeView
 * @param RecordType the Backend::Record type used by 'TStore'
 */
template<class TStore, class RecordType>
class BaseTreeView: public Gtk::TreeView {
public:
	/**
	 * @param backend the backend interface to use for the TreeView
	 */
	BaseTreeView(Backend::InterfaceBase<RecordType>* backend);

	virtual ~BaseTreeView() = default;

	//no copying or moving
	BaseTreeView(const BaseTreeView &other) = delete;
	BaseTreeView(BaseTreeView &&other) = delete;
	BaseTreeView& operator=(const BaseTreeView &other) = delete;
	BaseTreeView& operator=(BaseTreeView &&other) = delete;

protected:
	/**
	 * Returnd the TreeStore used by the TreeView.
	 *
	 * @return RefPtr to the TreeStore used by the TreeView
	 */
	inline Glib::RefPtr<TStore>& getTreeStore();

	/**
	 * Returns the backend interface.
	 *
	 * @return The backend interface used by the TreeView
	 */
	inline Backend::InterfaceBase<RecordType>* getDBInterface();

	/**
	 * Reload the TreeStore.
	 * Reloads the TreeStore to apply changes
	 */
	void reloadTreeStore();

	/**
	 * Creates the TreeView.
	 * Appends the columns to the TreeView.
	 * Needs to be implemented by derived classes and colled by their
	 * constructor.
	 */
	virtual void createView() = 0;

private:
	Backend::InterfaceBase<RecordType>* backend_interface;
	Glib::RefPtr<TStore> treeStore;
	sigc::connection signal_row_expanded_connection;
	sigc::connection signal_row_collapsed_connection;

	void onRowExpandedOrCollapsed(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
	void connectOnRowExpandedOrCollapsed();
	void disconnectOnRowExpandedOrCollapsed();
	void onRowExpanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
	void expandChildren(const Gtk::TreeModel::iterator &iter);
	bool onSignalExpandRow(const Gtk::TreeModel::Path& path, bool open_all);
	void onDragBegin(const Glib::RefPtr<Gdk::DragContext>& context);
	void onDragEnd(const Glib::RefPtr<Gdk::DragContext>& context);
};


//implementation
template<class TStore, class RecordType>
BaseTreeView<TStore,RecordType>::BaseTreeView(Backend::InterfaceBase<RecordType>* dbInterface) : backend_interface(dbInterface) {
	treeStore = TStore::create(dbInterface);

	set_model(treeStore);
	set_headers_visible(false);

	enable_model_drag_source();
	enable_model_drag_dest();

	signal_drag_begin().connect(sigc::mem_fun(*this, &BaseTreeView::onDragBegin));
	signal_drag_end().connect(sigc::mem_fun(*this, &BaseTreeView::onDragEnd));

	getTreeStore()->signalExpandRow().connect(sigc::mem_fun(*this, &BaseTreeView<TStore,RecordType>::onSignalExpandRow));
	getTreeStore()->initialise();

	connectOnRowExpandedOrCollapsed();
}

template<class TStore, class RecordType>
Glib::RefPtr<TStore>& BaseTreeView<TStore,RecordType>::getTreeStore() {
	return treeStore;
}

template<class TStore, class RecordType>
Backend::InterfaceBase<RecordType>* BaseTreeView<TStore,RecordType>::getDBInterface() {
	return backend_interface;
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::connectOnRowExpandedOrCollapsed() {
	signal_row_expanded_connection =
			signal_row_expanded().connect(sigc::mem_fun(*this, &BaseTreeView::onRowExpanded));
	signal_row_collapsed_connection =
			signal_row_collapsed().connect(sigc::mem_fun(*this, &BaseTreeView::onRowExpandedOrCollapsed));
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::disconnectOnRowExpandedOrCollapsed() {
	signal_row_expanded_connection.disconnect();
	signal_row_collapsed_connection.disconnect();
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::onRowExpanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path) {
	disconnectOnRowExpandedOrCollapsed();
	expandChildren (iter);
	connectOnRowExpandedOrCollapsed();
	onRowExpandedOrCollapsed(iter, path);
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::onRowExpandedOrCollapsed(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path) {
	RecordType entry(getDBInterface()->getEntry((*iter)[getTreeStore()->getColumns().id]));

	(*iter)[getTreeStore()->getColumns().expanded] = !(*iter)[getTreeStore()->getColumns().expanded];
	//change the ROW_EXPANDED bit for the entry and save change
	entry.setOptions() ^= Backend::RecordOptions::Options::ROW_EXPANDED;
	getDBInterface()->updateEntry((*iter)[getTreeStore()->getColumns().id], entry);
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::expandChildren(const Gtk::TreeModel::iterator &iter) {
	for (Gtk::TreeModel::iterator child_iter = (iter->children().begin()); child_iter; child_iter++)
		if ((*child_iter)[getTreeStore()->getColumns().expanded])
			onSignalExpandRow(getTreeStore()->get_path(child_iter), false);
}

template<class TStore, class RecordType>
bool BaseTreeView<TStore,RecordType>::onSignalExpandRow(const Gtk::TreeModel::Path& path, bool open_all) {
	bool successful = expand_row(path, open_all);
	expandChildren(getTreeStore()->get_iter(path));
	return successful;
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::onDragBegin(const Glib::RefPtr<Gdk::DragContext>& context) {
	disconnectOnRowExpandedOrCollapsed();
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::onDragEnd(const Glib::RefPtr<Gdk::DragContext>& context) {
	getTreeStore()->reload();
	connectOnRowExpandedOrCollapsed();
}

template<class TStore, class RecordType>
void BaseTreeView<TStore,RecordType>::reloadTreeStore() {
	disconnectOnRowExpandedOrCollapsed();
	getTreeStore()->reload();
	connectOnRowExpandedOrCollapsed();
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_BASETREEVIEW_H_ */
