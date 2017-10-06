cmake_minimum_required(VERSION 3.8)
project(rmlibServer C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp connection/ServerPrimary.cpp connection/ServerPrimary.h connection/ClientSocket.cpp connection/ClientSocket.h dataStructures/List.cpp dataStructures/List.h encapsulation/Message.cpp encapsulation/Message.h encapsulation/Encryption.cpp encapsulation/Encryption.h Memory.h grapher/FileUpdater.cpp grapher/FileUpdater.h connection/ServerPrimary.cpp connection/ServerBackup.cpp connection/ServerBackup.h)

add_executable(rmlibServer ${SOURCE_FILES})
set_target_properties(rmlibServer PROPERTIES LINKER_LANGUAGE CXX)
