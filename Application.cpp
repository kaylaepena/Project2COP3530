#include "Application.h"

bool Application::parseLine(string line, int id, LogEntry& log) {
    //The timestamp is stored between [ and ], so this finds those positions first
    int openBracket = line.find("[");
    int closeBracket = line.find("]");

    //If the brackets aren't found, the line is not formatted correctly
    if (openBracket == string::npos || closeBracket == string::npos) {
        return false;
    }

    //If it is found, continue
    log.timestamp = line.substr(
        openBracket + 1,
        closeBracket - openBracket - 1
    );

    //The HTTP request is inside quotation marks
    int firstQuote = line.find("\"", closeBracket);
    int secondQuote = line.find("\"", firstQuote + 1);
    if (firstQuote == string::npos || secondQuote == string::npos) {
        return false;
    }
    string request = line.substr(
        firstQuote + 1,
        secondQuote - firstQuote - 1
    );

    //Split the request into method, endpoint, and protocol
    //Fpr example: GET /usr/login HTTP/1.0
    string method;
    string endpoint;
    string protocol;
    stringstream requestStream(request);
    requestStream >> method;
    requestStream >> endpoint;
    requestStream >> protocol;

    //After the request, the next values are the status code and bytes sent
    string rest = line.substr(secondQuote + 1);
    stringstream restStream(rest);

    int statusCode;
    int bytes;
    restStream >> statusCode;
    restStream >> bytes;
    log.id = id;

    //Using the requested endpoint as the service name for our project
    log.serviceName = endpoint;

    //Convert the HTTP status code into a simple severity category
    if (statusCode >= 500) {
        log.severity = "ERROR";
    }
    else if (statusCode >= 400) {
        log.severity = "WARNING";
    }
    else {
        log.severity = "INFO";
    }

    //Make a simple readable message from the request information
    log.message = method + " " + endpoint + " returned " + to_string(statusCode);

    //Response time is the last value on each log line
    int lastSpace = line.find_last_of(" ");
    string response = line.substr(lastSpace + 1);
    log.responseTime = stoi(response);
    //Returning true means the line was successfully parsed
    return true;
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