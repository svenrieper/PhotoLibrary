add_executable(PhotoLibrary 
			main.cpp 
			examples.cpp
			GUI/AlbumStore.cpp
			GUI/AlbumView.cpp
			GUI/DirectoryStore.cpp
			GUI/DirectoryView.cpp
			GUI/EditKeywordDialogue.cpp
			GUI/GUIMain.cpp
			GUI/KeywordsStore.cpp
			GUI/KeywordsView.cpp
			GUI/MainWindow.cpp
			GUI/NewAlbumDialogue.cpp
			GUI/NewKeywordDialogue.cpp
			GUI/RenameAlbumDialogue.cpp
			GUI/CentrePane.cpp
			GUI/PhotoDrawingArea.cpp
			GUI/PhotoTile.cpp
			GUI/resources/grey_pixel.cc
			)

#target_include_directories(PhotoLibrary PUBLIC
	#"${PROJECT_BINARY_DIR}"
	#)

add_custom_command(TARGET PhotoLibrary POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/src/example pictures/"
		"$<TARGET_FILE_DIR:PhotoLibrary>/example pictures/")

# Check for libraries:
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules (GTKMM  REQUIRED gtkmm-3.0>=3.16)


include_directories(${EXTRA_INCDIR}
        ${GTKMM_INCLUDE_DIRS}
        )
link_directories(${EXTRA_LIBDIR}
        ${GTKMM_LIBRARY_DIRS}
        )

# Add linked libraries dependencies to executables targets
target_link_libraries(PhotoLibrary
	${EXTRA_LIBS}
	PhotoLibraryBackend
	${GTKMM_LIBRARIES}
	Threads::Threads
	)

