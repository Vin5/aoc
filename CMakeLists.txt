cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME aoc)
project(${PROJECT_NAME} C)

if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# specify source directories
set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Check platform features
include(CheckIncludeFiles)

check_include_files(stdbool.h HAVE_STDBOOL)
check_include_files(pthread.h HAVE_PTHREAD)

configure_file(${INCLUDE_DIRS}/config.h.in
               ${INCLUDE_DIRS}/config.h)
if(WIN32)
    add_definitions("/D_WIN32")
endif()
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


###############################################
function(create_test name libs)
    add_executable(${name} "${TESTS_DIR}/${name}.c" "${TESTS_DIR}/tests.c" ${ARGN})
    foreach(lib "${libs}")
        target_link_libraries(${name} ${lib})
    endforeach()
    add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
endfunction()
###############################################

enable_testing()

include_directories(${TESTS_DIR})
create_test(integration_test ${PROJECT_NAME})
create_test(queue_test ${PROJECT_NAME})
create_test(thread_test ${PROJECT_NAME})
create_test(mutex_test ${PROJECT_NAME})
create_test(condition_test ${PROJECT_NAME})
