#boilerplate
cmake_minimum_required (VERSION 2.8.11)
set(PROJECT_NAME server)

#name of project build
project (${PROJECT_NAME})

#directories with header files to include
include_directories(ext/poco-1.9.0/Foundation/include)
include_directories(ext/poco-1.9.0/JSON/include)
include_directories(ext/poco-1.9.0/Net/include)

#set src directory
set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable( ${PROJECT_NAME} ${LOCAL_SRCS} )

#link libraries by linking 'lib' directory and names of libs (.so files)
LINK_DIRECTORIES(ext/poco-1.9.0/lib/Linux/x86_64)
target_link_libraries( ${PROJECT_NAME} PocoNet PocoUtil PocoJSON PocoXML PocoFoundation )

