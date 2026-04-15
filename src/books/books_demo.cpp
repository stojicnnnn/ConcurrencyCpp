#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

struct Book {
    std::string title;
    std::string author;
    std::string isbn;
    double rating;
    int pages;
};

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cant open file " << filename << std::endl;
        return books;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Book book;
        std::getline(ss, book.title, ',');
        std::getline(ss, book.author, ',');
        std::getline(ss, book.isbn, ',');
        std::getline(ss, token, ',');
        book.rating = std::stod(token);
        std::getline(ss, token, ',');
        book.pages = std::stoi(token);
        books.push_back(book);
    }
    return books;
}

void printBook(const Book& book) {
    std::cout << "Title: " << book.title << ", Author: " << book.author
              << ", ISBN: " << book.isbn << ", Rating: " << book.rating
              << ", Pages: " << book.pages << std::endl;
}

void searchByAuthor(const std::vector<Book>& books, const std::string& author) {
    for (const auto& book : books) {
        if (book.author == author) {
            printBook(book);
        }
    }
}

void searchByTitle(const std::vector<Book>& books, const std::string& query) {
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    for (const auto& book : books) {
        std::string lowerTitle = book.title;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            printBook(book);
        }
    }
}

void searchByISBN(const std::vector<Book>& books, const std::string& isbn) {
    for (const auto& book : books) {
        if (book.isbn == isbn) {
            printBook(book);
        }
    }
}

void booksWithRatingAbove(const std::vector<Book>& books, double rating) {
    for (const auto& book : books) {
        if (book.rating > rating) {
            printBook(book);
        }
    }
}

void booksWithPagesBelow(const std::vector<Book>& books, int pages) {
    for (const auto& book : books) {
        if (book.pages < pages) {
            printBook(book);
        }
    }
}

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
            searchByAuthor(books, author);
        } else if (command.substr(0, 12) == "search title ") {
            std::string query = command.substr(12);
            searchByTitle(books, query);
        } else if (command.substr(0, 11) == "search isbn ") {
            std::string isbn = command.substr(11);
            searchByISBN(books, isbn);
        } else if (command.substr(0, 12) == "rating above ") {
            double rating = std::stod(command.substr(12));
            booksWithRatingAbove(books, rating);
        } else if (command.substr(0, 11) == "pages below ") {
            int pages = std::stoi(command.substr(11));
            booksWithPagesBelow(books, pages);
        } else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }
    return 0;
}