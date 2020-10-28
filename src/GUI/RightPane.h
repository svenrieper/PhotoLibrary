/*
 * RightFrame.h
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
