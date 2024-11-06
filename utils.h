#ifndef IRIS_UTILS_H_
#define IRIS_UTILS_H_


#include <cstddef>
#include <stddef.h>

inline int round_up_to_next_multiple_of_2(int n) {
    int bits = 0, ones = 0, oldn = n;
    while (n) {
        if (n & 1)
            ++ones;
        ++bits;
        n >>= 1;
    }

    if (ones == 1) // already a multiple of 2
        return oldn;
    return 1 << bits;
}

int Atoi(const char* str, size_t size) {
    int result = 0;
    for (size_t i = 0; i < size; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            continue;
        }
        result = result * 10 + (str[i] - '0');
    }
    return result;
}



/* utilitis for making sequence for tuple element retrieval */
template<size_t ...> struct seq {};
template<size_t idx, std::size_t N, std::size_t... S> struct seq_helper : seq_helper<idx + 1, N, S..., idx> {};
template<size_t N, size_t ...S> struct seq_helper<N, N, S...> {
    typedef seq<S...> type;
};
template<size_t N>
struct make_sequence {
    typedef typename seq_helper<0, N>::type type;
};


#endif