#include <openssl/evp.h>
#define MAsk 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111 //56位
#include<iostream>
#include <random>

uint64_t hash_masked(uint64_t x, uint64_t Mask)
{
    x = x & Mask;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_Digest(&x, 8, hash, &hash_len, EVP_sm3(), NULL);

    return ((uint64_t*)hash)[0]&Mask;
}

void printhash(unsigned int message_len, void* data) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_Digest(data, message_len, hash, &hash_len, EVP_sm3(), NULL);
    for (int i = 0; i < 32; i++)
        printf("%x", hash[i]);
    printf("\n");
}

int main() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dist(0, std::numeric_limits<long long>::max());
    // 生成64bit随机数
    long long random_number = dist(gen);
    long long start = random_number & MAsk;
    long long h1 = start;
    long long h2 = start;
    do {
        h1 = hash_masked(h1, MAsk);
        h2 = hash_masked(h2, MAsk);
        h2 = hash_masked(h2, MAsk);
    } while (h2!=h1);
    std::cout << "find circle" << std::endl;
    h1 = start;
    while (true) {
        long long tmp1, tmp2;
        tmp1 = hash_masked(h1, MAsk);
        tmp2 = hash_masked(h2, MAsk);
        if (tmp1 == tmp2) {
            std::cout << "m1=" << std::hex << h1 <<std::endl;
            printhash(8, &h1);
            std::cout << "m2=" << std::hex << h2 << std::endl;
            printhash(8, &h2);
            return 0;
        }
        h1 = tmp1;
        h2 = tmp2;

    }
}