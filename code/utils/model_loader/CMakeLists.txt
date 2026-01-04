add_library(ModelLoader STATIC)

target_include_directories(ModelLoader PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(ModelLoader PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ModelParser.cpp
)

target_link_libraries(ModelLoader PRIVATE
	gl::Utils
	utils::Logger
)


add_library(utils::ModelLoader ALIAS ModelLoader)