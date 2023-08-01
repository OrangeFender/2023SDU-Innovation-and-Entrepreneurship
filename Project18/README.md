# send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself

## 项目说明

本实验实现过程中使用的主要控制台指令见解析tx.txt文件

## 实现流程

* 下载 Bitcoin Core 和 Bitcoin Core test 并等待数据同步。数据同步后打开 Bitcoin Core test ，生成比特币地址。
* 获取免费的测试网络比特币：打开`https://coinfaucet.eu/en/btc-testnet`，输入第一步中生成的比特币地址即可领取。领取后钱包中“可使用余额”不再为0。如下图所示：

![image-20230801152548836](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230801152548836.png)



* 接下来进行比特币交易，发送地址仍然填写第一步生成的地址，即自己发送给自己，并添加手续费：



![image-20230801152757711](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230801152757711.png)



* 等待交易处理完毕，我们就可以查询交易信息。

  

  ![image-20230801152953129](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230801152953129.png)

  可以看到交易ID为：`5eed6708f64a33d41625c38772a08d724607b99b41d0a6806dfd0333a7a3b4c0`

  

* 在控制台输入命令：`gettransaction 5eed6708f64a33d41625c38772a08d724607b99b41d0a6806dfd0333a7a3b4c0`

  得到以下信息：

  ```
  {
    "amount": 0.00000000,
    "fee": -0.00000141,
    "confirmations": 9,
    "blockhash": "000000000000003e67833ef4cfebae091af9b37dd86036407a6c3afbe3181913",
    "blockheight": 2470170,
    "blockindex": 15,
    "blocktime": 1690871176,
    "txid": "5eed6708f64a33d41625c38772a08d724607b99b41d0a6806dfd0333a7a3b4c0",
    "walletconflicts": [
    ],
    "time": 1690871102,
    "timereceived": 1690871102,
    "bip125-replaceable": "no",
    "details": [
      {
        "address": "tb1qdnava283hrs0jswf7l8x8353aw4wuc8cgh02w7",
        "category": "send",
        "amount": -0.00099859,
        "label": "",
        "vout": 1,
        "fee": -0.00000141,
        "abandoned": false
      },
      {
        "address": "tb1qdnava283hrs0jswf7l8x8353aw4wuc8cgh02w7",
        "category": "receive",
        "amount": 0.00099859,
        "label": "",
        "vout": 1
      }
    ],
    "hex": "02000000000101445701b2fe3ef9689853f52c2c5544db93fb939fb5f58bfa78dc3436f4cd9cda0000000000fdffffff02855a2900000000001600145058ccd3be6a668ce196cbd44917a2397e380cc913860100000000001600146cfacea8f1b8e0f941c9f7ce63c691ebaaee60f80247304402204a6d7d86cb66c5011b043d345626b18f1dd67bcc28c5281a53c8598484c8d7ad02201a9276ad8269ff4a2a94943cd0bc3975c05aaf55aabad517458793c5c9d9ef2d0121025560dc8e49e062b0aa14ffb83b4fb892c838161170adcff16bf49eeac80f5a1919b12500"
  }
  ```

  

* 最后我们对上边结果给出的原始十六进制编码数据进行解析，命令为：

```
decoderawtransaction 02000000000101445701b2fe3ef9689853f52c2c5544db93fb939fb5f58bfa78dc3436f4cd9cda0000000000fdffffff02855a2900000000001600145058ccd3be6a668ce196cbd44917a2397e380cc913860100000000001600146cfacea8f1b8e0f941c9f7ce63c691ebaaee60f80247304402204a6d7d86cb66c5011b043d345626b18f1dd67bcc28c5281a53c8598484c8d7ad02201a9276ad8269ff4a2a94943cd0bc3975c05aaf55aabad517458793c5c9d9ef2d0121025560dc8e49e062b0aa14ffb83b4fb892c838161170adcff16bf49eeac80f5a1919b12500`
```

​	最终得到以下交易结果信息：

```

15:12:43
￼
{
  "txid": "5eed6708f64a33d41625c38772a08d724607b99b41d0a6806dfd0333a7a3b4c0",
  "hash": "201a7ac72d29397fed41540afb6c36767ec6ef078fcabbca91d0c3c01b5ae88b",
  "version": 2,
  "size": 222,
  "vsize": 141,
  "weight": 561,
  "locktime": 2470169,
  "vin": [
    {
      "txid": "da9ccdf43634dc78fa8bf5b59f93fb93db44552c2cf5539868f93efeb2015744",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "304402204a6d7d86cb66c5011b043d345626b18f1dd67bcc28c5281a53c8598484c8d7ad02201a9276ad8269ff4a2a94943cd0bc3975c05aaf55aabad517458793c5c9d9ef2d01",
        "025560dc8e49e062b0aa14ffb83b4fb892c838161170adcff16bf49eeac80f5a19"
      ],
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.02710149,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 5058ccd3be6a668ce196cbd44917a2397e380cc9",
        "hex": "00145058ccd3be6a668ce196cbd44917a2397e380cc9",
        "address": "tb1q2pvve5a7dfngecvke02yj9az89lrsrxflt597c",
        "type": "witness_v0_keyhash"
      }
    },
    {
      "value": 0.00099859,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 6cfacea8f1b8e0f941c9f7ce63c691ebaaee60f8",
        "hex": "00146cfacea8f1b8e0f941c9f7ce63c691ebaaee60f8",
        "address": "tb1qdnava283hrs0jswf7l8x8353aw4wuc8cgh02w7",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}

```



