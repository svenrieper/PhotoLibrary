add_executable(PLTests EXCLUDE_FROM_ALL
			test.cpp
			AlbumInterface_test.cpp
			DirectoryInterface_test.cpp
			DirectoryRecord_test.cpp
			KeywordInterface_test.cpp
			KeywordRecord_test.cpp
			PhotoInterface_test.cpp
			PhotoRecord_test.cpp
			AlbumRecord_test.cpp
			PhotosAlbumsRelationsInterface_test.cpp
			PhotosKeywordsRelationsInterface_test.cpp
			suppport_test.cpp
			ThreadSafeQueue_tests.cpp
			AccessTables_tests.cpp
			RelationsTable_test.cpp
			)

#target_include_directories(PLTests PUBLIC
	#"${PROJECT_BINARY_DIR}"
	#)

# Check for libraries:
find_package(Catch2)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(GLIBMM REQUIRED glibmm-2.4>=2.64)
pkg_check_modules (CATCH2  catch2)


include_directories(${EXTRA_INCDIR}
	${GLIBMM_INCLUDE_DIRS}
        )
link_directories(${EXTRA_LIBDIR}
	${GLIBMM_LIBRARY_DIRS}
        )

# Add linked libraries dependencies to executables targets
target_link_libraries(PLTests
	PhotoLibraryBackend
	Catch2::Catch2
	${GLIBMM_LIBRARIES}
	Threads::Threads
	)

