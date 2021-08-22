# 使用说明

&emsp;&emsp;该Doxyfile文件用于配置c/c++代码的文档输出，部分配置需根据自身需要修改。以下是接受啊如何使用该配置并对配置进行符合自身习惯的修改。

这里写详细描述。

## 目录

- [安全](#安全)
- [背景](#背景)
- [安装](#安装)
- [用法](#用法)
- [API](#API)
- [维护者](#维护者)
- [致谢](#致谢)
- [贡献](#贡献)
- [许可证](#许可证)

## 安全
&emsp;&emsp;使用Doxygen将不会修改任何源代码，它只是从源代码中提取关键注释生成对应的代码文档。

## 背景
&emsp;&emsp;提供已完成基础配置的Doxygen文件，提高效率，该配置用于编译c/c++代码。

## 安装
&emsp;&emsp;配置文件，通过Doxygen向导打开。
&emsp;&emsp;若配置开启使用 [Graphviz](http://www.graphviz.org/ "Graphviz") 生成图表则依赖于Graphviz，需要将其安装。[下载](http://www.graphviz.org/download/ "download") Graphviz。点击下一步直到完成安装即可。

## 用法
&emsp;&emsp;打开Doxygen向导，点击 **File** -> **Open** 选择该Doxygen配置文件。该项目要使用还需根据自身情况配置相关路径。以下请配置请根据自身需要修改。

### Wizard
#### Project
* Project name: 项目名，将其改为你自己的项目名称。建议配置；
* Project synopsis: 项目概要，在这里填写项目的简介。建议配置；
* Project version or id: 项目版本编号，改为当前项目的版本编号。建议配置；
* Project logo: 选择该项目的logo图标，为图片格式。可不配置；
* Source code directory: 源代码路径，选择要解析的源码路径，Doxygen将会解析该路径下的源码。必须配置；
* Destination directory: 选择Doxygen产物输出目录。必须配置；

#### Mode
* Include cross-referenced source code in the output: 勾选该项后将会在文档中加入源码，也即公开c文件中的源码，根据自身需要配置；

#### Output
* Select the output format(s) to generate: 勾选 **plain HTML** 表示生成HTML格式文档，不带侧边栏导航，若勾选 **with navigation panel** 则生成的HTML文档带侧边栏导航。根据自身需要配置；
* Change color: 该配置用于修改文档的主题配色。根据自身需要配置；

#### Diagrams
* Diagrams to generate: 当选择 **No diagrams** 时不会为生成图表，选择 **Use dot tool from the GraphViz package** 则依赖GraphViz工具为文档生成图表，比如函数调用关系和头文件包含关系的图表。根据自身需要配置；

### Expert
#### Preprocessor
* PREDEFINED: 预编译宏，Doxygen不能正确解析代码中的预编译宏，一些通过预编译宏控制的代码会被Doxygen忽视，无法解析到文档中，可以在此处添加这些宏定义来正确解析这些代码。比如： `DEBUG` 或 `SWITCH=1` ，注意 “=” 前后不要加空格。根据需要配置。

#### Dot
* DOT_PATH: 为Graphviz的bin目录路径，若配置了Graphviz生成图表则该路径必须给出。

## API
&emsp;&emsp;无。

## 维护者
[![calm-brightgreen](https://img.shields.io/badge/calm-brightgreen?style=plastic&logo=appveyor "calm")](https://github.com/onlycalm)

## 致谢
&emsp;&emsp;感谢所有参与该项目开发或维护的贡献者，感谢所有支持该项目的人。

## 贡献
&emsp;&emsp;目前由 [onlycalm](https://github.com/onlycalm "onlycalm") 独立贡献。

## 许可证
&emsp;&emsp;无。
