#include "Application.h"
#include <chrono>

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
    //If the request does not have all three parts, skip this log line
    if (requestStream.fail()) {
        return false;
    }

    //After the request, the next values are the status code and bytes sent
    string rest = line.substr(secondQuote + 1);
    stringstream restStream(rest);

    int statusCode = 0;
    int bytes = 0;
    restStream >> statusCode;
    restStream >> bytes;
    //If the status code or bytes could not be read, skip this log line to avoid issues
    if (restStream.fail()) {
        return false;
    }
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
    //If the value is invalid, use 0 instead of crashing the program
    try {
        log.responseTime = stoi(response);
    }
    catch (...) {
        log.responseTime = 0;
    }
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

    //Clears old data in case the user loads a different file later
    logs.clear();
    hashTable.clear();
    trie.clear();

    string line;
    int count = 0;
    int skipped = 0;
    //Read no more than the first 100,000 lines
    while (getline(file, line) && count < 100000) {
        LogEntry log;
        //parseLine fills in the LogEntry if the line is valid
        if (parseLine(line, logs.size(), log)) {
            //Saves the finished log in our vector
            logs.push_back(log);
        }
        else {
            skipped++;
        }
        count++;
    }
    file.close();
    cout << logs.size() << " logs were loaded." << endl;
    cout << skipped << " lines were skipped." << endl;

    //Only keep this when testing
    //printsamplelogsfortest();

    //For the Hash Table:
    auto hashBuildStart = chrono::high_resolution_clock::now();
    //Store the searchable log information in the Hash Table
    for (int i = 0; i < logs.size(); i++) {
        hashTable.insert(logs[i].serviceName, logs[i].id);
        hashTable.insert(logs[i].severity, logs[i].id);
        //Store each word in the message so message keywords can also be searched
        stringstream words(logs[i].message);
        string word;
        while (words >> word) {
            hashTable.insert(word, logs[i].id);
        }
    }
    auto hashBuildEnd = chrono::high_resolution_clock::now();
    auto hashBuildTime = chrono::duration_cast<chrono::milliseconds>(hashBuildEnd - hashBuildStart).count();
    cout << "Hash Table built successfully." << endl;
    cout << "Hash Table build time: " << hashBuildTime << " milliseconds" << endl;
    //Stats from the Hash Table.cpp/.h
    cout << "Keys stored: " << hashTable.size() << endl;
    cout << "Buckets: " << hashTable.bucketCount() << endl;
    cout << "Collisions: " << hashTable.collisionCount() << endl;
    cout << "Resizes: " << hashTable.resizeCount() << endl;
    cout << "Longest chain: " << hashTable.longestChain() << endl;

    //For the Trie:
    auto trieBuildStart = chrono::high_resolution_clock::now();
    //Stores the same searchable information in the Trie
    for (int i = 0; i < logs.size(); i++) {
        trie.insert(logs[i].serviceName, logs[i].id);
        trie.insert(logs[i].severity, logs[i].id);
        stringstream words(logs[i].message);
        string word;
        while (words >> word) {
            trie.insert(word, logs[i].id);
        }
    }
    auto trieBuildEnd = chrono::high_resolution_clock::now();
    auto trieBuildTime = chrono::duration_cast<chrono::milliseconds>(trieBuildEnd - trieBuildStart).count();
    cout << "Trie built successfully." << endl;
    cout << "Trie build time: " << trieBuildTime << " milliseconds" << endl;
    cout << "Keys stored: " << trie.size() << endl;

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
        //Makes sure the user entered a number
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice." << endl;
            choice = 0;
            continue;
        }
        if (choice == 1) {
            loadData();
            cout << "Loading Dataset!" << endl;
        }
        else if (choice == 2) {
            searchHashTable();
            cout << "Searching with Hash tables!" << endl;
        }
        else if (choice == 3) {
            searchTrie();
            cout << "Searching with Trie!" << endl;
        }
        else if (choice == 4) {
            comparePerformance();
            cout << "Comparing preformance!" << endl;
        }
        else if (choice == 5) {
            cout << "Goodbye!" << endl;
        }
        else {
            cout << "Invalid choice." << endl;
        }
    }
}

