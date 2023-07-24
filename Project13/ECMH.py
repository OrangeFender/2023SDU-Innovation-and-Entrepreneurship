import numpy as np
import sympy
import hashlib


p=int('0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3',16)
a=int('0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498',16)  
b=int('0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A',16) 


#最大公约数
def get_gcd(x, y):
    if y == 0:
        return x
    else:
        return get_gcd(y, x % y)

#p+q
def calculate_p_q(x1, y1, x2, y2, a, b, p):
    flag = 1

    if x1 == x2 and y1 == y2:
        member = 3 * (x1 ** 2) + a  
        denominator = 2 * y1  

    else:
        member = y2 - y1
        denominator = x2 - x1
        if member * denominator < 0:
            flag = 0
            member = abs(member)
            denominator = abs(denominator)

    gcd_value = get_gcd(member, denominator)
    member = member // gcd_value
    denominator = denominator // gcd_value

    inverse_value = sympy.mod_inverse(denominator, p)
    k = (member * inverse_value)
    if flag == 0:
        k = -k
    k = k % p

    x3 = (k ** 2 - x1 - x2) % p
    y3 = (k * (x1 - x3) - y1) % p
    return [x3, y3]


def sha256(data):
    hash_obj = hashlib.sha256()
    hash_obj.update(data)
    return hash_obj.digest()


def multi_hash(set):
    point_0=[0,0]
    for item in set:
        hash_result = sha256(item)
        x=int(hash_result.hex(),16)  
        tem=np.mod(x**2+a*x+b,p)
        y = sympy.ntheory.residue_ntheory.nthroot_mod(tem,2,p)
        while(y==None):
            tem=tem+1
            y = sympy.ntheory.residue_ntheory.nthroot_mod(tem,2,p)
        point=[x,y]
        calculate_p_q(point[0], point[1], point_0[0], point_0[1], a, b, p)
    return point

data_set = set()
data_set.add(b'\x01\x02\x03')
data_set.add(b'\x04\x05\x06')

print(multi_hash(data_set))



