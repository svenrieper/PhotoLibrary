/*
 * BackendFactory.h
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

#ifndef SRC_BACKEND_BACKENDFACTORY_H_
#define SRC_BACKEND_BACKENDFACTORY_H_

#include "FactoryBase.h"
#include <unordered_map>
#include <memory>
#include "DatabaseInterface/DatabaseFactory.h"

namespace PhotoLibrary {
namespace Backend {

/**
 * There is no backend for keywords so we just pass the database interface
 * for keywords through.
 */
using KeywordInterface = InterfaceBase<KeywordRecord>;

/**
 * There is no backend for directories so we just pass the database interface
 * for DirectoryInterface through.
 */
using DirectoryInterface = InterfaceBase<DirectoryRecord>;

/**
 * There is no backend for albums so we just pass the database interface
 * for DirectoryInterface through.
 */
using AlbumInterface = InterfaceBase<AlbumRecord>;

/**
 * There is no backend for photos so we just pass the database interface
 * for PhotoInterface through.
 */
using PhotoInterface = InterfaceBase<PhotoRecord>;

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
	~BackendFactory() = default;

	KeywordInterface* getKeywordInterface() override;
	const KeywordInterface* getKeywordInterface() const override;
	DirectoryInterface* getDirectoriesInterface() override;
	const DirectoryInterface* getDirectoriesInterface() const override;
	AlbumInterface* getAlbumInterface() override;
	const AlbumInterface* getAlbumInterface() const override;
	PhotoInterface* getPhotoInterface() override;
	const PhotoInterface* getPhotoInterface() const override;
	PhotosAlbumsRelationsInterface* getPhotosAlbumsRelationsInterface() override;
	const PhotosAlbumsRelationsInterface* getPhotosAlbumsRelationsInterface() const override;

	template<class RType>
	InterfaceBase<RType>* getInterface();
	template<class RType>
	const InterfaceBase<RType>* getInterface() const;

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
	std::unordered_map<WindowProperties,int> window_properties;

	//prevent copying and copy construction
	BackendFactory(const BackendFactory &other) = delete;
	BackendFactory(BackendFactory &&other) = delete;
	BackendFactory& operator=(const BackendFactory &other) = delete;
	BackendFactory& operator=(BackendFactory &&other) = delete;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_BACKENDFACTORY_H_ */
