add_library(DatabaseInterface #STATIC
	INTERFACE
	)

target_include_directories(DatabaseInterface
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
	)

include_directories(INTERFACE ${SQLiteAdapter_INCLUDE_DIRS})

target_link_libraries(DatabaseInterface
	INTERFACE SQLiteAdapter
	)
