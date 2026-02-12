#include <iostream>
#include <fstream>
using namespace std;

enum Color { RED, BLACK };

template <typename T>
class RBNode {
public:
    T data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    int size; // Size of subtree rooted at this node

    RBNode(T data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr), size(1) {}
};

template <typename T>
class RedBlackTree {
private:
    RBNode<T>* root;
    RBNode<T>* NIL;

    void leftRotate(RBNode<T>* x) {
        RBNode<T>* y = x->right;
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
        
        // Update sizes
        y->size = x->size;
        x->size = getSize(x->left) + getSize(x->right) + 1;
    }

    void rightRotate(RBNode<T>* y) {
        RBNode<T>* x = y->left;
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
        
        // Update sizes
        x->size = y->size;
        y->size = getSize(y->left) + getSize(y->right) + 1;
    }

    void insertFixup(RBNode<T>* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBNode<T>* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBNode<T>* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
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
        root->color = BLACK;
    }

    void transplant(RBNode<T>* u, RBNode<T>* v) {
        if (u->parent == NIL) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    RBNode<T>* minimum(RBNode<T>* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    void deleteFixup(RBNode<T>* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBNode<T>* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                RBNode<T>* w = x->parent->left;
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
        x->color = BLACK;
    }

    RBNode<T>* searchNode(T key) {
        RBNode<T>* current = root;
        while (current != NIL && current->data != key) {
            if (key < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    void updateSize(RBNode<T>* node) {
        while (node != NIL) {
            node->size = getSize(node->left) + getSize(node->right) + 1;
            node = node->parent;
        }
    }

    int getSize(RBNode<T>* node) {
        return (node == NIL) ? 0 : node->size;
    }

public:
    RedBlackTree() {
        NIL = new RBNode<T>(T());
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        NIL->size = 0;
        root = NIL;
    }

    bool insert(T key) {
        // Check if key already exists
        if (searchNode(key) != NIL) {
            return false;
        }

        RBNode<T>* z = new RBNode<T>(key);
        z->left = z->right = NIL;
        
        RBNode<T>* y = NIL;
        RBNode<T>* x = root;
        
        while (x != NIL) {
            y = x;
            y->size++; // Increment size for all nodes in the path
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;
        
        if (y == NIL) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        z->color = RED;
        insertFixup(z);
        return true;
    }

    bool remove(T key) {
        RBNode<T>* z = searchNode(key);
        if (z == NIL) {
            return false;
        }

        // Update sizes along the path
        RBNode<T>* temp = z->parent;
        while (temp != NIL) {
            temp->size--;
            temp = temp->parent;
        }

        RBNode<T>* y = z;
        RBNode<T>* x;
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
                // Update sizes from y's original position
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
        
        if (yOriginalColor == BLACK) {
            deleteFixup(x);
        }
        
        return true;
    }

    bool search(T key) {
        return searchNode(key) != NIL;
    }

    int countLessThan(T key) {
        int count = 0;
        RBNode<T>* current = root;
        
        while (current != NIL) {
            if (key <= current->data) {
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
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");
    
    int n;
    inFile >> n;
    
    RedBlackTree<int> rbt;
    
    outFile << n << endl;
    
    for (int i = 0; i < n; i++) {
        int ei, xi;
        inFile >> ei >> xi;
        
        int ri = 0;
        
        switch (ei) {
            case 0: // Terminate
                ri = rbt.remove(xi) ? 1 : 0;
                break;
            case 1: // Initiate
                ri = rbt.insert(xi) ? 1 : 0;
                break;
            case 2: // Search
                ri = rbt.search(xi) ? 1 : 0;
                break;
            case 3: // Count less than
                ri = rbt.countLessThan(xi);
                break;
        }
        
        outFile << ei << " " << xi << " " << ri << endl;
    }
    
    inFile.close();
    outFile.close();
    
    return 0;
}
