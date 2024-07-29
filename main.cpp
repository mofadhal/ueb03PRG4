
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <memory>
/*
 *
 *@author Mofadhal Al-Manari
 *@version 29.07.2024
 *
 */
// Forward declarations
class Publication;
class Book;
class Magazine;
class Author;
class Shelf;
class BookShelf;
class MagazineShelf;
class Customer;
class Library;

/**
 * @brief Generic Stack class template.
 *
 * This class implements a generic stack using a vector for storage.
 *
 * @tparam T The type of elements stored in the stack.
 */
template <typename T>
class Stack {
private:
    std::vector<T> data;

public:
    /**
   * @brief Push an item onto the stack.
   *
   * @param item The item to be pushed onto the stack.
   */
    void push(const T& item) {
        data.push_back(item);
    }
    /**
     * @brief Pop an item from the stack.
     *
     * @throws std::runtime_error if the stack is empty.
     */
    void pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        data.pop_back();
    }
    /**
     * @brief Get the top item of the stack.
     *
     * @return The top item of the stack.
     * @throws std::runtime_error if the stack is empty.
     */
    T top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
     /**
     * @brief Check if the stack is empty.
     *
     * @return True if the stack is empty, otherwise false.
     */
    bool isEmpty() const {
        return data.empty();
    }
    /**
     * @brief Get the size of the stack.
     *
     * @return The number of items in the stack.
     */
    /*int size() const {
        return data.size();
    }*/
};
/**
 * @brief Class representing an author.
 */
class Author {
public:
    std::string firstName;
    std::string lastName;

    /**
     * @brief Construct a new Author object.
     *
     * @param first The first name of the author.
     * @param last The last name of the author.
     */
    Author(const std::string& first, const std::string& last) : firstName(first), lastName(last) {}
        /**
     * @brief Get the full name of the author.
     *
     * @return The full name of the author.
     */
    std::string getFullName() const { // delet
        return firstName + " " + lastName;
    }
};
/**
 * @class Publication
 * @brief Base class for all publications in the library.
 */
class Publication {
public:
    int id;
    std::string title;
    int yearOfPublication;
    int totalCopies;
    int availableCopies;
     /**
     * @brief Constructs a new Publication object.
     * @param id Unique identifier for the publication.
     * @param title Title of the publication.
     * @param year Year of publication.
     * @param total Total number of copies.
     * @param available Number of available copies.
     */
    Publication(int id, const std::string& title, int year, int total, int available)
        : id(id), title(title), yearOfPublication(year), totalCopies(total), availableCopies(available) {}
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~Publication() = default;

};
    /**
     * @class Book
     * @brief Represents a book in the library, derived from Publication.
     */
class Book : public Publication {
public:
    Author author;
    int pageCount;
    /**
       * @brief Constructs a new Book object.
       * @param id Unique identifier for the book.
       * @param title Title of the book.
       * @param author Author of the book.
       * @param year Year of publication.
       * @param pages Number of pages in the book.
       * @param total Total number of copies.
       * @param available Number of available copies.
       */
    Book(int id, const std::string& title, const Author& author, int year, int pages, int total, int available)
        : Publication(id, title, year, total, available), author(author), pageCount(pages) {}

};
    /**
     * @class Magazine
     * @brief Represents a magazine in the library, derived from Publication.
     */
class Magazine : public Publication {
public:
    int issueNumber;
    /**
    * @brief Constructs a new Magazine object.
    * @param id Unique identifier for the magazine.
    * @param title Title of the magazine.
    * @param year Year of publication.
    * @param issue Issue number of the magazine.
    * @param total Total number of copies.
    * @param available Number of available copies.
    */
    Magazine(int id, const std::string& title, int year, int issue, int total, int available)
        : Publication(id, title, year, total, available), issueNumber(issue) {}

};
    /**
     * @class Shelf
     * @brief Abstract base class for shelves in the library.
     */
class Shelf {
protected:
    int maxCapacity;
    int floor;

public:
    /**
     * @brief Constructs a new Shelf object.
     * @param capacity Maximum capacity of the shelf.
     * @param floorNumber Floor number where the shelf is located.
     */
    Shelf(int capacity, int floorNumber) : maxCapacity(capacity), floor(floorNumber) {}
        //----------------------- start delet
    virtual ~Shelf() = default;

