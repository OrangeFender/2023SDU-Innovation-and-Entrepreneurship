#include"ZK.h"
#include<iostream>




int main() {

	uint32_t V = 0x3a87e2bf;
	uint32_t MDPv[maxdigit];
	int NofMDPs;
	countMDPs(V, MDPv, NofMDPs);
	unsigned char seed[maxdigit][Hashlength];
	unsigned char salt[maxdigit][Hashlength];
	unsigned char MDP[maxdigit][Hashlength];
	creatMDP(seed,salt,MDP,MDPv,NofMDPs);
	uint32_t shuffle[maxdigit];
	Node* r = CreatTree(shuffle,MDP,NofMDPs);
	uint32_t proveNum = 0x23a3f2ff;
	unsigned char rootcommit[Hashlength];
	unsigned char start[maxdigit][Hashlength];
	unsigned char Salt[Hashlength];
	uint32_t Shuffle;
	unsigned char pad[Treeheight - 1][Hashlength];
	//r->print();
	give_proof(proveNum, MDPv, NofMDPs, seed, salt, shuffle, r, rootcommit, start, Salt, Shuffle, pad);
	std::cout << confirmation(proveNum,rootcommit,start,Salt,Shuffle,pad)<<std::endl;
}