cmake_minimum_required(VERSION 3.8)
project(rmlibServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp connection/ServerSocket.cpp connection/ServerSocket.h connection/ClientSocket.cpp connection/ClientSocket.h dataStructures/List.cpp dataStructures/List.h encapsulation/Message.cpp encapsulation/Message.h encapsulation/Encryption.cpp encapsulation/Encryption.h Memory.h grapher/FileUpdater.cpp grapher/FileUpdater.h)
add_executable(rmlibServer ${SOURCE_FILES})

