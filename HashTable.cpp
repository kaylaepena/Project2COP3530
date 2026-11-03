#include "HashTable.h"
#include <utility>

HashTable::HashTable(int startingBuckets) {
    //Need at least one bucket so we never divide by zero
    if (startingBuckets < 1) {
        startingBuckets = 1;
    }
    buckets.resize(startingBuckets);
    numKeys = 0;
    numCollisions = 0;
    numResizes = 0;
}

unsigned long long HashTable::hashFunction(const string& key) const {
    //FNV-1a hash: for each character, XOR it into the hash and then multiply by a prime.
    //This spreads similar strings (like /usr/login and /usr/logout) across different buckets.
    unsigned long long hash = 14695981039346656037ULL;
    for (int i = 0; i < key.size(); i++) {
        hash ^= (unsigned char)key[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

int HashTable::getBucketIndex(const string& key, int bucketCount) const {
    return hashFunction(key) % bucketCount;
}

void HashTable::insert(string key, int logId) {
    int index = getBucketIndex(key, buckets.size());
    vector<Entry>& bucket = buckets[index];

    //If the key is already in this bucket's chain, add the ID to its list
    for (int i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            vector<int>& ids = bucket[i].logIds;
            //Logs are inserted in order, so a repeat of the same log would be the last ID
            if (ids.empty() || ids.back() != logId) {
                ids.push_back(logId);
            }
            return;
        }
    }

    //New key: if the bucket already has something in it, that is a collision
    if (!bucket.empty()) {
        numCollisions++;
    }
    Entry entry;
    entry.key = key;
    entry.logIds.push_back(logId);
    bucket.push_back(entry);
    numKeys++;

    //Grow the table once it gets too full so chains stay short
    if (loadFactor() > MAX_LOAD_FACTOR) {
        resize();
    }
}

vector<int> HashTable::search(string key) {
    int index = getBucketIndex(key, buckets.size());
    vector<Entry>& bucket = buckets[index];

    //Walk the chain until we find the matching key
    for (int i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            return bucket[i].logIds;
        }
    }
    //Key was never inserted
    return vector<int>();
}

void HashTable::resize() {
    //Double the number of buckets and move every entry to its new bucket
    vector<vector<Entry>> newBuckets(buckets.size() * 2);
    for (int i = 0; i < buckets.size(); i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            Entry& entry = buckets[i][j];
            int newIndex = getBucketIndex(entry.key, newBuckets.size());
            //Move instead of copy so the ID lists are not duplicated during the resize
            newBuckets[newIndex].push_back(move(entry));
        }
    }
    buckets = move(newBuckets);
    numResizes++;
}

void HashTable::clear() {
    int startingBuckets = 16;
    buckets.clear();
    buckets.resize(startingBuckets);
    numKeys = 0;
    numCollisions = 0;
    numResizes = 0;
}

int HashTable::size() const {
    return numKeys;
}

int HashTable::bucketCount() const {
    return buckets.size();
}

double HashTable::loadFactor() const {
    return (double)numKeys / buckets.size();
}

int HashTable::collisionCount() const {
    return numCollisions;
}

int HashTable::resizeCount() const {
    return numResizes;
}

int HashTable::longestChain() const {
    int longest = 0;
    for (int i = 0; i < buckets.size(); i++) {
        if ((int)buckets[i].size() > longest) {
            longest = buckets[i].size();
        }
    }
    return longest;
}

size_t HashTable::memoryUsage() const {
    //Estimate of the bytes used by the table (bucket array, entries, keys, and ID lists)
    size_t bytes = sizeof(HashTable);
    bytes += buckets.capacity() * sizeof(vector<Entry>);
    for (int i = 0; i < buckets.size(); i++) {
        bytes += buckets[i].capacity() * sizeof(Entry);
        for (int j = 0; j < buckets[i].size(); j++) {
            bytes += buckets[i][j].key.capacity();
            bytes += buckets[i][j].logIds.capacity() * sizeof(int);
        }
    }
    return bytes;
}
