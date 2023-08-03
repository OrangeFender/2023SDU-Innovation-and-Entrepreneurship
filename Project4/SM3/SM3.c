#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include "SM3.h"

#define LeftRotate(word, bits)( (word) << (bits) | (word) >> (32 - (bits)) )

#define p0(x) (x^LeftRotate(x,9)^LeftRotate(x,17))

typedef unsigned int UINT32;


const UINT32 Tj[64] = { 
    0x79cc4519U, 0xf3988a32U, 0xe7311465U, 0xce6228cbU,
    0x9cc45197U, 0x3988a32fU, 0x7311465eU, 0xe6228cbcU,
    0xcc451979U, 0x988a32f3U, 0x311465e7U, 0x6228cbceU,
    0xc451979cU, 0x88a32f39U, 0x11465e73U, 0x228cbce6U,
    0x9d8a7a87U, 0x3b14f50fU, 0x7629ea1eU, 0xec53d43cU,
    0xd8a7a879U, 0xb14f50f3U, 0x629ea1e7U, 0xc53d43ceU,
    0x8a7a879dU, 0x14f50f3bU, 0x29ea1e76U, 0x53d43cecU,
    0xa7a879d8U, 0x4f50f3b1U, 0x9ea1e762U, 0x3d43cec5U,
    0x7a879d8aU, 0xf50f3b14U, 0xea1e7629U, 0xd43cec53U,
    0xa879d8a7U, 0x50f3b14fU, 0xa1e7629eU, 0x43cec53dU,
    0x879d8a7aU, 0x0f3b14f5U, 0x1e7629eaU, 0x3cec53d4U,
    0x79d8a7a8U, 0xf3b14f50U, 0xe7629ea1U, 0xcec53d43U,
    0x9d8a7a87U, 0x3b14f50fU, 0x7629ea1eU, 0xec53d43cU,
    0xd8a7a879U, 0xb14f50f3U, 0x629ea1e7U, 0xc53d43ceU,
    0x8a7a879dU, 0x14f50f3bU, 0x29ea1e76U, 0x53d43cecU,
    0xa7a879d8U, 0x4f50f3b1U, 0x9ea1e762U, 0x3d43cec5U,
};

#define FF1(x,y,z) (x^y^z)
#define FF2(x,y,z) ((x&y)|(x&z)|(y&z))
#define GG1(x,y,z) (x^y^z)
#define GG2(x,y,z) ((x&y)|((~x)&z))

__m128i rotate_left_epi32(__m128i a, int bits) {
    __m128i leftShifted = _mm_slli_epi32(a, bits);
    __m128i rightShifted = _mm_srli_epi32(a, 32 - bits);
    return _mm_xor_si128(leftShifted, rightShifted);
}

void round(UINT32 b[16], UINT32 W[68 + 3], UINT32 V[8]) {
    memcpy(W, b, 16 * 4);
   

    UINT32 A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2;
    A = V[0]; B = V[1]; C = V[2]; D = V[3]; E = V[4]; F = V[5]; G = V[6]; H = V[7];

    /*
    for (int j = 16; j < 32; j += 1) {
        UINT32 tmp = W[j - 16] ^ W[j - 9] ^ LeftRotate(W[j - 3], 15);
        W[j] = LeftRotate(tmp, 15) ^ LeftRotate(tmp, 23) ^ tmp ^ W[j - 6] ^ LeftRotate(W[j - 13], 7);

        SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j - 16]), 7);
        SS2 = SS1 ^ LeftRotate(A, 12);
        TT1 = FF1(A, B, C) + D + SS2 + (W[j - 16] ^ W[j - 16 + 4]);
        TT2 = GG1(E, F, G) + H + SS1 + W[j - 16];
        D = C;
        C = LeftRotate(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = LeftRotate(F, 19);
        F = E;
        E = p0(TT2);
    }
    for (int j = 32; j < 68; j += 1) {
        UINT32 tmp = W[j - 16] ^ W[j - 9] ^ LeftRotate(W[j - 3], 15);
        W[j] = LeftRotate(tmp, 15) ^ LeftRotate(tmp, 23) ^ tmp ^ W[j - 6] ^ LeftRotate(W[j - 13], 7);

        SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j - 16]), 7);
        SS2 = SS1 ^ LeftRotate(A, 12);
        //printf("W:%x,%x\n", W[j - 16 + i], W[j - 16 + i + 4]);
        TT1 = FF2(A, B, C) + D + SS2 + (W[j - 16] ^ W[j - 16 + 4]);
        TT2 = GG2(E, F, G) + H + SS1 + W[j - 16];
        D = C;
        C = LeftRotate(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = LeftRotate(F, 19);
        F = E;
        E = p0(TT2);
    }
    for (int j = 68 - 16; (j) < 64; j++) {
        SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j]), 7);
        SS2 = SS1 ^ LeftRotate(A, 12);
        TT1 = FF2(A, B, C) + D + SS2 + (W[j] ^ W[j + 4]);
        TT2 = GG2(E, F, G) + H + SS1 + W[j];
        D = C;
        C = LeftRotate(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = LeftRotate(F, 19);
        F = E;
        E = p0(TT2);
        //printf("%d A:%x B:%x C:%x D:%x E:%x F:%x G:%x H:%x \n",j, A, B, C, D, E, F, G, H);
    }
    */
    
    __m128i va, vb, vc, vd, ve;
    const __m128i vmask = _mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff);
