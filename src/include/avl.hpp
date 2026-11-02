#ifndef ARVORE_AVL_HPP
#define ARVORE_AVL_HPP

#include <algorithm>
#include "veb.hpp"  

struct AVLNode {
    int key;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int val) : key(val), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* n) { return n ? n->height : 0; }
    int getBalance(AVLNode* n) { return n ? getHeight(n->left) - getHeight(n->right) : 0; }
    void updateHeight(AVLNode* n) { if (n) n->height = 1 + std::max(getHeight(n->left), getHeight(n->right)); }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    AVLNode* insertHelper(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);
        if (key < node->key) node->left = insertHelper(node->left, key);
        else if (key > node->key) node->right = insertHelper(node->right, key);
        else return node;

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    AVLNode* getMinValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->left) current = current->left;
        return current;
    }

    AVLNode* getMaxValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->right) current = current->right;
        return current;
    }

    AVLNode* deleteHelper(AVLNode* node, int key) {
        if (!node) return node;
        if (key < node->key) node->left = deleteHelper(node->left, key);
        else if (key > node->key) node->right = deleteHelper(node->right, key);
        else {
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                if (!temp) { temp = node; node = nullptr; }
                else *node = *temp;
                delete temp;
            } else {
                AVLNode* temp = getMinValueNode(node->right);
                node->key = temp->key;
                node->right = deleteHelper(node->right, temp->key);
            }
        }
        if (!node) return node;

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) return rightRotate(node);
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) return leftRotate(node);
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void destroy(AVLNode* node) {
        if (node) { destroy(node->left); destroy(node->right); delete node; }
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(int key) { root = insertHelper(root, key); }
    void remove(int key) { root = deleteHelper(root, key); }

    bool search(int key) const {
        AVLNode* curr = root;
        while (curr) {
            if (curr->key == key) return true;
            curr = (key < curr->key) ? curr->left : curr->right;
        }
        return false;
    }

    int minimum() const {
        if (!root) return vEB_NIL;
        AVLNode* curr = root;
        while (curr->left) curr = curr->left;
        return curr->key;
    }

    int maximum() const {
        if (!root) return vEB_NIL;
        AVLNode* curr = root;
        while (curr->right) curr = curr->right;
        return curr->key;
    }

    int successor(int x) const {
        AVLNode* curr = root;
        AVLNode* succ = nullptr;
        while (curr) {
            if (curr->key > x) {
                succ = curr;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return succ ? succ->key : vEB_NIL;
    }

    int predecessor(int x) const {
        AVLNode* curr = root;
        AVLNode* pred = nullptr;
        while (curr) {
            if (curr->key < x) {
                pred = curr;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
        }
        return pred ? pred->key : vEB_NIL;
    }
};

#endif
