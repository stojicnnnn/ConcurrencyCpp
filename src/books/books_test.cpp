#include <gtest/gtest.h>
#include "books.h"
#include <sstream>

using namespace book_lib;

class BooksTest : public ::testing::Test {
protected:
    std::string createTestCSV() {
        return "Title,Author,ISBN,Rating,Pages,Description,Year\n"
               "The C++ Programming Language,Bjarne Stroustrup,978-0321563842,4.5,1368,The definitive C++ book,2013\n"
               "\"Effective Modern C++, 42 Specific Ways\",Scott Meyers,978-1491903695,4.8,334,\"Great book, highly recommended\",2014\n"
               "Clean Code,Robert C. Martin,978-0132350884,4.7,464,,2008\n";
    }
};

TEST_F(BooksTest, LoadBooksValidFile) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    ASSERT_EQ(books.size(), 3);
    EXPECT_EQ(books[0].title, "The C++ Programming Language");
    EXPECT_EQ(books[0].author, "Bjarne Stroustrup");
    ASSERT_TRUE(books[0].isbn.has_value());
    EXPECT_EQ(books[0].isbn.value(), "978-0321563842");
    EXPECT_DOUBLE_EQ(books[0].rating, 4.5);
    EXPECT_EQ(books[0].pages, 1368);
    EXPECT_EQ(books[0].description, "The definitive C++ book");
    EXPECT_EQ(books[0].extra_fields["year"], "2013");
}

TEST_F(BooksTest, LoadBooksHandlesQuotesAndCommas) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    ASSERT_EQ(books.size(), 3);
    EXPECT_EQ(books[1].title, "Effective Modern C++, 42 Specific Ways");
    EXPECT_EQ(books[1].description, "Great book, highly recommended");
}

TEST_F(BooksTest, LoadBooksEmptyStreamThrows) {
    std::istringstream stream("");
    EXPECT_THROW(loadBooksFromStream(stream), ParseError);
}

TEST_F(BooksTest, SearchByAuthorFoundCaseInsensitive) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = searchByAuthor(books, "scott meyers");
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].title, "Effective Modern C++, 42 Specific Ways");
}

TEST_F(BooksTest, SearchByAuthorNotFound) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = searchByAuthor(books, "Unknown Author");
    EXPECT_TRUE(results.empty());
}

TEST_F(BooksTest, SearchByTitleFoundCaseInsensitive) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = searchByTitle(books, "c++");
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "The C++ Programming Language");
    EXPECT_EQ(results[1].title, "Effective Modern C++, 42 Specific Ways");
}

TEST_F(BooksTest, SearchByISBNFound) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = searchByISBN(books, "978-0132350884");
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].title, "Clean Code");
}

TEST_F(BooksTest, BooksWithRatingAbove) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = booksWithRatingAbove(books, 4.6);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "The C++ Programming Language");
    EXPECT_EQ(results[1].title, "Effective Modern C++, 42 Specific Ways");
}

TEST_F(BooksTest, BooksWithPagesBelow) {
    std::istringstream stream(createTestCSV());
    auto books = loadBooksFromStream(stream);
    auto results = booksWithPagesBelow(books, 500);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "Effective Modern C++, 42 Specific Ways");
    EXPECT_EQ(results[1].title, "Clean Code");
}
