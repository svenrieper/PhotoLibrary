/*
 * Keyword.h
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

#ifndef SRC_ADAPTER_RECORD_KEYWORDRECORD_H_
#define SRC_ADAPTER_RECORD_KEYWORDRECORD_H_

#include "Record.h"

namespace PhotoLibrary {
namespace Backend {

using KeywordTuple = std::tuple<int,RecordOptions::Options,Glib::ustring,Glib::ustring>;

/**
 * Class to hold a keyword.
 * A KeywordRecord holds a complete set of data for one keyword.
 * Used to retrieve keywords from the database, insert new
 * keyword, or update existing ones. A Keyword doesn't know
 * its own id.
 */
class KeywordRecord : public Record<KeywordTuple> {
public:
	/**
	 * @param parent_id id of the parent of the keyword
	 * @param options options of the keyword (can be any combination of RecordOptions::Options)
	 * @param key the keyword
	 * @param synonyms synonyms of the keyword
	 */
	KeywordRecord(int parent_id=0, Options options = Options::NONE, Glib::ustring&& key="", Glib::ustring&& synonyms="") :
		Record<KeywordTuple>(parent_id, options, std::move(key), std::move(synonyms)) {}

	/**
	 * \copydoc KeywordRecord
	 */
	KeywordRecord(int parent_id, Options options, const Glib::ustring& key, const Glib::ustring& synonyms="") :
		Record<KeywordTuple>(parent_id, options, key, synonyms) {}

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
	 * Set the keyword.
	 * Returns a reference to the keyword.
	 *
	 * @return reference to the keyword
	 */
	Glib::ustring& setKeyword() { return access<2>(); }

	/**
	 * Get the keyword.
	 *
	 * @return value of the keyword
	 */
	const Glib::ustring& getKeyword() const { return access<2>(); }

	/**
	 * Set the synonyms.
	 * Returns a reference to the synonyms.
	 *
	 * @return reference to the synonyms
	 */
	Glib::ustring& setSynonyms() { return access<3>(); }

	/**
	 * Get the synonyms.
	 *
	 * @return value of the synonyms
	 */
	const Glib::ustring& getSynonyms() const { return access<3>(); }

	/**
	 * Get the name of a data field.
	 * Returns the name of the column in the database.
	 *
	 * @param i number of the data field
	 * @return name of the data field
	 */
	static const Glib::ustring& getField(int i) { return fields.at(i); }

private:
	static inline const std::array<Glib::ustring,4> fields {"parent", "attributes", "keyword", "synonyms"};
};

/**
 * Structure to hold a keyword used to add a keyword.
 * It has another varable to hold a backup of parent id so that
 * the parent id in the underlying KeywordRecord can be switched between
 * the backup parent id and the root id.
 */
struct NewKeywordRecord : public KeywordRecord {
public:
	/**
	 * @param parent_id id of the parent of the keyword
	 * @param options options of the keyword (can be any combination of Keyword::Options)
	 * @param key the keyword
	 * @param synonyms synonyms of the keyword
	 */
	NewKeywordRecord(int parent_id, Options options = Options::NONE, Glib::ustring key="", Glib::ustring synonyms="") :
		KeywordRecord(parent_id, options, key, synonyms), new_parent_id_backup(parent_id) {}

	int new_parent_id_backup;	/**< Id of the potential parent of the new keyword */
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_RECORD_KEYWORDRECORD_H_ */
