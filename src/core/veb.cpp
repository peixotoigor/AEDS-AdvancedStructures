#include "veb.hpp"
#include <algorithm>

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