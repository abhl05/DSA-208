# Implementation Guide - Hash Table Algorithms

## 1. Prime Number Utilities

### Finding the Next Prime
```cpp
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
```
**Purpose**: Find the smallest prime number greater than n
**Used for**: Table expansion
**Example**: nextPrime(26) = 29

### Finding the Previous Prime
```cpp
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
```
**Purpose**: Find the largest prime number smaller than n
**Used for**: Table compaction
**Example**: previousPrime(26) = 23

## 2. Hash Functions Explained

### Hash1: DJB2 Algorithm
```cpp
unsigned long hash1(const string& key, int tableSize) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % tableSize;
}
```
**Properties**:
- Initial value: 5381 (magic number from original algorithm)
- Multiplier: 33 (optimal for string hashing)
- Left shift by 5 is equivalent to multiplication by 32
- Formula: hash = hash * 33 + character

**Why it works**: The prime multiplier (33) distributes bits well across the hash space

### Hash2: FNV-1a Algorithm
```cpp
unsigned long hash2(const string& key, int tableSize) {
    unsigned long hash = 2166136261u;  // FNV offset basis
    for (char c : key) {
        hash ^= c;
        hash *= 16777619;  // FNV prime
    }
    return hash % tableSize;
}
```
**Properties**:
- FNV offset basis: 2166136261
- FNV prime: 16777619
- Uses XOR before multiplication (FNV-1a variant)

**Why it works**: XOR with prime multiplication creates excellent avalanche effect

### Auxiliary Hash Function
```cpp
unsigned long auxHash(const string& key, int tableSize) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return (hash % (tableSize - 1)) + 1;  // Never returns 0
}
```
**Critical Property**: Must never return 0
**Range**: [1, tableSize-1]
**Why**: Zero step size would create infinite loop in probing

## 3. Chaining Implementation

### Insertion
```cpp
bool insert(K key, V value) {
    // 1. Check for duplicates
    if (search(key) != -1) {
        return false;
    }
    
    // 2. Calculate index
    int index = hashFunc(key, tableSize);
    
    // 3. Detect collision
    if (table[index] != nullptr) {
        collisions++;  // Slot already occupied
    }
    
    // 4. Insert at head of linked list
    Node<K, V>* newNode = new Node<K, V>(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    numElements++;
    
    // 5. Check for expansion
    double loadFactor = (double)numElements / tableSize;
    if (loadFactor > EXPANSION_LOAD_FACTOR && 
        insertionsSinceResize >= numElements / 2) {
        resize(nextPrime(2 * tableSize));
    }
    
    return true;
}
```

**Key Points**:
- Collisions counted only when slot is already occupied
- New nodes inserted at head (O(1) operation)
- Resize only after sufficient insertions

### Search
```cpp
int search(K key) {
    int index = hashFunc(key, tableSize);
    int probes = 0;
    
    Node<K, V>* current = table[index];
    while (current != nullptr) {
        probes++;
        if (current->key == key) {
            return probes;  // Found
        }
        current = current->next;
    }
    
    return -1;  // Not found
}
```

**Returns**: Number of probes (1 = found immediately, 2 = second node, etc.)

## 4. Open Addressing Implementation

### Double Hashing Probe Sequence
```cpp
int probe(const K& key, int i) {
    int h = hashFunc(key, tableSize);
    int aux = auxHash(key, tableSize);
    
    if (method == DOUBLE_HASH) {
        return (h + i * aux) % tableSize;
    }
}
```

**Sequence Example**:
- i=0: hash(key) mod N
- i=1: (hash(key) + auxHash(key)) mod N
- i=2: (hash(key) + 2*auxHash(key)) mod N
- ...

**Why it works**: Since N is prime and aux ∈ [1, N-1], we'll visit all slots

### Custom Probing Sequence
```cpp
int probe(const K& key, int i) {
    int h = hashFunc(key, tableSize);
    int aux = auxHash(key, tableSize);
    
    if (method == CUSTOM_PROBE) {
        return (h + C1 * i * aux + C2 * i * i) % tableSize;
    }
}
```

**Components**:
- Linear part: C1 * i * aux
- Quadratic part: C2 * i²
- Combines benefits of both approaches

