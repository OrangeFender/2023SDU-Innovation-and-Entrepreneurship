# 比较 Firefox 和 Google 的记住密码插件的实际区别

## Firefox：
Firefox浏览器记住密码存储在logins.json中，在windows中，具体路径为`C:\Users[YourUsername]\AppData\Roaming\Mozilla\Firefox\Profiles[Profile Folder]\`

其中"[Your Username]"是登录操作系统时使用的用户名，"[Profile Folder]"是一个随机字符串，代表Firefox配置文件文件夹。我们可以直接用文本编辑器打开进行查看。

我们取其中一个来作为示例进行展示：

```javascript
{"id":13,"hostname":"https://pass.sdu.edu.cn","httpRealm":null,"formSubmitURL":"https://pass.sdu.edu.cn","usernameField":"un","passwordField":"pd","encryptedUsername":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECMI05YHh3a+dBBAf0osEqYr2vvp3DFduApyP","encryptedPassword":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECMSboXhMyC7KBBA8uoHO0okGyPv7hwzU0GC5","guid":"{526287ab-1721-4707-a744-e18bfcfeb740}","encType":1,"timeCreated":1661675233273,"timeLastUsed":1661675233273,"timePasswordChanged":1661675233273,"timesUsed":38}
```

我们可以看到，这段话是一个 JSON 对象，描述了一个与网站 [https://pass.sdu.edu.cn ↗](https://pass.sdu.edu.cn/) 相关的登录信息。下面是每个字段的含义：

- "id": 13 - 登录信息的唯一标识符。
- "hostname": "[https://pass.sdu.edu.cn ↗](https://pass.sdu.edu.cn/)" - 登录信息所属的主机名或网址。
- "httpRealm": null - HTTP 领域，通常为空，用于特定的身份验证需求。
- "formSubmitURL": "[https://pass.sdu.edu.cn ↗](https://pass.sdu.edu.cn/)" - 登录表单提交的 URL。
- "usernameField": "un" - 用户名输入字段的名称。
- "passwordField": "pd" - 密码输入字段的名称。
- "encryptedUsername": "MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECMI05YHh3a+dBBAf0osEqYr2vvp3DFduApyP" - 加密后的用户名。
- "encryptedPassword": "MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECMSboXhMyC7KBBA8uoHO0okGyPv7hwzU0GC5" - 加密后的密码。
- "guid": "{526287ab-1721-4707-a744-e18bfcfeb740}" - 登录信息的全局唯一标识符。
- "encType": 1 - 加密类型的标识符，这里是 1。
- "timeCreated": 1661675233273 - 创建登录信息的时间戳。
- "timeLastUsed": 1661675233273 - 上次使用登录信息的时间戳。
- "timePasswordChanged": 1661675233273 - 上次更改密码的时间戳。
- "timesUsed": 38 - 使用该登录信息的次数。

总体来说，Firefox浏览器记住密码存储在logins.json中，logins.json中存储了一个个 JSON 对象，来描述了一个Firefox上特定网站的登录信息，包括用户名、密码和其他相关属性。



## Google：

Google浏览器记住密码存储在Login Data For Account中，在windows中，具体路径为：

`C:\Users\[YourUsername]\AppData\Local\Google\Chrome\User Data\Default`

其中"[Your Username]"是登录操作系统时使用的用户名。我们用文本编辑器打开，发现是加密后的乱码，需要我们通过 SQLite Studio来进行解析查看。

![image-20230731153127038](C:\Users\lzdwy\AppData\Roaming\Typora\typora-user-images\image-20230731153127038.png)

我们用SQLite Studio打开Login Data For Account文件，将其存储的密码账户导出。取其中一个作为样例进行展示（省略部分信息）：


| origin_url | https://pass.sdu.edu.cn/cas/login |
| ------- | ------- |
| origin_url | https://pass.sdu.edu.cn/cas/log |
| action_url | https://pass.sdu.edu.cn/cas/login |
| username_element | un   |
| username_value | 202122460178 |
| password_value | v10l?>?          ??k?;??          ??[???%?	5;???x}??? |
| signon_realm | https://pass.sdu.edu.cn/ |
| date_created | 13335260572760602 |
| date_last_used | 13335260560973605 |
| date_password_modified | 13335260601150024 |
| id | 1  |
