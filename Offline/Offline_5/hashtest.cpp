#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <unordered_set>
#include "hashtable.hpp"
#include "hashfunctions.hpp"
using namespace std;

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

int main() {
    WordGenerator generator;
    vector<string> words;
    for (int i = 0; i < 10000; i++) {
        words.push_back(generator.generateWord(10));
    }
    
    // // Test uniqueness of hash functions
    // unordered_set<int> uniqueHashes1, uniqueHashes2;
    // for (const string& word : words) {
    //     uniqueHashes1.insert(hash1(word, 10000));
    //     uniqueHashes2.insert(hash2(word, 10000));
    // }
    
    // cout << "Hash Function Statistics:" << endl;
    // cout << "  Hash1 unique values: " << uniqueHashes1.size() << " / 10000 (" 
    //      << fixed << setprecision(1) << (uniqueHashes1.size() * 100.0 / 10000) << "%)" << endl;
    // cout << "  Hash2 unique values: " << uniqueHashes2.size() << " / 10000 (" 
    //      << fixed << setprecision(1) << (uniqueHashes2.size() * 100.0 / 10000) << "%)" << endl;
    // cout << endl;
    
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
    cout << "PERFORMANCE REPORT" << endl;
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
    
    return 0;
}