    virtual void addPublication(std::shared_ptr<Publication> publication) = 0;
    virtual void removePublication(int id) = 0;
    virtual std::shared_ptr<Publication> borrowPublication(int id) = 0;
    virtual void returnPublication(std::shared_ptr<Publication> publication) = 0;
    virtual void addExemplar(int id) = 0;
    //------------------- ende delet
};
    /**
     * @class BookShelf
     * @brief Represents a shelf for books in the library.
     *  @throws std::runtime_error if the publication is not of type Book.
     */
class BookShelf : public Shelf {
private: //------------- start delet
    std::map<std::string, std::list<std::shared_ptr<Book>>> books;
        //-------------- ende delet
public:
     /**
     * @brief Constructs a new BookShelf object.
     * @param capacity Maximum capacity of the book shelf.
     * @param floorNumber Floor number where the book shelf is located.
     */
    BookShelf(int capacity, int floorNumber) : Shelf(capacity, floorNumber) {}
    /**
     * @brief Adds a publication to the book shelf.
     * @param publication Shared pointer to the publication to be added.
     * @throw std::runtime_error if the publication is not a Book.
     */
    void addPublication(std::shared_ptr<Publication> publication) override {
        auto book = std::dynamic_pointer_cast<Book>(publication);
        if (!book) {
            throw std::runtime_error("Can only add books to BookShelf");
        }
        std::string authorName = book->author.getFullName();
        auto& authorBooks = books[authorName];
        authorBooks.push_back(book);
        authorBooks.sort([](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b) {
            return a->title < b->title;
        });
    }
    /**
     * @brief Removes a book from the shelf by its ID.
     *
     * @param id The ID of the book to be removed.
     */
    void removePublication(int id) override {
        for (auto& [author, authorBooks] : books) {
            authorBooks.remove_if([id](const std::shared_ptr<Book>& book) { return book->id == id; });
        }
    }
    /**
     * @brief Borrows a book from the shelf by its ID, decreasing the available copy count.
     *
     * @param id The ID of the book to be borrowed.
     * @return std::shared_ptr<Publication> A shared pointer to the borrowed book.
     * @throws std::runtime_error if the book is not found or no copies are available.
     */
    std::shared_ptr<Publication> borrowPublication(int id) override {
        for (auto& [author, authorBooks] : books) {
            for (auto& book : authorBooks) {
                if (book->id == id && book->availableCopies > 0) {
                    book->availableCopies--;
                    return book;
                }
            }
        }
        throw std::runtime_error("Book not found or not available");
    }
    /**
     * @brief Returns a borrowed book to the shelf, increasing the available copy count.
     * @param publication A shared pointer to the book being returned. Must be of type Book.
     * @throws std::runtime_error if the publication is not of type Book.
     */
    void returnPublication(std::shared_ptr<Publication> publication) override {
        auto book = std::dynamic_pointer_cast<Book>(publication);
        if (!book) {
            throw std::runtime_error("Can only return books to BookShelf");
        }
        for (auto& [author, authorBooks] : books) {
            for (auto& existingBook : authorBooks) {
                if (existingBook->id == book->id) {
                    existingBook->availableCopies++;
                    return;
                }
            }
        }
        throw std::runtime_error("Book not found in shelf");
    }
        /**
         * @brief Adds an additional copy (exemplar) of a book by its ID.
         *
         * @param id The ID of the book to which a copy is to be added.
         * @throws std::runtime_error if the book is not found.
         */
    void addExemplar(int id) override {
        for (auto& [author, authorBooks] : books) {
            for (auto& book : authorBooks) {
                if (book->id == id) {
                    book->totalCopies++;
                    book->availableCopies++;
                    return;
                }
            }
        }
        throw std::runtime_error("Book not found");
    }
    /**
     * @brief Retrieves all books from a specific author.
     *
     * @param authorName The name of the author.
     * @return std::vector<std::shared_ptr<Book>> A vector of shared pointers to the books by the author.
     */
    /*std::vector<std::shared_ptr<Book>> getBooksFromAuthor(const std::string& authorName) const {
        auto it = books.find(authorName);
        if (it != books.end()) {
            return std::vector<std::shared_ptr<Book>>(it->second.begin(), it->second.end());
        }
        return {};
    }*/
    /**
     * @brief Retrieves all available books from a specific author.
     *
     * @param authorName The name of the author.
     * @return std::vector<std::shared_ptr<Book>> A vector of shared pointers to the available books by the author.
     */
    /*std::vector<std::shared_ptr<Book>> getAvailableBooksFromAuthor(const std::string& authorName) const {
        std::vector<std::shared_ptr<Book>> availableBooks;
        auto it = books.find(authorName);
        if (it != books.end()) {
            for (const auto& book : it->second) {
                if (book->availableCopies > 0) {
                    availableBooks.push_back(book);
                }
            }
        }
        return availableBooks;
    }*/
    /**
    * @brief Retrieves all available books on the shelf.
    *
    * @return std::vector<std::shared_ptr<Book>> A vector of shared pointers to all available books.
    */
    /*std::vector<std::shared_ptr<Book>> getAllAvailableBooks() const {
        std::vector<std::shared_ptr<Book>> availableBooks;
        for (const auto& [author, authorBooks] : books) {
            for (const auto& book : authorBooks) {
                if (book->availableCopies > 0) {
                    availableBooks.push_back(book);
                }
            }
        }
        return availableBooks;
    }*/
};

