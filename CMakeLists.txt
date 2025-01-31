cmake_minimum_required(VERSION 3.30)
project(Multiplayer_Game)

set(CMAKE_CXX_STANDARD 20)

add_executable(Multiplayer_Game main.cpp)
if(WIN32)
    target_link_libraries(Multiplayer_Game ws2_32)
endif()
