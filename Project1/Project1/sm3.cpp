#include <openssl/evp.h>
#include <stdio.h>
#include<iostream>
#include <stdint.h>
#include <Windows.h>


//#define find32bit
#define findBest

uint32_t hash_out4byte(uint32_t x)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_Digest(&x, 4, hash, &hash_len, EVP_sm3(), NULL);

    return ((uint32_t*)hash)[0];
}

void printhash(unsigned int message_len, void*data) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_Digest(data, 4, hash, &hash_len, EVP_sm3(), NULL);
    for (int i = 0; i < 32; i++)
        printf("%x", hash[i]);
    printf("\n");
}

unsigned int count_leading_zeros(unsigned char num) {
    unsigned long index; // BitScanReverse() 函数需要一个 unsigned long 参数来存储结果
    if (num == 0) {
        // 处理输入为0的情况
        return 8; // 返回整数的二进制表示的位数，通常是32或64
    }
    _BitScanReverse(&index, num); // 找到从最高位开始的第一个置位位的索引
    return 8 - 1 - index; // 计算前导零的个数
}

// 函数用于判断两个数组从前面开始连续相同的 bit 数
int countConsecutiveSameBits(unsigned char* arr1, unsigned char* arr2, int size) {
    int count = 0;

    for (int i = 0; i < size; i++) {
        unsigned char xor_result = arr1[i] ^ arr2[i]; // 计算两个元素的异或结果
        int leading_zeros = count_leading_zeros(xor_result); // 获取异或结果中前导0的个数

        // 如果异或结果全为0，表示两个元素相同，更新相同bit数
        if (xor_result == 0) {
            count += 8; // 一个字节有8个bit
        }
        else {
            // 否则，找到第一个不相同的bit，退出循环
            count += leading_zeros;
            break;
        }
    }

    return count;
}


int main() {
    //std::cout << count_leading_zeros(1);
    //std::cout << count_leading_zeros(3);
    //std::cout<< count_leading_zeros(5);

    const char* fileName = "Birthday";
    const ULONGLONG fileSize = (1LL << 32) * 4LL; // 
    const ULONGLONG numElements = fileSize / sizeof(uint32_t);

    // Create or open the file
    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create or open the file." << std::endl;
        return 1;
    }

    // Set the size of the file
    LARGE_INTEGER li;
    li.QuadPart = fileSize;

    if (!SetFilePointerEx(hFile, li, NULL, FILE_BEGIN)) {
        std::cout << "Failed to set the file size." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    if (!SetEndOfFile(hFile)) {
        std::cout << "Failed to set the end of the file." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // Create a file mapping
    HANDLE hMapFile = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        li.HighPart,
        li.LowPart,
        NULL
    );

    if (hMapFile == NULL) {
        std::cout << "Failed to create file mapping." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // Map the file into memory
    uint32_t* pData = static_cast<uint32_t*>(MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        fileSize
    ));

    if (pData == NULL) {
        std::cout << "Failed to map view of file." << std::endl;
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        return 1;
    }

    // Zero-initialize the memory (setting all values to zero)
    ZeroMemory(pData, fileSize);

#ifdef find32bit

    LARGE_INTEGER frequency, start, end;
    double elapsed_time;

    // 获取频率
    QueryPerformanceFrequency(&frequency);

    // 获取开始时间
    QueryPerformanceCounter(&start);
    uint32_t i = 1;unsigned long long ind;
    for (i = 1; i > 0; i++) {
        ind = hash_out4byte(i);
        if (pData[ind] != 0) {
            uint32_t v = pData[ind];
            printf("The 32bit of the collision is:%x\n", ind);
            printf("The original image is:%x\nSM3 is:", i); printhash(4, &i);
            printf("The original image is:%x\nSM3 is:", v); printhash(4, &v);
            break;
        }
        pData[ind] = i;
    }
    QueryPerformanceCounter(&end);
    // 计算运行时间（以毫秒为单位）
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // 输出运行时间
    printf("cost tion：%.2f ms\n", elapsed_time);
#elif defined findBest
    printf("try to find more collision\n");
    int best = 0;
    uint32_t i = 1; unsigned long long ind;
    uint32_t best1, best2;
    for (i = 1; i > 0; i++) {
        ind = hash_out4byte(i);
        if (pData[ind] != 0) {
            uint32_t v = pData[ind];
            unsigned char hash1[EVP_MAX_MD_SIZE];
            unsigned char hash2[EVP_MAX_MD_SIZE];
            unsigned int hash_len;
            //printf("The 32bit of the collision is:%x\n", ind);
            
            EVP_Digest(&v, 4, hash1, &hash_len, EVP_sm3(), NULL);
            EVP_Digest(&i, 4, hash2, &hash_len, EVP_sm3(), NULL);
            int sameBits=countConsecutiveSameBits(hash1, hash2, 32);
            //printf("%d\n", sameBits);
            if (best < sameBits) {
                best = sameBits;
                printf("find %d bits collision\n", best);
                printf("The original image is:%x\nSM3 is:", i); printhash(4, &i);
                printf("The original image is:%x\nSM3 is:", v); printhash(4, &v);
                best1 = v;
                best2 = i;
            }
        }
        pData[ind] = i;
    }
    printf("best collision is %d bits\n", best);
#endif

    // Unmap and close handles
    UnmapViewOfFile(pData);
    CloseHandle(hMapFile);
    CloseHandle(hFile);

    return 0;
}
