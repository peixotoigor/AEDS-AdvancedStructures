#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

// ============================================================================
// 1. VAN EMDE BOAS TREE (CLRS Chapter 20)
// ============================================================================
constexpr int vEB_NIL = -1;

class vanEmdeBoas {
public:
    int u;
    int min;
    int max;
    int upper_sqrt;
    int lower_sqrt;
    
    vanEmdeBoas* summary;
    std::vector<vanEmdeBoas*> cluster;

    explicit vanEmdeBoas(int universe_size) {
        int k = static_cast<int>(std::ceil(std::log2(universe_size)));
        if (k < 1) k = 1;
        u = 1 << k;

        min = vEB_NIL;
        max = vEB_NIL;

        if (u > 2) {
            upper_sqrt = 1 << static_cast<int>(std::ceil(k / 2.0));
            lower_sqrt = 1 << static_cast<int>(std::floor(k / 2.0));

            summary = new vanEmdeBoas(upper_sqrt);
            cluster.resize(upper_sqrt);
            for (int i = 0; i < upper_sqrt; ++i) {
                cluster[i] = new vanEmdeBoas(lower_sqrt);
            }
        } else {
            upper_sqrt = 0;
            lower_sqrt = 0;
            summary = nullptr;
        }
    }

    ~vanEmdeBoas() {
        delete summary;
        for (auto c : cluster) {
            delete c;
        }
    }

    inline int high(int x) const { return x / lower_sqrt; }
    inline int low(int x) const { return x % lower_sqrt; }
    inline int index(int h, int l) const { return h * lower_sqrt + l; }
};

int veb_minimum(vanEmdeBoas* V) { return V->min; }
int veb_maximum(vanEmdeBoas* V) { return V->max; }

bool veb_member(vanEmdeBoas* V, int x) {
    if (x == V->min || x == V->max) return true;
    if (V->u == 2) return false;
    return veb_member(V->cluster[V->high(x)], V->low(x));
}

void veb_empty_tree_insert(vanEmdeBoas* V, int x) {
    V->min = x;
    V->max = x;
}

void veb_insert(vanEmdeBoas* V, int x) {
    if (V->min == vEB_NIL) {
        veb_empty_tree_insert(V, x);
    } else {
        if (x < V->min) std::swap(x, V->min);
        if (V->u > 2) {
            int h = V->high(x);
            int l = V->low(x);
            if (veb_minimum(V->cluster[h]) == vEB_NIL) {
                veb_insert(V->summary, h);
                veb_empty_tree_insert(V->cluster[h], l);
            } else {
                veb_insert(V->cluster[h], l);
            }
        }
        if (x > V->max) V->max = x;
    }
}

int veb_successor(vanEmdeBoas* V, int x) {
    if (V->u == 2) {
        if (x == 0 && V->max == 1) return 1;
        return vEB_NIL;
    } else if (V->min != vEB_NIL && x < V->min) {
        return V->min;
    } else {
        int h = V->high(x);
        int l = V->low(x);
        int max_low = veb_maximum(V->cluster[h]);

        if (max_low != vEB_NIL && l < max_low) {
            int offset = veb_successor(V->cluster[h], l);
            return V->index(h, offset);
        } else {
            int succ_cluster = veb_successor(V->summary, h);
            if (succ_cluster == vEB_NIL) return vEB_NIL;
            int offset = veb_minimum(V->cluster[succ_cluster]);
            return V->index(succ_cluster, offset);
        }
    }
}

void veb_delete(vanEmdeBoas* V, int x) {
    if (V->min == V->max) {
        V->min = vEB_NIL;
        V->max = vEB_NIL;
    } else if (V->u == 2) {
        V->min = (x == 0) ? 1 : 0;
        V->max = V->min;
    } else {
        if (x == V->min) {
            int first_cluster = veb_minimum(V->summary);
            x = V->index(first_cluster, veb_minimum(V->cluster[first_cluster]));
            V->min = x;
        }
        int h = V->high(x);
        int l = V->low(x);
        veb_delete(V->cluster[h], l);

        if (veb_minimum(V->cluster[h]) == vEB_NIL) {
            veb_delete(V->summary, h);
            if (x == V->max) {
                int summary_max = veb_maximum(V->summary);
                if (summary_max == vEB_NIL) {
                    V->max = V->min;
                } else {
                    V->max = V->index(summary_max, veb_maximum(V->cluster[summary_max]));
                }
            }
        } else if (x == V->max) {
            V->max = V->index(h, veb_maximum(V->cluster[h]));
        }
    }
}

// ============================================================================
// 2. AVL TREE
// ============================================================================
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
        while (current->left) current = current->left;
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
};

// ============================================================================
// 3. RED-BLACK TREE (CLRS Chapter 13)
// ============================================================================
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

// ============================================================================
// BENCHMARK DRIVER
// ============================================================================
using namespace std::chrono;

struct BenchmarkResults {
    double insert_time_ms;
    double search_time_ms;
    double successor_time_ms;
    double delete_time_ms;
};

