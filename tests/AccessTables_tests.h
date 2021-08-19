/*
 * AccessTables_tests.h
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
#include <glibmm/ustring.h>

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
		"CREATE TABLE TwoInts( "
		"  id INTEGER PRIMARY KEY"
		", parent INTEGER"
		", value INTEGER"
		", FOREIGN KEY (parent) REFERENCES TwoInts ON DELETE CASCADE"
		", CHECK( id IS NOT parent OR parent IS 0 )"
		");"
		"CREATE INDEX idParentIndex ON TwoInts(parent);"
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


// Four Strings
template<SQLiteAdapter::String_type S =std::string>
class FourStrings : public Record<std::array<S,4>> {
public:
	FourStrings(const S& s1={}, const S& s2={}, const S& s3={}, const S& s4={}) :
		Record<std::array<S,4>>(s1, s2, s3, s4) {}
	static inline const std::array<const S,4> fields { "four", "one", "two", "three" };
	static inline const S table { "Strings" };
	static inline const char* create = "CREATE TABLE Strings( id INTEGER PRIMARY KEY, one TEXT, two TEXT,"
		"three TEXT, four TEXT, UNIQUE (one, four) );";
};


//Two Tables
namespace TwoTables {

enum TwoTablesEnum {
	E1, E2, E3, E4, E5, E6
};

class TwoTables1 : public Record<std::pair<int,Glib::ustring>> {
public:
	TwoTables1(int parent ={}, const Glib::ustring& value ={}) : Record<std::pair<int,Glib::ustring>>(parent, value) {}
	static inline const std::array<const Glib::ustring,2> fields { "parent", "value" };
	static inline const Glib::ustring table { "Table1" };
};

class TwoTables2 : public Record<std::tuple<int,int_least64_t,long long,short,char,TwoTablesEnum,unsigned long>> {
public:
	TwoTables2(
			int foreignID ={},
			int_least64_t int64 ={},
			long long ll ={},
			short s ={},
			char c ={},
			TwoTablesEnum e ={},
			unsigned long ul ={}
			) :
		Record<std::tuple<int,int_least64_t,long long,short,char,TwoTablesEnum,unsigned long>>
				(foreignID, int64, ll, s, c, e, ul) {}
	static inline const std::array<const Glib::ustring,7> fields
		{ "t1id", "int64", "llong", "short", "char", "enum", "ulong" };
	static inline const Glib::ustring table { "Table2" };
};

inline const char* twoTablesCreate = "PRAGMA foreign_keys = ON;"
	//Table1
		"CREATE TABLE Table1 ("
		"  id		INTEGER PRIMARY KEY"
		", parent	INTEGER"
		", value	TEXT "
		", UNIQUE (value, parent)"
		", FOREIGN KEY (parent) REFERENCES Table1 ON DELETE CASCADE"
		");"
		"CREATE INDEX t1IDParentIndex ON Table1(parent);"
		"INSERT INTO Table1 (id, parent, value) VALUES (0, 0, '');"
	//Table2
		"CREATE TABLE Table2 ("
		"  id		INTEGER PRIMARY KEY"
		", t1id		INTEGER NOT NULL"
		", int64	INTEGER"
		", llong	INTEGER"
		", ulong	INTEGER"
		", short	INTEGER"
		", char		INTEGER"
		", enum		INTEGER"
		", FOREIGN KEY (t1id) REFERENCES Table1 ON DELETE CASCADE"
		", UNIQUE (t1id, int64, enum)"
		");"
		"CREATE INDEX t2ForeignIndex ON Table2(t1id);"
		;
		
} /* namespace TwoTables */

} /* namespace DatabaseInterface_tests */
} /* namespace DatabaseInterface */
} /* namespace PhotoLibrary */

#endif /* TESTS_DATABASEINTERFACE_TESTS_H_ */
