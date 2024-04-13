cmake_minimum_required(VERSION 3.25)
project(ConsoleCardManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(ConsoleCardManager ConsoleCardManager.cpp Card.h handlers.h interface.h card_manager.h card_manager.cpp handlers.cpp interface.cpp)
