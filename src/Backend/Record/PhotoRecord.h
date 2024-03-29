/*
 * PhotoRecord.h
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

#ifndef SRC_BACKEND_RECORD_PHOTORECORD_H_
#define SRC_BACKEND_RECORD_PHOTORECORD_H_

#include <Record.h>
#include <glibmm/ustring.h>
#include <cstdint>

namespace PhotoLibrary {
namespace Backend {
namespace RecordClasses {

using PhotoTuple = std::tuple<int,Glib::ustring,int,int_least64_t,int,int>;

/**
 * Class to hold a photo entry.
 * A PhotoRecord holds a complete set of base data for one photo.
 * Used to retrieve base photo entries from the database, insert new
 * base photo entries, or update existing ones.
 * A PhotoRecord doesn't know its own id.
 */
class PhotoRecord: public PhotoLibrary::DatabaseInterface::Record<PhotoTuple> {
public:
	/**
	 * @param directory id of the parent directory of the file
	 * @param filename name of the file
	 * @param rating rating of the photo
	 * @param datetime date and time when the photo was taken (unix time)
	 * @param width width of the photo in pixel
	 * @param height height of the photo in pixel
	 */
	PhotoRecord(
			int directory=0,
			const Glib::ustring& filename={},
			int rating=0,
			int_least64_t datetime=0,
			int width=0,
			int height=0
			) :
		Record<PhotoTuple>(directory, filename, rating, datetime, width, height) {}

	/**
	* \copydoc PhotoRecord(int,Glib::ustring&&,int,int_least64_t,int,int)
	*/
	PhotoRecord(
			int directory,
			Glib::ustring&& filename,
			int rating=0,
			int_least64_t datetime=0,
			int width=0,
			int height=0
			) :
		Record<PhotoTuple>(directory, std::move(filename), rating, datetime, width, height) {}

	/**
	 * Set the directory.
	 * Returns a reference to the directory id.
	 *
	 * @return reference to the directory id
	 */
	int& setDirectory() noexcept { return access<0>(); }

	/**
	 * Get the directory id.
	 *
	 * @return value of the directory id
	 */
	int getDirectory() const noexcept { return access<0>(); }

	/**
	 * Set the filename of the photo.
	 * Returns a reference to the filename of the photo.
	 *
	 * @return reference to the filename of the photo
	 */
	Glib::ustring& setFilename() noexcept { return access<1>(); }

	/**
	 * Get the filename of the photo.
	 *
	 * @return value of the filename of the photo
	 */
	const Glib::ustring& getFilename() const noexcept { return access<1>(); }

	/**
	 * Set the rating.
	 * Returns a reference to the rating.
	 *
	 * @return reference to the rating
	 */
	int& setRating() noexcept { return access<2>(); }

	/**
	 * Get the rating.
	 *
	 * @return value of the rating
	 */
	int getRating() const noexcept { return access<2>(); }

	/**
	 * Set the date and time.
	 * Returns a reference to the date_time.
	 *
	 * @return reference to the date_time
	 */
	int_least64_t& setDatetime() noexcept { return access<3>(); }

	/**
	 * Get the date and time.
	 *
	 * @return value of the date_time
	 */
	int_least64_t getDatetime() const noexcept { return access<3>(); }

	/**
	 * Set the width.
	 * Returns a reference to the width in pixel.
	 *
	 * @return reference to the width in pixel
	 */
	int& setWidth() noexcept { return access<4>(); }

	/**
	 * Get the width.
	 *
	 * @return value of the width in pixel
	 */
	int getWidth() const noexcept { return access<4>(); }

	/**
	 * Set the height.
	 * Returns a reference to the height in pixel.
	 *
	 * @return reference to the height in pixel
	 */
	int& setHeight() noexcept { return access<5>(); }

	/**
	 * Get the height.
	 *
	 * @return value of the height in pixel
	 */
	int getHeight() const noexcept { return access<5>(); }

	static inline const std::array<const Glib::ustring,7>
		fields {"directory", "filename", "rating", "datetime", "width", "height", "Photos"};
	static inline const Glib::ustring table { "Photos" };
};

} /* namespace RecordClasses */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RECORD_PHOTORECORD_H_ */
