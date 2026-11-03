#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

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
    bool parseLine(string line, int id, LogEntry& log);

public:
    void loadData();
    void displayMenu();
    void searchHashTable();
    void searchTrie();
    void comparePerformance();
};


#endif