class MagazineShelf : public Shelf {
private:
    std::map<std::string, std::vector<std::shared_ptr<Magazine>>> magazines;
    /**
    * @brief Constructs a MagazineShelf with a given capacity and floor number.
    *
    * @param capacity The capacity of the shelf.
    * @param floorNumber The floor number where the shelf is located.
    */
public:
    MagazineShelf(int capacity, int floorNumber) : Shelf(capacity, floorNumber) {}
    /**
     * @brief Adds a magazine to the shelf.
     *
     * @param publication A shared pointer to the magazine to be added. Must be of type Magazine.
     * @throws std::runtime_error if the publication is not of type Magazine.
     */
    void addPublication(std::shared_ptr<Publication> publication) override {
        auto magazine = std::dynamic_pointer_cast<Magazine>(publication);
        if (!magazine) {
            throw std::runtime_error("Can only add magazines to MagazineShelf");
        }
        auto& titleMagazines = magazines[magazine->title];
        titleMagazines.push_back(magazine);
        std::sort(titleMagazines.begin(), titleMagazines.end(),
            [](const std::shared_ptr<Magazine>& a, const std::shared_ptr<Magazine>& b) {
                return std::tie(a->yearOfPublication, a->issueNumber) < std::tie(b->yearOfPublication, b->issueNumber);
            });
    }
    /**
     * @brief Removes a magazine from the shelf by its ID.
     *
     * @param id The ID of the magazine to be removed.
     */
    void removePublication(int id) override {
        for (auto& [title, titleMagazines] : magazines) {
            titleMagazines.erase(
                std::remove_if(titleMagazines.begin(), titleMagazines.end(),
                    [id](const std::shared_ptr<Magazine>& magazine) { return magazine->id == id; }),
                titleMagazines.end());
        }
    }
    /**
     * @brief Borrows a magazine from the shelf by its ID, decreasing the available copy count.
     *
     * @param id The ID of the magazine to be borrowed.
     * @return std::shared_ptr<Publication> A shared pointer to the borrowed magazine.
     * @throws std::runtime_error if the magazine is not found or no copies are available.
     */
    std::shared_ptr<Publication> borrowPublication(int id) override {
        for (auto& [title, titleMagazines] : magazines) {
            for (auto& magazine : titleMagazines) {
                if (magazine->id == id && magazine->availableCopies > 0) {
                    magazine->availableCopies--;
                    return magazine;
                }
            }
        }
        throw std::runtime_error("Magazine not found or not available");
    }
         /**
         * @brief Returns a borrowed magazine to the shelf, increasing the available copy count.
         *
         * @param publication A shared pointer to the magazine being returned. Must be of type Magazine.
         * @throws std::runtime_error if the publication is not of type Magazine.
         */
    void returnPublication(std::shared_ptr<Publication> publication) override {
        auto magazine = std::dynamic_pointer_cast<Magazine>(publication);
        if (!magazine) {
            throw std::runtime_error("Can only return magazines to MagazineShelf");
        }
        auto& titleMagazines = magazines[magazine->title];
        for (auto& existingMagazine : titleMagazines) {
            if (existingMagazine->id == magazine->id) {
                existingMagazine->availableCopies++;
                return;
            }
        }
        throw std::runtime_error("Magazine not found in shelf");
    }
    /**
    * @brief Adds an additional copy (exemplar) of a magazine by its ID.
    *
    * @param id The ID of the magazine to which a copy is to be added.
    * @throws std::runtime_error if the magazine is not found.
    */
    void addExemplar(int id) override {
        for (auto& [title, titleMagazines] : magazines) {
            for (auto& magazine : titleMagazines) {
                if (magazine->id == id) {
                    magazine->totalCopies++;
                    magazine->availableCopies++;
                    return;
                }
            }
        }
        throw std::runtime_error("Magazine not found");
    }
    /**
     * @brief Retrieves all magazines with a specific title.
     *
     * @param title The title of the magazines.
     * @return std::vector<std::shared_ptr<Magazine>> A vector of shared pointers to the magazines with the specified title.
     */
    /*std::vector<std::shared_ptr<Magazine>> getMagazinesWithTitle(const std::string& title) const {
        auto it = magazines.find(title);
        if (it != magazines.end()) {
            return it->second;
        }
        return {};
    }*/
    /**
     * @brief Retrieves all available magazines with a specific title and publication year.
     *
     * @param title The title of the magazines.
     * @param year The year of publication.
     * @return std::vector<std::shared_ptr<Magazine>> A vector of shared pointers to the available magazines with the specified title and year.
     */
    /*std::vector<std::shared_ptr<Magazine>> getAvailableMagazinesWithTitleAndYear(const std::string& title, int year) const {
        std::vector<std::shared_ptr<Magazine>> availableMagazines;
        auto it = magazines.find(title);
        if (it != magazines.end()) {
            for (const auto& magazine : it->second) {
                if (magazine->yearOfPublication == year && magazine->availableCopies > 0) {
                    availableMagazines.push_back(magazine);
                }
            }
        }
        return availableMagazines;
    }*/
};
/**
 * @class Customer
 * @brief Represents a customer of the library.
 */
