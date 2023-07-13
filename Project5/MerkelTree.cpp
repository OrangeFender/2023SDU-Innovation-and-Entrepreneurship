#include<iostream>
#include "openssl/sha.h"
#include<cstring>
#include <windows.h>

#define transection_length 256
#define hashlength 32
class Node;
typedef struct transection {
    int ID;
    unsigned char detial[transection_length];
    Node* father;
    
}transection;

class Node {
public:
    unsigned char hash[hashlength];
    Node* father;
    Node* lchild;
    Node* rchild;
    transection* son;
    bool isleftchild() {
        return father->lchild == this;
    }
    Node(transection* Son) {//用于构造叶子哈希节点
        father = NULL;
        lchild = NULL;
        rchild = NULL;
        son = Son;
        Son->father= this;
        unsigned char *s= new unsigned char[transection_length + 1];
        s[0] = 0x00;
        memcpy(s + 1, son->detial, transection_length);
        SHA256(s, transection_length + 1, hash);
        delete[] s;
    }
    Node(Node *l,Node * r) {//用于构造非叶子节点
        father = NULL;
        son = NULL;
        lchild = l;
        rchild = r;
        unsigned char *s= new unsigned char[hashlength*2+1];
        s[0] = 0x01;
        memcpy(s + 1, l->hash, hashlength);
        memcpy(s + 1 + hashlength, r->hash, hashlength);
        SHA256(s, hashlength * 2 + 1, hash);
        l->father = this;
        r->father = this;
        delete[] s;
    }
    ~Node() {

        if (son) {
            son->father = NULL;
        }
    }
    Node();
    void printhash() {
        for (int i = 0; i < hashlength; i++) {
            std::cout<< std::hex << (int)hash[i];
        }
        std::cout<<std::endl;
    }
};


Node* build(int amount, transection* trans) {
    int oldamlount=amount;
    Node** oldOrder = new Node*[oldamlount];
    for (int i = 0; i < oldamlount; i++)
        oldOrder[i] =new Node(&trans[i]);
    Node** newOrder;
    while (oldamlount > 1) {
        int newamount = (oldamlount + 1) >> 1;
        newOrder = new Node*[oldamlount];
        if (oldamlount % 2) {
            newOrder[newamount-1] = oldOrder[oldamlount -1];//如果结点数组的数目是奇数，复制最后一个结点，并将其添加到列表末尾
        }
        for (int i = 0; i < (newamount- (oldamlount % 2)); i += 1) {
            newOrder[i] = new Node(oldOrder[i*2], oldOrder[i*2 + 1]);//将原有的结点两两结合成父点
        }
        delete[] oldOrder;
        oldOrder = newOrder;
        oldamlount = newamount;
    }
    Node* ret = oldOrder[0];
    delete[] oldOrder;
    return ret;
}

