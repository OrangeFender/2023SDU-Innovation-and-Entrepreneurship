#include"ZK.h"
#include<iostream>
#include <windows.h>

int main() {
	LARGE_INTEGER frequency, Start, end;
	double elapsed_time;
	// 获取频率
	QueryPerformanceFrequency(&frequency);
	// 获取开始时间
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
	std::cout << "证明0x3a87e2bf大于0x23a3f2ff，结果：" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	QueryPerformanceCounter(&end);
	// 计算运行时间（以毫秒为单位）
	elapsed_time = (double)(end.QuadPart - Start.QuadPart) * 1000.0 / frequency.QuadPart;
	// 输出运行时间
	printf("总体运行时间：%.2f 毫秒\n", elapsed_time);
	
	proveNum = 0x33ffffff;
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << "证明0x3a87e2bf大于0x33ffffff，结果：" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	proveNum = 0x11111111;
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << "证明0x3a87e2bf大于0x11111111，结果：" << confirmation(proveNum, rootcommit, start, Salt, Shuffle, pad) << std::endl;
	delete r;
}