cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME aoc)
project(${PROJECT_NAME} C)

if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE "Debug")
endif()



set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(UNIX) 
    set(LIBS pthread)
endif()

file(GLOB INCLUDES
     ${INCLUDE_DIRS}/*h)
file(GLOB SOURCES
     ${SOURCE_DIR}/*.c)


include_directories(${INCLUDE_DIRS})
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${INCLUDES})

if(LIBS) 
    target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()