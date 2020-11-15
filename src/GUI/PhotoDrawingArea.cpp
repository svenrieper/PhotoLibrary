/*
 * PhotoDrawingArea.cpp
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

#include "PhotoDrawingArea.h"
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>
#include <filesystem>

namespace PhotoLibrary {
namespace GUI {

PhotoDrawingArea::PhotoDrawingArea(int tile_size, int width, int height) {
	try {
		photo_image = Gdk::Pixbuf::create_from_resource("/image/grey_pixel.png",
				width >= height ? 250 : 250 * width / height,
				height >= width ? 250 : 250 * height / width,
				false);
	}
	catch (const Gio::ResourceError &e) {
		std::cerr << "ResourceError: " << e.what() << std::endl;
	}
	catch (const Gdk::PixbufError &e) {
		std::cerr << "PixbufError: " << e.what() << std::endl;
	}

	set_size_request(tile_size, tile_size);
}

PhotoDrawingArea::PhotoDrawingArea(PhotoDrawingArea&& a) noexcept :
		Gtk::DrawingArea(std::move(a)),
		photo_image(a.photo_image) {
}

bool PhotoDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (photo_image) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		// Draw the image in the middle of the drawing area, or (if the image is
		// larger than the drawing area) draw the middle part of the image.
		Gdk::Cairo::set_source_pixbuf(cr, photo_image,
				(width - photo_image->get_width()) / 2, (height - photo_image->get_height()) / 2);
		cr->paint();
	}

	return false;
}

void PhotoDrawingArea::setPhoto(Glib::RefPtr<Gdk::Pixbuf> image) {
	photo_image = image;
	queue_draw();
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
