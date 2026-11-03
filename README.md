# Project 2 - Server Log Search
This project searches server log data using two different data structures: a Hash Table and a Trie. We are going to compare how both structures perform when searching the same dataset.

## What is done so far
The main structure of the program and the Application class is as follows:
- main.cpp
- Application.h
- Application.cpp
- CMakeLists.txt
- sample_logs.txt

main.cpp starts the program and calls the menu from the Application class.

The Application class currently handles:
- displaying the main menu
- loading the dataset
- reading up to the first 100,000 lines
- parsing each server log line
- storing the logs in a vector
- checking for invalid/malformed log lines
- keeping track of skipped lines
- getting the user's Hash Table search input
- getting the user's Trie search input
- getting a search term for the performance comparison
- displaying matching logs once the search structures are connected

Each log is currently stored as:
```cpp
struct LogEntry {
    int id;
    string timestamp;
    string serviceName;
    string severity;
    string message;
    int responseTime;
};
```

The parser gets the timestamp, request/endpoint, HTTP status code, and response time from each log line.

The HTTP status code is also used to give the log a basic severity:
- 500+ = ERROR
- 400-499 = WARNING
- everything else = INFO

The requested endpoint is currently being used as the serviceName.

The program also creates a simple message from the request, for example: GET /usr/login returned 404

## Current Menu
1. Load Dataset
2. Search Using Hash Table
3. Search Using Trie
4. Compare Performance
5. Exit

***The search options currently ask for the user's search term, but the real Hash Table and Trie searches still need to be connected.

## Dataset
The program is set up to read no more than the first 100,000 lines when we use the real dataset.
The loader also tells us how many logs were successfully loaded and how many lines were skipped because they could not be parsed.

## Hash Table

**ALEXIS PART HERE**

**Add this once the Hash Table is finished:**
- hash function used
- how collisions are handled
- what information is inserted
- how searches return matching log IDs

***Try to keep the Hash Table in HashTable.h and HashTable.cpp. Functions that would make integration easiest: 
- void insert(string key, int logId);
- vector<int> search(string key);
If you use different names, just let me (Kayla) know so I can change them in Application.cpp.

## Trie

***ALEJANDRA PART HERE

***Add this once the Trie is finished:
- how words/keys are inserted
- exact search
- prefix search
- how matching log IDs are returned

***Try to keep the Trie in Trie.h and Trie.cpp.
Functions that would make integration easiest:
-void insert(string key, int logId);
-vector<int> search(string key);
-vector<int> prefixSearch(string prefix);
If you use different names, just let me (Kayla) know so I can change them in Application.cpp.

## Performance Comparison

**ADD FINAL PERFORMANCE RESULTS HERE LATER**

## Important for everyone

Please do not make another main.cpp for the final version because the program already has one.

Try to keep the Hash Table and Trie in their own .h and .cpp files so they are easier to connect to the existing Application class.
