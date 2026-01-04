add_library(Layer INTERFACE)

target_include_directories(Layer INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Layer INTERFACE
	gl::Utils
)

add_library(gl::Layer ALIAS Layer)