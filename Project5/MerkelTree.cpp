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
    Node(transection* Son) {//���ڹ���Ҷ�ӹ�ϣ�ڵ�
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
    Node(Node *l,Node * r) {//���ڹ����Ҷ�ӽڵ�
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
            newOrder[newamount-1] = oldOrder[oldamlount -1];//�������������Ŀ���������������һ����㣬��������ӵ��б�ĩβ
        }
        for (int i = 0; i < (newamount- (oldamlount % 2)); i += 1) {
            newOrder[i] = new Node(oldOrder[i*2], oldOrder[i*2 + 1]);//��ԭ�еĽ��������ϳɸ���
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
void Inclusion(int num, int amount, transection* trans) {//Ϊ��ʡƪ�����ڴ�ֻ��ӡ��֤��·���������֤�������ɱ�֤����������֤
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID < num)
            break;
    Node* n = trans[i].father;
    std::cout << "��֤��Ҷ�ӵĹ�ϣ��" << std::endl;
    n->printhash();
    while (n->father != NULL) {
        if (n->isleftchild() ){
            std::cout <<"���Ҽ���"<<std::endl;
            n->father->rchild->printhash();
        }
        else{
            std::cout << "������" << std::endl;
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
        std::cout << "������Χ" << std::endl;
        return;
    }
    if (trans[i + 1].ID < num) {
        std::cout << "������Χ" << std::endl;
        return;
    }
    if (trans[i + 1].ID == num) {
        std::cout << "����"<<std::endl;
        return;
    }
    if (trans[i + 1].father->isleftchild()) {//�����Ҷ��������
        Node* rn = trans[i + 1].father->father;
        std::cout << "��Ҷ�ӹ�ϣ��" << std::endl;
        rn->printhash();
        std::cout << "��Ҷ�ӵ��ֵܹ�ϣ��" << std::endl;
        rn->father->rchild->printhash();
        std::cout << "��Ҷ�ӵĸ��׹�ϣ��" << std::endl;
        rn->father->printhash();

        Node* n = trans[i].father;
        std::cout << "��Ҷ�ӹ�ϣ��"<<std::endl;
        n->printhash();
        std::cout << "��֤����Ҷ�ӵĴ����ԣ���Ҷ�ӹ�ϣ" ;
        while (n->father != NULL) {
            Node* tmp;
            if (n->isleftchild()) {
                std::cout << "���Ҽ���" << std::endl;
                tmp = n->father->rchild;
                tmp->printhash();
            }
            else {
                std::cout << "������" << std::endl;
                tmp = n->father->rchild;
                n->father->lchild->printhash();
            }
            if (tmp == rn) {
                std::cout << "��Ҷ�ӽ��ĸ��׽���������ڵ��·���У���֤�������ڵ�����";
            }
            n = n->father;
        }
    }
    else{
         Node* rn = trans[i + 1].father;
         std::cout << "��Ҷ�ӹ�ϣ��" << std::endl;
         rn->printhash();
         Node* n = trans[i].father;
         std::cout << "��Ҷ�ӹ�ϣ��" << std::endl;
         n->printhash();
         std::cout << "��֤����Ҷ�ӵĴ����ԣ���Ҷ�ӹ�ϣ";
         
         while (n->father != NULL) {
             Node* tmp;
             if (n->isleftchild()) {
                 std::cout << "���Ҽ���" << std::endl;
                 tmp = n->father->rchild;
                    tmp->printhash();
                }
             else {
                 std::cout << "������" << std::endl;
                 tmp = n->father->rchild;
                 n->father->lchild->printhash();
             }
             if (tmp == rn) {
                 std::cout << "��Ҷ�ӽ���������ڵ��·���У���֤�������ڵ�����";
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


void InclusionProofer(int num,int amount,char hash[hashlength], proveNode* path, int &pathlen, transection* trans) {//����˸���֤��·��
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID == num)
            break;
    if (i == amount)return;
    Node* n = trans[i].father;
    //std::cout << "��֤��Ҷ�ӵĹ�ϣ��" << std::endl;
    
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

void ExclusionProofer(int num, int amount, char Lhash[hashlength], proveNode* Lpath, int &Lpathlen, char Rhash[hashlength], proveNode* Rpath, int &Rpathlen, transection* trans) {//����˸���֤��·��
    int i;
    for (i = 0; i < amount; i++)
        if (trans[i].ID > num)
            break;
    if (i == amount) {
        std::cout << "������Χ" << std::endl;
        return;
    }
    if (trans[i + 1].ID < num) {
        std::cout << "������Χ" << std::endl;
        return;
    }
    if (trans[i + 1].ID == num) {
        std::cout << "����" << std::endl;
        return;
    }
    Node* n = trans[i].father;
    Node* m= trans[i+1].father;
    //std::cout << "��֤��Ҷ�ӵĹ�ϣ��" << std::endl;
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
    //std::cout << "��֤��Ҷ�ӵĹ�ϣ��" << std::endl;
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
bool InclusionProof(proveNode* path,int pathlen,char*hash,char*root){//�ͻ��˽��ղ���֤֤���Ĺ���
    unsigned char H[hashlength];
    memcpy(H, hash, hashlength);
    for (int i = 0; i < pathlen; i++) {
        if (path[i].LorR) {//����߼���
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

bool ExclusionProof(proveNode* Lpath, int Lpathlen, char* Lhash, proveNode* Rpath, int Rpathlen, char* Rhash, char* root) {//�ͻ��˽��ղ���֤֤���Ĺ���
    if (InclusionProof(Lpath, Lpathlen, Lhash, root) == false || InclusionProof(Rpath, Rpathlen, Rhash, root) == false)
        return false;
    Lpathlen--; Rpathlen--;
    while (Lpath[Lpathlen].LorR == Rpath[Rpathlen].LorR){//��¼�ֲ����ʼ
        Lpathlen--;
        Rpathlen--;
    }
    Lpathlen--;
    Rpathlen--;
    while (Lpathlen >= 0) {
        if (Lpath[Lpathlen].LorR == 0) {//�ֲ��ֻ��������
            return false;
        }
        Lpathlen--;
    }
    while (Rpathlen >= 0) {
        if (Rpath[Rpathlen].LorR == 1) {//�ֲ��ֻ��������
            return false;
        }
        Rpathlen--;
    }
    return true;
}
int main() {//���Գ��򣬹���10w�ʶ������������Ϊ������ż����֤��һ��ż���������ڣ���֤��һ����������������
    int amount = 100000;
    transection* T=new transection[amount];
    for (int i = 0; i < amount; i++) {
        memset(T[i].detial, 0, transection_length);
        T[i].ID = i * 2;
        *(int*)(T[i].detial+4)= i * 2;
    }

    LARGE_INTEGER frequency, start, end;
    double elapsed_time;

    // ��ȡƵ��
    QueryPerformanceFrequency(&frequency);
    // ��ȡ��ʼʱ��
    QueryPerformanceCounter(&start);
    Node* root = build(amount, T);
    QueryPerformanceCounter(&end);
    // ��������ʱ�䣨�Ժ���Ϊ��λ��
    elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    // �������ʱ��
    printf("build����ʱ�䣺%.2f ����\n", elapsed_time);
    root->printhash();
    int pathlen1=0,pathlen2=0;

    char hash1[32],hash2[32];
    proveNode path1[20],path2[20];
    //֤��24912����
    InclusionProofer(24912,amount,hash1,path1,pathlen1,T);//������ṩ֤��
    bool poof=InclusionProof(path1, pathlen1, hash1, (char *)root->hash);//�ͻ������֤��
    std::cout << poof<<std::endl;
    //֤��24913������
    ExclusionProofer(24913, amount, hash1, path1, pathlen1, hash2, path2, pathlen2, T);//������ṩ֤��
    poof = ExclusionProof(path1, pathlen1, hash1, path2, pathlen2, hash2, (char*)root->hash);//�ͻ������֤��
    std::cout << poof << std::endl;
    deleteTree(root);
    
}