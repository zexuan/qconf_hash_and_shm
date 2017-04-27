cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRACE -g -ggdb -std=c++11")
set(AGENT_COMPILE_FLAGS "-c -D__STDC_FORMAT_MACROS")
project(qconf-shm)
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/qlibc
    )

aux_source_directory(. DIR_SRCS)
aux_source_directory(qlibc DIR_SRCS)

set_source_files_properties(qlibc/md5.c PROPERTIES LANGUAGE CXX )
set_source_files_properties(qlibc/qhash.c PROPERTIES LANGUAGE CXX )
set_source_files_properties(qlibc/qhasharr.c PROPERTIES LANGUAGE CXX )

add_definitions(${AGENT_COMPILE_FLAGS})
add_executable(qconf_shm ${DIR_SRCS})

# link libpthread
find_package (Threads)
target_link_libraries (qconf_shm ${CMAKE_THREAD_LIBS_INIT})

IF(WIN32)
    MESSAGE(STATUS "This is windows")
ELSEIF(APPLE)
    MESSAGE(STATUS "This is apple")
ELSEIF(UNIX)
    MESSAGE(STATUS "This is unix, and add the librt.so")
    target_link_libraries (qconf_shm rt)
ENDIF(WIN32)

