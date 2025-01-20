#include <bits/stdc++.h>
using namespace std;


struct Book {
    int id;
    string title;
    string author;
};

int getNextID() {
    ifstream file("library.txt");
    int lastID = 0;
    string line;

    while (getline(file, line)) {
        lastID = stoi(line.substr(0, line.find(',')));
    }
    return lastID + 1;
}

bool isValidName(const string &name) {
    for (char c : name) {
        if (!isalpha(c) && !isspace(c)) {
            return false;
        }
    }
    return true;
}

void addBook() {
    ofstream file("library.txt", ios::binary);
    Book book;

    book.id = getNextID();

    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, book.title);

    if (book.title.empty()) {
        cout << "Error: Book title cannot be empty.\n";
        return;
    }

    while (true) {
        cout << "Enter book author: ";
        getline(cin, book.author);
        if (book.author.empty()) {
            cout << "Error: Book author cannot be empty.\n";
        } else if (!isValidName(book.author)) {
            cout << "Error: Author name must contain only letters and spaces.\n";
        } else {
            break;
        }
    }

    file << book.id << "," << book.title << "," << book.author << endl;
    cout << "Book added successfully with ID " << book.id << "!\n";
}

void searchBook() {
    string searchTerm;
    cout << "Enter book title or author to search: ";
    cin.ignore();
    getline(cin, searchTerm);

    ifstream file("library.txt");
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(searchTerm) != string::npos) {
            cout << "Book found: " << line << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No books found matching that search term!\n";
    }
}

void deleteBook() {
    int idToDelete;
    cout << "Enter book ID to delete: ";
    cin >> idToDelete;

    ifstream file("library.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(file, line)) {
        int id = stoi(line.substr(0, line.find(',')));
        if (id != idToDelete) {
            tempFile << line << endl;
        } else {
            found = true;
        }
    }

    file.close();
    tempFile.close();

    if (found) {
        remove("library.txt");
        rename("temp.txt", "library.txt");
        cout << "Book with ID " << idToDelete << " deleted successfully.\n";
    } else {
        cout << "No book found with ID " << idToDelete << ".\n";
    }
}

void viewBooks() {
    ifstream file("library.txt");
    string line;
    bool isEmpty = true;

    cout << "\nBooks in the Library:\n";
    while (getline(file, line)) {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "No books found in the library.\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Book\n";
        cout << "3. Delete Book\n";
        cout << "4. View Books\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                deleteBook();
                break;
            case 4:
                viewBooks();
                break;
            case 5:
                cout << "Thank you.......!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
