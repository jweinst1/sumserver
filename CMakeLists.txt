# Project <proj> main cmake file

cmake_minimum_required(VERSION 3.3)

project(proj VERSION 0.0.1)

# Option to install targets to env location
if(DEFINED ENV{SUMSERV_HOME})
     message("-- Will install SUMSERV_HOME binaries and libraries to location: " $ENV{SUMSERV_HOME})
	 set(CMAKE_INSTALL_PREFIX $ENV{SUMSERV_HOME})
else(DEFINED ENV{SUMSERV_HOME})
     message("-- Will install SUMSERV_HOME binaries and libraries to location: " ${CMAKE_CURRENT_BINARY_DIR})
	 set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif(DEFINED ENV{SUMSERV_HOME})


# Disable some windows specific warnings for using more modern c functions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

# Options
option(WITH_testing "Allows tests to be run" ON)

if(WITH_testing MATCHES ON)
   enable_testing()
endif(WITH_testing MATCHES ON)

add_executable(fork_example fork_example.c)
add_executable(sumserver server_main.c)
