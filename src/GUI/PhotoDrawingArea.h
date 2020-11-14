/*
 * PhotoDrawingArea.h
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

#ifndef SRC_GUI_PHOTODRAWINGAREA_H_
#define SRC_GUI_PHOTODRAWINGAREA_H_

#include <gtkmm/drawingarea.h>

namespace PhotoLibrary {
namespace GUI {

class PhotoDrawingArea : public Gtk::DrawingArea {
public:
	PhotoDrawingArea(int tile_size, int width, int height);
	PhotoDrawingArea(PhotoDrawingArea&&) noexcept;
	virtual ~PhotoDrawingArea() = default;

	void setPhoto(Glib::RefPtr<Gdk::Pixbuf> image);

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
	Glib::RefPtr<Gdk::Pixbuf> photo_image;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_PHOTODRAWINGAREA_H_ */
