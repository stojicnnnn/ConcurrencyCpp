# Define test executable
add_executable(vector_test vector_test.cpp)

# Define the include path so vector_test.cpp can do #include "vector_implementation.hpp"
target_include_directories(vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add AddressSanitizer flags
target_compile_options(vector_test PRIVATE -fsanitize=address -g -O0)
target_link_options(vector_test PRIVATE -fsanitize=address)

# Link the test executable to gtest and pthread
target_link_libraries(vector_test PRIVATE gtest_main pthread)

# Add the test to ctest
add_test(NAME vector_test COMMAND vector_test)
