add_library(PhotoLibraryBackend STATIC
	BackendFactory.cpp
	)

target_include_directories(PhotoLibraryBackend
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
	)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4>=2.64)

include_directories(
	INTERFACE ${GLIBMM_INCLUDE_DIRS}
	INTERFACE ${DatabaseInterface_INCLUDE_DIRS}
	INTERFACE ${SQLiteAdapter_INCLUDE_DIRS}
	)

link_directories(INTERFACE ${GLIBMM_LIBRARY_DIRS})

target_link_libraries(PhotoLibraryBackend
	INTERFACE ${GLIBMM_LIBRARIES}
	DatabaseInterface
	SQLiteAdapter
	)
