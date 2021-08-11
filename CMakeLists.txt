cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PhotoLibrary)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
add_subdirectory(src/SQLiteAdapter)
set(SQLiteAdapter_INCLUDE_DIRS
	"${CMAKE_SOURCE_DIR}/src/SQLiteAdapter"
	)

add_subdirectory(src/Backend)
set(PhotoLibraryBackend_INCLUDE_DIRS
	"${CMAKE_SOURCE_DIR}/src/Backend"
	)

add_subdirectory(tests)
