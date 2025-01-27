#include <bits/stdc++.h>
using namespace std;

const string FILE_NAME = "library.txt";

void initializeFile() {
    ifstream file(FILE_NAME);
    if (!file) {
        ofstream newFile(FILE_NAME);
        if (!newFile) {
            cerr << "Error initializing library file.\n";
            exit(1);
        }
    }
}

int getNextID() {
    ifstream file(FILE_NAME);
    string line;
    int lastID = 0;

    while (getline(file, line)) {
        int id = stoi(line.substr(0, line.find(',')));
        lastID = max(lastID, id);
    }

    return lastID + 1;
}

// Validate author name to ensure it's not numeric
bool isValidName(const string &name) {
    for (char c : name) {
        if (isdigit(c)) {
            return false;
        }
    }
    return !name.empty();
}

// Add a new book to the library
void addBook() {
    ofstream file(FILE_NAME, ios::app);
    if (!file) {
        cerr << "Error opening library file.\n";
        return;
    }

    int id = getNextID();
    string title, author;

    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    if (title.empty()) {
        cout << "Error: Book title cannot be empty.\n";
        return;
    }

    do {
        cout << "Enter book author: ";
        getline(cin, author);
        if (!isValidName(author)) {
            cout << "Invalid name. Please enter a valid author name.\n";
        }
    } while (!isValidName(author));

    file << id << "," << title << "," << author << endl;
    cout << "Book added successfully with ID " << id << "!\n";
}

// View all books in the library
void viewBooks() {
    ifstream file(FILE_NAME);
    if (!file) {
        cerr << "Error opening library file.\n";
        return;
    }

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

// Delete a book by ID
void deleteBook() {
    int idToDelete;
    cout << "Enter book ID to delete: ";
    cin >> idToDelete;

    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");
    if (!file || !tempFile) {
        cerr << "Error opening files.\n";
        return;
    }

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
        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());
        cout << "Book with ID " << idToDelete << " deleted successfully.\n";
    } else {
        cout << "No book found with ID " << idToDelete << ".\n";
    }
}

// Search for books by title or author
void searchBooks() {
    string searchTerm;
    cout << "Enter book title or author to search: ";
    cin.ignore();
    getline(cin, searchTerm);

    ifstream file(FILE_NAME);
    if (!file) {
        cerr << "Error opening library file.\n";
        return;
    }

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

// Update book details by ID
void updateBook() {
    int idToUpdate;
    cout << "Enter book ID to update: ";
    cin >> idToUpdate;

    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");
    if (!file || !tempFile) {
        cerr << "Error opening files.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        int id = stoi(line.substr(0, line.find(',')));
        if (id == idToUpdate) {
            found = true;
            string newTitle, newAuthor;

            cout << "Enter new title (leave empty to keep current): ";
            cin.ignore();
            getline(cin, newTitle);

            do {
                cout << "Enter new author (leave empty to keep current): ";
                getline(cin, newAuthor);
                if (!newAuthor.empty() && !isValidName(newAuthor)) {
                    cout << "Invalid name. Please enter a valid author name.\n";
                }
            } while (!newAuthor.empty() && !isValidName(newAuthor));

            size_t firstComma = line.find(',');
            size_t secondComma = line.find(',', firstComma + 1);

            if (!newTitle.empty()) {
                line.replace(firstComma + 1, secondComma - firstComma - 1, newTitle);
            }
            if (!newAuthor.empty()) {
                line.replace(secondComma + 1, line.length() - secondComma - 1, newAuthor);
            }
        }
        tempFile << line << endl;
    }

    file.close();
    tempFile.close();

    if (found) {
        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());
        cout << "Book with ID " << idToUpdate << " updated successfully.\n";
    } else {
        cout << "No book found with ID " << idToUpdate << ".\n";
    }
}

int main() {
    initializeFile();

    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Books\n";
        cout << "3. Delete Book\n";
        cout << "4. View Books\n";
        cout << "5. Update Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBooks();
                break;
            case 3:
                deleteBook();
                break;
            case 4:
                viewBooks();
                break;
            case 5:
                updateBook();
                break;
            case 6:
                cout << "Thank you..........\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
