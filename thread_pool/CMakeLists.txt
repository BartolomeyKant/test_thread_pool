project(thrdpool CXX)

cmake_minimum_required(VERSION 3.21)

# создаем библиотеку
add_library(
	thrdpool
	STATIC
	thread.cpp
	thread_pool.cpp
)

target_link_libraries(thrdpool
PUBLIC pthread)

# подключаем инклюды
target_include_directories(thrdpool
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
