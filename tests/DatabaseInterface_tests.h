/*
 * DatabaseInterface_tests.h
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

#ifndef TESTS_DATABASEINTERFACE_TESTS_H_
#define TESTS_DATABASEINTERFACE_TESTS_H_

#include "Database.h"
#include <AccessTables.h>
#include <Record.h>
#include <catch2/catch.hpp>

namespace PhotoLibrary {
namespace DatabaseInterface {
namespace DatabaseInterface_tests {

// TwoInts
class TwoIntsPair : public Record<std::pair<int,int>> {
public:
	TwoIntsPair(int v1 ={}, int v2 ={}) : Record<std::pair<int,int>>(v1, v2) {}
	static inline const std::array<const std::string,2> fields { "parent", "value" };
	static inline const std::string table { "TwoInts" };
	static inline const char* create = "PRAGMA foreign_keys = ON;"
		"CREATE TABLE TwoInts( id INTEGER PRIMARY KEY, parent INTEGER, value INTEGER,"
		"FOREIGN KEY (parent) REFERENCES TwoInts ON DELETE CASCADE);"
		//"CREATE INDEX idParentIndex ON TwoInts(parent);"
		"INSERT INTO TwoInts (id, parent, value) VALUES (0, 0, 0);";
};

class TwoIntsArray : public Record<std::array<int,2>> {
public:
	TwoIntsArray(int v1 ={}, int v2 ={}) : Record<std::array<int,2>>(v1, v2) {}
	static inline const std::array<const std::string,2>& fields { TwoIntsPair::fields };
	static inline const std::string& table { TwoIntsPair::table };
};

class TwoIntsTuple : public  Record<std::tuple<int,int>> {
public:
	TwoIntsTuple(int v1 ={}, int v2 ={}) : Record<std::tuple<int,int>>(v1, v2) {}
	static inline const std::array<const std::string,2>& fields { TwoIntsPair::fields };
	static inline const std::string& table { TwoIntsPair::table };
};


//
class FourInts : public Record<std::array<int,4>> {
public:
	using Record<std::array<int,4>>::Record;
	static inline const std::array<const std::string,4> fields { "four", "one", "two", "three" };
	static inline const std::string table { "Integers" };
	static inline const char* create = "CREATE TABLE Integers( id INTEGER PRIMARY KEY, one INTEGER, two INTEGER,"
		"three INTEGER, four INTEGER, FOREIGN KEY (four) REFERENCES Integers ON DELETE CASCADE);";
};

} /* namespace DatabaseInterface_tests */
} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* TESTS_DATABASEINTERFACE_TESTS_H_ */
