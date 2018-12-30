cmake_minimum_required(VERSION 3.2.2)
project(global)

#cmake single build dir for multiple projects: https://stackoverflow.com/a/34439276
add_subdirectory(server)
add_subdirectory(client)
