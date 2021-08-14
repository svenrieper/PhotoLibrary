add_library(SQLiteAdapter STATIC
	Database.cpp
	SQLQuerry.cpp)

target_include_directories(SQLiteAdapter
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
	)

find_package(PkgConfig REQUIRED)
pkg_check_modules (SQLITE3 REQUIRED sqlite3)

include_directories(INTERFACE ${SQLITE3_INCLUDE_DIRS})

link_directories(INTERFACE ${SQLITE3_LIBRARY_DIRS})

target_link_libraries(SQLiteAdapter
	INTERFACE ${SQLITE3_LIBRARIES}
	)
