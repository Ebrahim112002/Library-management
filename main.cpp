#include <bits/stdc++.h>
using namespace std;

const string BOOKS_FILE = "books.dat";
const string USERS_FILE = "users.dat";

struct Book {
    int id;
    string title;
    string author;
    bool isBorrowed;

    Book() : isBorrowed(false) {}
};

struct User {
    int id;
    string username;
    string password;
    string role;

    User() {}
    User(int uid, string uname, string pass, string r) : id(uid), username(uname), password(pass), role(r) {}
};

// Store users in an unordered map for fast lookup
unordered_map<string, User> userMap;
User* loggedInUser = nullptr;
User* loggedInAdmin = nullptr;
vector<Book> books;

void initializeFiles() {
    ofstream bookFile(BOOKS_FILE, ios::app | ios::binary);
    ofstream userFile(USERS_FILE, ios::app | ios::binary);
}

// Save users to file
void saveUsers() {
    ofstream file(USERS_FILE, ios::binary);
    for (const auto& [username, user] : userMap) {
        int id = user.id;
        int nameLen = username.size();
        int passLen = user.password.size();
        int roleLen = user.role.size();

        file.write(reinterpret_cast<char*>(&id), sizeof(id));
        file.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        file.write(username.c_str(), nameLen);
        file.write(reinterpret_cast<char*>(&passLen), sizeof(passLen));
        file.write(user.password.c_str(), passLen);
        file.write(reinterpret_cast<char*>(&roleLen), sizeof(roleLen));
        file.write(user.role.c_str(), roleLen);
    }
    file.close();
}

// Load users into memory once
void loadUsers() {
    userMap.clear();
    ifstream file(USERS_FILE, ios::binary);
    if (!file) return;

    while (!file.eof()) {
        int id, nameLen, passLen, roleLen;
        if (!file.read(reinterpret_cast<char*>(&id), sizeof(id))) break;

        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string username(nameLen, '\0');
        file.read(&username[0], nameLen);

        file.read(reinterpret_cast<char*>(&passLen), sizeof(passLen));
        string password(passLen, '\0');
        file.read(&password[0], passLen);

        file.read(reinterpret_cast<char*>(&roleLen), sizeof(roleLen));
        string role(roleLen, '\0');
        file.read(&role[0], roleLen);

        userMap[username] = User(id, username, password, role);
    }
    file.close();
}

// Save books to file
void saveBooks() {
    ofstream file(BOOKS_FILE, ios::binary);
    for (const auto& book : books) {
        int titleLen = book.title.size();
        int authorLen = book.author.size();

        file.write(reinterpret_cast<const char*>(&book.id), sizeof(book.id));
        file.write(reinterpret_cast<const char*>(&titleLen), sizeof(titleLen));
        file.write(book.title.c_str(), titleLen);
        file.write(reinterpret_cast<const char*>(&authorLen), sizeof(authorLen));
        file.write(book.author.c_str(), authorLen);
        file.write(reinterpret_cast<const char*>(&book.isBorrowed), sizeof(book.isBorrowed));
    }
    file.close();
}

// Load books from file once
void loadBooks() {
    books.clear();
    ifstream file(BOOKS_FILE, ios::binary);
    if (!file) return;

    while (!file.eof()) {
        Book book;
        int titleLen, authorLen;
        if (!file.read(reinterpret_cast<char*>(&book.id), sizeof(book.id))) break;

        file.read(reinterpret_cast<char*>(&titleLen), sizeof(titleLen));
        book.title.resize(titleLen);
        file.read(&book.title[0], titleLen);

        file.read(reinterpret_cast<char*>(&authorLen), sizeof(authorLen));
        book.author.resize(authorLen);
        file.read(&book.author[0], authorLen);

        file.read(reinterpret_cast<char*>(&book.isBorrowed), sizeof(book.isBorrowed));
        books.push_back(book);
    }
    file.close();
}

