/*
 * RelationsDBInterface.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2021 Sven Rieper
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

#ifndef SRC_BACKEND_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_
#define SRC_BACKEND_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_

#include "../RelationsInterfaceBase.h"
#include <glibmm/ustring.h>
#include "../../SQLiteAdapter/Database.h"

namespace PhotoLibrary {
namespace Backend {
namespace DatabaseInterface {

/**
 * Implementation of the relations interface for the SQLite database.
 * Used to get, add, and delete relations for many-to-many relationships
 * with integer keys.
 * The interface assumes that there is some kind of collections-entries
 * relationship.
 */
class RelationsDBInterface: public Backend::RelationsInterfaceBase {
public:
	/**
	 * @param db Handle for the database to use
	 * @param table Name of the associated table
	 * @param entry_name Name of the database column containing the entry-like ids.
	 * @param collection_name Name of the database column containing the collection-like ids.
	 */
	RelationsDBInterface(SQLiteAdapter::Database* db, const Glib::ustring& table, const Glib::ustring& entry_name, const Glib::ustring& collection_name);
	virtual ~RelationsDBInterface() = default;

	std::vector<int> getEntries(int collection) const override;
	std::vector<int> getCollections(int entry) const override;
	void newRelation(int entry, int collection) override;
	void deleteRelation(int entry, int collection) override;

private:
	SQLiteAdapter::Database* const db;
	const Glib::ustring table;
	const Glib::ustring entry_name;
	const Glib::ustring collection_name;

	std::vector<int> getVector(int id, const Glib::ustring& reference_id, const Glib::ustring& return_id) const;
};

} /* namespace DatabaseInterface */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_DATABASEINTERFACE_RELATIONSDBINTERFACE_H_ */
