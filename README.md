# <center>lib</center>

[![onlycalm-lib-brightgreen](https://img.shields.io/badge/calm-lib-brightgreen?style=plastic&logo=appveyor "onlycalm/lib")](https://github.com/onlycalm/lib)

&emsp;&emsp;该仓库为常用通用、可移植模块库，存放 C / C++ / Python 语言的通用模块，代码以模块为单位存放。

&emsp;&emsp;构建该仓库的主要目的是提供一套可复用、可移植的通用模块，减少重复开发，统一代码与文档风格，方便未来项目继承与复用。

## 目录(Sections)

- [安全(Security)](#安全(Security))
- [背景(Background)](#背景(Background))
- [安装(Install)](#安装(Install))
- [用法(Usage)](#用法(Usage))
- [模块(Modules)](#模块(Modules))
- [API](#API)
- [维护者(Maintainers)](#维护者(Maintainers))
- [致谢(Thanks)](#致谢(Thanks))
- [贡献(Contributing)](#贡献(Contributing))
- [许可证(License)](#许可证(License))

## 安全(Security)
&emsp;&emsp;本库提供的代码一般是已经经过测试或实践项目使用的代码，但也不敢保证一定不会出现问题，因此建议更多的以参考为目的的使用，谨慎不经过任何验证的拿到项目中使用。

## 背景(Background)
&emsp;&emsp;在多年的开发生涯中总结了一些可反复使用的经验，于是将这些经验整理形成更方便继承的经验，方便未来使用和减少重复开发，形成统一的风格。

## 安装(Install)
&emsp;&emsp;无可执行文件，无安装运行过程。

## 用法(Usage)
&emsp;&emsp;按语言进入对应目录（c/、cpp/、python/），将所需模块代码拷贝到项目中，通过调用其 API 使用。

## 模块(Modules)

&emsp;&emsp;本仓库按语言分类存放常用通用、可移植模块，各模块以独立目录为单位组织。

### C 语言（c/）

| 模块 | 说明 |
| --- | --- |
| [com](c/com) | 公共基础模块，提供通用宏、数据类型、错误码、字节序与字节/字比较等。 |
| [log](c/log) | 分级彩色日志模块，支持终端与日志文件双输出、等级过滤、时间戳、字体样式与颜色等配置。 |
| [dtc](c/dtc) | DTC(Diagnostic Trouble Code) 诊断故障码定义。 |
| [sftiic](c/sftiic) | 软件 IIC（I2C）模块。 |
| [tpl](c/tpl) | 代码模板（源文件/头文件格式模板）。 |
| [rgque](c/RgQue) | 环形队列（Ring Queue）模块，基于数组的通用环形队列框架，用作通讯接收/发送缓存。 |
| [mon](c/Mon) | 模拟量监控（Monitor）模块，对采集模块传入的模拟量做阈值越限检测与回差/时间去抖，故障/恢复时触发回调。 |

&emsp;&emsp;log 模块文档在本地生成：进入 `c/log/` 执行 `doxygen Doxyfile`，输出到 `c/log/doc/html/`。

### C++ 语言（cpp/）
&emsp;&emsp;预留，暂无模块。

### Python 语言（python/）
&emsp;&emsp;预留，暂无模块。

## API
&emsp;&emsp;略。

## 维护者(Contributing)

[![onlycalm-brightgreen](https://img.shields.io/badge/onlycalm-brightgreen "onlycalm-brightgreen")](https://github.com/onlycalm)

## 致谢(Thanks)
&emsp;&emsp;无。

## 贡献(Contributing)
&emsp;&emsp;无。

## 许可证(License)
&emsp;&emsp;无。
