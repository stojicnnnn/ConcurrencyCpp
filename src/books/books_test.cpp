#include <gtest/gtest.h>
#include "books.h"
#include <fstream>
#include <cstdio>

class BooksTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file("test_books.csv");
        file << "Title,Author,ISBN,Rating,Pages,Description,Year\n";
        file << "The C++ Programming Language,Bjarne Stroustrup,978-0321563842,4.5,1368,The definitive C++ book,2013\n";
        file << "\"Effective Modern C++, 42 Specific Ways\",Scott Meyers,978-1491903695,4.8,334,\"Great book, highly recommended\",2014\n";
        file << "Clean Code,Robert C. Martin,978-0132350884,4.7,464,,2008\n";
        file.close();

        std::ofstream empty("empty.csv");
        empty.close();
    }

    void TearDown() override {
        std::remove("test_books.csv");
        std::remove("empty.csv");
    }
};

TEST_F(BooksTest, LoadBooksValidFile) {
    auto books = loadBooks("test_books.csv");
    ASSERT_EQ(books.size(), 3);
    EXPECT_EQ(books[0].title, "The C++ Programming Language");
    EXPECT_EQ(books[0].author, "Bjarne Stroustrup");
    EXPECT_EQ(books[0].isbn, "978-0321563842");
    EXPECT_DOUBLE_EQ(books[0].rating, 4.5);
    EXPECT_EQ(books[0].pages, 1368);
    EXPECT_EQ(books[0].description, "The definitive C++ book");
    EXPECT_EQ(books[0].extra_fields["year"], "2013");
}

TEST_F(BooksTest, LoadBooksHandlesQuotesAndCommas) {
    auto books = loadBooks("test_books.csv");
    ASSERT_EQ(books.size(), 3);
    EXPECT_EQ(books[1].title, "Effective Modern C++, 42 Specific Ways");
    EXPECT_EQ(books[1].description, "Great book, highly recommended");
}

TEST_F(BooksTest, LoadBooksMissingFileThrows) {
    EXPECT_THROW(loadBooks("non_existent_file.csv"), ParseError);
}

TEST_F(BooksTest, LoadBooksEmptyFileThrows) {
    EXPECT_THROW(loadBooks("empty.csv"), ParseError);
}

TEST_F(BooksTest, SearchByAuthorFound) {
    auto books = loadBooks("test_books.csv");
    auto results = searchByAuthor(books, "Scott Meyers");
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].title, "Effective Modern C++, 42 Specific Ways");
}

TEST_F(BooksTest, SearchByAuthorNotFound) {
    auto books = loadBooks("test_books.csv");
    auto results = searchByAuthor(books, "Unknown Author");
    EXPECT_TRUE(results.empty());
}

TEST_F(BooksTest, SearchByTitleFoundCaseInsensitive) {
    auto books = loadBooks("test_books.csv");
    auto results = searchByTitle(books, "c++");
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "The C++ Programming Language");
    EXPECT_EQ(results[1].title, "Effective Modern C++, 42 Specific Ways");
}

TEST_F(BooksTest, SearchByISBNFound) {
    auto books = loadBooks("test_books.csv");
    auto results = searchByISBN(books, "978-0132350884");
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].title, "Clean Code");
}

TEST_F(BooksTest, BooksWithRatingAbove) {
    auto books = loadBooks("test_books.csv");
    auto results = booksWithRatingAbove(books, 4.6);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "Effective Modern C++, 42 Specific Ways");
    EXPECT_EQ(results[1].title, "Clean Code");
}

TEST_F(BooksTest, BooksWithPagesBelow) {
    auto books = loadBooks("test_books.csv");
    auto results = booksWithPagesBelow(books, 500);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].title, "Effective Modern C++, 42 Specific Ways");
    EXPECT_EQ(results[1].title, "Clean Code");
}
