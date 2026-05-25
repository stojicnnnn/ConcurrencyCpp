#include "books.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <map>

namespace book_lib {

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

std::vector<Book> loadBooksFromStream(std::istream& input) {
    std::vector<Book> books;
    std::string line;
    if (!std::getline(input, line)) {
        throw ParseError("Error: Stream is empty or cannot read header");
    }

    auto headers = parseCSVLine(line);
    std::map<std::string, size_t> header_map;
    for (size_t i = 0; i < headers.size(); ++i) {
        header_map[toLower(trim(headers[i]))] = i;
    }

    int line_number = 2;
    while (std::getline(input, line)) {
        if (trim(line).empty()) {
            line_number++;
            continue;
        }

        auto tokens = parseCSVLine(line);
        if (tokens.size() != headers.size()) {
            std::cerr << "Warning: Line " << line_number << " has different number of columns. Skipping.\n";
            line_number++;
            continue;
        }

        Book book;
        
        for (const auto& [header_name, index] : header_map) {
            std::string value = trim(tokens[index]);
            
            if (header_name == "title") book.title = value;
            else if (header_name == "author") book.author = value;
            else if (header_name == "isbn") {
                if (!value.empty()) book.isbn = value;
            }
            else if (header_name == "rating") {
                try { book.rating = value.empty() ? 0.0 : std::stod(value); } 
                catch(...) { book.rating = 0.0; }
            }
            else if (header_name == "pages") {
                try { book.pages = value.empty() ? 0 : std::stoi(value); } 
                catch(...) { book.pages = 0; }
            }
            else if (header_name == "description") {
                book.description = value;
            }
            else {
                book.extra_fields[header_name] = value;
            }
        }
        
        if (book.title.empty()) {
            std::cerr << "Warning: Line " << line_number << " has empty title. Skipping.\n";
            line_number++;
            continue;
        }
        
        books.push_back(book);
        line_number++;
    }
    return books;
}

std::vector<Book> loadBooks(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ParseError("Error: Cannot open file " + filename);
    }
    return loadBooksFromStream(file);
}

void printBookSummary(const Book& book) {
    std::cout << "Title: " << book.title << ", Author: " << book.author
              << ", ISBN: " << (book.isbn ? book.isbn.value() : "N/A")
              << ", Rating: " << book.rating
              << ", Pages: " << book.pages << std::endl;
}

void printBookDetails(const Book& book) {
    std::cout << "----------------------------------------\n";
    std::cout << "Title:       " << book.title << "\n";
    std::cout << "Author:      " << book.author << "\n";
    std::cout << "ISBN:        " << (book.isbn ? book.isbn.value() : "N/A") << "\n";
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
    std::string lower_author = toLower(author);
    for (const auto& book : books) {
        if (toLower(book.author) == lower_author) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> searchByTitle(const std::vector<Book>& books, const std::string& query) {
    std::vector<Book> results;
    std::string lower_query = toLower(query);
    for (const auto& book : books) {
        if (toLower(book.title).find(lower_query) != std::string::npos) {
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

}  // namespace book_lib
