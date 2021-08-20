/*
 * BackendFactory.h
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

#ifndef SRC_BACKEND_BACKENDFACTORY_H_
#define SRC_BACKEND_BACKENDFACTORY_H_

#include "FactoryBase.h"
#include "DatabaseInterface/DatabaseFactory.h"
#include <AccessTables.h>
#include <unordered_map>
#include <memory>

namespace PhotoLibrary {
namespace Backend {

/**
 * The interface to get, set, and delete relations between photos and albums.
 */
using PhotosAlbumsRelationsInterface = RelationsInterfaceBase;

/**
 * Factory class to construct the backend.
 * Constructs the backend classes and hands them to the GUI on demand.
 */
class BackendFactory : public FactoryBase {
public:
	/**
	 * Properties of the main window.
	 * Properties of the main window that can be loaded or saved with
	 * getWindowProperty(WindowProperties) and setWindowProperty(WindowProperties, int).
	 */
	enum class WindowProperties {
		WINDOW_WIDTH,	/**< Width of the main window. */
		WINDOW_HEIGHT,	/**< Height of the main window. */
		LEFT_PANE_WIDTH,	/**< Width of the left pane. */
		RIGHT_PANE_WIDTH,	/**< Width of the right pane. */
		TILE_WIDTH,	/**< The width of a tile in the centre frame */
//		TILE_HEIGHT,	/**< The height of a tile in the centre frame */
		N_THREADS,	/**< Number of threads to use */
	};

	/**
	 * @param filename Filename and path of the database to use
	 */
	BackendFactory(const char* filename = nullptr);

	template<typename RecordType>
	RecordType getEntry(int id);

	template<typename RecordType>
	std::vector<int> getChildren(int parent);

	template<typename RecordType>
	int getNumberChildren(int parent);

	template<typename RecordType>
	void newEntry(const RecordType& entry);

	template<typename RecordType>
	void updateEntry(int id, const RecordType& entry);

	template<typename RecordType>
	void setParent(int child_id, int new_parent_id);

	template<typename RecordType>
	int getID(const RecordType& entry);

	template<typename RecordType>
	void deleteEntry(int id);

	PhotosAlbumsRelationsInterface* getPhotosAlbumsRelationsInterface() override;
	const PhotosAlbumsRelationsInterface* getPhotosAlbumsRelationsInterface() const override;
	PhotosAlbumsRelationsInterface* getPhotosKeywordsRelationsInterface() override;
	const PhotosAlbumsRelationsInterface* getPhotosKeywordsRelationsInterface() const override;

	/**
	 * Retrieve the value of a main window property.
	 *
	 * @param property Property for which the value should be returned
	 * @return Value of 'property'
	 */
	int getWindowProperty(WindowProperties property) const;

	/**
	 * Save the value of a main window property.
	 *
	 * @param property Property for which the value should be saved
	 * @param value New value of 'property'
	 */
	void setWindowProperty(WindowProperties property, int value);

	/**
	 * Get the width of the central pane.
	 *
	 * @return Width of the central pane
	 */
	int getCentreWidth() const;

private:
	std::unique_ptr<DatabaseInterface::DatabaseFactory> db;
	std::unique_ptr<PhotoLibrary::DatabaseInterface::AccessTables<Glib::ustring>> tables_interface;
	std::unordered_map<WindowProperties,int> window_properties;

	//prevent copying and copy construction
	BackendFactory(const BackendFactory &other) = delete;
	BackendFactory(BackendFactory &&other) = delete;
	BackendFactory& operator=(const BackendFactory &other) = delete;
	BackendFactory& operator=(BackendFactory &&other) = delete;
};

template<typename RecordType>
RecordType BackendFactory::getEntry(int id) {
	return tables_interface->getEntry<RecordType>(id);
}

template<typename RecordType>
std::vector<int> BackendFactory::getChildren(int parent) {
	return tables_interface->getChildren<RecordType>(parent);
}

template<typename RecordType>
int BackendFactory::getNumberChildren(int parent) {
	return tables_interface->getNumberChildren<RecordType>(parent);
}

template<typename RecordType>
void BackendFactory::newEntry(const RecordType& entry) {
	return tables_interface->newEntry<RecordType>(entry);
}

template<typename RecordType>
void BackendFactory::updateEntry(int id, const RecordType& entry) {
	return tables_interface->updateEntry<RecordType>(id, entry);
}

template<typename RecordType>
void BackendFactory::setParent(int child_id, int new_parent_id) {
	return tables_interface->setParent<RecordType>(child_id, new_parent_id);
}

template<typename RecordType>
int BackendFactory::getID(const RecordType& entry) {
	return tables_interface->getID<RecordType>(entry);
}

template<typename RecordType>
void BackendFactory::deleteEntry(int id) {
	return tables_interface->deleteEntry<RecordType>(id);
}


} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_BACKENDFACTORY_H_ */
