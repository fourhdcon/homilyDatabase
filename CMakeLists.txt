cmake_minimum_required(VERSION 3.15)
project(homilyDatabase)

#[[
		DEFINITIONS
]]

set(CMAKE_CXX_STANDARD 20)

# Debug and Release types of building
if (CMAKE_BUILD_TYPE MATCHES Debug)
	message(" ")
	message("CMAKE IN DEBUG MODE")
	message(" ")
	add_definitions(-DDebug)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
	message(" ")
	message("CMAKE IN RELEASE MODE")
	message(" ")
	add_definitions(-DRelease)
	add_compile_options(-O3)
else ()
	add_definitions(-DRelease)
endif ()

# Replecing SPLITTER and SPLITTERSTRING in source code
add_definitions(-DSPLITTER=<<std::string\(" | "\)<<)
add_definitions(-DSPLITTERSTRING=+" | "+)

# Set headers and sources variables that contains filenames of classes in classes dir
file(GLOB headers ${CMAKE_SOURCE_DIR}/classes/*.h)
file(GLOB sources ${CMAKE_SOURCE_DIR}/classes/*.cpp)

#[[
		LIBRARIES
]]

# Poco::Foundation
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/poco/include)

# SQLiteCpp
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/SQLiteCpp)
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/SQLiteCpp/include)

# restbed
include(ExternalProject)
externalproject_add(restbed
					PREFIX ${CMAKE_BINARY_DIR}/libs/restbed
					SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/libs/restbed
					BINARY_DIR ${CMAKE_BINARY_DIR}/libs/restbed
					INSTALL_COMMAND ""
					CMAKE_CACHE_ARGS -DBUILD_SSL:BOOL=OFF
					-DBUILD_TESTS:BOOL=OFF)
externalproject_get_property(restbed install_dir)
include_directories(${install_dir}/include)

# HTMLEntitiesToUTF8
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/HTMLEntitiesToUTF8)

#[[
		EXECUTABLE
]]

# Add warnings for main source
add_compile_options(-Wall
					-Wextra
					-Wnon-virtual-dtor
					-Wold-style-cast
					-Wcast-align
					-Woverloaded-virtual
					-Wpedantic
					-Wconversion
					-Wnull-dereference
					-Wdouble-promotion
					-Wformat=2
					-Wduplicated-cond
					-Wduplicated-branches
					-Wlogical-op
					-Wuseless-cast
					-Wsign-conversion
					-Wmisleading-indentation)

# Add executable homilyDatabase with source: main.cpp and all files from classes(DIR)
add_executable(homilyDatabase main.cpp ${headers} ${sources})

# Link SQLiteCpp, sqlite3 and pthread to executable
target_link_libraries(homilyDatabase SQLiteCpp sqlite3 pthread)

# Link Poco::Foundation to executable
target_link_libraries(homilyDatabase PocoFoundation)

# Link restbed to executable
add_dependencies(homilyDatabase restbed)
target_link_libraries(homilyDatabase ${CMAKE_BINARY_DIR}/libs/restbed/librestbed.a)

# Link HTMLEntitiesToUTF8 to executable
target_link_libraries(homilyDatabase ${CMAKE_SOURCE_DIR}/libs/HTMLEntitiesToUTF8/libentities.a)

# Set static type of build
target_link_libraries(homilyDatabase -static)

#[[
		OTHER FILES
]]

# Copy all html files from htmls dir to executable folder
file(GLOB PUBLIC_HTMLS "htmls/*.html")
add_custom_command(TARGET homilyDatabase POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${PUBLIC_HTMLS}
				   $<TARGET_FILE_DIR:homilyDatabase>)

# Copy all database files from main folder to executable folder
file(GLOB DATABASE_PUBLIC "*.db")
add_custom_command(TARGET homilyDatabase POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${DATABASE_PUBLIC}
				   $<TARGET_FILE_DIR:homilyDatabase>)