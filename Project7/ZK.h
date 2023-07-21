#pragma once
#include<stdint.h>
#define base 16
#define basebit 4
#define maxdigit 8
#define Hashlength 32
#define Treeheight 10//Merkel Tree�߶�Ϊ10
#define Shufflemask 0x1FF

typedef class Node {
public:
    unsigned char hash[Hashlength];
    Node* father;
    Node* lchild;
    Node* rchild;
    bool pad;
    bool finished;
    Node(Node* f);
    ~Node();
}Node;

void countMDPs(uint32_t V,
    uint32_t MDPv[maxdigit], //�洢ÿһ��֤����ֵ
    int& NofMDPs//��¼����ĳ���
);

void creatMDP(unsigned char seed[maxdigit][Hashlength],
    unsigned char salt[maxdigit][Hashlength],
    unsigned char MDP[maxdigit][Hashlength],
    uint32_t MDPv[maxdigit],
    int NofMDPs);

Node* CreatTree(uint32_t* shuffle,
    unsigned char MDP[maxdigit][Hashlength],
    int NofMDPs);

void give_proof(uint32_t ProveV,//����֤����ֵ
    uint32_t* MDPv,
    int NofMDPs,
    const unsigned char seed[maxdigit][Hashlength],
    const unsigned char salt[maxdigit][Hashlength],
    uint32_t* shuffle,
    Node* root,
    //////////////////////////////����Ϊ���͸���֤�ߵ�����
    unsigned char rootcommit[Hashlength],
    unsigned char start[maxdigit][Hashlength],
    unsigned char Salt[Hashlength],
    uint32_t& Shuffle,
    unsigned char pad[Treeheight - 1][Hashlength]);

bool confirmation(uint32_t Value,
    unsigned char rootcommit[Hashlength],
    unsigned char start[maxdigit][Hashlength],
    unsigned char Salt[Hashlength],
    uint32_t Shuffle,
    unsigned char pad[Treeheight - 1][Hashlength]);