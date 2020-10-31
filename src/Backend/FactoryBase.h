/*
 * FactoryBase.h
 *
 *  Created on: 26 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_BACKEND_FACTORYBASE_H_
#define SRC_BACKEND_FACTORYBASE_H_

#include "InterfaceBase.h"
#include "Record/KeywordRecord.h"
#include "Record/DirectoryRecord.h"
#include "Record/AlbumRecord.h"
#include "Record/PhotoRecord.h"

namespace PhotoLibrary {
namespace Backend {

/**
 * Abstract base class for the backend and database factory classes.
 */
class FactoryBase {
public:
	FactoryBase() = default;
	virtual ~FactoryBase() = default;

	/**
	 * Get a pointer to the keyword interface.
	 * @return Pointer to the Keywords interface.
	 */
	virtual InterfaceBase<KeywordRecord>* getKeywordInterface() = 0;

	/**
	 * Get a pointer to the keyword interface.
	 * @return Pointer to the Keywords interface.
	 */
	virtual const InterfaceBase<KeywordRecord>* getKeywordInterface() const = 0;

	/**
	 * Get a pointer to the directories interface.
	 * @return Pointer to the DirectoryInterface.
	 */
	virtual InterfaceBase<DirectoryRecord>* getDirectoriesInterface() = 0;

	/**
	 * Get a pointer to the directories interface.
	 * @return Pointer to the DirectoryInterface.
	 */
	virtual const InterfaceBase<DirectoryRecord>* getDirectoriesInterface() const = 0;

	/**
	 * Get a pointer to the albums interface.
	 * @return Pointer to the AlbumInterface.
	 */
	virtual InterfaceBase<AlbumRecord>* getAlbumInterface() = 0;

	/**
	 * Get a pointer to the albums interface.
	 * @return Pointer to the AlbumInterface.
	 */
	virtual const InterfaceBase<AlbumRecord>* getAlbumInterface() const = 0;

	/**
	 * Get a pointer to the photo interface.
	 * @return Pointer to the PhotoInterface.
	 */
	virtual InterfaceBase<PhotoRecord>* getPhotoInterface() = 0;

	/**
	 * Get a pointer to the photo interface.
	 * @return Pointer to the PhotoInterface.
	 */
	virtual const InterfaceBase<PhotoRecord>* getPhotoInterface() const = 0;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_FACTORYBASE_H_ */
