cmake_minimum_required(VERSION 3.8)
project(
	SSSearch
	DESCRIPTION "A tree search algorithm for the Shifting Stones game"
	VERSION 0.1.0
	LANGUAGES C CXX
)

set(CMAKE_REQUIRED_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_REQUIRED_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB SRC CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")

# Create the engine executable
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

# Link objects and libraries
target_link_directories(${PROJECT_NAME} PUBLIC "build")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
