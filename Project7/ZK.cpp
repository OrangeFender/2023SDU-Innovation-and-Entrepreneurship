#include<iostream>
#include"openssl/sha.h"
#include <ctime>
#include<stdint.h>
#define basevalue 16
#define basevaluebit 4
#define basevaluemask 0xF
#define maxdigit 8
#define Hashlength 32
#define Treeheight 10//Merkel Tree高度为10
#define Shufflemask 0x1FF


void generateRandomArray(unsigned char* arr, int size) {
    std::srand(static_cast<unsigned>(std::time(0)));
    for (int i = 0; i < size; ++i) {
        arr[i] = (unsigned char)std::rand();
    }
}

typedef class Node {
public:
    unsigned char hash[Hashlength];
    Node* father;
    Node* lchild;
    Node* rchild;
    bool pad;
    bool finished;
    Node(Node *f) { 
        father = f;
        lchild = NULL;
        rchild = NULL;
        pad = true;
        finished = false;
    };
    ~Node() {
        if(lchild)
            delete lchild;
        if(rchild)
            delete rchild;
    }
    void print();
}Node;

void Node::print() {
    for (int i = 0; i < Hashlength; i++) {
        std::cout << std::hex << (int)hash[i];
    }
    std::cout << std::endl;
    if (lchild)
        lchild->print();
    if (rchild)
        rchild->print();
}


void countMDPs(uint32_t Value,
    uint32_t MDPv[maxdigit], //存储每一种证明的值
    int &NofMDPs//记录数组的长度
    ) {
    
    uint32_t mask = 0;
    MDPv[0] = Value;
    int ind = 1;
    for (int i = 1; i < maxdigit; i++) {
        uint32_t V = Value;
        mask |= basevaluemask << (4 * i);
        if ((V & mask) ^ mask) {
            V -=0x10 << (4 * i);
            V |= mask;
            if (V >= Value)
                break;
            MDPv[ind] = V;
            ind++;
        }
    }
    NofMDPs = ind;
}

void creatMDP(unsigned char seed[maxdigit][Hashlength],
    unsigned char salt[maxdigit][Hashlength],
    unsigned char MDP[maxdigit][Hashlength],
    uint32_t MDPv[maxdigit],
    int NofMDPs) {
    for (int i = 0; i < maxdigit; i++)
        generateRandomArray(seed[i], Hashlength);
    for(int i=0;i<NofMDPs;i++)
        generateRandomArray(salt[i], Hashlength);
    unsigned char*** h = new unsigned char** [maxdigit];
    for (int i = 0; i < maxdigit; i++) {
        h[i] = new unsigned char* [basevalue];
        for (int j = 0; j < basevalue; j++) {
            h[i][j] = new unsigned char[Hashlength];
        }
    }//用于存储所有的中间哈希值
    uint32_t V = MDPv[0];
    int max = 0;
    for (int i = 0; i < maxdigit; i++) {
        memcpy(h[i][0], seed[i],Hashlength);
    }
    while (V >> basevaluebit !=0) {
        V=V >> basevaluebit;
        max++;
    }
    for (uint32_t i = 1; i <= V; i++) {
        SHA256(h[max][i - 1], Hashlength, h[max][i]);
    }
    for (int j = 0; j < max; j++) {
        for (int i = 1; i < basevalue; i++) {
            SHA256(h[j][i - 1], Hashlength, h[j][i]);
        }
    }
    unsigned char tmp[(maxdigit + 1) * Hashlength];
    for (int j = 0; j < NofMDPs;j++ ) {
        memcpy(tmp,salt[j],Hashlength);
        uint32_t t = MDPv[j];
        for (int i = 0; i < maxdigit; i++) {
            memcpy(tmp+(i+1)*Hashlength,h[i][t&basevaluemask], Hashlength);
            t=t >> basevaluebit;
        }
        SHA256(tmp,(maxdigit + 1) * Hashlength,MDP[j]);
    }
    for (int i = 0; i < maxdigit; i++) {
        for (int j = 0; j < basevalue; j++) {
            delete[] h[i][j];
        }
        delete[] h[i];
    }
    delete[] h;
}