class Customer {
public:
    int id;
    std::string firstName;
    std::string lastName;
    std::vector<std::shared_ptr<Publication>> borrowedPublications;
     /**
      * @brief Constructs a new Customer object.
      * @param id Unique identifier for the customer.
      * @param first First name of the customer.
      * @param last Last name of the customer.
      */
    Customer(int id, const std::string& first, const std::string& last)
        : id(id), firstName(first), lastName(last) {}
     /**
     * @brief Borrows a publication for the customer.
     * @param publication Shared pointer to the publication to be borrowed.
     * @throw std::runtime_error if the customer already has a publication with the same title.
     */
    void borrowPublication(std::shared_ptr<Publication> publication) {
        if (std::find_if(borrowedPublications.begin(), borrowedPublications.end(),
            [&publication](const std::shared_ptr<Publication>& p) { return p->title == publication->title; })
            == borrowedPublications.end()) {
            borrowedPublications.push_back(publication);
        } else {
            throw std::runtime_error("Customer already has a publication with this title");
        }
    }
    /**
     * @brief Returns a borrowed publication.
     * @param id Identifier of the publication to be returned.
     * @throw std::runtime_error if the publication is not found in the customer's borrowed list.
     */
    void returnPublication(int id) {
        auto it = std::find_if(borrowedPublications.begin(), borrowedPublications.end(),
            [id](const std::shared_ptr<Publication>& p) { return p->id == id; });
        if (it != borrowedPublications.end()) {
            borrowedPublications.erase(it);
        } else {
            throw std::runtime_error("Publication not found in customer's borrowed list");
        }
    }
};
/**
 * @class Library
 * @brief Manages the overall library system.
 */
class Library {
private:
  //  std::vector<std::shared_ptr<Shelf>> shelves;


public:
    // ... (existing methods)
    std::vector<std::shared_ptr<Customer>> customers;
    Stack<std::shared_ptr<Publication>> returnedPublications;
    std::vector<std::shared_ptr<Book>> books;
    std::vector<std::shared_ptr<Shelf>> shelves;
    std::vector<std::shared_ptr<Magazine>> magazines;

    const std::vector<std::shared_ptr<Customer>>& getCustomers() const {
        return customers;
    }
    /**
     * @brief Adds a new book to the library.
     * @param book Shared pointer to the book to be added.
     */
    void addBook(std::shared_ptr<Book> book) {
        books.push_back(book);
        for (auto& shelf : shelves) {
            if (auto bookShelf = std::dynamic_pointer_cast<BookShelf>(shelf)) {
                bookShelf->addPublication(book);
                //books.push_back(book);
                return;
            }
        }
        //throw std::runtime_error("No BookShelf found in the library");
    }

