# implement sm2 2P sign with real network communication

## 实现方式

![image-20230726152246643](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230726152246643.png)

## 实现思路

* 为实现真实网络中的SM2的两方协同签名方案，在此项目中采用python中的socket编程方法进行双方通讯

  **服务端：**

  ```python
  HOST = '127.0.0.1'
  PORT = 50007
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.bind((HOST, PORT)) # 绑定socket
  s.listen(1) # 开始监听客户端连接
  print('Listening on port:',PORT)
  
  conn, addr = s.accept()
  print('Connected by', addr)
  ```

  **客户端：**

  ```python
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
  ```

* SM2的实现和之前项目类似

  **参数选取：**

  ```python
  p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
  
  a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
  
  b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
  
  xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
  
  yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
  
  n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
  
  mess1="hello"
  
  IDA=0x414C494345313233405941484F4F2E434F4D
  ```

  **生成P1:**

  ```python
  d1=random.randint(1,n)
  def generate_P1(d1):
      #print(d1)
      d1_inv=mod_inverse(d1,n)
      #print(d1_inv)
      P1=calculate_np(xG, yG, d1_inv, a, b, p)
      return P1
  ```

  **生成Q1,e:**

  ```python
  def generate_e_Q1(ID,mess,P1,k1):
      M = mess.encode()
      ENTL = get_bitsize(ID) * 8
      data = ENTL.to_bytes(2, byteorder='big', signed=False) + int_to_bytes(ID) + 	int_to_bytes(a) + int_to_bytes(b) + int_to_bytes(xG) + int_to_bytes(yG) + 		int_to_bytes(P1[0]) + int_to_bytes(P1[1])
      # print(data)
      ZA = sm3(data)
      M_ = ZA + M
      e = bytes_to_int(sm3(M_))
      Q1 = calculate_np(xG, yG, k1, a, b, p)
      return Q1, e
  ```

  **生成r,s2,s3:**

  ```python
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
  ```

## 结果展示

![image-20230726153326850](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230726153326850.png)

![image-20230726153330319](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230726153330319.png)