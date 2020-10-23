/*
 * LeftPane.h
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
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

private:
	Backend::BackendFactory* backend;

	Gtk::Box left_box;
	SidePaneElement<DirectoryView> directories;
	SidePaneElement<AlbumView> albums;

	inline void onSizeAllocate(Gdk::Rectangle& allocation);
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
}

/// \todo find a solution that isn't called every time any child widget changes
void LeftPane::onSizeAllocate(Gdk::Rectangle& allocation) {
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::LEFT_PANE_WIDTH, allocation.get_width());
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_LEFTPANE_H_ */
