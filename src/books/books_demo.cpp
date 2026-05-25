#include "books.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace book_lib;

void printHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "search author <author_name>" << std::endl;
    std::cout << "search title <title_query>" << std::endl;
    std::cout << "search isbn <isbn>" << std::endl;
    std::cout << "rating above <rating>" << std::endl;
    std::cout << "pages below <pages>" << std::endl;
    std::cout << "help" << std::endl;
    std::cout << "exit" << std::endl;
}

void printResults(const std::vector<Book>& results) {
    if (results.empty()) {
        std::cout << "No books found." << std::endl;
    } else if (results.size() == 1) {
        printBookDetails(results[0]);
    } else {
        std::cout << "Found " << results.size() << " books:\n";
        for (const auto& book : results) {
            printBookSummary(book);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::vector<Book> books;
    
    try {
        books = loadBooks(filename);
    } catch (const ParseError& e) {
        std::cerr << "Failed to load books: " << e.what() << std::endl;
        return 1;
    }

    if (books.empty()) {
        std::cerr << "Error: No books loaded from file." << std::endl;
        return 1;
    }

    std::string command;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, command)) break;

        std::string cleaned_command;
        bool in_space = false;
        for (char c : command) {
            if (std::isspace(c)) {
                if (!in_space && !cleaned_command.empty()) {
                    cleaned_command += ' ';
                    in_space = true;
                }
            } else {
                cleaned_command += c;
                in_space = false;
            }
        }
        if (!cleaned_command.empty() && cleaned_command.back() == ' ') {
            cleaned_command.pop_back();
        }

        if (cleaned_command == "exit") {
            break;
        } else if (cleaned_command == "help") {
            printHelp();
        } else if (cleaned_command.find("search author ") == 0) {
            std::string author = cleaned_command.substr(14);
            auto results = searchByAuthor(books, author);
            printResults(results);
        } else if (cleaned_command.find("search title ") == 0) {
            std::string query = cleaned_command.substr(13);
            auto results = searchByTitle(books, query);
            printResults(results);
        } else if (cleaned_command.find("search isbn ") == 0) {
            std::string isbn = cleaned_command.substr(12);
            auto results = searchByISBN(books, isbn);
            printResults(results);
        } else if (cleaned_command.find("rating above ") == 0) {
            try {
                double rating = std::stod(cleaned_command.substr(13));
                auto results = booksWithRatingAbove(books, rating);
                printResults(results);
            } catch (...) {
                std::cout << "Invalid rating value." << std::endl;
            }
        } else if (cleaned_command.find("pages below ") == 0) {
            try {
                int pages = std::stoi(cleaned_command.substr(12));
                auto results = booksWithPagesBelow(books, pages);
                printResults(results);
            } catch (...) {
                std::cout << "Invalid pages value." << std::endl;
            }
        } else if (!cleaned_command.empty()) {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }
    return 0;
}