    const std::vector<std::shared_ptr<Book>>& getBooks() const {
        return books;
    }
        /**
     * @brief Allows a customer to borrow a book.
     * @param customerId ID of the customer borrowing the book.
     * @param bookId ID of the book to be borrowed.
     * @throw std::runtime_error if the customer or book is not found, or if no copies are available.
     */
    void borrowBook(int customerId, int bookId) {
        auto customer = findCustomer(customerId);
        if (!customer) {
            throw std::runtime_error("Customer not found");
        }

        auto book = findBook(bookId);
        if (!book) {
            throw std::runtime_error("Book not found");
        }

        if (book->availableCopies == 0) {
            throw std::runtime_error("No available copies of this book");
        }

        book->availableCopies--;
        customer->borrowPublication(book);
    }
    /**
     * @brief Processes the return of a book by a customer.
     * @param customerId ID of the customer returning the book.
     * @param bookId ID of the book being returned.
     * @throw std::runtime_error if the customer or book is not found.
     */
    void returnBook(int customerId, int bookId) {
        auto customer = findCustomer(customerId);
        if (!customer) {
            throw std::runtime_error("Customer not found");
        }

        auto book = findBook(bookId);
        if (!book) {
            throw std::runtime_error("Book not found");
        }

        customer->returnPublication(bookId);
        returnedPublications.push(book);
    }
    /**
     * @brief Retrieves a list of returned books.
     *
     * @return std::vector<std::shared_ptr<Publication>> A vector of shared pointers to returned books.
     */
    std::vector<std::shared_ptr<Publication>> getReturnedBooks() const {
        std::vector<std::shared_ptr<Publication>> returnedBooks;
        Stack<std::shared_ptr<Publication>> tempStack = returnedPublications;
        while (!tempStack.isEmpty()) {
            returnedBooks.push_back(tempStack.top());
            tempStack.pop();
        }
        return returnedBooks;
    }

    /**
     * @brief Creates sample data for testing purposes.
     *
     * @param numberOfObjekts The number of objects to create.
     * @param customerId The starting customer ID.
     * @param bookID The starting book ID.
     */
  /* void createSampleData(int numberOfObjekts,int customerId,int bookID) {
        // Ensure there's a BookShelf
        if (std::find_if(shelves.begin(), shelves.end(),
            [](const std::shared_ptr<Shelf>& shelf) {
                return std::dynamic_pointer_cast<BookShelf>(shelf) != nullptr;
            }) == shelves.end()) {
            shelves.push_back(std::make_shared<BookShelf>(100, 1));
            }

        // Create  customers
        for (int i = customerId; i <= numberOfObjekts+customerId; i++) {
            addCustomer(std::make_shared<Customer>(i, "Customer" + std::to_string(i), "LastName" + std::to_string(i)));
        }

        // Create  books
        for (int i = bookID; i <= numberOfObjekts; i++) {
            addBook(std::make_shared<Book>(i, "Book" + std::to_string(i), Author("Author", std::to_string(i)), 2000 + i, 200 + i, 5, 5));
        }

        // Borrow some books
        for (int i = 1; i <= numberOfObjekts; i++) {
            try {
                borrowBook(i, i);
            } catch (const std::exception& e) {
                std::cerr << "Error borrowing book " << i << ": " << e.what() << std::endl;
            }
        }

        // Return some books
        for (int i = 1; i <= numberOfObjekts; i++) {
            try {
                returnBook(i, i);
            } catch (const std::exception& e) {
                std::cerr << "Error returning book " << i << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Sample data created successfully.\n";
    }*/
    /**
     * @brief Adds a new customer to the library.
     * @param customer Shared pointer to the customer to be added.
     */
    void addCustomer(std::shared_ptr<Customer> customer) {
        customers.push_back(customer);
    }

    std::shared_ptr<Customer> findCustomer(int customerId) {
        auto it = std::find_if(customers.begin(), customers.end(),
            [customerId](const std::shared_ptr<Customer>& c) { return c->id == customerId; });
        return (it != customers.end()) ? *it : nullptr;
    }

