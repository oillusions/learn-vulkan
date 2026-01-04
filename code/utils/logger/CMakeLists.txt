add_library(Logger INTERFACE)

target_include_directories(Logger INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(utils::Logger ALIAS Logger)