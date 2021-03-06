/*
 * DirectoryRecord.h
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

#ifndef SRC_ADAPTER_RECORD_DIRECTORYRECORD_H_
#define SRC_ADAPTER_RECORD_DIRECTORYRECORD_H_

#include "Record.h"

namespace PhotoLibrary {
namespace Backend {

using DirectoryTuple = std::tuple<int,RecordOptions::Options,Glib::ustring,Glib::ustring>;

/**
 * Class to hold a directory record.
 * A DirectoryRecord holds a complete set of data for one directory.
 * Used to retrieve directory entries from the database, insert new
 * directory entries, or update existing one. A DirectoryRecord doesn't
 * know its own id.
 */
class DirectoryRecord : public Record<DirectoryTuple> {
public:
	/**
	 * @param parent_id id of the parent directory
	 * @param options options of the directory (can be any combination of RecordOptions::Options)
	 * @param name the directory name
	 * @param fullname full path for 'root' directories, the directory name otherwise
	 */
	DirectoryRecord(int parent_id=0, Options options = Options::NONE, Glib::ustring name="", Glib::ustring fullname="") :
		Record<DirectoryTuple>(std::make_tuple(parent_id, options, name, fullname)) {}

	~DirectoryRecord() = default;

	/**
	 * Set the parent id.
	 * Returns a reference to the parent id.
	 *
	 * @return reference to the parent id
	 */
	int& setParent() { return access<0>(); }

	/**
	 * Get the parent id.
	 *
	 * @return value of the parent id
	 */
	int getParent() const { return access<0>(); }

	/**
	 * Set the options.
	 * Returns a reference to the options.
	 *
	 * @return reference to the options
	 */
	Options& setOptions() { return access<1>(); }

	/**
	 * Get the options.
	 *
	 * @return value ot the options
	 */
	const Options& getOptions() const { return access<1>(); }

	/**
	 * Set the directroy name.
	 * Returns a reference to the directory name.
	 *
	 * @return reference to the directory name
	 */
	Glib::ustring& setDirectory() { return access<2>(); }

	/**
	 * Get the dierectory name.
	 *
	 * @return value of the directory name
	 */
	const Glib::ustring& getDirectory() const { return access<2>(); }

	/**
	 * Set the full directory name.
	 * Returns a reference to the full directory name.
	 *
	 * @return reference to the full directory name
	 */
	Glib::ustring& setFullDirectory() { return access<3>(); }

	/**
	 * Get the full directory name.
	 *
	 * @return value of the full directory name
	 */
	const Glib::ustring& getFullDirectory() const { return access<3>(); }

	/**
	 * Get the name of a data field.
	 * Returns the name of the column in the database.
	 *
	 * @param i number of the data field
	 * @return name of the data field
	 */
	static const Glib::ustring& getField(int i) { return fields.at(i); }

private:
	static inline const std::array<Glib::ustring,4> fields {"parent", "attributes", "name", "fullname"};
};

} /* namespace Backend */
} /* namespace PhotoLibrary */



#endif /* SRC_ADAPTER_RECORD_DIRECTORYRECORD_H_ */
