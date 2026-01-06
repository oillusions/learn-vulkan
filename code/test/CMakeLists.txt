add_library(Test STATIC)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(stb_image REQUIRED)

target_include_directories(Test PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestRender.cpp
)

target_link_libraries(Test PRIVATE
	Vulkan::Vulkan
	glfw

	stb::stb
	glm::glm

	vk::Context

	gl::Utils

	utils::Container
	utils::EventBus
	utils::Logger
	utils::ModelLoader
	utils::Resource
)
