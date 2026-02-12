#include <bits/stdc++.h>
using namespace std;

enum Color { RED, BLACK };

template<typename K, typename V = K>
class RBTree {
    struct Node {
        K key;
        V value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(K key, V value) : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
        Node(K k) : key(k), value(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    RBTree() : root(nullptr) {}
};