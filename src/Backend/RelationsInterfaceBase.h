/*
 * RelationsInterfaceBase.h
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

#ifndef SRC_BACKEND_RELATIONSINTERFACEBASE_H_
#define SRC_BACKEND_RELATIONSINTERFACEBASE_H_

#include <vector>

namespace PhotoLibrary {
namespace Backend {

/**
 * Abstract base class for the backend relations interface.
 * Used to get, add, and delete relations for many-to-many relationships
 * with integer keys.
 * The interface assumes that there is some kind of collections-entries
 * relationship.
 */
class RelationsInterfaceBase {
public:
	RelationsInterfaceBase() = default;
	virtual ~RelationsInterfaceBase() = default;

	/**
	 * Get a vector of 'entries' based on a 'collection' id.
	 * @param collection Id of the 'collection' for which the 'entries'
	 * 		should be returned.
	 * @return Vector of ids of the 'entries'
	 */
	virtual std::vector<int> getEntries(int collection) const = 0;

	/**
	 * Get the number of 'entries' based on a 'collection' id.
	 * @param collection Id of the 'collection' for which the number
	 * 		of 'entries' should be returned.
	 * @return Number of the 'entries'
	 */
	virtual int getNumberEntries(int collection) const = 0;

	/**
	 * Get a vector of 'collections' based on an 'entry' id.
	 * @param entry Id of the 'entry' for which the 'collections'
	 * 		containig it should be returned.
	 * @return Vector of ids of the 'collections'
	 */
	virtual std::vector<int> getCollections(int entry) const = 0;

	/**
	 * Get the number of 'collections' based on a 'entriy' id.
	 * @param collection Id of the 'entry' for which the number
	 * 		of 'collections' should be returned.
	 * @return Number of the 'entries'
	 */
	virtual int getNumberCollections(int entry) const = 0;

	/**
	 * Add a new relations.
	 * @param entry Id of the 'entry'
	 * @param collection Id of the 'collection'
	 */
	virtual void newRelation(int entry, int collection) = 0;

	/**
	 * Remove a relation.
	 * @param entry Id of the 'entry'
	 * @param collection Id of the 'collection'
	 */
	virtual void deleteRelation(int entry, int collection) = 0;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RELATIONSINTERFACEBASE_H_ */
