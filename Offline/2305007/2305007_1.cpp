#include <bits/stdc++.h>
using namespace std;

enum Color { RED, BLACK };

template<typename K, typename V = K>
class RBTree {
    struct RBNode {
        K key;
        V value;
        Color color;
        RBNode* left;
        RBNode* right;
        RBNode* parent;
        int size; // size of subtree rooted at this node

        RBNode(K key, V value) : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr), size(1) {}
        RBNode(K k) : key(k), value(k), color(RED), left(nullptr), right(nullptr), parent(nullptr), size(1) {}
        RBNode() : key(K()), value(V()), color(RED), left(nullptr), right(nullptr), parent(nullptr), size(1) {}
    };

    RBNode* root;
    RBNode* NIL;

    void leftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        
        if (y->left != NIL) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == NIL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
        
        y->size = x->size;
        x->size = getSize(x->left) + getSize(x->right) + 1;
    }

    void rightRotate(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        
        if (x->right != NIL) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        
        if (y->parent == NIL) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        
        x->right = y;
        y->parent = x;
        
        x->size = y->size;
        y->size = getSize(y->left) + getSize(y->right) + 1;
    }

    void insertFixup(RBNode* z) { // case 1.a, 1.b, 2
        while (z->parent->color == RED) { // while red-red violation
            if (z->parent == z->parent->parent->left) { // inserted node is in left subtree
                RBNode* uncle = z->parent->parent->right;
                if (uncle->color == RED) { // if uncle is red, swap colors and move up the tree(case 2)
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent; // move up the tree
                } else { // if uncle is black, perform rotations
                    if (z == z->parent->right) { // if z is right child, left rotate at parent, make it a left-left case(case 1.b)
                        z = z->parent;
                        leftRotate(z);
                    } // swap color between parent and grandparent, then right rotate at grandparent(case 1.a)
                    z->parent->color = BLACK; 
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else { // inserted node is in right subtree (mirror case)
                RBNode* uncle = z->parent->parent->left;
                if (uncle->color == RED) { // if uncle is red, swap colors and move up the tree
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else { // if uncle is black, perform rotations
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK; // Ensure the root is always black
    }

    void deleteFixup(RBNode* x) {
        while (x != root && x->color == BLACK) { // while x is not root and x is black. if root and black, done(case 2)
            if (x == x->parent->left) { // x is left child
                RBNode* sibling = x->parent->right;
                if (sibling->color == RED) { // if sibling is red, rotate(p, s) and swap color of p and s(case 3)
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    sibling = x->parent->right; // update sibling
                }
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) { 
                    // if both of sibling's children are black, recolor sibling to red and move up the tree(case 4.3)
                    // only case that goes up the tree
                    sibling->color = RED;
                    x = x->parent;
                } else {
                    if (sibling->right->color == BLACK) { 
                        // if sibling's right child is black, rotate(s, left) and swap colors(case 4.2)
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rightRotate(sibling);
                        sibling = x->parent->right;
                    }
                    sibling->color = x->parent->color; // swap colors between parent and sibling, then rotate(p, right)(case 4.1)
                    x->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                RBNode* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK; // if red just recolor to black(case 1)
    }

    RBNode* searchNode(K key) {
        RBNode* current = root;
        while (current != NIL && current->key != key) {
            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    int getSize(RBNode* node) {
        return (node == NIL) ? 0 : node->size;
    }

    void transplant(RBNode* u, RBNode* v) {
        if (u->parent == NIL) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    RBNode* minimum(RBNode* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

public:
    RBTree() {
        NIL = new RBNode();
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        NIL->size = 0;
        root = NIL;
    }

    bool insert(K key, V value) {
        if (searchNode(key) != NIL) { 
            return false; 
        }

        RBNode* z = new RBNode(key, value);
        z->left = z->right = z->parent = NIL;

        RBNode* y = NIL;
        RBNode* x = root;

        while (x != NIL) {
            y = x;
            y->size++; 
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == NIL) {
            root = z; 
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->color = RED;
        insertFixup(z);
        return true;
    }

    bool remove(K key) {
        RBNode* z = searchNode(key);
        if (z == NIL) {
            return false;
        }

        RBNode* temp = z->parent;
        while (temp != NIL) {
            temp->size--;
            temp = temp->parent;
        }

        RBNode* y = z;
        RBNode* x;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                temp = y->parent;
                while (temp != z) {
                    temp->size--;
                    temp = temp->parent;
                }
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            y->size = getSize(y->left) + getSize(y->right) + 1;
        }

        delete z;

        if (yOriginalColor == BLACK) { // fix black height violation
            deleteFixup(x);
        }
        return true;
    }

    bool search(K key) {
        return searchNode(key) != NIL;
    }

    int countLessThan(K key) {
        int count = 0;
        RBNode* current = root;
        
        while (current != NIL) {
            if (key <= current->key) {
                current = current->left;
            } else {
                // All nodes in left subtree + current node are less than key
                count += getSize(current->left) + 1;
                current = current->right;
            }
        }
        
        return count;
    }
};

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int tt;
    cin >> tt;
    cout << tt << endl;
    RBTree<int> rbt;
    int res;

    for (int i = 0; i < tt; i++) {
        int e, x;
        cin >> e >> x;
        switch (e) {
            case 1: 
                res = rbt.insert(x, x) ? 1 : 0;
                break;
            case 0: 
                res = rbt.remove(x) ? 1 : 0;
                break;
            case 2:
                res = rbt.search(x) ? 1 : 0;
                break;
            case 3:
                res = rbt.countLessThan(x);
                break;
        }
        cout << e << " " << x << " " << res << endl;
    }

    cin.close();
    cout.close();

    return 0;
}