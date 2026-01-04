add_library(Container INTERFACE)

target_include_directories(Container INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(Container INTERFACE
)


add_library(utils::Container ALIAS Container)
