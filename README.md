# Project 2 - Server Log Search

This project searches server log data using two different data structures: a Hash Table and a Trie. We are going to compare how both structures perform when searching the same dataset.

## How to Run the Program

You will need:
- A C++ compiler that supports C++17
- CMake 3.20 or newer (or CLion, which can use CMake for you)
- The logfiles.log dataset

**Dataset:** The server logs are from [Server-Log-Dataset on GitHub](https://github.com/vishnu-u/Server-Log-Dataset). Download logfiles.log and put it in the main project folder, in the same place as main.cpp and CMakeLists.txt.

### Running in CLion

1. Download the project and open its folder in CLion.
2. Make sure logfiles.log is in the main project folder.
3. Let CLion load the CMake project.
4. Go to Run > Edit Configurations and select the Project2 configuration. Set the Working directory to $ProjectFileDir$.
5. Click the green Run button.
6. When the menu appears, enter 1 to load the dataset.
7. When it asks Enter file name:, type logfiles.log and press Enter.
8. Once the logs are loaded, you can use the other menu options to search or compare the structures.

If the working directory is still cmake-build-debug instead, you can usually enter ../logfiles.log for the file name.

### Running from the Terminal (Mac/Linux)

Open the terminal in the project folder and enter:
cmake -S . -B build
cmake --build build
./build/Project2

Then select option 1 and enter logfiles.log when asked for the file name. Run the program from the main project folder so it can find the file.

## Project Files and Application Class

The main structure of the program and the Application class is as follows:
- main.cpp
- Application.h
- Application.cpp
- HashTable.h and HashTable.cpp
- Trie.h and Trie.cpp
- CMakeLists.txt
- logfiles.log (downloaded separately)

main.cpp starts the program and calls the menu from the Application class.

The Application class handles:
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
- displaying matching logs from both structures
- timing how long it takes to build and search each structure

Each log is stored as:

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

The requested endpoint is being used as the serviceName.

The program also creates a simple message from the request, for example: GET /usr/login returned 404.

## Main Menu

1. Load Dataset
2. Search Using Hash Table
3. Search Using Trie
4. Compare Performance
5. Exit

**Example inputs:**
- Choose 1, then enter logfiles.log to load the logs.
- Choose 2, then enter /usr/admin/developer to search the Hash Table.
- Choose 3, enter /usr/admin/developer, then choose 1 for an exact Trie search.
- Choose 3, enter /usr/admin, then choose 2 for a prefix Trie search.
- Choose 4, then enter /usr/admin/developer to compare search times.
- Choose 5 to exit.

Make sure to load the dataset before searching. The program prints the total number of matching results but only displays the first 10 logs so the output is not too long.

## Dataset

The program reads no more than the first 100,000 lines from the dataset. The loader also tells us how many logs were successfully loaded and how many lines were skipped because they could not be parsed.

For searching, the program inserts each log's service name, severity, and words from its message into both the Hash Table and Trie. Each key points to the IDs of the logs that contain it.

## Hash Table

The Hash Table is in HashTable.h and HashTable.cpp. It is written from scratch (no std::unordered_map).

**Functions**
- void insert(string key, int logId); adds a log ID under a key
- vector<int> search(string key); returns every log ID stored under that key, or an empty vector if the key isn't there
- void clear(); empties the table so it can be rebuilt after loading a new file
- Stats for the performance comparison: size(), bucketCount(), loadFactor(), collisionCount(), resizeCount(), longestChain(), memoryUsage()

**Hash function**

FNV-1a. For each character, the hash is XORed with the character and then multiplied by a large prime. The bucket index is hash % bucketCount. This spreads out keys that look alike, such as /usr/login and /usr/logout.

**Collision handling**

Separate chaining. Each bucket is a list of entries (key + list of log IDs). When a new key lands in a bucket that already has something in it, it gets added to that bucket's chain, and the collision counter goes up.

**Resizing**

The table starts with 16 buckets. When the load factor (keys / buckets) goes over 0.75, the number of buckets doubles and every entry is rehashed into the new array. This keeps chains short, so lookups stay O(1) on average.

**What gets inserted**

Each key maps to the IDs of every log that has that key. For example, insert(log.serviceName, log.id) and insert(log.severity, log.id) let you search /usr/login or ERROR. Logs are inserted in ID order, so each ID list is already sorted, and inserting the same log twice under one key does not create a duplicate.

**Searching**

search hashes the key, goes to that bucket, walks the chain until the key matches, and returns that key's list of log IDs. Searches are exact and case-sensitive (ERROR matches, error does not).

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

***Try to keep the Trie in Trie.h and Trie.cpp and similar to the Hash one table

## Performance Comparison

The program measures how long it takes to build the Hash Table and Trie and prints the build times in milliseconds.

Option 4 compares how fast both structures can do an exact search for the same term. It runs each search 1,000 times using chrono and divides the total time by 1,000 to get an average search time in microseconds.

The program also shows how many matches each structure found, checks if they returned the same log IDs, and prints which one was faster. The exact times may be different depending on the computer and the search term.
