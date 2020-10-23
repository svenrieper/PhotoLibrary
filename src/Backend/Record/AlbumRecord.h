/*
 * AlbumRecord.h
 *
 *  Created on: 19 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_BACKEND_RECORD_ALBUMRECORD_H_
#define SRC_BACKEND_RECORD_ALBUMRECORD_H_

#include "Record.h"

namespace PhotoLibrary {
namespace Backend {

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
	AlbumRecord(int parent_id=0, Options options = Options::NONE, Glib::ustring name="") :
		Record<AlbumTuple>(std::make_tuple(parent_id, options, name)) {};

	virtual ~AlbumRecord() = default;

	/**
	 * Get the name of a data field.
	 * Returns the name of the column in the database.
	 *
	 * @param i number of the data field
	 * @return name of the data field
	 */
	const Glib::ustring& getField(int i) const override { return fields[i]; }

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
	 * Set the album name.
	 * Returns a reference to the directory name.
	 *
	 * @return reference to the directory name
	 */
	Glib::ustring& setAlbumName() { return access<2>(); }

	/**
	 * Get the album name.
	 *
	 * @return value of the directory name
	 */
	const Glib::ustring& getAlbumName() const { return access<2>(); }

private:
	static inline const std::array<Glib::ustring,4> fields {"parent", "attributes", "name"};
};

/**
 * Structure to hold an album record used to add a albums.
 * It has another varable to hold a backup of parent id so that
 * the parent id in the underlying AlbumRecord can be switched between
 * the backup parent id and the root id.
 */
class NewAlbumRecord : public AlbumRecord {
public:
	/**
	 * @param parent_id id of the parent album set
	 * @param options options of the album (can be any combination of RecordOptions::Options)
	 * @param name the album name
	 */
	NewAlbumRecord(int parent_id=0, Options options = Options::NONE, Glib::ustring name="") :
		AlbumRecord(parent_id, options, name), new_parent_id_backup(parent_id) {};

	virtual ~NewAlbumRecord() = default;

	int new_parent_id_backup;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RECORD_ALBUMRECORD_H_ */
