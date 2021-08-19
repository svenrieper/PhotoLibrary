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

#include <tuple>

#ifndef SRC_ADAPTER_RECORD_RECORD_H_
#define SRC_ADAPTER_RECORD_RECORD_H_

namespace PhotoLibrary {
namespace DatabaseInterface {

/**
 * Base class for database records.
 *
 * @tparam Tuple class to hold the data. Should have the interface
 * of an std::tuple inlcuding std::get and std::tuple_size
 * (e.g. std::tuple, std::pair, or std::array).
 */
template<typename Tuple>
class Record {
public:
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
		return std::get<N>(values);
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
		return std::get<N>(values);
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

} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* SRC_ADAPTER_RECORD_RECORD_H_ */
