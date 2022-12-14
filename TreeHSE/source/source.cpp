#include <cstdio>
#include <memory.h>

#define NONE(K) (1ULL << K)

template <unsigned int K>
class Veb {
private:
    unsigned long long T_min, T_max;
    Veb<(K >> 1)> *T[1ULL << (K >> 1)], *aux;
public:
    Veb(): T_min(NONE(K)), aux(NULL) {
        memset(T, 0, sizeof(T));
    }

    ~Veb() {
        delete aux;
        for (unsigned long long i = 0; i < (1ULL << (K >> 1)); ++i) {
            delete T[i];
        }
    }

    inline bool empty() const {
        return T_min == NONE(K);
    }

    inline unsigned long long get_min() const {
        return T_min;
    }

    inline unsigned long long get_max() const {
        return T_max;
    }

    inline unsigned long long high(unsigned long long key) const {
        return key >> (K >> 1);
    }

    inline unsigned long long low(unsigned long long key) const {
        return key & ((1ULL << (K >> 1)) - 1ULL);
    }

    inline unsigned long long merge(unsigned long long high, unsigned long long low) const {
        return (high << (K >> 1)) + low;
    }

    void insert(unsigned long long key) {
        if (empty()) {
            T_min = T_max = key;
        } else {
            if (key < T_min) {
                unsigned long long temp_key = key;
                key = T_min;
                T_min = temp_key;
            }
            if (key > T_max) {
                T_max = key;
            }
            if (K != 1) {
                unsigned long long key_high = high(key);
                unsigned long long key_low = low(key);
                if (T[key_high] == NULL) {
                    T[key_high] = new Veb<(K >> 1)>();
                }
                if (T[key_high]->empty()) {
                    if (aux == NULL) {
                        aux = new Veb<(K >> 1)>();
                    }
                    aux->insert(key_high);
                }
                T[key_high]->insert(key_low);
            }
        }
    }

    unsigned long long find_next(unsigned long long key) {
        if (key <= T_min) {
            return T_min;
        }
        if (empty() || key > T_max) {
            return NONE(K);
        }
        if (K == 1) {
            return T_max == key ? key : NONE(K);
        }
        unsigned long long key_high = high(key);
        unsigned long long key_low = low(key);
        if (T[key_high] != NULL && key_low <= T[key_high]->get_max()) {
            return merge(key_high, T[key_high]->find_next(key_low));
        } else if (aux != NULL) {
            unsigned long long prev_high = aux->find_next(key_high + 1);
            if (prev_high != NONE(K >> 1)) {
                return merge(prev_high, T[prev_high]->get_min());
            }
        }
        return NONE(K);
    }

    bool lookup(unsigned long long key) {
        if (key == T_min || key == T_max) {
            return true;
        } else {
            unsigned long long key_high = high(key);
            unsigned long long key_low = low(key);
            return T[key_high] != NULL && T[key_high]->lookup(key_low);
        }
    }
};
