/*
 * Concepts.h
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

#ifndef SUPPORT_CONCEPTS_H_
#define SUPPORT_CONCEPTS_H_

#include <concepts>
#include <string>
#include <type_traits>

namespace PhotoLibrary {
namespace SQLiteAdapter {

template<typename T>
concept String_type = std::default_initializable<T> 
		&& std::constructible_from<T,const char*>
		&& std::constructible_from<T,std::string>
		&& requires (T x, std::string y, const char* z) {
			{ x+y } -> std::convertible_to<T>;
			{ y+x } -> std::convertible_to<T>;
			{ x+y } -> std::convertible_to<T>;
			{ z+x } -> std::convertible_to<T>;
		};

template<typename T>
concept Integral_or_enum = std::integral<T>
		|| std::is_enum<T>::value;

} /* namespace SQLiteAdapter */
} /* namespace PhotoLibrary */

#endif /* SUPPORT_CONCEPTS_H_ */
