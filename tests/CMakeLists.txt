cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PhotoLibrary LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(PhotoLibraryTests 
			test.cpp
			AlbumInterface_test.cpp
			DirectoryInterface_test.cpp
			DirectoryRecord_test.cpp
			KeywordInterface_test.cpp
			KeywordRecord_test.cpp
			PhotoInterface_test.cpp
			PhotoRecord_test.cpp
			PhotosAlbumsRelationsInterface_test.cpp
			PhotosKeywordsRelationsInterface_test.cpp
			suppport_test.cpp
			../src/SQLiteAdapter/Database.cpp
			../src/SQLiteAdapter/SQLQuerry.cpp
			../src/Backend/DatabaseInterface/DatabaseFactory.cpp
			../src/Backend/DatabaseInterface/RelationsDBInterface.cpp
			../src/Backend/DatabaseInterface/suppport.cpp
			../src/Backend/BackendFactory.cpp
			)

target_include_directories(PhotoLibraryTests PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# Check for libraries:
find_package(Catch2 REQUIRED)
find_package(PkgConfig)
find_package(Boost)

pkg_check_modules (SQLITE3 REQUIRED sqlite3)
pkg_check_modules (GTKMM REQUIRED  gtkmm-3.0>=3.16)
pkg_check_modules (CATCH2  REQUIRED catch2)


include_directories(${EXTRA_INCDIR}
        ${GTKMM_INCLUDE_DIRS}
        ${SQLITE3_INCLUDE_DIRS}
        )
link_directories(${EXTRA_LIBDIR}
        ${GTKMM_LIBRARY_DIRS}
        ${SQLITE3_LIBRARY_DIRS}
        )

# Add linked libraries dependencies to executables targets
target_link_libraries(PhotoLibraryTests
	Catch2::Catch2
	${GTKMM_LIBRARIES}
	${SQLITE3_LIBRARIES}
	)

