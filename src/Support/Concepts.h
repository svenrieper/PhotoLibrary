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

namespace PhotoLibrary {
namespace Support {

template<typename T>
concept String_type = std::default_initializable<T> 
		&& std::constructible_from<T,const char*>
		&& std::constructible_from<T,std::string>;

} /* namespace Support */
} /* namespace PhotoLibrary */

#endif /* SUPPORT_CONCEPTS_H_ */