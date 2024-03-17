cmake_minimum_required(VERSION 3.20)
project(
	SSSearch
	DESCRIPTION "A tree search algorithm for the Shifting Stones game"
	VERSION 0.1.0
	LANGUAGES C CXX
)

set(CMAKE_REQUIRED_C_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(CMAKE_REQUIRED_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Set the compiler include path
set(PROJ_INCLUDE_PATH include submodules/boost/libs/graph/include/ submodules/boost/libs/property_map/include/ submodules/graphviz/lib/)

set(SRC src/main.cpp src/treeutils.c)

# Add submodules
add_subdirectory(submodules/boost)
add_subdirectory(submodules/graphviz EXCLUDE_FROM_ALL)

# Create the executable
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link objects and libraries
#target_link_directories(${PROJECT_NAME} PUBLIC "build")
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::graph cgraph gvc cdt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
