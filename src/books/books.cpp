#include "books.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_map>

static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::string current;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            result.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    result.push_back(current);
    return result;
}

static std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }
    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

static std::string toLower(const std::string& s) {
    std::string lowerS = s;
    std::transform(lowerS.begin(), lowerS.end(), lowerS.begin(), ::tolower);
    return lowerS;
}

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ParseError("Error: Cannot open file " + filename);
    }
    std::string line;
    if (!std::getline(file, line)) {
        throw ParseError("Error: File is empty or cannot read header");
    }

    auto headers = parseCSVLine(line);
    std::unordered_map<std::string, size_t> header_map;
    for (size_t i = 0; i < headers.size(); ++i) {
        header_map[toLower(trim(headers[i]))] = i;
    }

    int lineNum = 1;
    while (std::getline(file, line)) {
        lineNum++;
        if (trim(line).empty()) continue;

        auto tokens = parseCSVLine(line);
        Book book;
        
        for (const auto& [headerName, index] : header_map) {
            std::string value = index < tokens.size() ? trim(tokens[index]) : "";
            
            if (headerName == "title") book.title = value;
            else if (headerName == "author") book.author = value;
            else if (headerName == "isbn") book.isbn = value;
            else if (headerName == "rating") {
                try { book.rating = value.empty() ? 0.0 : std::stod(value); } 
                catch(...) { book.rating = 0.0; }
            }
            else if (headerName == "pages") {
                try { book.pages = value.empty() ? 0 : std::stoi(value); } 
                catch(...) { book.pages = 0; }
            }
            else if (headerName == "description") {
                book.description = value;
            }
            else {
                book.extra_fields[headerName] = value;
            }
        }
        
        books.push_back(book);
    }
    return books;
}

void printBookSummary(const Book& book) {
    std::cout << "Title: " << book.title << ", Author: " << book.author
              << ", ISBN: " << book.isbn << ", Rating: " << book.rating
              << ", Pages: " << book.pages << std::endl;
}

void printBookDetails(const Book& book) {
    std::cout << "----------------------------------------\n";
    std::cout << "Title:       " << book.title << "\n";
    std::cout << "Author:      " << book.author << "\n";
    std::cout << "ISBN:        " << book.isbn << "\n";
    std::cout << "Rating:      " << book.rating << "\n";
    std::cout << "Pages:       " << book.pages << "\n";
    if (!book.description.empty()) {
        std::cout << "Description: " << book.description << "\n";
    }
    for (const auto& [key, val] : book.extra_fields) {
        if (!val.empty()) {
            std::cout << key << ": " << val << "\n";
        }
    }
    std::cout << "----------------------------------------\n";
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
