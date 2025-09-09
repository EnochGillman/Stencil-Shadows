cmake_minimum_required(VERSION 3.30)
project(FirstSDL3Use VERSION 1.0.0)

# SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
#Location that stores my dependency checks
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#Make the compiler paranoid ;p
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++23 -Wall -Wpedantic -Werror -Wextra -march=native")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -O3 -Wall -Wpedantic -Werror -Wextra -march=native")

#Find dependencies
find_package(GLM REQUIRED)
find_package(SDL3 REQUIRED)

#Include source files throughout the project
file(GLOB primary_sources "Scripts/*.cpp" "Scripts/*.c")

#Allow my code to know where it lives
ADD_COMPILE_DEFINITIONS("SOURCE_ROOT=${CMAKE_SOURCE_DIR}/")

#Main program
ADD_EXECUTABLE(program ${primary_sources})

#All directories where we may find header files
TARGET_INCLUDE_DIRECTORIES(program PUBLIC
	${GLM_INCLUDE_DIRS}# ${SDL2_INCLUDE_DIRS}
)
TARGET_INCLUDE_DIRECTORIES(program PUBLIC Headers)
TARGET_INCLUDE_DIRECTORIES(program PUBLIC Include)

#Libraries that the program needs
TARGET_LINK_LIBRARIES(program PUBLIC SDL3::SDL3)
