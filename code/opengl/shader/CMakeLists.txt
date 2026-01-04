add_library(Shader STATIC)

find_package(glm REQUIRED)

target_include_directories(Shader PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Shader PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderUniform.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderProgram.cpp
)

target_link_libraries(Shader PRIVATE
	glm::glm
)

add_library(gl::Shader ALIAS Shader)