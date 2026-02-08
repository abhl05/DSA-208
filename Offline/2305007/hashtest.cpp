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

template<typename HashTable>
int collisions(HashTable& ht, const vector<string>& words) {
    for (size_t i = 0; i < words.size(); i++) {
        ht.insert(words[i], (int)i + 1);
    }
    
    return ht.getCollisions();
}

template<typename HashTable>
double averageHits(HashTable& ht, const vector<string>& words) {
    vector<string> searchWords;
    mt19937 rng(7); 
    uniform_int_distribution<int> dist(0, words.size() - 1);
    
    for (int i = 0; i < 1000; i++) {
        searchWords.push_back(words[dist(rng)]);
    }
    
    int totalHits = 0;
    for (const string& word : searchWords) {
        int hits = ht.search(word);
        if (hits != -1) {
            totalHits += hits;
        }
    }
    
    double avgHits = (double)totalHits / 1000.0;
    return avgHits;
}

int main() {
    WordGenerator generator;
    vector<string> words;
    for (int i = 0; i < 10000; i++) {
        words.push_back(generator.generateWord(10));
    }
    HashTableChaining<string, int> chainHash1(hash1);
    int collisions_chain_h1 = collisions(chainHash1, words);
    double avgHits_chain_h1 = averageHits(chainHash1, words);
    
    // Chaining with Hash2
    HashTableChaining<string, int> chainHash2(hash2);
    int collisions_chain_h2 = collisions(chainHash2, words);
    double avgHits_chain_h2 = averageHits(chainHash2, words);
    
    // Double Hashing with Hash1
    HashTableOpenAddressing<string, int> doubleHash1(hash1, HashTableOpenAddressing<string, int>::DOUBLE_HASH);
    int collisions_double_h1 = collisions(doubleHash1, words);
    double avgHits_double_h1 = averageHits(doubleHash1, words);
    
    // Double Hashing with Hash2
    HashTableOpenAddressing<string, int> doubleHash2(hash2, HashTableOpenAddressing<string, int>::DOUBLE_HASH);
    int collisions_double_h2 = collisions(doubleHash2, words);
    double avgHits_double_h2 = averageHits(doubleHash2, words);
    
    // Custom Probing with Hash1
    HashTableOpenAddressing<string, int> customHash1(hash1, HashTableOpenAddressing<string, int>::CUSTOM_PROBE);
    int collisions_custom_h1 = collisions(customHash1, words);
    double avgHits_custom_h1 = averageHits(customHash1, words);
    
    // Custom Probing with Hash2
    HashTableOpenAddressing<string, int> customHash2(hash2, HashTableOpenAddressing<string, int>::CUSTOM_PROBE);
    int collisions_custom_h2 = collisions(customHash2, words);
    double avgHits_custom_h2 = averageHits(customHash2, words);
    
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
         << setw(14) << collisions_chain_h1 
         << setw(16) << fixed << setprecision(4) << avgHits_chain_h1 << " | "
         << setw(14) << collisions_chain_h2 
         << setw(16) << fixed << setprecision(4) << avgHits_chain_h2 << endl;
    
    cout << setw(20) << left << "Double Hashing" << " | "
         << setw(14) << collisions_double_h1 
         << setw(16) << fixed << setprecision(4) << avgHits_double_h1 << " | "
         << setw(14) << collisions_double_h2 
         << setw(16) << fixed << setprecision(4) << avgHits_double_h2 << endl;
    
    cout << setw(20) << left << "Custom Probing" << " | "
         << setw(14) << collisions_custom_h1 
         << setw(16) << fixed << setprecision(4) << avgHits_custom_h1 << " | "
         << setw(14) << collisions_custom_h2 
         << setw(16) << fixed << setprecision(4) << avgHits_custom_h2 << endl;
    
    return 0;
}
