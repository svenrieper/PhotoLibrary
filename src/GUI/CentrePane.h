/*
 * CentrePane.h
 *
 *  Created on: 23 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_CENTREPANE_H_
#define SRC_GUI_CENTREPANE_H_

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/flowbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include "../Backend/BackendFactory.h"

namespace PhotoLibrary {
namespace GUI {

class CentrePane: public Gtk::ScrolledWindow {
public:
	CentrePane(Backend::BackendFactory* backend);
	virtual ~CentrePane() = default;
	void fillGrid();

private:
	Backend::BackendFactory* backend;
	Gtk::VBox box;
	Gtk::FlowBox flowbox;
	//use make_managed instead?
	std::vector<Gtk::Frame> tiles;
	int tiles_per_row;

	sigc::connection size_allocation_connection;

	void onSizeAllocate(Gdk::Rectangle& allocation);
	void calculateTilePerRow();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_CENTREPANE_H_ */
