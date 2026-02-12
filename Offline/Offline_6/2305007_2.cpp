#include <bits/stdc++.h>
using namespace std;

template<typename K, typename V = K>
class AVLTree {
public:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;

        Node(K key, V value) : key(key), value(value), left(nullptr), right(nullptr), height(1) {}

        Node(K k) : key(k), value(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    AVLTree() : root(nullptr), node_count(0) {}

    int node_count;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + max(height(y->left), height(y->right));
        x->height = 1 + max(height(x->left), height(x->right));

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));

        return y;
    }

    Node* insert(Node* node, K key, int& is_inserted) {
        if (!node) {
            is_inserted = 1;
            return new Node(key);
        }

        if (key < node->key)
            node->left = insert(node->left, key, is_inserted);
        else if (key > node->key)
            node->right = insert(node->right, key, is_inserted);
        else {
            return node; // Duplicate keys not allowed
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    int insert(K key) {
        int inserted = 0;
        root = insert(root, key, inserted);
        if (inserted) node_count++;
        return inserted;
    }

    void preOrder(Node* node) {
        if (node) {
            cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void preOrder() {
        preOrder(root);
    }

    void inOrder(Node* node) {
        if (node) {
            inOrder(node->left);
            cout << node->key << " ";
            inOrder(node->right);
        }
    }

    void inOrder() {
        inOrder(root);
    }

    void postOrder(Node* node) {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->key << " ";
        }
    }

    void postOrder() {
        postOrder(root);
    }

    Node* remove(Node* node, K key, int& is_removed) {
        if (!node)
            return node;

        if (key < node->key)
            node->left = remove(node->left, key, is_removed);
        else if (key > node->key)
            node->right = remove(node->right, key, is_removed);
        else {
            is_removed = 1;
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;

                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else
                    *node = *temp;

                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = remove(node->right, temp->key, is_removed);
            }
        }

        if (!node) {
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    int remove(K key) {
        int is_removed = 0;
        root = remove(root, key, is_removed);
        if (is_removed) node_count--;
        return is_removed;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    void levelOrder() {
        if (!root)
            return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            cout << node->key << " ";

            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }
    }

};


int main() {
    AVLTree <int> tree;
    int tt;
    cin >> tt;
    cout << tt << endl;
    while (tt--) {
        int e, x;
        cin >> e >> x;
        if (e == 0) {
            cout << e << " " << x << " " << tree.remove(x) << endl;
        }
        else if (e == 1) {
            cout << e << " " << x << " " << tree.insert(x) << endl;
        } else if (e == 2) {
            switch (x) {
                case 1:
                    tree.preOrder();
                    cout << endl;
                    break;
                case 2:
                    tree.inOrder();
                    cout << endl;
                    break;
                case 3:
                    tree.postOrder();
                    cout << endl;
                    break;
                case 4:
                    tree.levelOrder();
                    cout << endl;
                    break;
            }
        }
        else break;
    }

    return 0;
}