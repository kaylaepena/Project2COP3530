#include "Application.h"

bool Application::parseLine(string line, int id, LogEntry& log) {
    return false;
}

void Application::loadData() {
    //This checks if the file can be opened
    string filename;
    cin.ignore();
    cout << "Enter file name: ";
    getline(cin, filename);
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file." << endl;
        return;
    }
    cout << "File opened successfully!" << endl;
    string line;
    int count = 0;
    while (getline(file, line) && count < 100000) {
        count++;
    }
    cout << count << " lines were read." << endl;
    file.close();
}

void Application::displayMenu() {
    int choice = 0;
    while (choice != 5) {
        cout << endl;
        cout << "-------------------------------" << endl;
        cout << "       SERVER LOG SEARCH       " << endl;
        cout << "1. Load Dataset" << endl;
        cout << "2. Search Using Hash Table" << endl;
        cout << "3. Search Using Trie" << endl;
        cout << "4. Compare Performance" << endl;
        cout << "5. Exit" << endl;
        cout << "-------------------------------" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            loadData();
            cout << "Loading Dataset!" << endl;
        }
        if (choice == 2) {
            searchHashTable();
            cout << "Searching with Hash tables!" << endl;
        }
        if (choice == 3) {
            searchTrie();
            cout << "Searching with Trie!" << endl;
        }
        if (choice == 4) {
            comparePerformance();
            cout << "Comparing preformance!" << endl;
        }
        if (choice == 5) {
            cout << "Goodbye!" << endl;
        }
        else if (choice < 1 || choice > 5) {
            cout << "Invalid choice." << endl;
        }
    }
}


void Application::searchHashTable() {

}

void Application::searchTrie() {

}

void Application::comparePerformance() {

}