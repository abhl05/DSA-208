#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <unordered_set>

using namespace std;

// ============================================================================
// CONFIGURATION CONSTANTS (Single-source variables for easy modification)
// ============================================================================
const int INITIAL_TABLE_SIZE = 13;
const double EXPANSION_LOAD_FACTOR = 0.5;
const double COMPACTION_LOAD_FACTOR = 0.25;

// Custom probing constants
const int C1 = 1;
const int C2 = 3;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Find the smallest prime greater than n
int nextPrime(int n) {
    if (n <= 1) return 2;
    
    int prime = n;
    bool found = false;
    
    while (!found) {
        prime++;
        if (isPrime(prime))
            found = true;
    }
    
    return prime;
}

// Find the largest prime smaller than n
int previousPrime(int n) {
    if (n <= 2) return 2;
    
    int prime = n;
    bool found = false;
    
    while (!found) {
        prime--;
        if (prime < 2) return 2;
        if (isPrime(prime))
            found = true;
    }
    
    return prime;
}

// Random word generator
class WordGenerator {
private:
    mt19937 rng;
    unordered_set<string> generatedWords;
    
public:
    WordGenerator() {
        rng.seed(time(0));
    }
    
    // Generate a random word of length n
    string generateWord(int length) {
        string word;
        const string chars = "abcdefghijklmnopqrstuvwxyz";
        
        do {
            word.clear();
            for (int i = 0; i < length; i++) {
                word += chars[rng() % chars.length()];
            }
        } while (generatedWords.find(word) != generatedWords.end());
        
        generatedWords.insert(word);
        return word;
    }
    
    void reset() {
        generatedWords.clear();
    }
};

// ============================================================================
// HASH FUNCTIONS
// ============================================================================

// Hash Function 1: DJB2 Hash
unsigned long hash1(const string& key, int tableSize) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % tableSize;
}

// Hash Function 2: FNV-1a Hash
unsigned long hash2(const string& key, int tableSize) {
    unsigned long hash = 2166136261u;
    for (char c : key) {
        hash ^= c;
        hash *= 16777619;
    }
    return hash % tableSize;
}

// Auxiliary hash function (must return non-zero value)
unsigned long auxHash(const string& key, int tableSize) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    // Ensure it's never 0 and relatively prime to tableSize
    return (hash % (tableSize - 1)) + 1;
}

// ============================================================================
// NODE STRUCTURE FOR CHAINING
// ============================================================================
template<typename K, typename V>
struct Node {
    K key;
    V value;
    Node* next;
    
    Node(K k, V v) : key(k), value(v), next(nullptr) {}
};

// ============================================================================
// HASH TABLE WITH CHAINING
// ============================================================================
template<typename K, typename V>
class HashTableChaining {
private:
    vector<Node<K, V>*> table;
    int tableSize;
    int numElements;
    int collisions;
    int insertionsSinceResize;
    int deletionsSinceResize;
    unsigned long (*hashFunc)(const K&, int);
    
    void resize(int newSize) {
        vector<Node<K, V>*> oldTable = table;
        int oldSize = tableSize;
        
        table.clear();
        table.resize(newSize, nullptr);
        tableSize = newSize;
        numElements = 0;
        
        // Rehash all elements
        for (int i = 0; i < oldSize; i++) {
            Node<K, V>* current = oldTable[i];
            while (current != nullptr) {
                Node<K, V>* next = current->next;
                
                // Insert into new table
                int index = hashFunc(current->key, tableSize);

                // count collision if slot is occupied
                if (table[index] != nullptr) {
                    collisions++;
                }

                // Insert at head of the list
                current->next = table[index];
                table[index] = current;
                numElements++;
                
                current = next;
            }
        }
        
        insertionsSinceResize = 0;
        deletionsSinceResize = 0;
    }
    
public:
    HashTableChaining(unsigned long (*hf)(const K&, int)) 
        : tableSize(INITIAL_TABLE_SIZE), numElements(0), collisions(0),
          insertionsSinceResize(0), deletionsSinceResize(0), hashFunc(hf) {
        table.resize(tableSize, nullptr);
    }
    