void deleteTree(Node* root) {
    if (root == NULL) {
        return;
    }
    deleteTree(root->lchild);
    deleteTree(root->rchild);
    delete root;
}
/*
void Inclusion(int num, int amount, transection* trans) {//为节省篇幅，在此只打印出证明路径，具体的证明计算由被证明者自行验证
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID < num)
            break;
    Node* n = trans[i].father;
    std::cout << "待证明叶子的哈希是" << std::endl;
    n->printhash();
    while (n->father != NULL) {
        if (n->isleftchild() ){
            std::cout <<"在右级联"<<std::endl;
            n->father->rchild->printhash();
        }
        else{
            std::cout << "在左级联" << std::endl;
            n->father->lchild->printhash();
        }
        n = n->father;
    }
}

void Exclusion(int num,int amount, transection* trans) {
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID < num)
            break;
    if (i == amount) {
        std::cout << "超出范围" << std::endl;
        return;
    }
    if (trans[i + 1].ID < num) {
        std::cout << "超出范围" << std::endl;
        return;
    }
    if (trans[i + 1].ID == num) {
        std::cout << "存在"<<std::endl;
        return;
    }
    if (trans[i + 1].father->isleftchild()) {//如果右叶子是左子
        Node* rn = trans[i + 1].father->father;
        std::cout << "右叶子哈希是" << std::endl;
        rn->printhash();
        std::cout << "右叶子的兄弟哈希是" << std::endl;
        rn->father->rchild->printhash();
        std::cout << "右叶子的父亲哈希是" << std::endl;
        rn->father->printhash();

        Node* n = trans[i].father;
        std::cout << "左叶子哈希是"<<std::endl;
        n->printhash();
        std::cout << "现证明左叶子的存在性：左叶子哈希" ;
        while (n->father != NULL) {
            Node* tmp;
            if (n->isleftchild()) {
                std::cout << "在右级联" << std::endl;
                tmp = n->father->rchild;
                tmp->printhash();
            }
            else {
                std::cout << "在左级联" << std::endl;
                tmp = n->father->rchild;
                n->father->lchild->printhash();
            }
            if (tmp == rn) {
                std::cout << "右叶子结点的父亲结点出现在左节点的路径中，故证明了两节点相邻";
            }
            n = n->father;
        }
    }
    else{
         Node* rn = trans[i + 1].father;
         std::cout << "右叶子哈希是" << std::endl;
         rn->printhash();
         Node* n = trans[i].father;
         std::cout << "左叶子哈希是" << std::endl;
         n->printhash();
         std::cout << "现证明左叶子的存在性：左叶子哈希";
         
         while (n->father != NULL) {
             Node* tmp;
             if (n->isleftchild()) {
                 std::cout << "在右级联" << std::endl;
                 tmp = n->father->rchild;
                    tmp->printhash();
                }
             else {
                 std::cout << "在左级联" << std::endl;
                 tmp = n->father->rchild;
                 n->father->lchild->printhash();
             }
             if (tmp == rn) {
                 std::cout << "右叶子结点出现在左节点的路径中，故证明了两节点相邻";
             }
             n = n->father;
             }
    }
}
*/

typedef class proveNode{
public:
    bool LorR;
    char hash[hashlength];
    proveNode(bool LR, char* h) {
        memcpy(hash, h, hashlength);
        LorR = LR;
    }
    proveNode() {
    };
}proveNode;


void InclusionProofer(int num,int amount,char hash[hashlength], proveNode* path, int &pathlen, transection* trans) {//服务端给出证明路径
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID == num)
            break;
    if (i == amount)return;
    Node* n = trans[i].father;
    //std::cout << "待证明叶子的哈希是" << std::endl;
    
    memcpy(hash, n->hash, hashlength);
    i = 0;
    while (n->father != NULL) {
        if (n->isleftchild()) {
            path[i]=proveNode(0, (char*)n->father->rchild->hash);
        }
        else {
            path[i] = proveNode(1, (char*)n->father->lchild->hash);
        }
        n = n->father;
        i++;
        }
    pathlen = i;
}

