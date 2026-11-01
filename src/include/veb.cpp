#ifndef VEB_HPP
#define VEB_HPP

#include <vector>
#include <cmath>

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

int veb_minimum(vanEmdeBoas* V);
int veb_maximum(vanEmdeBoas* V);
bool veb_member(vanEmdeBoas* V, int x);
void veb_empty_tree_insert(vanEmdeBoas* V, int x);
void veb_insert(vanEmdeBoas* V, int x);
int veb_successor(vanEmdeBoas* V, int x);
void veb_delete(vanEmdeBoas* V, int x);

#endif