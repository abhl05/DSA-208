#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <unordered_set>
#include "hashfunctions.hpp"
using namespace std;



#define CUT_OFF_LF 0.5
#define MIN_LF 0.25
#define INIT_SIZE 13    
#define UPDATE_LF load_factor = element_cnt / (double) table_size; 
#define C1 1
#define C2 3

bool isPrime(int n) {
    for(int i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            return false;
        }
    }
    return true;
}   

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

int prevPrime(int n) {
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

class WordGenerator {
private:
    mt19937 rng;
    unordered_set<string> generatedWords;
    
public:
    WordGenerator() {
        rng.seed(time(0));
    }
    
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

template<typename K, typename V>
struct Node {
    K key;
    V value;
    Node* next;
    Node(K k, V v) : key(k), value(v), next(nullptr) {}
};

template<typename K, typename V>
class HashTableChaining {
    int table_size;
    int element_cnt;
    double load_factor; 
    int collisions;
    int insert_cnt;
    int delete_cnt;
    vector<Node<K, V>*> table;
    unsigned long (*hashFunc)(const K&, int);


    void resize(int new_size) {
        vector<Node<K, V>*> oldTable = table;
        int oldSize = table_size;
        
        table.clear();
        table.resize(new_size, nullptr);
        table_size = new_size;
        element_cnt = 0;
        
        // Rehash 
        for (int i = 0; i < oldSize; i++) {
            Node<K, V>* current = oldTable[i];
            while (current != nullptr) {
                Node<K, V>* next = current->next;
                
                int idx = hashFunc(current->key, table_size);

                if (table[idx] != nullptr) {
                    collisions++;
                }

                // Insert at head 
                current->next = table[idx];
                table[idx] = current;
                element_cnt++;
                
                current = next;
            }
        }
        
        insert_cnt = 0;
        delete_cnt = 0;
    }


public:
    HashTableChaining(unsigned long (*hf)(const K&, int)) 
        : table_size(INIT_SIZE), element_cnt(0), collisions(0),
          insert_cnt(0), delete_cnt(0), hashFunc(hf) {
        table.resize(table_size, nullptr);
        UPDATE_LF;
    }

    ~HashTableChaining() {
        for (int i = 0; i < table_size; i++) {
            Node<K, V>* current = table[i];
            while (current != nullptr) {
                Node<K, V>* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    bool insert(K key, V value) {
        if (search(key) != -1) {
            return false;
        }

        int idx = hashFunc(key, table_size);

        if (table[idx] != nullptr) {
            collisions++;
        }

        // Insert at head
        Node<K, V>* newNode = new Node<K, V>(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        element_cnt++;
        insert_cnt++;

        UPDATE_LF;
        if (load_factor > CUT_OFF_LF && insert_cnt >= element_cnt / 2) {
            int new_size = nextPrime(2 * table_size);
            resize(new_size);
        }
        return true;
    }
    
    int search(K key) {
        int idx = hashFunc(key, table_size);
        int probes = 0;

        Node<K, V>* current = table[idx];
        while (current != nullptr) {
            probes++;
            if (current->key == key) {
                return probes;
            }
            current = current->next;
        }

        return -1;
    }

    bool remove(K key) {
        int idx = hashFunc(key, table_size);

        Node<K, V>* current = table[idx];
        Node<K, V>* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[idx] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                element_cnt--;
                delete_cnt++;
                
                UPDATE_LF;
                if (load_factor < MIN_LF && 
                    table_size > INIT_SIZE && 
                    delete_cnt >= element_cnt / 2) {
                    int new_size = prevPrime(table_size / 2);
                    if (new_size >= INIT_SIZE) {
                        resize(new_size);
                    }
                }
                
                return true;
            }
            prev = current;
            current = current->next;
        }
        
        return false; 
    }

    void print() const {
        cout << "\n--- Chaining Table (Size: " << table_size << ") ---" << endl;
        for (int i = 0; i < table_size; i++) {
            cout << "[" << i << "]: ";
            Node<K, V>* current = table[i];
            while (current != nullptr) {
                cout << "(" << current->key << ": " << current->value << ") -> ";
                current = current->next;
            }
            cout << "NULL" << endl;
        }
    }

    V& operator[](const K& key) {
        int idx = hashFunc(key, table_size);
        Node<K, V>* current = table[idx];
        while (current != nullptr) {
            if (current->key == key) return current->value;
            current = current->next;
        }
        // If not found, insert with default value
        insert(key, V());
        // Re-find after potential resize to return correct reference
        idx = hashFunc(key, table_size);
        return table[idx]->value;
    }
    int getCollisions() const { return collisions; }
    void resetCollisions() { collisions = 0; }
};

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
    int table_size;
    int element_cnt;
    int collisions;
    int insert_cnt;
    int delete_cnt;
    unsigned long (*hashFunc)(const K&, int);
    double load_factor;
    ProbingMethod method;
    
    int probe(const K& key, int i) {
        int h = hashFunc(key, table_size);
        int aux = auxHash(key, table_size);
        
        if (method == DOUBLE_HASH) {
            return (h + i * aux) % table_size;
        } else { 
            return (h + C1 * i * aux + C2 * i * i) % table_size;
        }
    }
    
    void resize(int new_size) {
        vector<Entry> oldTable = table;
        int oldSize = table_size;
        
        table.clear();
        table.resize(new_size);
        table_size = new_size;
        element_cnt = 0;
        
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        
        insert_cnt = 0;
        delete_cnt = 0;
    }
    
public:
    HashTableOpenAddressing(unsigned long (*hf)(const K&, int), ProbingMethod pm)
        : table_size(INIT_SIZE), element_cnt(0), collisions(0),
          insert_cnt(0), delete_cnt(0), hashFunc(hf), method(pm) {
        table.resize(table_size);
         UPDATE_LF;
    }
    
    bool insert(K key, V value) {
        if (search(key) != -1) {
            return false;
        }
        
        int i = 0;
        int idx;
        bool collision_check = false;
        
        while (i < table_size) {
            idx = probe(key, i);
            
            if (collision_check) {
                collisions++;
            }

            if (!table[idx].occupied || table[idx].deleted) {
                table[idx].key = key;
                table[idx].value = value;
                table[idx].occupied = true;
                table[idx].deleted = false;
                element_cnt++;
                insert_cnt++;
                
                UPDATE_LF;
                if (load_factor > CUT_OFF_LF && insert_cnt >= element_cnt / 2) {
                    int new_size = nextPrime(2 * table_size);
                    resize(new_size);
                }
                
                return true;
            }
            
            collision_check = true;
            i++;
        }
        
        return false; 
    }
    
    int search(K key) {
        int i = 0;
        int probes = 0;
        
        while (i < table_size) {
            int idx = probe(key, i);
            probes++;
            
            if (!table[idx].occupied) {
                return -1; 
            }
            
            if (table[idx].occupied && !table[idx].deleted && table[idx].key == key) {
                return probes;
            }
            
            i++;
        }
        
        return -1; 
    }
    
    bool remove(K key) {
        int i = 0;
        
        while (i < table_size) {
            int idx = probe(key, i);
            
            if (!table[idx].occupied) {
                return false; 
            }
            
            if (table[idx].occupied && !table[idx].deleted && table[idx].key == key) {
                table[idx].deleted = true;
                element_cnt--;
                delete_cnt++;
                
                UPDATE_LF;
                if (load_factor < MIN_LF && 
                    table_size > INIT_SIZE && 
                    delete_cnt >= element_cnt / 2) {
                    int new_size = prevPrime(table_size / 2);
                    if (new_size >= INIT_SIZE) {
                        resize(new_size);
                    }
                }
                
                return true;
            }
            
            i++;
        }
        
        return false; 
    }

    void print() const {
        cout << "\n--- Open Addressing Table (" << 
             (method == DOUBLE_HASH ? "Double" : "Custom") << " Size: " << table_size << ") ---" << endl;
        for (int i = 0; i < table_size; i++) {
            cout << "[" << i << "]: ";
            if (!table[i].occupied) cout << "<EMPTY>";
            else if (table[i].deleted) cout << "<DELETED>";
            else cout << "(" << table[i].key << ": " << table[i].value << ")";
            cout << endl;
        }
    }
    int getCollisions() const { return collisions; }
    void resetCollisions() { collisions = 0; }
};


