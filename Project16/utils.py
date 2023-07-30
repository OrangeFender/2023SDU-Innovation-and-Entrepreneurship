import gmssl.sm3
import sympy
import random

p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3

a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498

b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A

xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D

yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2

n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7

M="hello"

IDA=0x414C494345313233405941484F4F2E434F4D






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

#2p
def calculate_2p(p_x, p_y, a, b, p):
    tem_x = p_x
    tem_y = p_y
    p_value = calculate_p_q(tem_x, tem_y, p_x, p_y, a, b, p)
    tem_x = p_value[0]
    tem_y = p_value[1]
    return p_value


# nP
def calculate_np(p_x, p_y, n, a, b, p):
    p_value = ["0", "0"]
    p_temp = [0, 0]
    while n != 0:
        if n & 1:
            if (p_value[0] == "0" and p_value[1] == "0"):
                p_value[0], p_value[1] = p_x, p_y
            else:
                p_value = calculate_p_q(p_value[0], p_value[1], p_x, p_y, a, b, p)
        n >>= 1
        p_temp = calculate_2p(p_x, p_y, a, b, p)
        p_x, p_y = p_temp[0], p_temp[1]
    return p_value

def get_key():
    dA=random.randrange(0,n)
    PA=calculate_np(xG, yG, dA, a, b, p)
    return dA,PA

def sm3(mess):
    msg = bytearray(mess)  # 将 bytes 转换为 bytearray
    #hash_obj = sm3.sm3_hash(msg) #返回值str类型
    hash_obj = gmssl.sm3.sm3_hash(msg)
    hash_bytes=hash_obj.encode() #转化为字节串
    return hash_bytes

def get_bitsize(num):
    len=0
    while num/256:
        len+=1
        num=num//256
    return len

def int_to_bytes(num):
    return num.to_bytes(get_bitsize(num),byteorder='big', signed=False)

def bytes_to_int(bytes):
    return int.from_bytes(bytes,byteorder='big')

def sign_with_RFC6979(m):
    return bytes_to_int(sm3(m))

def KDF(Z,klen):
    v=256
    n = (klen // v) + 1
    if n >= 2 ** 32 - 1: return 'error'
    K = ''
    for i in range(n):
        bit_str = bin(i)[2:].rjust(32, '0')
        tem=Z+bit_str
        int_data = int(tem,2)
        byte_data = int_data.to_bytes((len(tem) + 7) // 8, byteorder='big')
        msg = bytearray(byte_data)
        hash_obj = gmssl.sm3.sm3_hash(msg)
        K=K+hash_obj
    bit_len = 256 * n
    K = (bin(int(K, 16))[2:]).rjust(bit_len, '0')
    K = K[:klen]  # MSB(K, klen)
    return K

def str_to_bits(s):
    """将一个字符串转换为一个比特串"""
    bits = ''
    for c in s:
        bits += bin(ord(c))[2:].zfill(8)  # 将每个字符转换为8位二进制数，并拼接成比特串
    return bits

def bits_to_str(bits):
    """将一个比特串转换为一个字符串"""
    if len(bits) % 8 != 0:
        raise ValueError('比特串的长度必须是8的倍数')
    s = ''
    for i in range(0, len(bits), 8):
        byte = bits[i:i+8]
        s += chr(int(byte, 2))
    return s
def xor(bitstr1, bitstr2):
    """将两个二进制串进行异或"""
    if len(bitstr1) != len(bitstr2):
        raise ValueError('长度不一致')
    result = ''
    for i in range(len(bitstr1)):
        if bitstr1[i] == bitstr2[i]:
            result += '0'
        else:
            result += '1'
    return result

M_bits=str_to_bits(M)
klen=len(M_bits)

def Encrypt(M,P,klen):
    k = random.randint(1,n-1)
    C1=calculate_np(xG, yG, k, a, b, p)
    kP=calculate_np(P[0], P[1], k, a, b, p)
    str_x2 = str(bin(kP[0]))
    str_y2 = str(bin(kP[1]))
    str_x2y2=(str_x2[2:]+str_y2[2:])

    t=KDF(str_x2y2,klen)
    C2=xor(M_bits,t)

    tem=str_x2+M_bits+str_y2
    tem1=tem.encode()
    msg = bytearray(tem1)
    C3 = gmssl.sm3.sm3_hash(msg)

    return C1,C2,C3


d1=random.randint(1,n-1)

d2=random.randint(1,n-1)

d=sympy.mod_inverse(d1*d2, n)-1


P=calculate_np(xG, yG, d, a, b, p)



C1,C2,C3=Encrypt(M,P,klen)


if(C1==0):
    print("C1=0,False\n")



d1_inv=sympy.mod_inverse(d1, n)
d2_inv=sympy.mod_inverse(d2, n)

T1=calculate_np(C1[0], C1[1], d1_inv, a, b, p)

T2=calculate_np(T1[0], T1[1], d2_inv, a, b, p)

C1_inv=[0,0]
C1_inv[0]=C1[0]
C1_inv[1]=p-C1[1]

T3=calculate_p_q(T2[0], T2[1], C1_inv[0], C1_inv[1], a, b, p)

str_x2 = str(bin(T3[0]))
str_y2 = str(bin(T3[1]))
str_x2y2=(str_x2[2:]+str_y2[2:])
t=KDF(str_x2y2,klen)
M_=xor(C2,t)

tem = str_x2 + M_ + str_y2
tem1 = tem.encode()
msg = bytearray(tem1)
u = gmssl.sm3.sm3_hash(msg)

if(u==C3):
    print(bits_to_str(M_))

print(klen)

print(C1)

print(C2)

print(C3)

print(d1)

print(d2)

print(P)