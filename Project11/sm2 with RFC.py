#from gmssl import sm3
import gmssl.sm3
import sympy
import random

p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3

a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498  

b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A  

xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D  

yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2 

n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7 

mess111="hello"

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


def sig(mess,ID):
    M=mess.encode()
    #print(M)
    dA,PA=get_key()
    #print(dA)
    #print(PA)
    ENTL=get_bitsize(ID)*8
    data=ENTL.to_bytes(2,byteorder='big',signed=False)+int_to_bytes(ID)+int_to_bytes(a)+int_to_bytes(b)+int_to_bytes(xG)+int_to_bytes(yG)+int_to_bytes(PA[0])+int_to_bytes(PA[1])
    #print(data)
    ZA=sm3(data)
    M_=ZA+M
    e=bytes_to_int(sm3(M_))
    k=sign_with_RFC6979(M+int_to_bytes(dA))
    print("k=")
    print(k)
    while 1:
        x1,y1=calculate_np(xG,yG,k,a,b,p)
        r=(e+x1)%n
        s=(sympy.mod_inverse(1+dA,n)*(k-r*dA))%n
        if(s!=0 and r!=0 and r+k!=n):
            break
        k=(k+1)%n
    print("签名(r,s)为：")
    print(r)
    print(s)
    return r,s,ZA,dA,PA

def veri(r1,s1,mess,ZA,PA):
    if(r1<1 or r1>n-1):
        print("false")
        return False
    if(s1<1 or s1>n-1):
        print("false")
        return False
    M=mess.encode()
    M1=ZA+M
    e=bytes_to_int(sm3(M1))
    t=(r1+s1)%n
    if(t==0):
        print("false")
        return False
    x1,y1=calculate_np(xG,yG,s1,a,b,p)
    x2,y2=calculate_np(PA[0],PA[1],t,a,b,p)
    x3,y3=calculate_p_q(x1, y1, x2, y2, a, b, p)
    R=e+x3
    if(R==r1):
        print("yes")
        return True
    print("false")
    return False

sig(mess111,IDA)

    
