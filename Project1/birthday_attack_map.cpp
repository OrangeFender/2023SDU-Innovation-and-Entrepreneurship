#include <openssl/evp.h>
#define MAsk 0b11111111'11111111'11111111'11111111'11111111'11111111 //48
#include <map>
#include <iostream>

uint64_t hash_masked(uint64_t x, uint64_t Mask)
{
    x = x & Mask;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_Digest(&x, 8, hash, &hash_len, EVP_sm3(), NULL);

    return ((uint64_t*)hash)[0] & Mask;
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
    std::map<uint64_t, uint64_t> myMap;
    for (uint64_t i = 1; i < MAsk; i++) {
        uint64_t h = hash_masked(i, MAsk);
        std::map<uint64_t, uint64_t>::iterator iter = myMap.find(h);
        if (iter != myMap.end()) {
            uint64_t m = iter->second;
            std::cout << "m1=" << std::hex<< m << std::endl;
            std::cout << "hash:\n";
            printhash(8, &m);
            std::cout << "m2=" << std::hex<< i << std::endl;
            std::cout << "hash:\n";
            printhash(8, &i);
            break;
        }
        else {
            myMap[h] = i;
        }
    }
    return 0;
}