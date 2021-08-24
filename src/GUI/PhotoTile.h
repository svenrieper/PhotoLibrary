/*
 * PhotoTile.h
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

#ifndef SRC_GUI_PHOTOTILE_H_
#define SRC_GUI_PHOTOTILE_H_

#include "PhotoDrawingArea.h"
#include "BackendFactory.h"
#include "Record/PhotoRecord.h"
#include <gtkmm/hvbox.h>

namespace PhotoLibrary {
namespace GUI {

/**
 * Tile for the grid view in the centre pane.
 *
 * For performance reasons PhotoTile's constructor doesn't load
 * the tumbnail to be displayed in the tile, it needs to be
 * loaded externally and set with setPhoto(Glib::RefPtr<Gdk::Pixbuf>).
 *
 * \todo display information about the photos
 */
class PhotoTile : public Gtk::VBox {
public:
	/**
	 * Constructor.
	 * The constructor does not load the thumbnail, it needs
	 * to be set with setPhoto(Glib::RefPtr<Gdk::Pixbuf>)
	 *
	 * @param backend pointer to the BackendFactory object
	 *
	 * \todo construct with PhotoRecord, full_path, and tile width instead?
	 */
	PhotoTile(Backend::BackendFactory* backend, int photo_id);
	PhotoTile(PhotoTile&&) noexcept;
	virtual ~PhotoTile() = default;

	/**
	 * Sets the thumbnail.
	 * After construction the thumbnail needs to be set.
	 *
	 * @param image RefPtr to the tumbnail to be displayed
	 * 		in the tile
	 */
	inline void setPhoto(Glib::RefPtr<Gdk::Pixbuf> image);

	/**
	 * Get the filename of the image.
	 * Returns the full path and filename of the image
	 * to be displayed in the tile.
	 * \todo move to backend?
	 *
	 * @return Full path and filname of the image to be
	 * 		displayed in the tile.
	 */
	Glib::ustring getFilename();

private:
	Backend::BackendFactory* backend;
	Backend::RecordClasses::PhotoRecord photo_record;
	PhotoDrawingArea photo_image;

	///\todo move to backend
	Glib::ustring getFullPath();
};


//implementation
void PhotoTile::setPhoto(Glib::RefPtr<Gdk::Pixbuf> image) {
	photo_image.setPhoto(image);
}

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_PHOTOTILE_H_ */
