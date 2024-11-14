cmake_minimum_required(VERSION 3.10)
project(OrderHut)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(lib)

file(GLOB SOURCES "src/*.cpp")

add_executable(OrderHut
    ${SOURCES}
)

# Link against pthreads library
target_link_libraries(OrderHut PUBLIC pthread)