void Application::searchHashTable() {
    //Makes sure the dataset has been loaded before searching
    if (logs.empty()) {
        cout << "You must load the dataset first." << endl;
        return;
    }

    string search;
    //Removes the Enter left over from the menu choice
    cin.ignore();
    cout << "Enter what you want to search for: ";
    getline(cin, search);
    cout << endl;
    cout << "Searching Hash Table for: " << search << endl;

    //Start timing right before the actual search
    auto start = chrono::high_resolution_clock::now();
    vector<int> results = hashTable.search(search);
    //Stop timing immediately after the search
    auto end = chrono::high_resolution_clock::now();
    auto searchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Hash Table search time: " << searchTime << " microseconds" << endl;
    displayresults(results);
}

void Application::searchTrie() {
    //Makes sure the dataset has been loaded before searching
    if (logs.empty()) {
        cout << "Please load the dataset first." << endl;
        return;
    }
    string search;
    cin.ignore();
    cout << "Enter what you want to search for: ";
    getline(cin, search);
    cout << "Searching Trie for: " << search << endl;
    cout << "1. Exact Search" << endl;
    cout << "2. Prefix Search" << endl;
    cout << "Enter search type: ";
    string searchType;
    getline(cin, searchType);

    //Writes and stores matching logs
    vector<int> results;
    auto start = chrono::high_resolution_clock::now();
    if (searchType == "1") {
        results = trie.search(search);
    }
    else if (searchType == "2") {
        results = trie.searchPrefix(search);
    }
    else {
        cout << "Invalid search type." << endl;
        return;
    }
    auto end = chrono::high_resolution_clock::now();
    auto searchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Trie search time: " << searchTime << " microseconds" << endl;
    displayresults(results);
}

void Application::comparePerformance() {
    //Both structures need the dataset before they can be compared
    if (logs.empty()) {
        cout << "Please load the dataset first." << endl;
        return;
    }
    string search;

    //Removes the Enter left over from the menu choice
    cin.ignore();

    cout << "Enter a search term to compare: ";
    getline(cin, search);

    cout << endl;
    cout << "Comparing performance for: " << search << endl;

    //Later give the searches to both to test:
    //hashTable.search("/usr/login");
    //trie.search("/usr/login");
}

void Application::printsamplelogsfortest() {
    //Only prints a couple logs so I can make sure parsing worked
    int amount = 2;
    if (logs.size() < 2) {
        amount = logs.size();
    }
    for (int i = 0; i < amount; i++) {
        cout << endl;
        cout << "ID: " << logs[i].id << endl;
        cout << "Timestamp: " << logs[i].timestamp << endl;
        cout << "Service: " << logs[i].serviceName << endl;
        cout << "Severity: " << logs[i].severity << endl;
        cout << "Message: " << logs[i].message << endl;
        cout << "Response Time: " << logs[i].responseTime << endl;
    }
}

void Application::displayresults(vector<int> resultids) {
    cout << endl;
    cout << "Matches found: " << resultids.size() << endl;

    //Each number in resultIds represents the ID of a matching log
    for (int i = 0; i < resultids.size(); i++) {
        int id = resultids[i];
        //Make sure the ID is valid before using it
        if (id >= 0 && id < logs.size()) {
            cout << endl;
            cout << "ID: " << logs[id].id << endl;
            cout << "Timestamp: " << logs[id].timestamp << endl;
            cout << "Service: " << logs[id].serviceName << endl;
            cout << "Severity: " << logs[id].severity << endl;
            cout << "Message: " << logs[id].message << endl;
            cout << "Response Time: " << logs[id].responseTime << endl;
        }
    }
}