    ~HashTableChaining() {
        for (int i = 0; i < tableSize; i++) {
            Node<K, V>* current = table[i];
            while (current != nullptr) {
                Node<K, V>* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
    
    bool insert(K key, V value) {
        // Check if key already exists
        if (search(key) != -1) {
            return false; // Duplicate key
        }
        
        int index = hashFunc(key, tableSize);
        
        // Check for collision
        if (table[index] != nullptr) {
            collisions++;
        }
        
        // Insert at the beginning of the list
        Node<K, V>* newNode = new Node<K, V>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        numElements++;
        insertionsSinceResize++;
        
        // Check for expansion
        double loadFactor = (double)numElements / tableSize;
        if (loadFactor > EXPANSION_LOAD_FACTOR && insertionsSinceResize >= numElements / 2) {
            int newSize = nextPrime(2 * tableSize);
            resize(newSize);
        }
        
        return true;
    }
    
    int search(K key) {
        int index = hashFunc(key, tableSize);
        int probes = 0;
        
        Node<K, V>* current = table[index];
        while (current != nullptr) {
            probes++;
            if (current->key == key) {
                return probes;
            }
            current = current->next;
        }
        
        return -1; // Not found
    }
    
    bool remove(K key) {
        int index = hashFunc(key, tableSize);
        
        Node<K, V>* current = table[index];
        Node<K, V>* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                numElements--;
                deletionsSinceResize++;
                
                // Check for compaction
                double loadFactor = (double)numElements / tableSize;
                if (loadFactor < COMPACTION_LOAD_FACTOR && 
                    tableSize > INITIAL_TABLE_SIZE && 
                    deletionsSinceResize >= numElements / 2) {
                    int newSize = previousPrime(tableSize / 2);
                    if (newSize >= INITIAL_TABLE_SIZE) {
                        resize(newSize);
                    }
                }
                
                return true;
            }
            prev = current;
            current = current->next;
        }
        
        return false; // Not found
    }
    
    int getCollisions() const { return collisions; }
    void resetCollisions() { collisions = 0; }
};

// ============================================================================
// HASH TABLE WITH OPEN ADDRESSING (Double Hashing & Custom Probing)
// ============================================================================
template<typename K, typename V>
class HashTableOpenAddressing {
public:
    enum ProbingMethod { DOUBLE_HASH, CUSTOM_PROBE };

private:
    struct Entry {
        K key;
        V value;
        bool occupied;
        bool deleted;
        
        Entry() : occupied(false), deleted(false) {}
    };
    
    vector<Entry> table;
    int tableSize;
    int numElements;
    int collisions;
    int insertionsSinceResize;
    int deletionsSinceResize;
    unsigned long (*hashFunc)(const K&, int);
    ProbingMethod method;
    
    int probe(const K& key, int i) {
        int h = hashFunc(key, tableSize);
        int aux = auxHash(key, tableSize);
        
        if (method == DOUBLE_HASH) {
            return (h + i * aux) % tableSize;
        } else { // CUSTOM_PROBE
            return (h + C1 * i * aux + C2 * i * i) % tableSize;
        }
    }
    
    void resize(int newSize) {
        vector<Entry> oldTable = table;
        int oldSize = tableSize;
        
        table.clear();
        table.resize(newSize);
        tableSize = newSize;
        numElements = 0;
        
        // Rehash all elements
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        
        insertionsSinceResize = 0;
        deletionsSinceResize = 0;
    }
    
public:
    HashTableOpenAddressing(unsigned long (*hf)(const K&, int), ProbingMethod pm)
        : tableSize(INITIAL_TABLE_SIZE), numElements(0), collisions(0),
          insertionsSinceResize(0), deletionsSinceResize(0), hashFunc(hf), method(pm) {
        table.resize(tableSize);
    }
    
