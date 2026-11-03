# Project 2 - Server Log Search
I (Kayla) started setting up the main part of the program.
Right now I have:
- `main.cpp`
- `Application.h`
- `Application.cpp`
- `CMakeLists.txt`
`main.cpp` just starts the program and `Application` is handling loading the data, the menu, searches, and eventually connecting both of your structures together.

For now I am storing the logs like this:
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

Alexis - for the Hash Table, just try to keep it in its own `.h` and `.cpp` files. I mainly just need to be able to insert a key with a log ID and search a key and get back the matching log IDs.
Something like:
```cpp
void insert(string key, int logId);
vector<int> search(string key);
```
The exact names do not matter that much, just let me know what you end up using.

Alejandra - same idea for the Trie. Keep it in its own `.h` and `.cpp` files. I mainly need to be able to insert a key/log ID, search for an exact match, and do a prefix search.
Something like:
```cpp
void insert(string key, int logId);
vector<int> search(string key);
vector<int> prefixSearch(string prefix);
```
I am going to handle loading the first 100,000 logs and then pass the same data into both structures.

The menu I have right now is:
1. Load Dataset
2. Search Using Hash Table
3. Search Using Trie
4. Compare Performance
5. Exit

Later I will connect both of your classes into `Application.cpp` and use `chrono` to compare the search times.

Also please do not make another `main.cpp` for the final version because I already have that part set up.
