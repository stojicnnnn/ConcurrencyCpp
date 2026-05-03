#include "books.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cant open file " << filename << std::endl;
        return books;
    }
    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Book book;
        std::getline(ss, book.title, ',');
        std::getline(ss, book.author, ',');
        std::getline(ss, book.isbn, ',');
        std::getline(ss, token, ',');
        try { book.rating = std::stod(token); } catch(...) { book.rating = 0.0; }
        std::getline(ss, token, ',');
        try { book.pages = std::stoi(token); } catch(...) { book.pages = 0; }
        books.push_back(book);
    }
    return books;
}

void printBook(const Book& book) {
    std::cout << "Title: " << book.title << ", Author: " << book.author
              << ", ISBN: " << book.isbn << ", Rating: " << book.rating
              << ", Pages: " << book.pages << std::endl;
}

std::vector<Book> searchByAuthor(const std::vector<Book>& books, const std::string& author) {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (book.author == author) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> searchByTitle(const std::vector<Book>& books, const std::string& query) {
    std::vector<Book> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    for (const auto& book : books) {
        std::string lowerTitle = book.title;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> searchByISBN(const std::vector<Book>& books, const std::string& isbn) {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (book.isbn == isbn) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> booksWithRatingAbove(const std::vector<Book>& books, double rating) {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (book.rating > rating) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> booksWithPagesBelow(const std::vector<Book>& books, int pages) {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (book.pages < pages) {
            results.push_back(book);
        }
    }
    return results;
}
