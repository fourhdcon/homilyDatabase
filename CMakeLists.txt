cmake_minimum_required(VERSION 3.15)
project(homilyDatabase)

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
endif ()

# Replecing SPLITTER and SPLITTERSTRING in source code
add_definitions(-DSPLITTER=<<std::string\(" | "\)<<)
add_definitions(-DSPLITTERSTRING=+" | "+)

# Include Fundation conmponent of Poco package
find_package(Poco REQUIRED COMPONENTS Foundation)

# Add and include SQLiteCpp package - handling connection with database
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/SQLiteCpp)
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/SQLiteCpp/include)

# Add and include restbed package - serving http
set(BUILD_SSL OFF CACHE BOOL "Build secure socket layer support." FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/restbed)
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/restbed/include)

# Include HTMLEntitiesToUTF8 package - replacing HTML entities in code to UTF-8 characters
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/HTMLEntitiesToUTF8)

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

# Set headers and sources variables that contains filenames of classes in classes dir
file(GLOB headers ${CMAKE_SOURCE_DIR}/classes/*.h)
file(GLOB sources ${CMAKE_SOURCE_DIR}/classes/*.cpp)

# Add executable homilyDatabase with source: main.cpp and all files from classes
add_executable(homilyDatabase main.cpp ${headers} ${sources})

# Link SQLiteCpp, sqlite3, pthread and Fundation from Poco to executable
target_link_libraries(homilyDatabase SQLiteCpp sqlite3 pthread Poco::Foundation)

# Link restbed to executable
target_link_libraries(homilyDatabase ${CMAKE_BINARY_DIR}/libs/restbed/librestbed.a)

# Link HTMLEntitiesToUTF8 to executable
target_link_libraries(homilyDatabase ${CMAKE_SOURCE_DIR}/libs/HTMLEntitiesToUTF8/libentities.a)

# Set static type of build
target_link_libraries(homilyDatabase -static)

# Copy database to executable folder
configure_file(homilyDatabase.db homilyDatabase.db COPYONLY)

# Copy all html files from htmls dir to executable folder
file(GLOB PUBLIC_HTMLS "htmls/*.html")
file(COPY ${PUBLIC_HTMLS} DESTINATION ${CMAKE_BINARY_DIR})