#pragma loop(hint_parallel(4))
    for (int j = 16; j < 32; j += 4) {
        vc = _mm_loadu_si128((__m128i*)(W + j - 3));
        va = _mm_load_si128((__m128i*)(W + j - 16));
        vb = _mm_loadu_si128((__m128i*)(W + j - 9));
        //vc = _mm_setr_epi32(0, W[j - 3 + 2], W[j - 3 + 1], W[j - 3]);
        vd = _mm_loadu_si128((__m128i*)(W + j - 13));
        vc = _mm_and_si128(vc, vmask);
        ve = _mm_loadu_si128((__m128i*)(W + j - 6));

        __m128i tmp1 = _mm_xor_si128(_mm_xor_si128(va, vb), rotate_left_epi32(vc, 15));
        __m128i tmp2 = _mm_xor_si128(_mm_xor_si128(rotate_left_epi32(tmp1, 15), rotate_left_epi32(tmp1, 23)), tmp1);
        __m128i tmp3 = _mm_xor_si128(tmp2, _mm_xor_si128(rotate_left_epi32(vd, 7), ve));
        _mm_store_si128((__m128i*)(W + j), tmp3);
        UINT32 t = LeftRotate(W[j], 15);
        W[j + 3] = W[j + 3] ^ t ^ LeftRotate(t, 15) ^ LeftRotate(t, 23);

#pragma loop(hint_parallel(4))
        for (int i = 0; i < 4; i++) {
            SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j-16+i]), 7);
            SS2 = SS1 ^ LeftRotate(A, 12);
            TT1 = FF1(A, B, C) + D + SS2 + (W[j-16+i]^ W[j - 16 + i+4]);
            TT2 = GG1(E, F, G) + H + SS1 + W[j-16+i];
            D = C;
            C = LeftRotate(B, 9);
            B = A;
            A = TT1;
            H = G;
            G = LeftRotate(F, 19);
            F = E;
            E = p0(TT2);
            //printf("%d A:%x B:%x C:%x D:%x E:%x F:%x G:%x H:%x \n",j-16+i, A, B, C, D, E, F, G, H);
        }
    }
