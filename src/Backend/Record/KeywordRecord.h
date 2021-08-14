/*
 * KeywordRecord.h
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

#include "Options.h"
#include <Record.h>
#include <glibmm/ustring.h>

namespace PhotoLibrary {
namespace Backend {
namespace RecordClasses {

using KeywordTuple = std::tuple<int,RecordOptions::Options,Glib::ustring,Glib::ustring>;

/**
 * Class to hold a keyword.
 * A KeywordRecord holds a complete set of data for one keyword.
 * Used to retrieve keywords from the database, insert new
 * keyword, or update existing ones. A Keyword doesn't know
 * its own id.
 */
class KeywordRecord : public PhotoLibrary::DatabaseInterface::Record<KeywordTuple> {
public:
	using Options = RecordOptions::Options;

	/**
	 * @param parent_id id of the parent of the keyword
	 * @param options options of the keyword (can be any combination of RecordOptions::Options)
	 * @param key the keyword
	 * @param synonyms synonyms of the keyword
	 */
	KeywordRecord(
			int parent_id=0,
			Options options=Options::NONE,
			const Glib::ustring& key={},
			const Glib::ustring& synonyms={}
			) :
		Record<KeywordTuple>(parent_id, options, key, synonyms) {}

	/**
	 * \copydoc KeywordRecord
	 */
	KeywordRecord(int parent_id, Options options, Glib::ustring&& key, const Glib::ustring& synonyms={}) :
		Record<KeywordTuple>(parent_id, options, std::move(key), synonyms) {}

	/**
	 * \copydoc KeywordRecord
	 */
	KeywordRecord(int parent_id, Options options, Glib::ustring&& key, Glib::ustring&& synonyms) :
		Record<KeywordTuple>(parent_id, options, std::move(key), std::move(synonyms)) {}

	/**
	 * \copydoc KeywordRecord
	 */
	KeywordRecord(int parent_id, Options options, const Glib::ustring& key, Glib::ustring&& synonyms) :
		Record<KeywordTuple>(parent_id, options, key, std::move(synonyms)) {}

	virtual ~KeywordRecord() noexcept = default;

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
	 * Set the keyword.
	 * Returns a reference to the keyword.
	 *
	 * @return reference to the keyword
	 */
	Glib::ustring& setKeyword() noexcept { return access<2>(); }

	/**
	 * Get the keyword.
	 *
	 * @return value of the keyword
	 */
	const Glib::ustring& getKeyword() const noexcept { return access<2>(); }

	/**
	 * Set the synonyms.
	 * Returns a reference to the synonyms.
	 *
	 * @return reference to the synonyms
	 */
	Glib::ustring& setSynonyms() noexcept { return access<3>(); }

	/**
	 * Get the synonyms.
	 *
	 * @return value of the synonyms
	 */
	const Glib::ustring& getSynonyms() const noexcept { return access<3>(); }

	static inline const std::array<const Glib::ustring,5> fields {"parent", "attributes", "keyword", "synonyms", "Keywords"};
	static inline const Glib::ustring table { "Keywords" };
};

/**
 * Structure to hold a keyword used to add a keyword.
 * It has another varable to hold a backup of parent id so that
 * the parent id in the underlying KeywordRecord can be switched between
 * the backup parent id and the root id.
 *
 * \todo refactor GUI::NewKeywordDialogue and remove
 */
struct NewKeywordRecord : public KeywordRecord {
public:
	/**
	 * @param parent_id id of the parent of the keyword
	 * @param options options of the keyword (can be any combination of Keyword::Options)
	 * @param key the keyword
	 * @param synonyms synonyms of the keyword
	 */
	NewKeywordRecord(
			int parent_id,
			Options options = Options::NONE,
			const Glib::ustring& key={},
			const Glib::ustring& synonyms={}
			) :
		KeywordRecord(parent_id, options, key, synonyms), new_parent_id_backup(parent_id) {}

	/**
	 * \copydoc NewKeywordRecord
	 */
	NewKeywordRecord(int parent_id, Options options, Glib::ustring&& key, const Glib::ustring& synonyms={}) :
		KeywordRecord(parent_id, options, std::move(key), synonyms), new_parent_id_backup(parent_id) {}

	/**
	 * \copydoc NewKeywordRecord
	 */
	NewKeywordRecord(int parent_id, Options options, Glib::ustring&& key, Glib::ustring&& synonyms) :
		KeywordRecord(parent_id, options, std::move(key), std::move(synonyms)), new_parent_id_backup(parent_id) {}

	/**
	 * \copydoc NewKeywordRecord
	 */
	NewKeywordRecord(int parent_id, Options options, const Glib::ustring& key, Glib::ustring&& synonyms) :
		KeywordRecord(parent_id, options, key, std::move(synonyms)), new_parent_id_backup(parent_id) {}

	virtual ~NewKeywordRecord() noexcept = default;

	int new_parent_id_backup;	/**< Id of the potential parent of the new keyword */
};

} /* namespace RecordClasses */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_RECORD_KEYWORDRECORD_H_ */
