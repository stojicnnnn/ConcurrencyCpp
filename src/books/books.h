#pragma once

#include <string>
#include <vector>

struct Book {
    std::string title;
    std::string author;
    std::string isbn;
    double rating;
    int pages;

    bool operator==(const Book& other) const {
        return title == other.title &&
               author == other.author &&
               isbn == other.isbn &&
               rating == other.rating &&
               pages == other.pages;
    }
};

std::vector<Book> loadBooks(const std::string& filename);
void printBook(const Book& book);
std::vector<Book> searchByAuthor(const std::vector<Book>& books, const std::string& author);
std::vector<Book> searchByTitle(const std::vector<Book>& books, const std::string& query);
std::vector<Book> searchByISBN(const std::vector<Book>& books, const std::string& isbn);
std::vector<Book> booksWithRatingAbove(const std::vector<Book>& books, double rating);
std::vector<Book> booksWithPagesBelow(const std::vector<Book>& books, int pages);
