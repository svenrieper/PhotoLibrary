/*
 * CentrePane.h
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

#ifndef SRC_GUI_CENTREPANE_H_
#define SRC_GUI_CENTREPANE_H_

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/flowbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <glibmm/dispatcher.h>
//#include <lockfree/queue.hpp>
#include <thread>
#include "../Backend/BackendFactory.h"
#include "PhotoTile.h"
#include "../Support/ThreadSafeQueue.h"

namespace PhotoLibrary {
namespace GUI {

class CentrePane: public Gtk::ScrolledWindow {
public:
	CentrePane(Backend::BackendFactory* backend);
	virtual ~CentrePane();
	void fillGrid(std::vector<int> photos);

private:
	Backend::BackendFactory* backend;
	Gtk::VBox box;
	Gtk::FlowBox flowbox;
	std::unordered_map<int,std::unique_ptr<PhotoTile>> tiles;
	///\todo compare performance of boost::lockfree::queue and Support::ThreadSafeQueue
//	boost::lockfree::queue<int> tiles_to_update;
	Support::ThreadSafeQueue<int> tiles_to_update;
//	// boost::lockfree::queue requires a type with a trivial destructor
//	// (which neither std::pair, nor Glib::RefPtr, nor Gdk::Pixbuf satisfies)
//	boost::lockfree::queue<std::pair<int,Glib::RefPtr<Gdk::Pixbuf>>*> loaded_images;
	Support::ThreadSafeQueue<std::pair<int,Glib::RefPtr<Gdk::Pixbuf>>> loaded_images;
	Glib::Dispatcher image_dispatcher;
	std::vector<std::thread> threads;
	int tiles_per_row;

	sigc::connection size_allocation_connection;
	sigc::connection dispatcher_connection;

	void onSizeAllocate(Gdk::Rectangle& allocation);
	void calculateTilePerRow();
	static void loadPhotos(CentrePane* object);
	void abortThreads();
	void updateDisplayedImage();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_CENTREPANE_H_ */
