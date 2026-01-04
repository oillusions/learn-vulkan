add_library(EventBus INTERFACE)

target_include_directories(EventBus INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(EventBus INTERFACE
)

target_link_libraries(EventBus INTERFACE
)


add_library(utils::EventBus ALIAS EventBus)