    std::shared_ptr<Book> findBook(int bookId) {
        auto it = std::find_if(books.begin(), books.end(),
            [bookId](const std::shared_ptr<Book>& b) { return b->id == bookId; });
        return (it != books.end()) ? *it : nullptr;
    }
private:

};
// Main function with a simple text dialog (continued)
int main() {
    Library library;
    int nextCustomerId = 1;
    int nextBookId = 1;
    int numberOfObject = 0;

    while (true) {
        std::cout << "\nLibrary Management System\n";
        std::cout << "1. Create a customer\n";
        std::cout << "2. Show list of customers\n";
        std::cout << "3. Create a book\n";
        std::cout << "4. Show list of books\n";
        std::cout << "5. Borrow a book\n";
        std::cout << "6. Return a book\n";
        std::cout << "7. Show returned books\n";
        std::cout << "8. Show borrowed books\n";
        std::cout << "9. Create objects for Customers and Books automatically\n";
        std::cout << "10. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string firstName, lastName;
                std::cout << "Enter customer's first name: ";
                std::cin >> firstName;
                std::cout << "Enter customer's last name: ";
                std::cin >> lastName;
                library.addCustomer(std::make_shared<Customer>(nextCustomerId++, firstName, lastName));
                std::cout << "Customer created successfully.\n";
                break;
            }
            case 2: {
                std::cout << "List of customers:\n";
                for (const auto& customer : library.getCustomers()) {
                    std::cout << "ID: " << customer->id << ", Name: " << customer->firstName << " " << customer->lastName << "\n";
                }
                break;
            }
            case 3: {
                std::string title, authorFirstName, authorLastName;
                int year, pages, total, available;
                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "Enter author's first name: ";
                std::cin >> authorFirstName;
                std::cout << "Enter author's last name: ";
                std::cin >> authorLastName;
                std::cout << "Enter publication year: ";
                std::cin >> year;
                std::cout << "Enter number of pages: ";
                std::cin >> pages;
                std::cout << "Enter total copies: ";
                std::cin >> total;
                std::cout << "Enter available copies: ";
                std::cin >> available;
                auto book = std::make_shared<Book>(nextBookId++, title, Author(authorFirstName, authorLastName), year, pages, total, available);
                library.addBook(book);
                std::cout << "Book added successfully.\n";
                break;
            }
            case 4: {
                std::cout << "List of books:\n";
                for (const auto& book : library.getBooks()) {
                    std::cout << "ID: " << book->id << ", Title: " << book->title
                              << ", Author: " << book->author.getFullName()
                              << ", Available: " << book->availableCopies << "/" << book->totalCopies << "\n";
                }
                break;
            }
            case 5: {
                int customerId, bookId;
                std::cout << "Enter customer ID: ";
                std::cin >> customerId;
                std::cout << "Enter book ID: ";
                std::cin >> bookId;
                try {
                    library.borrowBook(customerId, bookId);
                    std::cout << "Book borrowed successfully.\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                int customerId, bookId;
                std::cout << "Enter customer ID: ";
                std::cin >> customerId;
                std::cout << "Enter book ID: ";
                std::cin >> bookId;
                try {
                    library.returnBook(customerId, bookId);
                    std::cout << "Book returned successfully.\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 7: {
                std::cout << "Returned books:\n";
                for (const auto& book : library.getReturnedBooks()) {
                    std::cout << "ID: " << book->id << ", Title: " << book->title << "\n";
                }
                break;
            }
            case 8: {
                std::cout << "Borrowed books:\n";
                for (const auto& customer : library.getCustomers()) {
                    for (const auto& book : customer->borrowedPublications) {
                        std::cout << "Customer: " << customer->firstName << " " << customer->lastName
                                  << ", Book ID: " << book->id << ", Title: " << book->title << "\n";
                    }
                }
                break;
            }
            case 9: {
                std::cout << "Enter Number of Objects:";
                std::cin >> numberOfObject;
               // library.createSampleData(numberOfObject,nextCustomerId,nextBookId);

                    if (std::find_if(library.shelves.begin(), library.shelves.end(),
                         [](const std::shared_ptr<Shelf>& shelf) {
                       return std::dynamic_pointer_cast<BookShelf>(shelf) != nullptr;
                           }) == library.shelves.end()) {
                        library.shelves.push_back(std::make_shared<BookShelf>(100, 1));
                       }

                    // Create  customers
                    for (int i = nextCustomerId; i <= (numberOfObject+nextCustomerId)-1; i++) {
                        library.addCustomer(std::make_shared<Customer>(i, "Customer" + std::to_string(i), "LastName" + std::to_string(i)));
                    }

                    // Create  books
                    for (int i = nextBookId; i <= (numberOfObject+nextBookId)-1; i++) {
                        library.addBook(std::make_shared<Book>(i, "Book" + std::to_string(i), Author("Author", std::to_string(i)), 2000 + i, 200 + i, 5, 5));
                    }
                nextBookId += numberOfObject;
                nextCustomerId += numberOfObject;

                break;
            }
            case 10:
                std::cout << "Thank you for using the Library Management System.\n";
                return 0;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    //return 0;

}

