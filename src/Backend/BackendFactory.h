/*
 * BackendFactory.h
 *
 *  Created on: 3 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_BACKEND_BACKENDFACTORY_H_
#define SRC_BACKEND_BACKENDFACTORY_H_

#include "../Adapter/DatabaseFactory.h"
#include "FactoryBase.h"
#include <unordered_map>

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
		TILE_WIDTH	/**< The width of a tile in the centre frame */
//		, TILE_HEIGHT	/**< The height of a tile in the centre frame */
	};

	/**
	 * @param filename Filename and path of the database to use
	 */
	BackendFactory(const char* filename = nullptr);
	~BackendFactory();

	/**
	 * Get a pointer to the keyword interface of the backend.
	 * @return Pointer to the Keywords interface of the backend.
	 */
	KeywordInterface* getKeywordInterface() override;

	/**
	 * Get a pointer to the keyword interface of the backend.
	 * @return Pointer to the Keywords interface of the backend.
	 */
	const KeywordInterface* getKeywordInterface() const override;

	/**
	 * Get a pointer to the directories interface of the backend.
	 * @return Pointer to the DirectoryInterface of the backend.
	 */
	DirectoryInterface* getDirectoriesInterface() override;

	/**
	 * Get a pointer to the directories interface of the backend.
	 * @return Pointer to the DirectoryInterface of the backend.
	 */
	const DirectoryInterface* getDirectoriesInterface() const override;

	/**
	 * Get a pointer to the albums interface of the backend.
	 * @return Pointer to the AlbumInterface of the backend.
	 */
	AlbumInterface* getAlbumInterface() override;

	/**
	 * Get a pointer to the albums interface of the backend.
	 * @return Pointer to the AlbumInterface of the backend.
	 */
	const AlbumInterface* getAlbumInterface() const override;

	/**
	 * Get a pointer to the photo interface of the backend.
	 * @return Pointer to the PhotoInterface of the backend.
	 */
	PhotoInterface* getPhotoInterface() override;

	/**
	 * Get a pointer to the photo interface of the backend.
	 * @return Pointer to the PhotoInterface of the backend.
	 */
	const PhotoInterface* getPhotoInterface() const override;

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
	 * @retun Width of the central pane
	 */
	int getCentreWidth() const;

private:
	Adapter::DatabaseFactory* db;
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
