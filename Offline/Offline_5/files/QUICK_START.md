# Quick Start Guide - Hash Table Implementation

## Files Included

1. **hash_table.cpp** - Complete implementation (750+ lines)
2. **Makefile** - Build automation
3. **README.md** - Comprehensive documentation
4. **IMPLEMENTATION_GUIDE.md** - Detailed algorithm explanations

## How to Run

### Option 1: Using Make (Recommended)
```bash
make run
```

### Option 2: Manual Compilation
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o hash_table hash_table.cpp
./hash_table
```

## What You'll See

The program will:
1. Generate 10,000 unique random words (length 10)
2. Test hash function uniqueness
3. Run 6 performance tests (3 methods × 2 hash functions)
4. Display results in a formatted table

Execution time: ~5-10 seconds (depending on system)

## Modifying Parameters

Edit these constants in `hash_table.cpp` (lines 15-20):

```cpp
const int INITIAL_TABLE_SIZE = 13;       // Initial size (must be prime)
const double EXPANSION_LOAD_FACTOR = 0.5; // Expand threshold
const double COMPACTION_LOAD_FACTOR = 0.25; // Compact threshold
const int C1 = 1;  // Custom probing linear coefficient
const int C2 = 3;  // Custom probing quadratic coefficient
```

After changes:
```bash
make clean
make run
```

## Understanding the Output

### Hash Function Statistics
```
Hash1 unique values: 6341 / 10000 (63.4%)
```
This shows Hash1 creates 6341 unique hash values for 10000 keys.
**Requirement**: ≥60% uniqueness ✓

### Performance Table
```
Method               | Hash1                          | Hash2
                     | Collisions    Avg Hits         | Collisions    Avg Hits
-------------------------------------------------------------------------------------
Chaining             | 2963          1.1480           | 2896          1.1290
```

- **Collisions**: Number of times insertion encountered an occupied slot
  - Lower is better
  - Chaining typically has fewer collisions

- **Avg Hits**: Average number of table accesses per successful search
  - Lower is better (1.0 = perfect, found immediately)
  - Values of 1.1-1.2 are excellent

## Typical Results Interpretation

### Chaining Method
- **Best overall performance** in most cases
- Low collision count (only first collision per bucket)
- Fast searches (O(1 + α) where α is load factor)

### Double Hashing
- More collisions than chaining (every probe counts)
- Good for cache locality
- No extra pointer storage

### Custom Probing
- Similar to double hashing
- Quadratic component reduces primary clustering
- May outperform double hashing with good constants

## Testing with Different Word Lengths

To test with different word lengths, modify line 535:
```cpp
words.push_back(generator.generateWord(10));  // Change 10 to desired length
```

## Troubleshooting

### "command not found: make"
Use manual compilation instead (Option 2)

### "Segmentation fault"
Unlikely with this implementation, but check:
- C++11 support in compiler
- Sufficient system memory

### Unexpected Performance
- Results vary with random word generation
- Run multiple times for average behavior
- Different word lengths affect hash distribution

## Assignment Requirements Checklist

✅ Dynamic resizing (expansion and compaction)
✅ Prime table sizes (always)
✅ Single-source configuration variables
✅ Generic key-value pairs (templates)
✅ Random word generator with uniqueness
✅ Duplicate word handling
✅ Two hash functions (Hash1: DJB2, Hash2: FNV-1a)
✅ ≥60% unique hash values achieved
✅ Three collision resolution methods
✅ Chaining with linked lists
✅ Double hashing implementation
✅ Custom probing with C₁ and C₂
✅ Performance testing (10,000 insertions)
✅ Search testing (1,000 random searches)
✅ Tabular results format

## Code Quality Features

- **Well-commented**: Every major section explained
- **Modular design**: Separate classes for each method
- **Template-based**: Works with any key-value types
- **Memory safe**: Proper cleanup in destructors
- **Const-correct**: Read-only operations marked const
- **Standard compliance**: C++11 standard

## Performance Expectations

For 10,000 words (length 10):
- Generation: < 1 second
- All 6 tests: 5-10 seconds total
- Memory usage: < 5 MB

## Next Steps

1. Run the program to verify it works
2. Read README.md for detailed documentation
3. Study IMPLEMENTATION_GUIDE.md for algorithm details
4. Experiment with different parameters
5. Analyze the performance results

## Support

For questions or issues:
1. Check the error messages
2. Review README.md and IMPLEMENTATION_GUIDE.md
3. Verify compilation flags match requirements
4. Test with smaller datasets first (e.g., 100 words)

## Academic Integrity

This implementation is provided as a reference for understanding hash table concepts. Make sure you understand every part of the code before submission.

---
Happy Coding! 🚀
