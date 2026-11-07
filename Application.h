#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "HashTable.h"
#include "Trie.h"

using namespace std;

struct LogEntry {
    int id;
    string timestamp;
    string serviceName;
    string severity;
    string message;
    int responseTime;
};

class Application {
private:
    vector<LogEntry> logs;
    HashTable hashTable;
    Trie trie;
    bool parseLine(string line, int id, LogEntry& log);
    void printsamplelogsfortest();
    void displayresults(vector<int> resultids);

public:
    void loadData();
    void displayMenu();
    void searchHashTable();
    void searchTrie();
    void comparePerformance();
};

#endif