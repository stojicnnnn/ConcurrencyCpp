#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <optional>
#include <iostream>

namespace book_lib {

struct Book {
    std::string title;
    std::string author;
    std::optional<std::string> isbn;
    double rating = 0.0;
    int pages = 0;
    std::string description;
    std::map<std::string, std::string> extra_fields;

    bool operator==(const Book& other) const = default;
};

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

std::vector<Book> loadBooks(const std::string& filename);
std::vector<Book> loadBooksFromStream(std::istream& input);
void printBookSummary(const Book& book);
void printBookDetails(const Book& book);
std::vector<Book> searchByAuthor(const std::vector<Book>& books, const std::string& author);
std::vector<Book> searchByTitle(const std::vector<Book>& books, const std::string& query);
std::vector<Book> searchByISBN(const std::vector<Book>& books, const std::string& isbn);
std::vector<Book> booksWithRatingAbove(const std::vector<Book>& books, double rating);
std::vector<Book> booksWithPagesBelow(const std::vector<Book>& books, int pages);

}  // namespace book_lib