    bool insert(K key, V value) {
        // Check if key already exists
        if (search(key) != -1) {
            return false; // Duplicate key
        }
        
        int i = 0;
        int index;
        bool collisionOccurred = false;
        
        while (i < tableSize) {
            index = probe(key, i);
            
            if (!table[index].occupied || table[index].deleted) {
                table[index].key = key;
                table[index].value = value;
                table[index].occupied = true;
                table[index].deleted = false;
                numElements++;
                insertionsSinceResize++;
                
                if (collisionOccurred) {
                    collisions++;
                }
                
                // Check for expansion
                double loadFactor = (double)numElements / tableSize;
                if (loadFactor > EXPANSION_LOAD_FACTOR && insertionsSinceResize >= numElements / 2) {
                    int newSize = nextPrime(2 * tableSize);
                    resize(newSize);
                }
                
                return true;
            }
            
            collisionOccurred = true;
            i++;
        }
        
        return false; // Table full
    }
    
    int search(K key) {
        int i = 0;
        int probes = 0;
        
        while (i < tableSize) {
            int index = probe(key, i);
            probes++;
            
            if (!table[index].occupied) {
                return -1; // Not found
            }
            
            if (table[index].occupied && !table[index].deleted && table[index].key == key) {
                return probes;
            }
            
            i++;
        }
        
        return -1; // Not found
    }
    
    bool remove(K key) {
        int i = 0;
        
        while (i < tableSize) {
            int index = probe(key, i);
            
            if (!table[index].occupied) {
                return false; // Not found
            }
            
            if (table[index].occupied && !table[index].deleted && table[index].key == key) {
                table[index].deleted = true;
                numElements--;
                deletionsSinceResize++;
                
                // Check for compaction
                double loadFactor = (double)numElements / tableSize;
                if (loadFactor < COMPACTION_LOAD_FACTOR && 
                    tableSize > INITIAL_TABLE_SIZE && 
                    deletionsSinceResize >= numElements / 2) {
                    int newSize = previousPrime(tableSize / 2);
                    if (newSize >= INITIAL_TABLE_SIZE) {
                        resize(newSize);
                    }
                }
                
                return true;
            }
            
            i++;
        }
        
        return false; // Not found
    }
    
