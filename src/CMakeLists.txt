# Define a static library target
add_library(assignment1 STATIC organ_transplant.cpp organ_transplant.h)

# Define the include path
target_include_directories(assignment1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define test executable
add_executable(assignment1_test organ_transplant_test.cpp)

# Link the test executable to the library and gtest
target_link_libraries(assignment1_test PRIVATE assignment1 gtest_main pthread)