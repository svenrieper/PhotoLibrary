/*
 * PhotoTile.h
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

#ifndef SRC_GUI_PHOTOTILE_H_
#define SRC_GUI_PHOTOTILE_H_

#include "../Backend/BackendFactory.h"
#include "PhotoDrawingArea.h"
#include <gtkmm/hvbox.h>

namespace PhotoLibrary {
namespace GUI {

class PhotoTile : public Gtk::VBox {
public:
	/// \todo construct with PhotoRecord, full_path, and tile width instead
	PhotoTile(Backend::BackendFactory* backend, int photo_id);
	PhotoTile(PhotoTile&&) noexcept;
	virtual ~PhotoTile() = default;

	inline void setPhoto(Glib::RefPtr<Gdk::Pixbuf> image);
	Glib::ustring getFilename();

private:
	Backend::BackendFactory* backend;
	Backend::PhotoRecord photo_record;
	PhotoDrawingArea photo_image;

	///\todo find better solution
	Glib::ustring getFullPath();
};

void PhotoTile::setPhoto(Glib::RefPtr<Gdk::Pixbuf> image) {
	photo_image.setPhoto(image);
}

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_PHOTOTILE_H_ */
