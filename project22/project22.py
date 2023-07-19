import math
import hashlib,json
import random

N=7


def Create(DATA):
    MerkleTree = [[]]
    length = len(DATA)
    Depth = math.ceil(math.log(length, 2))+1
    MerkleTree[0] = [(hashlib.sha256(i.encode())).hexdigest() for i in DATA]

    for i in range(1, Depth):
        p = math.floor(len(MerkleTree[i-1])/2)
        MerkleTree.append([(hashlib.sha256(MerkleTree[i-1][2*j].encode() + \
                              MerkleTree[i-1][2*j+1].encode())).hexdigest() for j in range(0, p)])
        if len(MerkleTree[i-1])%2 == 1:
            MerkleTree[i].append(MerkleTree[i-1][-1])

    return MerkleTree



def GenerateMessages():
    return [''.join(random.sample('abcdefghijklmnopqrstuvwxyz0123456789',\
                                  5)) for i in range(0,N)]



def GenerateEvidence(m,Tree):
    h = (hashlib.sha256(m.encode())).hexdigest()
    
    try:
        n=Tree[0].index(h)
    except:
        print("The leaf node is  in the tree!!!")

    Depth = len(Tree)
    Evidence = []
    
    for d in range(0,Depth):
        if n%2==0:
            if n == len(Tree[d]) - 1:
                pass
            else:
                Evidence.append([Tree[d][n],Tree[d][n+1]])

        else:
            Evidence.append([Tree[d][n-1], Tree[d][n]])

        n = math.floor(n/2)

    Evidence.append([Tree[-1][0]])

    return Evidence


def VerifyEvidence(m,Evidence,Top):
    h = (hashlib.sha256(m.encode())).hexdigest()
    if h != Evidence[0][0] and h != Evidence[0][1]:
        return False

    if Evidence[-1][0] != Top:
        return False

    Depth = len(Evidence)
    for i in range(0,Depth-1):
        node = (hashlib.sha256(Evidence[i][0].encode() + Evidence\
                               [i][1].encode())).hexdigest()
        if node != Evidence[i+1][0] and node != Evidence[i+1][1]:
            return False

    if (hashlib.sha256(Evidence[-2][0].encode() + Evidence[-2][1].\
                       encode())).hexdigest() != Evidence[-1][0]:
        return False

    return True

#test


DATA = ['a','b','c','d','e','f','g']
Tree_1 = Create(DATA)
print('1.Create a MerkleTree\n')
print(Tree_1)
print('\n')


TestMessages = GenerateMessages()
Tree_2 = Create(TestMessages)
print('2.Construct a Merkle Tree \n')
print(Tree_2)
print('\n')


print('3.Build inclusion proof for specified element')
n=random.randint(0,N-1)
Evidence = GenerateEvidence(TestMessages[n],Tree_2)
print("The evidence is:")
print(VerifyEvidence(TestMessages[n],Evidence,Tree_2[-1][0]))
print('\n')


print('4.Build exclusion proof for specified element')
Evidence=GenerateEvidence("k",Tree_1)   
print("The evidence is:")
print(VerifyEvidence("k",Evidence,Tree_1[-1][0]))
