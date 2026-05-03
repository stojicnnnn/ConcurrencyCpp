#include "books.h"
#include <iostream>
#include <string>
#include <vector>

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
    } else {
        for (const auto& book : results) {
            printBook(book);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    auto books = loadBooks(filename);
    if (books.empty()) {
        std::cerr << "No books loaded. Exiting." << std::endl;
        return 1;
    }

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        } else if (command == "help") {
            printHelp();
        } else if (command.substr(0, 13) == "search author ") {
            std::string author = command.substr(13);
            auto results = searchByAuthor(books, author);
            printResults(results);
        } else if (command.substr(0, 12) == "search title ") {
            std::string query = command.substr(12);
            auto results = searchByTitle(books, query);
            printResults(results);
        } else if (command.substr(0, 11) == "search isbn ") {
            std::string isbn = command.substr(11);
            auto results = searchByISBN(books, isbn);
            printResults(results);
        } else if (command.substr(0, 12) == "rating above ") {
            try {
                double rating = std::stod(command.substr(12));
                auto results = booksWithRatingAbove(books, rating);
                printResults(results);
            } catch (...) {
                std::cout << "Invalid rating value." << std::endl;
            }
        } else if (command.substr(0, 11) == "pages below ") {
            try {
                int pages = std::stoi(command.substr(11));
                auto results = booksWithPagesBelow(books, pages);
                printResults(results);
            } catch (...) {
                std::cout << "Invalid pages value." << std::endl;
            }
        } else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }
    return 0;
}