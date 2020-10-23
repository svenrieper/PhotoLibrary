/*
 * RightFrame.h
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_RIGHTPANE_H_
#define SRC_GUI_RIGHTPANE_H_

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include "KeywordsView.h"
#include "../Backend/BackendFactory.h"
#include "SidePaneElement.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The pane on the right side of the window.
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1ScrolledWindow.html
 */
class RightPane: public Gtk::ScrolledWindow {
public:
	/**
	 * @param backend pointer to the Backend::BackendFactory factory object
	 */
	inline RightPane(Backend::BackendFactory* backend);
	~RightPane() = default;

private:
	Backend::BackendFactory* backend;

	Gtk::Box right_box;
	SidePaneElement<KeywordsView> keywords;

	inline void onSizeAllocate(Gdk::Rectangle& allocation);
};


//implentation
RightPane::RightPane(Backend::BackendFactory* backend) :
		Gtk::ScrolledWindow(),
		backend(backend),
		keywords("Keywords", backend) {
	add(right_box);
	right_box.set_orientation(Gtk::ORIENTATION_VERTICAL);

	right_box.add(keywords);

	signal_size_allocate().connect(sigc::mem_fun(*this, &RightPane::onSizeAllocate));
}

/// \todo find a solution that isn't called every time any child widget changes
void RightPane::onSizeAllocate(Gdk::Rectangle& allocation) {
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::RIGHT_PANE_WIDTH, allocation.get_width());
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_RIGHTPANE_H_ */
