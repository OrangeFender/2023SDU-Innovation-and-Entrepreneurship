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
    unsigned long index; // BitScanReverse() ������Ҫһ�� unsigned long �������洢���
    if (num == 0) {
        // ��������Ϊ0�����
        return 8; // ���������Ķ����Ʊ�ʾ��λ����ͨ����32��64
    }
    _BitScanReverse(&index, num); // �ҵ������λ��ʼ�ĵ�һ����λλ������
    return 8 - 1 - index; // ����ǰ����ĸ���
}

// ���������ж����������ǰ�濪ʼ������ͬ�� bit ��
int countConsecutiveSameBits(unsigned char* arr1, unsigned char* arr2, int size) {
    int count = 0;

    for (int i = 0; i < size; i++) {
        unsigned char xor_result = arr1[i] ^ arr2[i]; // ��������Ԫ�ص������
        int leading_zeros = count_leading_zeros(xor_result); // ��ȡ�������ǰ��0�ĸ���

        // ��������ȫΪ0����ʾ����Ԫ����ͬ��������ͬbit��
        if (xor_result == 0) {
            count += 8; // һ���ֽ���8��bit
        }
        else {
            // �����ҵ���һ������ͬ��bit���˳�ѭ��
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

    // ��ȡƵ��
    QueryPerformanceFrequency(&frequency);

    // ��ȡ��ʼʱ��
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
    // ��������ʱ�䣨�Ժ���Ϊ��λ��
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // �������ʱ��
    printf("cost tion��%.2f ms\n", elapsed_time);
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
