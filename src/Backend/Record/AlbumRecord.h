/*
 * AlbumRecord.h
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

#ifndef SRC_BACKEND_RECORD_ALBUMRECORD_H_
#define SRC_BACKEND_RECORD_ALBUMRECORD_H_

#include "Record.h"

namespace PhotoLibrary {
namespace Backend {
namespace RecordClasses {

using AlbumTuple = std::tuple<int,RecordOptions::Options,Glib::ustring>;

/**
 * Class to hold an album record.
 * An AlbumRecord holds a complete set of data for one album.
 * Used to retrieve album entries from the database, insert new
 * album entries, or update existing one. A AlbumRecord doesn't
 * know its own id.
 */
class AlbumRecord: public Record<AlbumTuple> {
public:
	/**
	 * @param parent_id id of the parent album set
	 * @param options options of the album (can be any combination of RecordOptions::Options)
	 * @param name the album name
	 */
	AlbumRecord(int parent_id=0, Options options=Options::NONE, const Glib::ustring& name={}) :
		Record<AlbumTuple>(parent_id, options, name) {}

	/**
	 * \copydoc AlbumRecord
	 */
	AlbumRecord(int parent_id, Options options, Glib::ustring&& name) :
		Record<AlbumTuple>(parent_id, options, std::move(name)) {};

	virtual ~AlbumRecord() noexcept = default;

	/**
	 * Set the parent id.
	 * Returns a reference to the parent id.
	 *
	 * @return reference to the parent id
	 */
	int& setParent() noexcept { return access<0>(); }

	/**
	 * Get the parent id.
	 *
	 * @return value of the parent id
	 */
	int getParent() const noexcept { return access<0>(); }

	/**
	 * Set the options.
	 * Returns a reference to the options.
	 *
	 * @return reference to the options
	 */
	Options& setOptions() noexcept { return access<1>(); }

	/**
	 * Get the options.
	 *
	 * @return value ot the options
	 */
	const Options getOptions() const noexcept { return access<1>(); }

	/**
	 * Set the album name.
	 * Returns a reference to the directory name.
	 *
	 * @return reference to the directory name
	 */
	Glib::ustring& setAlbumName() noexcept { return access<2>(); }

	/**
	 * Get the album name.
	 *
	 * @return value of the directory name
	 */
	const Glib::ustring& getAlbumName() const noexcept { return access<2>(); }

	/**
	 * Get the name of a data field.
	 * Returns the name of the column in the database.
	 *
	 * @param i number of the data field
	 * @return name of the data field
	 *
	 * @throws std::out_of_range if std::array<T>::operator[] is range checked and i is not in [0;size())
	 */
	static const Glib::ustring& getField(int i) { return fields[i]; }

//private:
	static inline const std::array<const Glib::ustring,4> fields {"parent", "attributes", "name", "Albums"};

	//static_assert(fields.size() == size());
};

/**
 * Structure to hold an album record used to add a albums.
 * It has another varable to hold a backup of parent id so that
 * the parent id in the underlying AlbumRecord can be switched between
 * the backup parent id and the root id.
 *
 * \todo Refactor GUI::NewKeywordDialogue and remove
 */
class NewAlbumRecord : public AlbumRecord {
public:
	/**
	 * @param parent_id id of the parent album set
	 * @param options options of the album (can be any combination of RecordOptions::Options)
	 * @param name the album name
	 */
	NewAlbumRecord(int parent_id=0, Options options = Options::NONE, const Glib::ustring& name={}) :
		AlbumRecord(parent_id, options, name), new_parent_id_backup(parent_id) {};

	/**
	 * \copydoc NewAlbumRecord
	 */
	NewAlbumRecord(int parent_id, Options options, Glib::ustring&& name) :
		AlbumRecord(parent_id, options, std::move(name)), new_parent_id_backup(parent_id) {};

	virtual ~NewAlbumRecord() noexcept = default;

	int new_parent_id_backup;	/**< Id of the potential parent of the new album. */
};

} /* namespace RecordClasses */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RECORD_ALBUMRECORD_H_ */