Node* CreatTree(uint32_t*shuffle,  unsigned char MDP[maxdigit][Hashlength], int NofMDPs) {
    Node** leaf = new Node*[NofMDPs];
    for (int i = 0; i < NofMDPs; i++) {
        while (true)
        {
            shuffle[i] = std::rand()&Shufflemask;
            for (int j = 0; j < i; j++)
                if (shuffle[j] = shuffle[j])
                    continue;
            break;
        }
    }
    Node* root = new Node(NULL);
    
    for (int j = 0; j < NofMDPs; j++) {
        Node* n=root;
        for (int i = Treeheight - 2; i >= 0; i--) {
            if (n->rchild == NULL) {//确保每个节点都有兄弟节点
                n->rchild = new Node(n);
            }
            if (n->lchild == NULL) {
                n->lchild = new Node(n);
            }
            if (shuffle[j] >> i & 1) {//位为1则向右
                n = n->rchild;
            }
            else {
                n = n->lchild;
            }
            n->pad = false;
        }
        leaf[j] = n;
        memcpy(n->hash, MDP[j], Hashlength);
        n->finished = true;
    }
    for (int j = 0; j < NofMDPs; j++) {
        Node* n = leaf[j]->father;
        while (n != NULL) {
            if (n->lchild->finished == false && n->lchild->pad == true) {
                generateRandomArray(n->lchild->hash, Hashlength);
                n->lchild->finished = true;
            }
            if (n->rchild->finished == false && n->rchild->pad == true) {
                generateRandomArray(n->rchild->hash, Hashlength);
                n->rchild->finished = true;
            }
            if (n->lchild->finished == false || n->rchild->finished == false)
                break;
            else
            {
                unsigned char tmp[Hashlength * 2];
                memcpy(tmp, n->lchild->hash, Hashlength);
                memcpy(tmp + Hashlength, n->rchild->hash, Hashlength);
                SHA256(tmp, Hashlength * 2, n->hash);
                n->finished = true;
                n = n->father;
            }
        }
    }
    return root;
}
void give_proof(uint32_t ProveV,//请求证明的值
    uint32_t* MDPv,
    int NofMDPs,
    const unsigned char seed[maxdigit][Hashlength],
    const unsigned char salt[maxdigit][Hashlength],
    uint32_t* shuffle,
    Node* root,
    //////////////////////////////以下为发送给验证者的内容
    unsigned char rootcommit[Hashlength],
    unsigned char start[maxdigit][Hashlength],
    unsigned char Salt[Hashlength],
    uint32_t&Shuffle,
    unsigned char pad[Treeheight-1][Hashlength]) {
    memcpy(rootcommit, root->hash, Hashlength);
    if (ProveV > MDPv[0])
        return;
    int i;
    for(i=0;i<NofMDPs;i++){
        if (MDPv[i] < ProveV) {
            i--;
            break;
        }
    }
    if (i == NofMDPs)
        i--;
    Shuffle = shuffle[i];
    memcpy(Salt, salt[i], Hashlength);
    uint32_t diff=MDPv[i] - ProveV;
    for (int i = 0; i < maxdigit; i++) {
        memcpy(start[i], seed[i], Hashlength);
    }
    for (int i = 0; i < maxdigit; i++) {
        int v = diff & basevaluemask;
        for (int j = 0; j < v; j++) {
            SHA256(start[i], Hashlength, start[i]);
        }
        diff=diff>>basevaluebit;
    }
    Node *n= root;
    Node* tmp;
    int s = Shuffle;
    for (int i = Treeheight-2;i>=0;i--) {//从下往上索引
        if ((s >> i) & 1) {
            tmp = n->lchild;
            n = n->rchild;
        }
        else {
            tmp = n->rchild;
            n = n->lchild;
        }    
        memcpy(pad[i], tmp->hash, Hashlength);
    }
}


bool confirmation(uint32_t Value,
    unsigned char rootcommit[Hashlength],
    unsigned char start[maxdigit][Hashlength],
    unsigned char Salt[Hashlength],
    uint32_t Shuffle,
    unsigned char pad[Treeheight - 1][Hashlength]) {
    for (int i = 0; i < maxdigit; i++) {
        int v = Value>>(i*basevaluebit) & basevaluemask;
        for (int j = 0; j < v; j++) {
            SHA256(start[i], Hashlength, start[i]);
        }
    }
    unsigned char tmp[Hashlength * (maxdigit + 1)];
    memcpy(tmp, Salt, Hashlength);
    for (int i = 0; i < maxdigit; i++) {
        memcpy(tmp + (i + 1) * Hashlength, start[i], Hashlength);
    }
    unsigned char s[Hashlength];
    SHA256(tmp, Hashlength * (maxdigit + 1), s);
    for (int i = 0; i <Treeheight-1; i++) {
        if (Shuffle >> i & 1) {//如果是右兄
            memcpy(tmp + Hashlength, s, Hashlength);
            memcpy(tmp, pad[i], Hashlength);
        }
        else {
            memcpy(tmp, s, Hashlength);
            memcpy(tmp + Hashlength, pad[i], Hashlength);
        }
        SHA256(tmp, Hashlength * 2, s);
    }
    return memcmp(s, rootcommit, Hashlength)==0;
}

