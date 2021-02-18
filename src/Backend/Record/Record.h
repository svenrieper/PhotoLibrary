/*
 * Record.h
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

#include <array>
#include <glibmm/ustring.h>
#include <tuple>

#ifndef SRC_ADAPTER_RECORD_RECORD_H_
#define SRC_ADAPTER_RECORD_RECORD_H_

namespace PhotoLibrary {
namespace Backend {

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
}

/**
 * Base class for database records.
 *
 * @tparam Tuple class to hold the data. Should have the interface
 * of an std::tuple (inlcuding std::get and std::tuple_size).
 * For constraints see also Adapter::DBInterface
 */
template<typename Tuple>
class Record {
public:
	using Options = RecordOptions::Options;
	virtual ~Record() = default;

	/**
	 * Get the number of data fields in the record.
	 *
	 * @return number of data fields in the record
	 */
	static constexpr int size() { return std::tuple_size<Tuple>::value; }

	/**
	 * Access a data field.
	 * Returns a reference to the Nth data field.
	 *
	 * @tparam N number of the data field
	 * @return reference to the data
	 */
	template<int N>
	auto& access() {
		return std::get<N%std::tuple_size<Tuple>::value>(values);
	}

	/**
	 * Get data.
	 * Returns the value of the Nth data field.
	 *
	 * @tparam N number of the data field
	 * @return value to the data
	 */
	template<int N>
	const auto& access() const {
		return std::get<N%std::tuple_size<Tuple>::value>(values);
	}

protected:
	/**
	 * @param values the tuple containig the data
	 */
	Record(const Tuple& values) : values(values) {}

private:
	Tuple values;
};

/**
 * Default compare equal operator.
 */
template<typename Tuple>
bool operator==(const Record<Tuple>& a, const Record<Tuple>& b) {
	constexpr int i = 6;
	return &a==&b ||
			(a.size() <= i
			&& a.template access<1>() == b.template access<1>()
			&& a.template access<2>() == b.template access<2>()
			&& a.template access<3>() == b.template access<3>()
			&& a.template access<4>() == b.template access<4>()
			&& a.template access<5>() == b.template access<5>()
			&& a.template access<6>() == b.template access<6>()
			);
}

/**
 * Default compare not equal operator.
 */
template<typename Tuple>
bool operator!=(const Record<Tuple>& a, const Record<Tuple>& b) {
	return !(a==b);
}

namespace RecordOptions {
/**
 * Default bitwise and, or, and xor on two Options returns an Options
 */
inline Options& operator^=(Options& a, const Options& b) {
	return (a = static_cast<Options>(a ^ b));
}

/**
 * Default bitwise and, or, and xor on two Options returns an Options
 */
inline Options operator|(Options a, Options b) {
	return static_cast<Options>(static_cast<int>(a)|b);
}
}

} /* namespace Backend */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_RECORD_RECORD_H_ */
