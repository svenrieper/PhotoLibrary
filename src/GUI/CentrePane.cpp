/*
 * CentrePane.cpp
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

#include "CentrePane.h"
#include <giomm/resource.h>
#include <filesystem>
#include <typeinfo>

namespace PhotoLibrary {
namespace GUI {

using Backend::BackendFactory;
using Backend::DirectoryRecord;

CentrePane::CentrePane(Backend::BackendFactory* backend) :
		backend(backend),
//		tiles_to_update(0),
//		loaded_images(0),
		threads(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::N_THREADS)),
		tiles_per_row(0) {
	flowbox.set_homogeneous(true);
	flowbox.set_activate_on_single_click(true);
	flowbox.set_column_spacing(0);

	//put the flobox inside a box so that the child widgets aren't expanded vertically
	add(box);
	box.pack_start(flowbox, false, false);

	calculateTilePerRow();
}

CentrePane::~CentrePane() {
	try {
		abortThreads();
	}
	catch (...) {

	}
}

/// \todo consider the space between the tiles and add space around the images
void CentrePane::calculateTilePerRow() {
	tiles_per_row = backend->getCentreWidth()
			/ backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH);
	flowbox.set_min_children_per_line(tiles_per_row);
	flowbox.set_max_children_per_line(tiles_per_row);
}

void CentrePane::abortThreads() {
	// empty queue and wait for the threads to finish
//	for(int i; tiles_to_update.pop(i););
	tiles_to_update.clear();
	for(std::thread& t : threads)
		if(t.joinable())
			t.join();
	// empty the queue with loaded (but not yet displayed) images
//	for(std::pair<int,Glib::RefPtr<Gdk::Pixbuf>>* image; loaded_images.pop(image);)
//		delete image;
	loaded_images.clear();
}

///\todo verify that the program doesn't crash anymore, if the method is called before all images are displayed
void CentrePane::fillGrid(std::vector<int> photos) {
	size_allocation_connection.disconnect();
	dispatcher_connection.disconnect();

	abortThreads();
	for(auto& tile : tiles)
		flowbox.remove(*(tile.second));
	tiles.clear();

	for(int photo_id : photos) {
		tiles.insert(std::make_pair(photo_id, new PhotoTile(backend, photo_id)));
		tiles[photo_id]->set_size_request(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH),
				backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH));
		flowbox.add(*tiles[photo_id]);
		tiles_to_update.push(photo_id);
	}
	show_all();
	dispatcher_connection = image_dispatcher.connect(sigc::mem_fun(*this, &CentrePane::updateDisplayedImage));
	for(std::thread& t : threads)
		t = std::thread(&CentrePane::loadPhotos, this);

	size_allocation_connection = signal_size_allocate().connect(sigc::mem_fun(*this, &CentrePane::onSizeAllocate));
}

/// \todo find a solution that isn't called every time any child widget changes
void CentrePane::onSizeAllocate(Gdk::Rectangle& allocation) {
	calculateTilePerRow();
}

void CentrePane::loadPhotos(CentrePane* object) {
	for(int tile_id; object->tiles_to_update.pop(tile_id);) {
		Glib::ustring filename = object->tiles[tile_id]->getFilename();
		if(std::filesystem::exists(filename.c_str())) { // @suppress("Invalid arguments")
			try {
				int size = object->backend->getWindowProperty(BackendFactory::WindowProperties::TILE_WIDTH);
				auto photo_image = Gdk::Pixbuf::create_from_file(filename, size, size, true);
//				object->loaded_images.push(new std::pair<int,Glib::RefPtr<Gdk::Pixbuf>>(tile_id, photo_image));
				/// \todo use emplace?
				object->loaded_images.push(std::make_pair(tile_id, photo_image));
				object->image_dispatcher.emit();
			}
			catch (const Gio::ResourceError &e) {
				std::cerr << "ResourceError: " << e.what() << std::endl;
			}
			catch (const Gdk::PixbufError &e) {
				std::cerr << "PixbufError: " << e.what() << std::endl;
			}
		}
	}
}

void CentrePane::updateDisplayedImage() {
//	if(std::pair<int,Glib::RefPtr<Gdk::Pixbuf>>* image; loaded_images.pop(image)) {
//		tiles[image->first]->setPhoto(image->second);
//		delete image;
//	}
	if(std::pair<int,Glib::RefPtr<Gdk::Pixbuf>> image; loaded_images.pop(image))
		tiles[image.first]->setPhoto(image.second);
}

} /* namespace GUI */
} /* namespace PhotoLibrary */