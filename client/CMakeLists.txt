#boilerplate
cmake_minimum_required (VERSION 3.2.2)
set(PROJECT_NAME client)

#name of project build
project (${PROJECT_NAME})

#directories with header files to include
#include_directories($ENV{POCO_HOME}/Foundation/include)
#include_directories($ENV{POCO_HOME}/JSON/include)
#include_directories($ENV{POCO_HOME}/Net/include)
# ZMQ includes
include_directories($ENV{CPPZMQ_HOME})

#set src directory
set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable( ${PROJECT_NAME} ${LOCAL_SRCS} )

#link libraries by linking 'lib' directory and names of libs (.so files)
#LINK_DIRECTORIES(ext/poco-1.9.0/lib/Linux/x86_64)
target_link_libraries( ${PROJECT_NAME} zmq )

