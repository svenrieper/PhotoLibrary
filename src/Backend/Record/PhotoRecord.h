/*
 * PhotoRecord.h
 *
 *  Created on: 25 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_BACKEND_RECORD_PHOTORECORD_H_
#define SRC_BACKEND_RECORD_PHOTORECORD_H_

#include "Record.h"
#include <cstdint>

namespace PhotoLibrary {
namespace Backend {

using PhotoTuple = std::tuple<int,Glib::ustring,int,int_least64_t>;

/**
 * Class to hold a photo entry.
 * A PhotoRecord holds a complete set of base data for one photo.
 * Used to retrieve base photo entries from the database, insert new
 * base photo entries, or update existing ones.
 * A PhotoRecord doesn't know its own id.
 */
class PhotoRecord: public Record<PhotoTuple> {
public:
	PhotoRecord(int directory=0, Glib::ustring filename="", int rating=0, int_least64_t datetime=0) :
			Record<PhotoTuple>(std::make_tuple(directory, filename, rating, datetime)) {}

	virtual ~PhotoRecord() = default;

	/**
	 * Set the directory.
	 * Returns a reference to the directory id.
	 *
	 * @return reference to the directory id
	 */
	int& setDirectory() { return access<0>(); }

	/**
	 * Get the directory id.
	 *
	 * @return value of the directory id
	 */
	int getDirectory() const { return access<0>(); }

	/**
	 * Set the filename of the photo.
	 * Returns a reference to the filename of the photo.
	 *
	 * @return reference to the filename of the photo
	 */
	Glib::ustring& setFilename() { return access<1>(); }

	/**
	 * Get the filename of the photo.
	 *
	 * @return value of the filename of the photo
	 */
	const Glib::ustring& getFilename() const { return access<1>(); }

	/**
	 * Set the rating.
	 * Returns a reference to the rating.
	 *
	 * @return reference to the rating
	 */
	int& setRating() { return access<2>(); }

	/**
	 * Get the rating.
	 *
	 * @return value of the rating
	 */
	int getRating() const { return access<2>(); }

	/**
	 * Set the date and time.
	 * Returns a reference to the date_time.
	 *
	 * @return reference to the date_time
	 */
	int_least64_t& setDatetime() { return access<3>(); }

	/**
	 * Get the date and time.
	 *
	 * @return value of the date_time
	 */
	int_least64_t getDatetime() const { return access<3>(); }

	/**
	 * Get the name of a data field.
	 * Returns the name of the column in the database.
	 *
	 * @param i number of the data field
	 * @return name of the data field
	 */
	const Glib::ustring& getField(int i) const override { return fields[i]; }

private:
	static inline const std::array<Glib::ustring,4> fields {"directory", "filename", "rating", "datetime"};
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RECORD_PHOTORECORD_H_ */
