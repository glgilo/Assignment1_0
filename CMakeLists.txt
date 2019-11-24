cmake_minimum_required(VERSION 3.15)
project(Assignment1_0)

set(CMAKE_CXX_STANDARD 11)

add_executable(Assignment1_0 src/Main.cpp include/Watchable.h include/Session.h include/Action.h include/User.h src/Action.cpp src/Watchable.cpp src/User.cpp src/Session.cpp include/json.hpp)