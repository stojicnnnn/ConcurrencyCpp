add_library(books_lib STATIC books.cpp books.h)
target_include_directories(books_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(books_demo books_demo.cpp)
target_link_libraries(books_demo PRIVATE books_lib)

add_executable(books_test books_test.cpp)
target_link_libraries(books_test PRIVATE books_lib gtest_main pthread)

# To run tests automatically with ctest
add_test(NAME BooksTest COMMAND books_test)
