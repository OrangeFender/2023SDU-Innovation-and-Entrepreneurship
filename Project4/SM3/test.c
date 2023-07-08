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

    // ��ȡƵ��
    QueryPerformanceFrequency(&frequency);

    // ��ȡ��ʼʱ��
    QueryPerformanceCounter(&start);

    SM3(33554432, mtest, h);
    QueryPerformanceCounter(&end);
    for (int i = 0; i < 32; i++) {
        printf("%x", h[i]);
    }
    printf("\n");
    // ��������ʱ�䣨�Ժ���Ϊ��λ��
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // �������ʱ��
    printf("�Լ���SM3��������ʱ�䣺%.2f ����\n", elapsed_time);

    return 0;
}