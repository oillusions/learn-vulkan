add_library(Utils INTERFACE)

target_include_directories(Utils INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Utils INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/Transform.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Bezier.cpp
)

target_link_libraries(Utils INTERFACE
	glm::glm
	utils::Container
	utils::Resource
	utils::Logger
)


add_library(gl::Utils ALIAS Utils)
