cmake_minimum_required(VERSION 3.20)
project(base_code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

add_executable(base_code
        include/Action.h
        include/Customer.h
        include/Studio.h
        include/Trainer.h
        include/Workout.h
        src/Action.cpp
        src/main.cpp
        src/Studio.cpp
        src/Trainer.cpp
        src/Workout.cpp
        src/Customer.cpp
        )
