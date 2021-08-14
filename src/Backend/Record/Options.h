/*
 * Options.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020-2021 Sven Rieper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 a
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SRC_BACKEND_RECORD_OPTIONS_H_
#define SRC_BACKEND_RECORD_OPTIONS_H_

namespace PhotoLibrary {
namespace Backend {
namespace RecordClasses {
namespace RecordOptions {

/**
 * Possible options of a record.
 */
enum Options {
	NONE = 0, /**< No options are set */
	ROW_EXPANDED = 1, /**< The row is expanded in the GUI::BaseTreeView */
	PRIVATE = 2, /**< For keywords: whether the keyword is private. */
	INCLUDE_ON_EXPORT = 4, /**< For keywords: if not set a keyword can't be included on exported */
	INCLUDE_SYNONYMS_ON_EXPORT = 8, /**< For keywords: if not set the synonyms of a keyword are not exportet */
	ALBUM_IS_SET = 16 /**< For albums: the album is a set */
};

/**
 * Default bitwise and, or, and xor on two Options returns an Options
 */
inline Options& operator^=(Options& a, const Options& b) noexcept {
	return (a = static_cast<Options>(a ^ b));
}

/**
 * Default bitwise and, or, and xor on two Options returns an Options
 */
inline constexpr Options operator|(Options a, Options b) noexcept {
	return static_cast<Options>(static_cast<int>(a)|b);
}

} /* namespace RecordOptions */
} /* namespace RecordClasses */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_BACKEND_RECORD_OPTIONS_H_ */
