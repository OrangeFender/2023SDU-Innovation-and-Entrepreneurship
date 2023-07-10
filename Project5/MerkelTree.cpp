#include<iostream>
#include "openssl/sha.h"

#define transection_length 1024//假设交易信息为1024字节
struct Node;
typedef struct transection {
    unsigned char detial[1024];
    Node* father;
}transection;

class Node {
public:
    unsigned char hash[256];
    Node* father;
    Node* lchild;
    Node* rchild;
    transection* lleaf;
    transection* rleaf;

    Node(transection* lleaf,transection* rleaf) {
        lchild = NULL;
        rchild = NULL;
        unsigned char s[2* transection_length+1];
        memcpy(S)
        transection

    }
    Node()
};

Node* buildFulltree(transection * leaf,int N) {
    if (N == 2) {
        leaf[0]
    }
}

int build(int amount, transection* trans) {
    int depth;
    for (depth = 0; (amount >> 1) > 0; depth++);

    Node* roots[32];
    for (int i = depth-1; i>=0; i--) {
        buildFulltree();

    }
}
