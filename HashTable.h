#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>

using namespace std;

//Custom hash table that maps a key (service name, severity, word, etc.)
//to the list of log IDs that contain that key.
//Collisions are handled with separate chaining: each bucket holds a list of entries.
class HashTable {
private:
    struct Entry {
        string key;
        vector<int> logIds;
    };

    //Each bucket is a chain of entries whose keys hashed to the same index
    vector<vector<Entry>> buckets;
    int numKeys;

    //Stats for the performance analysis
    int numCollisions;
    int numResizes;

    const double MAX_LOAD_FACTOR = 0.75;

    unsigned long long hashFunction(const string& key) const;
    int getBucketIndex(const string& key, int bucketCount) const;
    void resize();

public:
    HashTable(int startingBuckets = 16);

    //Adds logId to the list for key (creates the key if it is new)
    void insert(string key, int logId);

    //Returns all log IDs stored under key, or an empty vector if the key is not found
    vector<int> search(string key);

    //Removes everything so the table can be rebuilt for a new dataset
    void clear();

    //Getters used for the performance comparison
    int size() const;
    int bucketCount() const;
    double loadFactor() const;
    int collisionCount() const;
    int resizeCount() const;
    int longestChain() const;
    size_t memoryUsage() const;
};

#endif