### Insertion with Open Addressing
```cpp
bool insert(K key, V value) {
    int i = 0;
    int index;
    bool collisionOccurred = false;
    
    while (i < tableSize) {
        index = probe(key, i);
        
        // Can insert if slot is empty or deleted
        if (!table[index].occupied || table[index].deleted) {
            table[index].key = key;
            table[index].value = value;
            table[index].occupied = true;
            table[index].deleted = false;
            numElements++;
            
            if (collisionOccurred) {
                collisions++;  // Count collision
            }
            
            return true;
        }
        
        collisionOccurred = true;  // Had to probe
        i++;
    }
    
    return false;  // Table full
}
```

**Collision Counting**: Only incremented if we had to probe (i > 0)

### Deletion with Lazy Deletion
```cpp
bool remove(K key) {
    int i = 0;
    
    while (i < tableSize) {
        int index = probe(key, i);
        
        if (!table[index].occupied) {
            return false;  // Not found
        }
        
        if (table[index].occupied && 
            !table[index].deleted && 
            table[index].key == key) {
            
            table[index].deleted = true;  // Mark as deleted
            numElements--;
            
            // Check for compaction...
            
            return true;
        }
        
        i++;
    }
    
    return false;
}
```

**Lazy Deletion**: Mark deleted but keep occupied flag true
**Why**: Prevents breaking probe sequences for other keys

## 5. Dynamic Resizing Logic

### Expansion Condition
```cpp
double loadFactor = (double)numElements / tableSize;
if (loadFactor > EXPANSION_LOAD_FACTOR && 
    insertionsSinceResize >= numElements / 2) {
    
    int newSize = nextPrime(2 * tableSize);
    resize(newSize);
}
```

**Example**:
- Current size: 13, Elements: 7
- Load factor: 7/13 ≈ 0.54 > 0.5 ✓
- Need at least 7/2 = 3 insertions since last resize
- New size: nextPrime(26) = 29

### Compaction Condition
```cpp
double loadFactor = (double)numElements / tableSize;
if (loadFactor < COMPACTION_LOAD_FACTOR && 
    tableSize > INITIAL_TABLE_SIZE && 
    deletionsSinceResize >= numElements / 2) {
    
    int newSize = previousPrime(tableSize / 2);
    if (newSize >= INITIAL_TABLE_SIZE) {
        resize(newSize);
    }
}
```

**Example**:
- Current size: 29, Elements: 6
- Load factor: 6/29 ≈ 0.21 < 0.25 ✓
- Table size > initial size (29 > 13) ✓
- Need at least 6/2 = 3 deletions since last resize
- New size: previousPrime(14) = 13

### Resize Operation
```cpp
void resize(int newSize) {
    // 1. Save old table
    vector<Entry> oldTable = table;
    int oldSize = tableSize;
    
    // 2. Create new table
    table.clear();
    table.resize(newSize);
    tableSize = newSize;
    numElements = 0;
    
    // 3. Rehash all elements
    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i].occupied && !oldTable[i].deleted) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
    
    // 4. Reset counters
    insertionsSinceResize = 0;
    deletionsSinceResize = 0;
}
```

**Important**: All elements must be rehashed with new table size

## 6. Performance Testing

### Test Flow
```cpp
1. Generate 10,000 unique words
2. Insert all words into hash table
3. Record collision count
4. Randomly select 1,000 words
5. Search for each word
6. Calculate average probes
```

### Collision vs Probes
- **Collision**: Insertion encounters occupied slot
- **Probe**: Each table access during search
- **Hit**: Successful probe (found the key)

### Example Search
```
Table: [_, A, B, C, _, _, ...]
Search for C with Hash(C) = 1

Probe 1: table[1] = A (not C)
Probe 2: table[2] = B (not C)
Probe 3: table[3] = C (found!)

Result: 3 hits
```

## 7. Why Both Hash Functions?

Testing both hash functions allows comparison:
- Different distribution patterns
- Different collision rates
- Validates that implementation works with any hash function
- Shows impact of hash quality on performance

## 8. Expected Results Interpretation

### Chaining Usually Wins Because:
1. Lower collision count (only first collision per slot counted)
2. Fast search (only checks collided keys)
3. No clustering issues
4. No probe sequence complications

### Open Addressing Trade-offs:
1. Better cache locality (contiguous memory)
2. No pointer overhead
3. More sensitive to hash function quality
4. Can suffer from clustering

## Conclusion

This implementation demonstrates:
- Proper dynamic resizing with prime table sizes
- Three distinct collision resolution strategies
- Fair performance comparison methodology
- Clean, maintainable code structure
