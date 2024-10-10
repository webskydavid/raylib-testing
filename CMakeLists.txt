cmake_minimum_required(VERSION 3.22)
include(FetchContent)

project(game
	VERSION 1.0.0
	DESCRIPTION "Raylib testing"
	LANGUAGES C
)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG master
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)


set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
FetchContent_Declare(
	box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG main
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)



###########################
# Setup
###########################

set(CMAKE_PROJECT_INCLUDE "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_INCLUDE "${PROJECT_SOURCE_DIR}/src")

message("SRC " ${SRC_FILES})

# Executable
add_executable(game 
	"${CMAKE_PROJECT_INCLUDE}/main.c"
	"${CMAKE_PROJECT_INCLUDE}/libs/asteroid.c"
	"${CMAKE_PROJECT_INCLUDE}/libs/ship.c"
	"${CMAKE_PROJECT_INCLUDE}/libs/debug.c"
)

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE box2d)

###########################
# Build
###########################



if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()