#pragma loop(hint_parallel(9))
    for (int j = 32; j < 68; j += 4) {
        vc = _mm_loadu_si128((__m128i*)(W + j - 3));
        va = _mm_load_si128((__m128i*)(W + j - 16));
        vb = _mm_loadu_si128((__m128i*)(W + j - 9));
        //vc = _mm_setr_epi32(0, W[j - 3 + 2], W[j - 3 + 1], W[j - 3]);
        vd = _mm_loadu_si128((__m128i*)(W + j - 13));
        vc = _mm_and_si128(vc, vmask);
        ve = _mm_loadu_si128((__m128i*)(W + j - 6));
        __m128i tmp1 = _mm_xor_si128(_mm_xor_si128(va, vb), rotate_left_epi32(vc, 15));
        __m128i tmp2 = _mm_xor_si128(_mm_xor_si128(rotate_left_epi32(tmp1, 15), rotate_left_epi32(tmp1, 23)), tmp1);
        __m128i tmp3 = _mm_xor_si128(tmp2, _mm_xor_si128(rotate_left_epi32(vd, 7), ve));
        _mm_store_si128((__m128i*)(W + j), tmp3);

        UINT32 t = LeftRotate(W[j], 15);
        W[j + 3] = W[j + 3] ^ t ^ LeftRotate(t, 15) ^ LeftRotate(t, 23);

#pragma loop(hint_parallel(4))
        for (int i = 0; i < 4; i++) {
            int ind = j - 16 + i;
            SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j-16+i]), 7);
            SS2 = SS1 ^ LeftRotate(A, 12);
            //printf("W:%x,%x\n", W[j - 16 + i], W[j - 16 + i + 4]);
            TT1 = FF2(A, B, C) + D + SS2 + (W[j - 16 + i] ^ W[j - 16 + i + 4]);
            TT2 = GG2(E, F, G) + H + SS1 + W[j-16+i];
            D = C;
            C = LeftRotate(B, 9);
            B = A;
            A = TT1;
            H = G;
            G = LeftRotate(F, 19);
            F = E;
            E = p0(TT2);
            //printf("%d A:%x B:%x C:%x D:%x E:%x F:%x G:%x H:%x \n", j - 16 + i, A, B, C, D, E, F, G, H);
        }
    }
#pragma loop(hint_parallel(12))
    for (int j = 68-16; (j) < 64; j++) {
        SS1 = LeftRotate((LeftRotate(A, 12) + E + Tj[j]), 7);
        SS2 = SS1 ^ LeftRotate(A, 12);
        TT1 = FF2(A, B, C) + D + SS2 +( W[j] ^ W[j + 4]);
        TT2 = GG2(E, F, G) + H + SS1 + W[j];
        D = C;
        C = LeftRotate(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = LeftRotate(F, 19);
        F = E;
        E = p0(TT2);
        //printf("%d A:%x B:%x C:%x D:%x E:%x F:%x G:%x H:%x \n",j, A, B, C, D, E, F, G, H);
    }
    
    V[0] ^= A; V[1] ^= B; V[2] ^= C; V[3] ^= D; V[4] ^= E; V[5] ^= F; V[6] ^= G; V[7] ^= H;
}
int SM3(int length, unsigned char* m, unsigned char* hash) {
    UINT32* V = hash;
    const __m256i ShuffleRev = _mm256_set_epi8(
        12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3,   // first 128-bit lane
        12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3  // second 128-bit lane
    );
    V[0] = 0x7380166f; V[1] = 0x4914b2b9; V[2] = 0x172442d7; V[3] = 0xda8a0600; V[4] = 0xa96f30bc; V[5] = 0x163138aa; V[6] = 0xe38dee4d; V[7] = 0xb0fb0e4e;
    int n=((length+1+8+63)/64);
    unsigned long long l=64*n;//填充后长度为64字节的倍数(分组为512bit)
    unsigned char*M=(unsigned char*)malloc(l);
    /*
    for (int i = 0; i < l; i++) {
        M[i] = 0;
    }*/
    memset(M,0,l);
    /*
    for(int i=0;i<length;i++){
        M[i]=m[i];
    }
    */
    memcpy(M, m, length);
    M[length]=0x80;
    
    for (int i = 0; i < length + 1; i += 32) {
        __m256i b = _mm256_load_si256((__m256i*)(M + i));
         b = _mm256_shuffle_epi8(b, ShuffleRev);
        _mm256_store_si256((__m256i*)(M+i), b);
    }
    long long lenbit = length * 8;
    *(unsigned int *)(&M[l-4])= (int)lenbit;//末尾填充长度，单位是bit
    *(unsigned int*)(&M[l - 8]) = lenbit>>32;
    UINT32 W[68+7]; 

    for(int i=0;i<n;i++){
        round((UINT32*)(M + i * 64), W, V);
    }
    
    __m256i b = _mm256_load_si256((__m256i*)V);
    b = _mm256_shuffle_epi8(b, ShuffleRev);
    _mm256_store_si256((__m256i*)V, b);
    free(M);
    return 0;
}

