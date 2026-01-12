cmake_minimum_required(VERSION 3.12)

project("learn" VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
list(APPEND CMAKE_PREFIX_PATH "C:/libs/cmake/glfw")
list(APPEND CMAKE_PREFIX_PATH "C:/libs/cmake/glm")

add_executable(${CMAKE_PROJECT_NAME} ./code/main.cpp)


find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(stb_image REQUIRED)

set(GLM_BUILD_LIBRARY OFF)

add_subdirectory(code/opengl/utils)
add_subdirectory(code/vulkan/context)
add_subdirectory(code/utils/container)
add_subdirectory(code/utils/event_bus)
add_subdirectory(code/utils/logger)
add_subdirectory(code/utils/model_loader)
add_subdirectory(code/utils/resource)
add_subdirectory(code/test)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
	Vulkan::Vulkan
	glfw

	vk::Context

	glm::glm
	stb::stb

	gl::Utils
	utils::Container
	utils::EventBus
	utils::Logger
	utils::ModelLoader
	utils::Resource

	Test
)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
		-static-libgcc
		-static-libstdc++
		-static
	)
#	if(WIN32)
#		if(MSVC)
#			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
#		elseif(MINGW)
#			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
#		endif()
#	endif()
endif()
