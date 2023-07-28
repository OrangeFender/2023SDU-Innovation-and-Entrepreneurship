import socket
from os.path import commonprefix


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












HOST = '127.0.0.1'
PORT = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT)) # 绑定socket
s.listen(1) # 开始监听客户端连接
print('Listening on port:',PORT)

conn, addr = s.accept()
print('Connected by', addr)

'''while True: # 开始聊天
    data = conn.recv(1024).decode()
    if not data: break
    print('Received message:', data)
    conn.sendall('Sorry.'.encode()) # 回答
'''


data1 = conn.recv(1024).decode()
conn.sendall('get'.encode())
data2 = conn.recv(1024).decode()
conn.sendall('get'.encode())

P1=(int(data1),int(data2))


d2=random.randint(1,n)
def genrate_P(P1,d2):

    d2_inv = mod_inverse(d2, n)
    tem=calculate_np(P1[0], P1[1], d2_inv, a, b, p)
    P=calculate_p_q(tem[0], tem[1], xG, yG, a, b, p)
    return P

P=genrate_P(P1,d2)


data3 = conn.recv(1024).decode()
conn.sendall('get'.encode())
data4 = conn.recv(1024).decode()
conn.sendall('get'.encode())

Q1 = (int(data3),int(data4))


data5 = conn.recv(1024).decode()

e = int(data5)


def generate_r_s2_s3(Q1,e,d2):
    k2 = random.randint(1, n)
    Q2 = calculate_np(xG, yG, k2, a, b, p)

    k3 = random.randint(1, n)
    tem = calculate_np(Q1[0], Q1[1], k3, a, b, p)
    res = calculate_p_q(tem[0], tem[1], Q2[0], Q2[1], a, b, p)
    r = (res[0] + e) % n
    s2 = (d2*k3) % n
    s3 = (d2*(r+k2)) % n
    return r, s2, s3

r,s2,s3=generate_r_s2_s3(Q1,e,d2)


conn.sendall(str(r).encode())

data6 = conn.recv(1024).decode()
conn.sendall(str(s2).encode())
data7 = conn.recv(1024).decode()
conn.sendall(str(s3).encode())

conn.close()
s.close()