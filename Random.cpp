#include "stdafx.h"
#include "Random.h"

#include <ctime>

// Constructors
Random::Random(const i32& s) {
    m_mtArr = new i32[MERSENNE_ARRAY_SIZE];
    seed(s);
}
Random::Random() : Random(clock()) {
    /// Empty
}
Random::~Random() {
    if (m_mtArr) {
        delete[] m_mtArr;
        m_mtArr = nullptr;
    }
}

// Re-seed the generator
#define MERSENNE_SEED_PRIME 1812433253
void Random::seed(const i32& s) {
    m_seed = s;

    m_mtIndex = 0;
    *m_mtArr = m_seed;
    i32 prev = m_seed;
    for (size_t i = 1; i < MERSENNE_ARRAY_SIZE; i++) {
        prev = MERSENNE_SEED_PRIME * (prev ^ ((prev >> 30))) + i;
        m_mtArr[i] = prev;
    }

    m_mhKey = m_seed;
}

// Obtain random numbers
#define MERSENNE_GEN_PRIME1 2636928640
#define MERSENNE_GEN_PRIME2 4022730752
f32 Random::genMT() {
    if(m_mtIndex == 0) perturbMT();

    i32 y = m_mtArr[m_mtIndex];
    y ^= y >> 11;
    y ^= (y << 7) & MERSENNE_GEN_PRIME1;
    y ^= (y << 15) & MERSENNE_GEN_PRIME2;
    y ^= y >> 18;

    m_mtIndex = (m_mtIndex + 1) % MERSENNE_ARRAY_SIZE;
    return y / (f32)(0x7fffffff);
}
#define MURMUR_GEN_PRIME1 0xcc9e2d51
#define MURMUR_GEN_PRIME2 0x1b873593
#define MURMUR_GEN_PRIME3 0xe6546b64
#define MURMUR_GEN_PRIME4 0x85ebca6b
#define MURMUR_GEN_PRIME5 0xc2b2ae35
f32 Random::genMH() {
        ui32 hash = m_seed;
        ui32 k = m_mhKey;
        k *= MURMUR_GEN_PRIME1;
        k = (k << 15) | (k >> (32 - 15));
        k *= MURMUR_GEN_PRIME2;

        hash ^= k;
        hash = ((hash << 13) | (hash >> (32 - 13))) * 5 + MURMUR_GEN_PRIME3;
        ui32 k1 = 0;
        hash ^= 4;
        hash ^= (hash >> 16);
        hash *= MURMUR_GEN_PRIME4;
        hash ^= (hash >> 13);
        hash *= MURMUR_GEN_PRIME5;
        hash ^= (hash >> 16);
        m_mhKey = hash;
        return hash / (f32)(0xffffffffu);
}

// Perturbs mersenne twister array after a full traversal
#define MERSENNE_PERT_PRIME1 397
#define MERSENNE_PERT_PRIME2 2567483615
void Random::perturbMT() {
    for(size_t i = 0; i < MERSENNE_ARRAY_SIZE; i++) {
        i32 y = (m_mtArr[i] & 0x80000000) + (m_mtArr[(i + 1) % MERSENNE_ARRAY_SIZE] & 0x7fffffff);
        m_mtArr[i] = m_mtArr[(i + MERSENNE_PERT_PRIME1) % MERSENNE_ARRAY_SIZE] ^ (y >> 1);
        if(y & 1) m_mtArr[i] ^= MERSENNE_PERT_PRIME2;
    }
}