    int getCollisions() const { return collisions; }
    void resetCollisions() { collisions = 0; }
};

// ============================================================================
// PERFORMANCE TESTING
// ============================================================================

struct PerformanceMetrics {
    int collisions;
    double avgHits;
};

template<typename HashTable>
PerformanceMetrics testHashTable(HashTable& ht, const vector<string>& words) {
    // Insert all words
    for (size_t i = 0; i < words.size(); i++) {
        ht.insert(words[i], (int)i + 1);
    }
    
    int collisions = ht.getCollisions();
    
    // Randomly select 1000 words for search
    vector<string> searchWords;
    mt19937 rng(42); // Fixed seed for reproducibility
    uniform_int_distribution<int> dist(0, words.size() - 1);
    
    for (int i = 0; i < 1000; i++) {
        searchWords.push_back(words[dist(rng)]);
    }
    
    // Perform searches
    int totalHits = 0;
    for (const string& word : searchWords) {
        int hits = ht.search(word);
        if (hits != -1) {
            totalHits += hits;
        }
    }
    
    double avgHits = (double)totalHits / 1000.0;
    
    return {collisions, avgHits};
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "Hash Table Performance Evaluation" << endl;
    cout << "========================================" << endl;
    cout << "Configuration:" << endl;
    cout << "  Initial Table Size: " << INITIAL_TABLE_SIZE << endl;
    cout << "  Expansion Load Factor: " << EXPANSION_LOAD_FACTOR << endl;
    cout << "  Compaction Load Factor: " << COMPACTION_LOAD_FACTOR << endl;
    cout << "  Number of Words: 10,000" << endl;
    cout << "  Word Length: 10" << endl;
    cout << "  Search Sample Size: 1,000" << endl;
    cout << "========================================" << endl << endl;
    
    // Generate 10,000 unique words
    WordGenerator generator;
    vector<string> words;
    
    cout << "Generating 10,000 unique words..." << endl;
    for (int i = 0; i < 10000; i++) {
        words.push_back(generator.generateWord(10));
    }
    cout << "Word generation complete!" << endl << endl;
    
    // Test uniqueness of hash functions
    unordered_set<int> uniqueHashes1, uniqueHashes2;
    for (const string& word : words) {
        uniqueHashes1.insert(hash1(word, 10000));
        uniqueHashes2.insert(hash2(word, 10000));
    }
    
    cout << "Hash Function Statistics:" << endl;
    cout << "  Hash1 unique values: " << uniqueHashes1.size() << " / 10000 (" 
         << fixed << setprecision(1) << (uniqueHashes1.size() * 100.0 / 10000) << "%)" << endl;
    cout << "  Hash2 unique values: " << uniqueHashes2.size() << " / 10000 (" 
         << fixed << setprecision(1) << (uniqueHashes2.size() * 100.0 / 10000) << "%)" << endl;
    cout << endl;
    
    // Performance testing
    cout << "Running performance tests..." << endl << endl;
    
    // Chaining with Hash1
    HashTableChaining<string, int> chainHash1(hash1);
    auto metrics_chain_h1 = testHashTable(chainHash1, words);
    
    // Chaining with Hash2
    HashTableChaining<string, int> chainHash2(hash2);
    auto metrics_chain_h2 = testHashTable(chainHash2, words);
    
    // Double Hashing with Hash1
    HashTableOpenAddressing<string, int> doubleHash1(hash1, HashTableOpenAddressing<string, int>::DOUBLE_HASH);
    auto metrics_double_h1 = testHashTable(doubleHash1, words);
    
    // Double Hashing with Hash2
    HashTableOpenAddressing<string, int> doubleHash2(hash2, HashTableOpenAddressing<string, int>::DOUBLE_HASH);
    auto metrics_double_h2 = testHashTable(doubleHash2, words);
    
    // Custom Probing with Hash1
    HashTableOpenAddressing<string, int> customHash1(hash1, HashTableOpenAddressing<string, int>::CUSTOM_PROBE);
    auto metrics_custom_h1 = testHashTable(customHash1, words);
    
    // Custom Probing with Hash2
    HashTableOpenAddressing<string, int> customHash2(hash2, HashTableOpenAddressing<string, int>::CUSTOM_PROBE);
    auto metrics_custom_h2 = testHashTable(customHash2, words);
    
    // Print results in tabular format
    cout << "========================================" << endl;
    cout << "PERFORMANCE RESULTS" << endl;
    cout << "========================================" << endl << endl;
    
    cout << setw(20) << left << "Method" << " | "
         << setw(30) << "Hash1" << " | "
         << setw(30) << "Hash2" << endl;
    cout << setw(20) << "" << " | "
         << setw(14) << "Collisions" << setw(16) << "Avg Hits" << " | "
         << setw(14) << "Collisions" << setw(16) << "Avg Hits" << endl;
    cout << string(85, '-') << endl;
    
    cout << setw(20) << left << "Chaining" << " | "
         << setw(14) << metrics_chain_h1.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_chain_h1.avgHits << " | "
         << setw(14) << metrics_chain_h2.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_chain_h2.avgHits << endl;
    
    cout << setw(20) << left << "Double Hashing" << " | "
         << setw(14) << metrics_double_h1.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_double_h1.avgHits << " | "
         << setw(14) << metrics_double_h2.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_double_h2.avgHits << endl;
    
    cout << setw(20) << left << "Custom Probing" << " | "
         << setw(14) << metrics_custom_h1.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_custom_h1.avgHits << " | "
         << setw(14) << metrics_custom_h2.collisions 
         << setw(16) << fixed << setprecision(4) << metrics_custom_h2.avgHits << endl;
    
    cout << endl;
    cout << "========================================" << endl;
    cout << "Notes:" << endl;
    cout << "- Collisions: Number of times keys hashed to occupied slots" << endl;
    cout << "- Avg Hits: Average probes needed to find a key in search" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
