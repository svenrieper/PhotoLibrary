/*
 * FactoryBase.h
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

#ifndef SRC_BACKEND_FACTORYBASE_H_
#define SRC_BACKEND_FACTORYBASE_H_

#include "InterfaceBase.h"
#include "Record/KeywordRecord.h"
#include "Record/DirectoryRecord.h"
#include "Record/AlbumRecord.h"
#include "Record/PhotoRecord.h"
#include "RelationsInterfaceBase.h"

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
	 * \copydoc getKeywordInterface()
	 */
	virtual const InterfaceBase<KeywordRecord>* getKeywordInterface() const = 0;

	/**
	 * Get a pointer to the directories interface.
	 * @return Pointer to the Directory interface.
	 */
	virtual InterfaceBase<DirectoryRecord>* getDirectoriesInterface() = 0;

	/**
	 * \copydoc getDirectoriesInterface()
	 */
	virtual const InterfaceBase<DirectoryRecord>* getDirectoriesInterface() const = 0;

	/**
	 * Get a pointer to the albums interface.
	 * @return Pointer to the Album interface.
	 */
	virtual InterfaceBase<AlbumRecord>* getAlbumInterface() = 0;

	/**
	 * \copydoc getAlbumInterface()
	 */
	virtual const InterfaceBase<AlbumRecord>* getAlbumInterface() const = 0;

	/**
	 * Get a pointer to the photo interface.
	 * @return Pointer to the Photo interface.
	 */
	virtual InterfaceBase<PhotoRecord>* getPhotoInterface() = 0;

	/**
	 * \copydoc getPhotoInterface()
	 */
	virtual const InterfaceBase<PhotoRecord>* getPhotoInterface() const = 0;

	/**
	 * Get a pointer to the photos-albums-relations interface.
	 * Photos are considered to be the entry-type, albums are considered
	 * to be the collection-type.
	 * @return Pointer to the PhotosAlbumsRelations interface
	 */
	virtual RelationsInterfaceBase* getPhotosAlbumsRelationsInterface() = 0;

	/**
	 * \copydoc getPhotosAlbumsRelationsInterface()
	 */
	virtual const RelationsInterfaceBase* getPhotosAlbumsRelationsInterface() const = 0;

	/**
	 * Get a pointer to the photos-keywords-relations interface.
	 * Photos are considered to be the entry-type, albums are considered
	 * to be the collection-type.
	 * @return Pointer to the PhotosKeywordsRelations interface
	 */
	virtual RelationsInterfaceBase* getPhotosKeywordsRelationsInterface() = 0;

	/**
	 * \copydoc getPhotosAlbumsRelationsInterface()
	 */
	virtual const RelationsInterfaceBase* getPhotosKeywordsRelationsInterface() const = 0;
};

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_FACTORYBASE_H_ */