int main(int argc, char* argv[]) {
    std::string dataset_file = "dataset_benchmark.txt";
    if (argc > 1) dataset_file = argv[1];

    std::ifstream in(dataset_file);
    if (!in.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de dataset: " << dataset_file << std::endl;
        return 1;
    }

    int u, n, m;
    in >> u >> n >> m;

    std::vector<int> insert_keys(n);
    for (int i = 0; i < n; ++i) in >> insert_keys[i];

    std::vector<int> search_queries(m);
    for (int i = 0; i < m; ++i) in >> search_queries[i];

    std::vector<int> successor_queries(m);
    for (int i = 0; i < m; ++i) in >> successor_queries[i];

    std::vector<int> delete_keys(n / 2);
    for (int i = 0; i < n / 2; ++i) in >> delete_keys[i];

    std::cout << "=========================================================\n";
    std::cout << " BENCHMARK COMPARATIVO: vEB vs AVL vs RED-BLACK TREE\n";
    std::cout << "=========================================================\n";
    std::cout << "Universo (u): " << u << " (2^" << static_cast<int>(std::log2(u)) << ")\n";
    std::cout << "Elementos Inseridos (N): " << n << "\n";
    std::cout << "Consultas (M): " << m << "\n\n";

    // ------------------------------------------------------------------------
    // 1. BENCHMARK: van Emde Boas Tree
    // ------------------------------------------------------------------------
    BenchmarkResults veb_res;
    vanEmdeBoas* veb = new vanEmdeBoas(u);

    auto t1 = high_resolution_clock::now();
    for (int x : insert_keys) veb_insert(veb, x);
    auto t2 = high_resolution_clock::now();
    veb_res.insert_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    volatile bool dummy_found = false;
    for (int q : search_queries) dummy_found = veb_member(veb, q);
    t2 = high_resolution_clock::now();
    veb_res.search_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    volatile int dummy_succ = 0;
    for (int q : successor_queries) dummy_succ = veb_successor(veb, q);
    t2 = high_resolution_clock::now();
    veb_res.successor_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int x : delete_keys) veb_delete(veb, x);
    t2 = high_resolution_clock::now();
    veb_res.delete_time_ms = duration<double, std::milli>(t2 - t1).count();

    delete veb;

    // ------------------------------------------------------------------------
    // 2. BENCHMARK: AVL Tree
    // ------------------------------------------------------------------------
    BenchmarkResults avl_res;
    AVLTree avl;

    t1 = high_resolution_clock::now();
    for (int x : insert_keys) avl.insert(x);
    t2 = high_resolution_clock::now();
    avl_res.insert_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int q : search_queries) dummy_found = avl.search(q);
    t2 = high_resolution_clock::now();
    avl_res.search_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int q : successor_queries) dummy_succ = avl.successor(q);
    t2 = high_resolution_clock::now();
    avl_res.successor_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int x : delete_keys) avl.remove(x);
    t2 = high_resolution_clock::now();
    avl_res.delete_time_ms = duration<double, std::milli>(t2 - t1).count();

    // ------------------------------------------------------------------------
    // 3. BENCHMARK: Red-Black Tree
    // ------------------------------------------------------------------------
    BenchmarkResults rb_res;
    RedBlackTree rb;

    t1 = high_resolution_clock::now();
    for (int x : insert_keys) rb.insert(x);
    t2 = high_resolution_clock::now();
    rb_res.insert_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int q : search_queries) dummy_found = rb.search(q);
    t2 = high_resolution_clock::now();
    rb_res.search_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int q : successor_queries) dummy_succ = rb.successor(q);
    t2 = high_resolution_clock::now();
    rb_res.successor_time_ms = duration<double, std::milli>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    for (int x : delete_keys) rb.remove(x);
    t2 = high_resolution_clock::now();
    rb_res.delete_time_ms = duration<double, std::milli>(t2 - t1).count();

    // ------------------------------------------------------------------------
    // TABELA COMPARATIVA DE RESULTADOS
    // ------------------------------------------------------------------------
    std::cout << std::left << std::setw(20) << "Operacao (ms)"
              << std::setw(18) << "vEB Tree"
              << std::setw(18) << "AVL Tree"
              << std::setw(18) << "Red-Black Tree" << "\n";
    std::cout << "-------------------------------------------------------------------------\n";

    std::cout << std::left << std::setw(20) << "Insercao (N)"
              << std::setw(18) << std::fixed << std::setprecision(2) << veb_res.insert_time_ms
              << std::setw(18) << avl_res.insert_time_ms
              << std::setw(18) << rb_res.insert_time_ms << "\n";

    std::cout << std::left << std::setw(20) << "Busca / Member (M)"
              << std::setw(18) << veb_res.search_time_ms
              << std::setw(18) << avl_res.search_time_ms
              << std::setw(18) << rb_res.search_time_ms << "\n";

    std::cout << std::left << std::setw(20) << "Sucessor (M)"
              << std::setw(18) << veb_res.successor_time_ms
              << std::setw(18) << avl_res.successor_time_ms
              << std::setw(18) << rb_res.successor_time_ms << "\n";

    std::cout << std::left << std::setw(20) << "Remocao (N/2)"
              << std::setw(18) << veb_res.delete_time_ms
              << std::setw(18) << avl_res.delete_time_ms
              << std::setw(18) << rb_res.delete_time_ms << "\n";

    std::cout << "-------------------------------------------------------------------------\n";

    return 0;
}
