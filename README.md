### 简介
XTP.NET是[中泰证券极速交易接口](https://xtp.zts.com.cn/home)的.NET版本接口，采用CLI封装C++，输出的DLL可供原生.NET程序调用。）


### 项目目标
项目fork自https://github.com/zerochocobo/XTP.NET,
在该项目的基础上，想做以下改动：

1. 更新XTP API到最新版本，即版本XTP_API_20190401_1.1.18.19（**已完成**）
2. 增加.NET版本使用的样例（**完成行情部分**）
3. 修改非托管代码转化为托管代码的方式（**已完成**）

### 开发环境
- .NET版本：4.6.1
- Windows SDK 版本：10.0.16299.0
- VC++的平台工具集：Visual Studio 2017(v141)

### 使用说明
1. 下载项目后，首先编译项目XTP,会在解决方案文件夹的Debug文件夹中生成XTP.dll；
2. 编译项目XtpTest；
3. 拷贝**sdk\lib\x86**下的dll到XtpTest生成的**bin\Degbug**文件夹下；
4. 可以直接运行**XtpTest.exe**或者调试项目XtpTest
### 其他注意事项：

- API的所有文档在doc\XTP_API_20171204_1.1.16.9.zip中，API介绍在doc\XTP_intro_v3.4.pdf


### 项目维护：
精力有限，暂不提供技术问题解答，请见谅；如在使用过程中发现bug,欢迎提交issue。


### TODO List

1. C# 模拟 C++ 结构体的union
2. 业务demo 

