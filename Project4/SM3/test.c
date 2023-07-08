#include <stdio.h>
#include"SM3.h"
#include <windows.h>

unsigned char mtest[33554432];
int main() {
    unsigned char h[32];
    unsigned char m[4] = "abc";
    SM3(3, m, h);
    for (int i = 0; i < 32; i++) {
        printf("%x", h[i]);
    }
    printf("\n");

    LARGE_INTEGER frequency, start, end;
    double elapsed_time;

    // 获取频率
    QueryPerformanceFrequency(&frequency);

    // 获取开始时间
    QueryPerformanceCounter(&start);

    SM3(33554432, mtest, h);
    QueryPerformanceCounter(&end);
    for (int i = 0; i < 32; i++) {
        printf("%x", h[i]);
    }
    printf("\n");
    // 计算运行时间（以毫秒为单位）
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // 输出运行时间
    printf("自己的SM3程序运行时间：%.2f 毫秒\n", elapsed_time);

    return 0;
}