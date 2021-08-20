/*
 * CentrePane.cpp
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020-2021 Sven Rieper
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
#include <iostream>
#include <typeinfo>

namespace PhotoLibrary {
namespace GUI {

using Backend::BackendFactory;

CentrePane::CentrePane(Backend::BackendFactory* backend) :
		backend(backend),
		threads(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::N_THREADS)),
		tiles_per_row(0) {
	flowbox.set_homogeneous(true);
	flowbox.set_activate_on_single_click(true);
	flowbox.set_column_spacing(0);

	//put the flowbox inside a box so that the child widgets aren't expanded vertically
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
	tiles_to_update.clear();
	for(std::thread& t : threads)
		if(t.joinable())
			/// \todo catch std::system_error to continue loop if joining one thread fails?
			/// \todo handle error after loop?
			t.join();
	// empty the queue with loaded (but not yet displayed) images
	loaded_images.clear();
}

void CentrePane::fillGrid(std::vector<int> photos) {
	size_allocation_connection.disconnect();
	dispatcher_connection.disconnect();

	abortThreads();
	for(auto& tile : tiles)
		flowbox.remove(*(tile.second));
	tiles.clear();

	for(int photo_id : photos) {
		std::unique_ptr<PhotoTile> newTile(new PhotoTile(backend, photo_id));
		newTile->set_size_request(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH),
				backend->getWindowProperty(Backend::BackendFactory::WindowProperties::TILE_WIDTH));
		flowbox.add(*newTile);
		if(!tiles.emplace(photo_id, std::move(newTile)).second)
			throw std::runtime_error("Error adding new tile to GridView tiles map.");
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
	for(int photo_id; object->tiles_to_update.pop(photo_id);) try {
		Glib::ustring filename = object->tiles.at(photo_id)->getFilename();
		if(std::filesystem::exists(filename.c_str())) { // @suppress("Invalid arguments")
			try {
				int size = object->backend->getWindowProperty(BackendFactory::WindowProperties::TILE_WIDTH);
				auto photo_image = Gdk::Pixbuf::create_from_file(filename, size, size, true);
				/// \todo use emplace?
				object->loaded_images.push(std::make_pair(photo_id, photo_image));
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
	catch (std::out_of_range&) {}
}

void CentrePane::updateDisplayedImage() try {
	if(std::pair<int,Glib::RefPtr<Gdk::Pixbuf>> image; loaded_images.pop(image))
		tiles.at(image.first)->setPhoto(image.second);
}
catch (std::out_of_range&) {}

} /* namespace GUI */
} /* namespace PhotoLibrary */
