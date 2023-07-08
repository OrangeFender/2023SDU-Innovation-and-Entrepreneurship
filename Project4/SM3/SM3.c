#include <stdlib.h>
#include <immintrin.h>
#include "SM3.h"

#define LeftRotate(word, bits)( (word) << (bits) | (word) >> (32 - (bits)) )

#define p0(x) (x^LeftRotate(x,9)^LeftRotate(x,17))
#define T1 0x79cc4519
#define T2 0x7a879d8a
#define FF1(x,y,z) (x^y^z)
#define FF2(x,y,z) ((x&y)|(x&z)|(y&z))
#define GG1(x,y,z) (x^y^z)
#define GG2(x,y,z) ((x&y)|((~x)&z))

typedef unsigned int UINT32;

void Expand(UINT32 B[16], UINT32 W[68 + 3], UINT32 Ww[64]);
void compress(UINT32 V[8],UINT32 W[68], UINT32 Ww[64]){
UINT32 A,B,C,D,E,F,G,H,SS1,SS2,TT1,TT2;
A=V[0];B=V[1];C=V[2];D=V[3];E=V[4];F=V[5];G=V[6];H=V[7];
for(int j=0;j<16;j++){
SS1=LeftRotate((LeftRotate(A,12)+E+LeftRotate(T1,j)),7);
SS2=SS1^LeftRotate(A,12);
TT1=FF1(A,B,C)+D+SS2+Ww[j];
TT2=GG1(E,F,G)+H+SS1+W[j];
D=C;
C=LeftRotate(B,9);
B=A;
A=TT1;
H=G;
G=LeftRotate(F,19);
F=E;
E=p0(TT2);

}
for(int j=16;j<64;j++){
SS1=LeftRotate((LeftRotate(A,12)+E+LeftRotate(T2,j)),7);
SS2=SS1^LeftRotate(A,12);
TT1=FF2(A,B,C)+D+SS2+Ww[j];
TT2=GG2(E,F,G)+H+SS1+W[j];
D=C;
C=LeftRotate(B,9);
B=A;
A=TT1;
H=G;
G=LeftRotate(F,19);
F=E;
E=p0(TT2);

}
V[0]^=A;V[1]^=B;V[2]^=C;V[3]^=D;V[4]^=E;V[5]^=F;V[6]^=G;V[7]^=H;


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
    for (int i = 0; i < l; i++) {
        M[i] = 0;
    }

    for(int i=0;i<length;i++){
        M[i]=m[i];
    }
    M[length]=0x80;

    for (int i = 0; i < length + 1; i += 32) {
        __m256i b = _mm256_load_si256((__m256i*)(M + i));
         b = _mm256_shuffle_epi8(b, ShuffleRev);
        _mm256_store_si256((__m256i*)(M+i), b);
    }
    long long lenbit = length * 8;
    *(unsigned int *)(&M[l-4])= (int)lenbit;//末尾填充长度，单位是bit
    *(unsigned int*)(&M[l - 8]) = lenbit>>32;
    UINT32 W[68+3]; UINT32 Ww[64];

    for(int i=0;i<n;i++){
        Expand((UINT32*)(M+i*64),W,Ww);
        compress(V,W,Ww);
    }
    __m256i b = _mm256_load_si256((__m256i*)V);
    b = _mm256_shuffle_epi8(b, ShuffleRev);
    _mm256_store_si256((__m256i*)V, b);
    free(M);
    return 0;
}