void ExclusionProofer(int num, int amount, char Lhash[hashlength], proveNode* Lpath, int &Lpathlen, char Rhash[hashlength], proveNode* Rpath, int &Rpathlen, transection* trans) {//服务端给出证明路径
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID > num)
            break;
    if (i == amount) {
        std::cout << "超出范围" << std::endl;
        return;
    }
    if (trans[i + 1].ID < num) {
        std::cout << "超出范围" << std::endl;
        return;
    }
    if (trans[i + 1].ID == num) {
        std::cout << "存在" << std::endl;
        return;
    }
    Node* n = trans[i].father;
    Node* m= trans[i+1].father;
    //std::cout << "待证明叶子的哈希是" << std::endl;
    n->printhash();
    memcpy(Lhash, n->hash, hashlength);
    i = 0;
    while (n->father != NULL) {
        if (n->isleftchild()) {
            Lpath[i] = proveNode(0, (char*)n->father->rchild->hash);
        }
        else {
            Lpath[i] = proveNode(1, (char*)n->father->lchild->hash);
        }
        n = n->father;
        i++;
    }
    Lpathlen = i;
    //std::cout << "待证明叶子的哈希是" << std::endl;
    m->printhash();
    memcpy(Rhash, m->hash, hashlength);
    i = 0;
    while (m->father != NULL) {    
        if (m->isleftchild()) {
            Rpath[i] = proveNode(0, (char*)m->father->rchild->hash);
        }
        else {
            Rpath[i] = proveNode(1, (char*)m->father->lchild->hash);
        }
        m = m->father;
        i++;
    }
    Rpathlen = i;

}
bool InclusionProof(proveNode* path,int pathlen,char*hash,char*root){//客户端接收并验证证明的过程
    unsigned char H[hashlength];
    memcpy(H, hash, hashlength);
    for (int i = 0; i < pathlen; i++) {
        if (path[i].LorR) {//在左边级联
            char s[hashlength * 2 + 1];
            s[0] = 1;
            memcpy(s + 1, path[i].hash, hashlength);
            memcpy(s + 1 + hashlength, H, hashlength);
            SHA256((unsigned char*)s, hashlength * 2 + 1, (unsigned char*)H);

        }
        else {
            char s[hashlength * 2 + 1];
            s[0] = 1;
            memcpy(s + 1+ hashlength , path[i].hash, hashlength);
            memcpy(s + 1, H, hashlength);
            SHA256((unsigned char*)s, hashlength * 2 + 1, (unsigned char*)H);
        }
        /*
        for (int i = 0; i < hashlength; i++) {
            std::cout << std::hex << (int)H[i];
        }
        std::cout << std::endl;*/
    }
    bool ret=1;
    for (int i = 0; i < hashlength; i++)
        ret &= (H[i] == (unsigned char)root[i]);
    return ret;
}

bool ExclusionProof(proveNode* Lpath, int Lpathlen, char* Lhash, proveNode* Rpath, int Rpathlen, char* Rhash, char* root) {//客户端接收并验证证明的过程
    if (InclusionProof(Lpath, Lpathlen, Lhash, root) == false || InclusionProof(Rpath, Rpathlen, Rhash, root) == false)
        return false;
    Lpathlen--; Rpathlen--;
    while (Lpath[Lpathlen].LorR == Rpath[Rpathlen].LorR){//记录分叉的起始
        Lpathlen--;
        Rpathlen--;
    }
    Lpathlen--;
    Rpathlen--;
    while (Lpathlen >= 0) {
        if (Lpath[Lpathlen].LorR == 0) {//分叉后只能往右走
            return false;
        }
        Lpathlen--;
    }
    while (Rpathlen >= 0) {
        if (Rpath[Rpathlen].LorR == 1) {//分叉后只能往左走
            return false;
        }
        Rpathlen--;
    }
    return true;
}
int main() {//测试程序，构造10w笔订单，订单编号为连续的偶数，证明一个偶数订单存在，并证明一个奇数订单不存在
    int amount = 100000;
    transection* T=new transection[amount];
    for (int i = 0; i < amount; i++) {
        memset(T[i].detial, 0, transection_length);
        T[i].ID = i * 2;
        *(int*)(T[i].detial+4)= i * 2;
    }

    LARGE_INTEGER frequency, start, end;
    double elapsed_time;

    // 获取频率
    QueryPerformanceFrequency(&frequency);
    // 获取开始时间
    QueryPerformanceCounter(&start);
    Node* root = build(amount, T);
    QueryPerformanceCounter(&end);
    // 计算运行时间（以毫秒为单位）
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    // 输出运行时间
    printf("build运行时间：%.2f 毫秒\n", elapsed_time);
    root->printhash();
    int pathlen1=0,pathlen2=0;

    char hash1[32],hash2[32];
    proveNode path1[20],path2[20];
    //证明24912存在
    InclusionProofer(24912,amount,hash1,path1,pathlen1,T);//服务端提供证明
    bool poof=InclusionProof(path1, pathlen1, hash1, (char *)root->hash);//客户端完成证明
    std::cout << poof<<std::endl;
    //证明24913不存在
    ExclusionProofer(24913, amount, hash1, path1, pathlen1, hash2, path2, pathlen2, T);//服务端提供证明
    poof = ExclusionProof(path1, pathlen1, hash1, path2, pathlen2, hash2, (char*)root->hash);//客户端完成证明
    std::cout << poof << std::endl;
    deleteTree(root);
    
}