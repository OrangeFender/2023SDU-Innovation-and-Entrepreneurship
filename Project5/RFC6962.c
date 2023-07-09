struct Node;
typedef struct transection{
    char detial[1024];//假设交易信息为1024字节
    Node* father;
}transection;

typedef struct Node{
Node* father;
Node*lchild;
Node*rchild;
transection*lleaf;
transection*rleaf;
}Node;

Node* buildFulltree(){
    
}

int build(int amount,transection* trans){
int depth;
for(depth=0;(amount>>1)>0;depth++);

Node * roots;
for(int i=0;i<depth;i++)
}
