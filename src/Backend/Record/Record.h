/*
 * Record.h
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

#include <glibmm/ustring.h>
#include <array>
#include <tuple>

#ifndef SRC_ADAPTER_RECORD_RECORD_H_
#define SRC_ADAPTER_RECORD_RECORD_H_

namespace PhotoLibrary {
namespace Backend {
namespace RecordClasses {

namespace RecordOptions {
#define MACRO_PHOTOLIBRARY_TUPLE_SIZE_LIMIT 6 /**< maximum number of elements for Record|s Tuple parameter */

/**
 * Possible options of a record.
 */
//can't be part of Record since that is a template and Options is needed as part ot the template argument
enum Options {
	NONE = 0, /**< No options are set */
	ROW_EXPANDED = 1, /**< The row is expanded in the GUI::BaseTreeView */
	PRIVATE = 2, /**< For keywords: whether the keyword is private. */
	INCLUDE_ON_EXPORT = 4, /**< For keywords: if not set a keyword can't be included on exported */
	INCLUDE_SYNONYMS_ON_EXPORT = 8, /**< For keywords: if not set the synonyms of a keyword are not exportet */
	ALBUM_IS_SET = 16 /**< For albums: the album is a set */
};
}

/**
 * Base class for database records.
 *
 * @tparam Tuple class to hold the data. Should have the interface
 * of an std::tuple inlcuding std::get and std::tuple_size
 * (e.g. std::tuple, std::pair, or std::array; only tested for std::tuple|s).
 * It works with a tuple size up to six (defined in the
 * MACRO_PHOTOLIBRARY_TUPLE_SIZE_LIMIT macro).
 */
template<typename Tuple>
class Record {
public:
	static_assert(std::tuple_size<Tuple>::value <= MACRO_PHOTOLIBRARY_TUPLE_SIZE_LIMIT);
	using Options = RecordOptions::Options;

	/**
	 * Get the number of data fields in the record.
	 *
	 * @return number of data fields in the record
	 */
	static constexpr int size() noexcept { return std::tuple_size<Tuple>::value; }

	/**
	 * Access a data field.
	 * Returns a reference to the Nth data field.
	 *
	 * @tparam N number of the data field
	 * @return reference to the data
	 */
	template<int N>
	auto& access() noexcept {
		return std::get<N%size()>(values);
	}

	/**
	 * Get data.
	 * Returns the value of the Nth data field.
	 *
	 * @tparam N number of the data field
	 * @return value to the data
	 */
	template<int N>
	const auto& access() const noexcept {
		return std::get<N%size()>(values);
	}

protected:
	/**
	 * @param values the tuple containig the data
	 *
	 * @throws Anything the copy constructors of the
	 * Types arguments of Tuple may throw
	 */
	Record(const Tuple& values) : values(values) {}

	/**
	 * @param values the tuple containig the data
	 *
	 * @throws Anything the copy or move constructors
	 * of the Types arguments of Tuple may throw
	 */
	Record(Tuple&& values) : values(std::move(values)) {}

	/**
	 * @tparam Ts types of the elements of Tuple
	 * @param args initial values to be saved in the Record
	 *
	 * @throws Anything Ts' move or copy constructors may throw
	 */
	template<typename... Ts>
	Record(Ts&&... args) : values{std::forward<Ts>(args)...} {}

	/**
	 * @tparam Ts types of the elements of Tuple
	 * @param args initial values to be saved in the Record
	 *
	 * @throws Anything Ts' copy constructors may throw
	 */
	template<typename... Ts>
	Record(const Ts&... args) : values{args...} {}

private:
	Tuple values;

	template<typename T>
	friend constexpr auto operator<=>(const Record<T>&, const Record<T>&);
	template<typename T>
	friend constexpr bool operator==(const Record<T>&, const Record<T>&);
};

/**
 * Default compare equal operator for Record<Tuple>|s.
 */
template<typename Tuple>
constexpr bool operator==(const Record<Tuple>& a, const Record<Tuple>& b) {
	return a.values == b.values;
}

/**
 * Default three-way comparison operator for Record<Tuple>|s
 */
template<typename Tuple>
constexpr auto operator<=>(const Record<Tuple>& a, const Record<Tuple>& b) {
	return a.values <=> b.values;
}

namespace RecordOptions {
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
}

} /* namespace RecordClasses */
} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_RECORD_RECORD_H_ */
