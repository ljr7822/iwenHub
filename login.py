'''
西南大学校园网自动登录脚本
@author: iwen
@parameter: LOGIN_PAGE_URL: 西南大学校园网登陆地址
            data1：浏览器登陆校园网的post请求
            name: 用户名
            password: 密码
@date:2019年12月11日
'''

# 函数：登录指定网络
import datetime, requests, base64

# 校园网地址
LOGIN_PAGE_URL = "http://192.168.3.5/include/auth_action.php"

# 账号密码
name = '######'
password = '######'

# 登录函数
def login_request(name, password):
    # 网络未链接时打印提示
    # print("[03] {} raspberry is offline ， request now... ".format(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    #password =  base64.b64encode(password.encode()).decode()  # 加密
    # 打包请求数据
    data1 = {"action": "login",
             "username": name,
             "password": password,
             "ac_id": 1,
             "save_me": 1,
             "ajax": 1}
    try:
        result = requests.post(LOGIN_PAGE_URL, data=data1)
        # 打印请求结果
        print(result.text)
        # 打印登录成功的提示信息
        print("[01] {} login success  ".format(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    except:
        print("[00] {} requsest error ，raspberry isnot connected to WIFI ".format(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

# 调用登录函数
login_request(name,password)