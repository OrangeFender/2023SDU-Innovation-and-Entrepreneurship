import socket
import sys

import gmssl.sm3
from sympy import mod_inverse
import sympy
import random
p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3

a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498

b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A

xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D

yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2

n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7

mess1="hello"

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

d1=random.randint(1,n)
def generate_P1(d1):

    #print(d1)
    d1_inv=mod_inverse(d1,n)
    #print(d1_inv)
    P1=calculate_np(xG, yG, d1_inv, a, b, p)
    return P1

P1=generate_P1(d1)

# 服务端主机IP地址和端口号
HOST = '127.0.0.1'
PORT = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    # 连接服务器
    s.connect((HOST, PORT))
except Exception as e:
    print('Server not found or not open')
    sys.exit()

'''while True:
    c = input('Input the content you want to send:')
    # 发送数据
    s.sendall(c.encode())
    # 从服务端接收数据
    data = s.recv(1024)
    data = data.decode()
    print('Received:', data)
    if c.lower() == 'bye':
        break
'''
#c1 = input('Input the content you want to send:')
# 发送数据
#s.sendall(c1.encode())

s.sendall(str(P1[0]).encode())
data1 = s.recv(1024).decode()

s.sendall(str(P1[1]).encode())
data2 = s.recv(1024).decode()


k1 = random.randint(1, n)
def generate_e_Q1(ID,mess,P1,k1):
    M = mess.encode()
    ENTL = get_bitsize(ID) * 8
    data = ENTL.to_bytes(2, byteorder='big', signed=False) + int_to_bytes(ID) + int_to_bytes(a) + int_to_bytes(
        b) + int_to_bytes(xG) + int_to_bytes(yG) + int_to_bytes(P1[0]) + int_to_bytes(P1[1])
    # print(data)
    ZA = sm3(data)
    M_ = ZA + M
    e = bytes_to_int(sm3(M_))


    Q1 = calculate_np(xG, yG, k1, a, b, p)

    return Q1, e
Q1,e=generate_e_Q1(IDA,mess1,P1,k1)


s.sendall(str(Q1[0]).encode())
data3 = s.recv(1024).decode()

s.sendall(str(Q1[1]).encode())
data4 = s.recv(1024).decode()

s.sendall(str(e).encode())
data5 = s.recv(1024).decode()
r=int(data5)

s.sendall(str(e).encode())
data6 = s.recv(1024).decode()
s2=int(data6)

s.sendall(str(e).encode())
data7 = s.recv(1024).decode()
s3=int(data7)

# 从服务端接收数据
#data7 = s.recv(1024).decode()
#print('s3:', data7)
#s3=int(data7)
def generate_sig(r, d1, k1, s2, s3):
    s = ((d1*k1)*s2+d1*s3-r) % n
    if(s!=0 and s != n-r):
        sig=(r,s)
        return sig
    else:
        return False

if(generate_sig(r, d1, k1, s2, s3)!=False):
    print("sign:",generate_sig(r, d1, k1, s2, s3))
# 关闭连接
s.close()