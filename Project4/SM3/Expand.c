typedef unsigned int UINT32;
#include <immintrin.h>


#define LeftRotate(word, bits)( (word) << (bits) | (word) >> (32 - (bits)) )
#define SIMD

__m128i rotate_left_epi32(__m128i a, int bits) {
    __m128i leftShifted = _mm_slli_epi32(a, bits);
    __m128i rightShifted = _mm_srli_epi32(a, 32 - bits);
    return _mm_xor_si128(leftShifted, rightShifted);
}

void Expand(UINT32 B[16], UINT32 W[68 + 3], UINT32 Ww[64]) {//W长度为68但是需要加三以确保足够SIMD寄存器存放
    {

        memcpy(W, B, 16*4);
        
    }

#ifdef SIMD
    /*

    //register __m256i w16, w8, w9, w13;
    register __m256i  w13,tmp;
#pragma loop(hint_parallel(7))

    for (int j = 16; j < 68; j += 8) {
        //w16 = _mm256_load_si256((__m256i*)(W + j - 16));
        //w9 = _mm256_loadu_si256((__m256i*)(W + j - 9));
        w13 = _mm256_loadu_si256((__m256i*)(W + j - 13));
        //register __m256i tmpv = _mm256_xor_si256(w16, w9);
        //_mm256_store_si256((__m256i*)(W + j), _mm256_xor_si256(
        //    _mm256_xor_si256(_mm256_xor_si256(_mm256_slli_epi32(tmpv, 15), _mm256_srli_epi32(tmpv, 32 - 15)), _mm256_xor_si256(_mm256_slli_epi32(tmpv, 23), _mm256_srli_epi32(tmpv, 32 - 23)))
        //    , _mm256_xor_si256(tmpv, _mm256_xor_si256(_mm256_slli_epi32(w13, 7), _mm256_srli_epi32(w13, 32 - 7)))));
        _mm256_store_si256((__m256i*)(W + j), _mm256_xor_si256(_mm256_slli_epi32(w13, 7), _mm256_srli_epi32(w13, 32 - 7)));
        //tmp = _mm256_xor_si256(_mm256_slli_epi32(w13, 7), _mm256_srli_epi32(w13, 32 - 7));


#pragma loop(hint_parallel(8))
        for (int i = 0; i < 8; i++) {
            UINT32 a = LeftRotate(W[j + i - 3], 15)^ W[j + i - 16]^W[j + i - 9];
            UINT32 b = LeftRotate(a, 15) ^ LeftRotate(a, 23) ^ a;
            W[j + i] ^= (W[j + i - 6] ^ b);
            //printf("%x ", W[j + i]);
        }//printf("\n");
    }
    */
    
    __m128i va, vb, vc, vd, ve;
    for (int j = 16; j < 68; j += 4) {
        
        va = _mm_load_si128((__m128i*)(W + j - 16));
        vb = _mm_loadu_si128((__m128i*)(W + j - 9));
        //vc = _mm_loadu_si128((__m128i*)(W + j - 3));
        vc = _mm_set_epi32(0,W[j - 3+2], W[j - 3 + 1], W[j - 3]);
        vd = _mm_loadu_si128((__m128i*)(W + j - 13));
        ve = _mm_loadu_si128((__m128i*)(W + j - 6));
        __m128i tmp1 = _mm_xor_si128(_mm_xor_si128(va, vb), rotate_left_epi32(vc, 15));
        __m128i tmp2 = _mm_xor_si128(_mm_xor_si128(rotate_left_epi32(tmp1, 15), rotate_left_epi32(tmp1, 23)),tmp1);
        __m128i tmp3 = _mm_xor_si128(tmp2, _mm_xor_si128(rotate_left_epi32(vd, 7), ve));
        _mm_store_si128((__m128i*)(W + j), tmp3);
      
        UINT32 t = LeftRotate(W[j], 15);
        W[j + 3] = W[j+3] ^ t ^ LeftRotate(t, 15) ^ LeftRotate(t, 23);
        
    }
    
    //__m128i X, T, R;
    //__m128i M = _mm_setr_epi32(0, 0, 0, 0xffffffff);
    //for (int j = 16; j < 68; j += 4) {
    //    /* X = (W[j - 3], W[j - 2], W[j - 1], 0) */
    //    X = _mm_loadu_si128((__m128i*)(W + j - 3));
    //    X = _mm_andnot_si128(M, X);

    //    X = rotate_left_epi32(X, 15);
    //    T = _mm_loadu_si128((__m128i*)(W + j - 9));
    //    X = _mm_xor_si128(X, T);
    //    T = _mm_loadu_si128((__m128i*)(W + j - 16));
    //    X = _mm_xor_si128(X, T);

    //    /* P1() */
    //    T = rotate_left_epi32(X, (23 - 15));
    //    T = _mm_xor_si128(T, X);
    //    T = rotate_left_epi32(T, 15);
    //    X = _mm_xor_si128(X, T);

    //    T = _mm_loadu_si128((__m128i*)(W + j - 13));
    //    T = rotate_left_epi32(T, 7);
    //    X = _mm_xor_si128(X, T);
    //    T = _mm_loadu_si128((__m128i*)(W + j - 6));
    //    X = _mm_xor_si128(X, T);

    //    /* W[j + 3] ^= P1(ROL32(W[j + 1], 15)) */
    //    R = _mm_shuffle_epi32(X, 0);
    //    R = _mm_and_si128(R, M);
    //    T = rotate_left_epi32(R, 15);
    //    T = _mm_xor_si128(T, R);
    //    T = rotate_left_epi32(T, 9);
    //    R = _mm_xor_si128(R, T);
    //    R = rotate_left_epi32(R, 6);
    //    X = _mm_xor_si128(X, R);

    //    _mm_storeu_si128((__m128i*)(W + j), X);


#else   
    for (int j = 16; j < 68; j ++) {
            UINT32 tmp = W[j - 16] ^ W[j - 9] ^ LeftRotate(W[j - 3], 15);
            W[j] = LeftRotate(tmp, 15) ^ LeftRotate(tmp, 23) ^ tmp ^ W[j - 6] ^ LeftRotate(W[j - 13], 7);
        }
#endif
       
    {
        register __m256i w1, w2;
        for (int i = 0; i < 64; i += 8)
        {
            w1 = _mm256_loadu_si256(W + i);
            w2 = _mm256_loadu_si256(W + i + 4);
            w1 = _mm256_xor_si256(w1, w2);
            _mm256_store_si256((__m256*)(Ww + i), w1);
        }
    }
}