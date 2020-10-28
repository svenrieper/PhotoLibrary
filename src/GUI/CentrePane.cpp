/*
 * CentrePane.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: Sven Rieper
 */

#include "CentrePane.h"

namespace PhotoLibrary {
namespace GUI {

CentrePane::CentrePane(Backend::BackendFactory* backend) :
		backend(backend),
		tiles_per_row(0) {
	flowbox.set_homogeneous(true);
	flowbox.set_activate_on_single_click(true);

	//put the flobox inside a box so that the child widgets aren't expanded vertically
	add(box);
	box.pack_start(flowbox, false, false);

	calculateTilePerRow();

	fillGrid();
}

void CentrePane::calculateTilePerRow() {
	tiles_per_row = backend->getCentreWidth()
			/ backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH);
	flowbox.set_min_children_per_line(tiles_per_row);
	flowbox.set_max_children_per_line(tiles_per_row);
}

void CentrePane::fillGrid() {
	size_allocation_connection.disconnect();

	/// \todo fill with the actual widgets once they exist
	for(int i=0; i<15; ++i) {
		tiles.push_back(Gtk::Frame());
		tiles.back().set_size_request(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH),
				backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH));
		flowbox.add(tiles.back());
	}

	size_allocation_connection = signal_size_allocate().connect(sigc::mem_fun(*this, &CentrePane::onSizeAllocate));
}

/// \todo find a solution that isn't called every time any child widget changes
void CentrePane::onSizeAllocate(Gdk::Rectangle& allocation) {
	calculateTilePerRow();
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
