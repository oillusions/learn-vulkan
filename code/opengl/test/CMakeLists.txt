add_library(test INTERFACE)

target_include_directories(test INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(test INTERFACE
)

target_link_libraries(test INTERFACE
	glm::glm
	utils::Container
	utils::Logger
	utils::Resource
)


add_library(gl::test ALIAS test)