// Register a new user
void registerUser(string role) {
    User user;
    cout << "Enter username: ";
    cin >> user.username;
    cout << "Enter password: ";
    cin >> user.password;
    user.role = role;
    user.id = userMap.size() + 1;

    userMap[user.username] = user;
    saveUsers();
    cout << role << " registered successfully!\n";
}

// Login user
bool login(User*& loggedIn, string role) {
    loadUsers();
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (userMap.count(username) && userMap[username].password == password && userMap[username].role == role) {
        loggedIn = &userMap[username];
        cout << role << " login successful!\n";
        return true;
    }
    cout << "Invalid credentials or incorrect role!\n";
    return false;
}

// View books
void viewBooks() {
    loadBooks();
    if (books.empty()) {
        cout << "No books found!\n";
        return;
    }
    for (const auto& book : books) {
        cout << "ID: " << book.id << " | Title: " << book.title << " | Author: " << book.author
             << " | " << (book.isBorrowed ? "Borrowed" : "Available") << "\n";
    }
}

// Add book (Admin only)
void addBook() {
    if (!loggedInAdmin) {
        cout << "Admin must be logged in to add books!\n";
        return;
    }

    Book book;
    cout << "Enter Book ID: ";
    cin >> book.id;
    cin.ignore();
    cout << "Enter Title: ";
    getline(cin, book.title);
    cout << "Enter Author: ";
    getline(cin, book.author);
    book.isBorrowed = false;

    books.push_back(book);
    saveBooks();
    cout << "Book added successfully!\n";
}

// Borrow book
void borrowBook() {
    if (!loggedInUser) {
        cout << "You must be logged in to borrow a book!\n";
        return;
    }

    int bookID;
    cout << "Enter book ID to borrow: ";
    cin >> bookID;

    for (auto& book : books) {
        if (book.id == bookID && !book.isBorrowed) {
            book.isBorrowed = true;
            saveBooks();
            cout << "Book borrowed successfully!\n";
            return;
        }
    }
    cout << "Book not found or already borrowed!\n";
}

// Return book
void returnBook() {
    if (!loggedInUser) {
        cout << "You must be logged in to return a book!\n";
        return;
    }

    int bookID;
    cout << "Enter book ID to return: ";
    cin >> bookID;

    for (auto& book : books) {
        if (book.id == bookID && book.isBorrowed) {
            book.isBorrowed = false;
            saveBooks();
            cout << "Book returned successfully!\n";
            return;
        }
    }
    cout << "Book not found or not borrowed!\n";
}

void adminPanel() {
    int choice;
    do {
        cout << "\nAdmin Panel\n1. Register Admin\n2. Login Admin\n3. Back\nEnter choice: ";
        cin >> choice;

        if (choice == 1) registerUser("Admin");
        else if (choice == 2) {
            if (login(loggedInAdmin, "Admin")) {
                int adminChoice;
                do {
                    cout << "\nAdmin Menu\n1. Add Book\n2. View Books\n3. Logout\nEnter choice: ";
                    cin >> adminChoice;

                    if (adminChoice == 1) addBook();
                    else if (adminChoice == 2) viewBooks();
                } while (adminChoice != 3);
                loggedInAdmin = nullptr;
            }
        }
    } while (choice != 3);
}

int main() {
    initializeFiles();
    int choice;
    do {
        cout << "\nLibrary Management System\n1. Register\n2. Login\n3. Admin Panel\n4. View Books\n5. Exit\nEnter choice: ";
        cin >> choice;
        if (choice == 1) registerUser("Member");
        else if (choice == 2 && login(loggedInUser, "Member")) {
            int userChoice;
            do {
                cout << "\nUser Menu\n1. Borrow Book\n2. Return Book\n3. View Books\n4. Logout\nEnter choice: ";
                cin >> userChoice;
                if (userChoice == 1) borrowBook();
                else if (userChoice == 2) returnBook();
                else if (userChoice == 3) viewBooks();
            } while (userChoice != 4);
            loggedInUser = nullptr;
        } else if (choice == 3) adminPanel();
        else if (choice == 4) viewBooks();
    } while (choice != 5);
    return 0;
}
