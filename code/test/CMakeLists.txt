add_library(Test STATIC)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(stb_image REQUIRED)

target_include_directories(Test PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
)

target_link_libraries(Test PRIVATE
	glm::glm
	glfw
	stb::stb
	gl::Utils
	utils::Container
	utils::EventBus
	utils::ModelLoader
	utils::Resource
	utils::Logger
)
