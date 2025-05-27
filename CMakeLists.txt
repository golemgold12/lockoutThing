cmake_minimum_required(VERSION 3.15)
project(SecurityCheckServer)

set(CMAKE_CXX_STANDARD 17)

# Define executable
add_executable(lockupManager main.cpp)

# Use vcpkg toolchain (automatically set via command line)

# Find packages
find_package(CURL REQUIRED)
find_package(crow CONFIG REQUIRED)

# Include Crow (header-only)
# If CMake can't find crow::crow, fall back to manual include
target_include_directories(lockupManager PRIVATE 
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-mingw-static/include"
)

# Link libraries
target_link_libraries(lockupManager
    PRIVATE
        CURL::libcurl
        ws2_32
)
target_link_libraries(lockupManager PRIVATE ws2_32 mswsock)
