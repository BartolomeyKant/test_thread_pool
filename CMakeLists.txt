cmake_minimum_required(VERSION 3.21)

project(thrdpool_task, CXX)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_GENERATOR} STREQUAL "Ninja" ) # add color output for gcc in ninja
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thread_pool)

add_executable(thrdpool_task
	src/main.cpp
	src/simple_delims.cpp
)

target_include_directories(thrdpool_task PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(thrdpool_task PRIVATE thrdpool)
