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

The Hash Table is in HashTable.h and HashTable.cpp. It is written from scratch (no std::unordered_map).

**Functions**
- `void insert(string key, int logId);` adds a log ID under a key
- `vector<int> search(string key);` returns every log ID stored under that key, or an empty vector if the key isn't there
- `void clear();` empties the table so it can be rebuilt after loading a new file
- Stats for the performance comparison: `size()`, `bucketCount()`, `loadFactor()`, `collisionCount()`, `resizeCount()`, `longestChain()`, `memoryUsage()`

**Hash function**
FNV-1a. For each character, the hash is XORed with the character and then multiplied by a large prime. The bucket index is `hash % bucketCount`. This spreads out keys that look alike, such as `/usr/login` and `/usr/logout`.

**Collision handling**
Separate chaining. Each bucket is a list of entries (key + list of log IDs). When a new key lands in a bucket that already has something in it, it gets added to that bucket's chain, and the collision counter goes up.

**Resizing**
The table starts with 16 buckets. When the load factor (keys / buckets) goes over 0.75, the number of buckets doubles and every entry is rehashed into the new array. This keeps chains short, so lookups stay O(1) on average.

**What gets inserted**
Each key maps to the IDs of every log that has that key. For example, `insert(log.serviceName, log.id)` and `insert(log.severity, log.id)` let you search `/usr/login` or `ERROR`. Logs are inserted in ID order, so each ID list is already sorted, and inserting the same log twice under one key does not create a duplicate.

**Searching**
`search` hashes the key, goes to that bucket, walks the chain until the key matches, and returns that key's list of log IDs. Searches are exact and case-sensitive (`ERROR` matches, `error` does not).

**Complexity**
- insert / search: O(1) average and O(n) worst case (every key in one bucket), plus O(L) to hash a key of length L. Returning the results is O(k) for k matching IDs.
- resize: O(n) but rare (amortized O(1) per insert).
- memory: O(buckets + keys + total IDs stored).

**Tested**
Tested against a brute-force reference with 1,000,000 logs and about 5,000 distinct keys: every search matched. Also tested missing keys, empty keys, a repeated ID, case sensitivity, forced collisions in a 1-bucket table, resizing, and clear().

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
