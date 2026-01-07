add_library(Context STATIC)
target_include_directories(Context PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Context PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/VkContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EvkContext.cpp
)

target_link_libraries(Context PRIVATE
	Vulkan::Vulkan
	glfw

	stb::stb
	glm::glm

	gl::Utils

	utils::Logger
	utils::Resource
)

add_library(vk::Context ALIAS Context)