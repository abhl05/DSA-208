#pragma once
#include <string>

// --- Helper for numeric/POD types (int, double, etc.) ---
// These treat the raw memory of the object as a sequence of bytes.
template<typename K>
unsigned long get_hash1_raw(const K& key, int tableSize) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&key);
    unsigned long hash = 5381;
    for (size_t i = 0; i < sizeof(K); i++) {
        hash = ((hash << 5) + hash) + p[i];
    }
    return hash % tableSize;
}

template<typename K>
unsigned long get_hash2_raw(const K& key, int tableSize) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&key);
    unsigned long hash = 2166136261u;
    for (size_t i = 0; i < sizeof(K); i++) {
        hash ^= p[i];
        hash *= 16777619;
    }
    return hash % tableSize;
}

template<typename K>
unsigned long get_aux_raw(const K& key, int tableSize) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&key);
    unsigned long hash = 0;
    for (size_t i = 0; i < sizeof(K); i++) {
        hash = hash * 31 + p[i];
    }
    return (hash % (tableSize - 1)) + 1;
}

// --- Public Generic Interfaces ---

// Hash Function 1: DJB2
template<typename K>
unsigned long hash1(const K& key, int tableSize) {
    return get_hash1_raw(key, tableSize);
}
// Overload for string
inline unsigned long hash1(const std::string& key, int tableSize) {
    unsigned long hash = 5381;
    for (unsigned char c : key) hash = ((hash << 5) + hash) + c;
    return hash % tableSize;
}

// Hash Function 2: FNV-1a
template<typename K>
unsigned long hash2(const K& key, int tableSize) {
    return get_hash2_raw(key, tableSize);
}
// Overload for string
inline unsigned long hash2(const std::string& key, int tableSize) {
    unsigned long hash = 2166136261u;
    for (unsigned char c : key) { hash ^= c; hash *= 16777619; }
    return hash % tableSize;
}

// Auxiliary Hash
template<typename K>
unsigned long auxHash(const K& key, int tableSize) {
    return get_aux_raw(key, tableSize);
}
// Overload for string
inline unsigned long auxHash(const std::string& key, int tableSize) {
    unsigned long hash = 0;
    for (unsigned char c : key) hash = hash * 31 + c;
    return (hash % (tableSize - 1)) + 1;
}