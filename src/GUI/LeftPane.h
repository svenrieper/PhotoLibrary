/*
 * LeftPane.h
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

#ifndef SRC_GUI_LEFTPANE_H_
#define SRC_GUI_LEFTPANE_H_

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include "../Backend/BackendFactory.h"
#include "DirectoryView.h"
#include "AlbumView.h"
#include "SidePaneElement.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The pane on the left side of the window.
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1ScrolledWindow.html
 */
class LeftPane: public Gtk::ScrolledWindow {
public:
	/**
	 * @param backend pointer to the Backend::BackendFactory factory object
	 */
	inline LeftPane(Backend::BackendFactory* backend);
	virtual ~LeftPane() = default;

	/**
	 * Signal emitted when a new directory is selcted.
	 *
	 * Signal emitted when a new directory is selected in the DirectoryView.
	 *
	 * @return sigc::signal; use connect() to connect a signal handler
	 *
	 * \par Prototype
	 * void onNewDirectorySelected(int id)
	 * @param id id of the newly selected directory
	 */
	inline sigc::signal<void,int> signaleNewDirectorySelected();

	/**
	 * Signal emitted when a new Album is selected.
	 *
	 * Signal emitted when a new Album is selceted in the AlbumView.
	 *
	 * @return sigc::signal; use connect() to connect a signal handler
	 *
	 * \par Prototype
	 * void onNewAlbumSelected(int id)
	 * @param id id of the newly selected album
	 */
	inline sigc::signal<void,int> signaleNewAlbumSelected();

private:
	Backend::BackendFactory* backend;

	Gtk::Box left_box;
	SidePaneElement<DirectoryView> directories;
	SidePaneElement<AlbumView> albums;
	sigc::signal<void,int> signal_new_directory_selected;
	sigc::signal<void,int> signal_new_album_selected;

	inline void onSizeAllocate(Gdk::Rectangle& allocation);
	inline void onDirectorySelectionChanged(int id);
	inline void onAlbumsSelectionChanged(int id);
};

//implementation
LeftPane::LeftPane(Backend::BackendFactory* backend) :
		Gtk::ScrolledWindow(),
		backend(backend),
		directories("Directories", backend),
		albums("Albums", backend) {
	add(left_box);
	left_box.set_orientation(Gtk::ORIENTATION_VERTICAL);

	left_box.add(directories);
	left_box.add(albums);

	signal_size_allocate().connect(sigc::mem_fun(*this, &LeftPane::onSizeAllocate));

	directories.getContent()->signalSelectionChanged().connect(sigc::mem_fun(*this, &LeftPane::onDirectorySelectionChanged));
	albums.getContent()->signalSelectionChanged().connect(sigc::mem_fun(*this, &LeftPane::onAlbumsSelectionChanged));
}

/// \todo find a solution that isn't called every time any child widget changes
void LeftPane::onSizeAllocate(Gdk::Rectangle& allocation) {
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::LEFT_PANE_WIDTH, allocation.get_width());
}

void LeftPane::onDirectorySelectionChanged(int id) {
	if(id) {
		albums.getContent()->unselectAll();
		signaleNewDirectorySelected().emit(id);
	}
}

void LeftPane::onAlbumsSelectionChanged(int id) {
	if(id) {
		directories.getContent()->unselectAll();
		signaleNewAlbumSelected().emit(id);
	}
}

sigc::signal<void,int> LeftPane::signaleNewDirectorySelected() {
	return signal_new_directory_selected;
}

sigc::signal<void,int> LeftPane::signaleNewAlbumSelected() {
	return signal_new_album_selected;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_LEFTPANE_H_ */
