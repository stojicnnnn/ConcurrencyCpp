cmake_minimum_required(VERSION 3.14)
project(OrganTransplantWaitingList)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for thread sanitizer
add_compile_options(-fsanitize=thread -g -O1)
add_link_options(-fsanitize=thread)

# Download and include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add subdirectory
add_subdirectory(src)