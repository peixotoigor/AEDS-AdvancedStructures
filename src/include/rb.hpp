#ifndef ARVORE_RUBRO_NEGRA_HPP
#define ARVORE_RUBRO_NEGRA_HPP

#include "veb.hpp"

enum Color { RED, BLACK };

struct RBNode {
    int data;
    Color color;
    RBNode *left, *right, *parent;
    RBNode(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBNode* root;
    RBNode* T_NIL;

    void leftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != T_NIL) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode* x) {
        RBNode* y = x->left;
        x->left = y->right;
        if (y->right != T_NIL) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fixInsert(RBNode* k) {
        RBNode* u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) break;
        }
        root->color = BLACK;
    }

    void rbTransplant(RBNode* u, RBNode* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    void fixDelete(RBNode* x) {
        RBNode* s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    RBNode* minimum(RBNode* node) {
        while (node->left != T_NIL) node = node->left;
        return node;
    }

    void deleteTree(RBNode* node) {
        if (node != T_NIL) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    RedBlackTree() {
        T_NIL = new RBNode(0);
        T_NIL->color = BLACK;
        T_NIL->left = T_NIL->right = nullptr;
        root = T_NIL;
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete T_NIL;
    }

    void insert(int key) {
        RBNode* node = new RBNode(key);
        node->parent = nullptr;
        node->left = node->right = T_NIL;
        node->color = RED;

        RBNode* y = nullptr;
        RBNode* x = root;

        while (x != T_NIL) {
            y = x;
            if (node->data < x->data) x = x->left;
            else x = x->right;
        }

        node->parent = y;
        if (!y) root = node;
        else if (node->data < y->data) y->left = node;
        else y->right = node;

        if (!node->parent) { node->color = BLACK; return; }
        if (!node->parent->parent) return;

        fixInsert(node);
    }

    void remove(int data) {
        RBNode* z = T_NIL;
        RBNode* x;
        RBNode* y;
        RBNode* node = root;

        while (node != T_NIL) {
            if (node->data == data) z = node;
            if (node->data <= data) node = node->right;
            else node = node->left;
        }

        if (z == T_NIL) return;

        y = z;
        Color y_original_color = y->color;
        if (z->left == T_NIL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == T_NIL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) x->parent = y;
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;

        if (y_original_color == BLACK) fixDelete(x);
    }

    bool search(int k) const {
        RBNode* curr = root;
        while (curr != T_NIL && k != curr->data) {
            curr = (k < curr->data) ? curr->left : curr->right;
        }
        return curr != T_NIL;
    }

    int minimum() const {
        if (root == T_NIL) return vEB_NIL;
        RBNode* curr = root;
        while (curr->left != T_NIL) curr = curr->left;
        return curr->data;
    }

    int maximum() const {
        if (root == T_NIL) return vEB_NIL;
        RBNode* curr = root;
        while (curr->right != T_NIL) curr = curr->right;
        return curr->data;
    }

    int predecessor(int x) const {
        RBNode* curr = root;
        RBNode* pred = nullptr;
        while (curr != T_NIL) {
            if (curr->data < x) {
                pred = curr;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
        }
        return pred ? pred->data : vEB_NIL;
    }

    int successor(int x) const {
        RBNode* curr = root;
        RBNode* succ = nullptr;
        while (curr != T_NIL) {
            if (curr->data > x) {
                succ = curr;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return succ ? succ->data : vEB_NIL;
    }
};

#endif