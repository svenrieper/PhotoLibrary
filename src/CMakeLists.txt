cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PhotoLibrary)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(PhotoLibrary 
			main.cpp 
			examples.cpp
			Adapter/Database.cpp
			Adapter/DatabaseFactory.cpp
			Adapter/SQLQuerry.cpp
			Adapter/suppport.cpp
			Backend/BackendFactory.cpp
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
			)

#configure_file(CmakeConfig.h.in CmakeConfig.h)

target_include_directories(PhotoLibrary PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# Check for libraries:
find_package(PkgConfig)

pkg_check_modules (SQLITE3  sqlite3)
pkg_check_modules (GTKMM   gtkmm-3.0>=3.16)


include_directories(${EXTRA_INCDIR}
        ${GTKMM_INCLUDE_DIRS}
        ${SQLITE3_INCLUDE_DIRS}
        )
link_directories(${EXTRA_LIBDIR}
        ${GTKMM_LIBRARY_DIRS}
        ${SQLITE3_LIBRARY_DIRS}
        )

# Add linked libraries dependencies to executables targets
target_link_libraries(PhotoLibrary
    ${GTKMM_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    )

