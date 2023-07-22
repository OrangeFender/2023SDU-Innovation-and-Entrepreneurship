#include"ZK.h"
#include<iostream>
#include <windows.h>

int main() {
	LARGE_INTEGER frequency, Start, end;
	double elapsed_time;
	// ��ȡƵ��
	QueryPerformanceFrequency(&frequency);
	// ��ȡ��ʼʱ��
	QueryPerformanceCounter(&Start);
	uint32_t V = 0x3a87e2bf;
	uint32_t MDPv[maxdigit];
	int NofMDPs;
	countMDPs(V, MDPv, NofMDPs);
	unsigned char seed[maxdigit][Hashlength];
	unsigned char salt[maxdigit][Hashlength];
	unsigned char MDP[maxdigit][Hashlength];
	creatMDP(seed, salt, MDP, MDPv, NofMDPs);
	uint32_t shuffle[maxdigit];
	Node* r = CreatTree(shuffle, MDP, NofMDPs);
	uint32_t proveNum = 0x23a3f2ff;
	unsigned char rootcommit[Hashlength];
	unsigned char start[maxdigit][Hashlength];
	unsigned char Salt[Hashlength];
	uint32_t Shuffle;
	unsigned char pad[Treeheight - 1][Hashlength];
	//r->print();
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << "֤��0x3a87e2bf����0x23a3f2ff�������" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	QueryPerformanceCounter(&end);
	// ��������ʱ�䣨�Ժ���Ϊ��λ��
	elapsed_time = (double)(end.QuadPart - Start.QuadPart) * 1000.0 / frequency.QuadPart;
	// �������ʱ��
	printf("��������ʱ�䣺%.2f ����\n", elapsed_time);
	
	proveNum = 0x33ffffff;
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << "֤��0x3a87e2bf����0x33ffffff�������" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	proveNum = 0x11111111;
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << "֤��0x3a87e2bf����0x11111111�������" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	delete r;
}