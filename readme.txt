1 目录结构说明:
- bin: 编译之后的二进制文件, 包括工具以及相关的服务
- doc: 文档
- include: 需要对外提供访问的include目录
  - config: 配置中心服务的头文件 
  - hessian: hessian的头文件
  - jce: jce编解码头文件
  - jmem: 基于jce协议的共享内存, 文件的循环队列和hashmap
  - log: 日志服务的头文件   
  - nodeF: 给node异步上报心跳的库头文件
  - parse: 解析jce文件的库的文件
  - patch: 发布服务的头文件
  - proxy: 中间件proxy部分头文件
  - servant: 中间件服务部分头文件
  - util: 工具类
- java: java的相关工具和代码类
  - hessian2jce: 根据hessian的接口文件类, 可以自动生成绝大部分的hce文件, 根据hce文件可以自动生成hessian的c++客户端调用代码, 具体参见src/hessian, src/hce2cpp
  - jce: java的jce编解码源码以及proxy部分的代码, 所有需要用到jce协议的代码必须包含该代码.
- javalib: java需要的公共lib, 门户的统一组件
- jce: jce文件目录
  - config: config服务的jce文件
  - log: log服务的jce文件
  - node: node服务的jce文件
  - nodeF: 给node上报心跳的jce文件
  - patch: patch服务的jce文件
  - proxy: proxy需要的jce文件
  - registry: registry服务的jce文件
  - servant: servant需要的jce文件
  - stat: stat服务的jce文件
- lib: lib库
- script: 代码自动生成脚本以及demo
  - create_taf_server.sh: 自动生成空服务的脚本
  - demo: demo服务
- src: 源码目录
  - adminclient: registry的客户端管理工具, 可执行程序编译到bin目录下
  - config: 配置中心服务 , 可执行程序编译到bin目录下
  - hce2cpp: hce生成cpp的源码目录, 可执行程序编译到bin目录下  
  - jce2cpp: jce生成cpp的源码目录, 可执行程序编译到bin目录下
  - libconfig: 配置中心客户端lib库, lib库编译到lib目录下
  - libhessian: hessian调用客户端库, lib库编译到lib目录下
  - liblog: 日志中心客户端lib库, lib库编译到lib目录下
  - libparse: 解析jce文件的库, lib库编译到lib目录下
  - libpatch: 发布服务客户端lib库, lib库编译到lib目录下
  - libproxy: proxy部分的lib库
  - libservant: jce服务部分的lib库, 服务端支持同步和异步返回
  - libstat: 模块间调用服务的客户端lib库, lib库编译到lib目录下
  - libutil: 工具类库, lib库编译到lib目录下
  - log: 日志中心服务, 可执行程序编译到bin目录下
  - node: node的源码目录, 可执行程序编译到bin目录下
  - patch: patch的源码目录, 可执行程序编译到bin目录下
  - patchclient: patch客户端可执行文件, 可执行程序编译到bin目录下
  - registry: registry的源码目录, 可执行程序编译到bin目录下
  - stat: 模块间调用服务的源码目录, 可执行程序编译到bin目录下
- test: 测试程序
  - client: 测试server客户端的代码
  - config: 测试配置中心服务  
  - echoserver: 基于libserver的echoserver
  - hce2cpp: 测试hce2cpp工具的源码
  - hessian: 测试hessian编解码的源码
  - jce2cpp: 测试jce2cpp工具的源码
  - jce2java: 测试jce2java的源码
  - jmem: 测试jmem组件
  - log: 测试log服务
  - node: 测试node源码
  - registry: 测试registry服务
  - server: 测试服务框架
  - util: 测试工具类

运行说明:
	1 服务编译后可以通过如下运行, 例如:
	./NodeServer --config=node.config.conf
	2 客户端测试程序可以发包到该服务, 具体可以参加test/node下面的代码编写;
	
普通服务的Makefile说明:
	#--------------------------------------------------------
	#当前目录到taf目录的相对路径
	TOPDIR  := ../..
	#应用通用规则
	include ${TOPDIR}/make.rules
	#-------------------------------------------------------------------------------
	#需要额外的include目录, taf/include路径已经在make.rules定义了, 需要引用的头文件, 需要加相对路径, 例如"servant/Application.h"
	INCLUDE   += 
	#需要额外的lib: taf/lib路径已经在make.rules定义了, 仅仅需要指定需要依赖的库就可以了, 不需要用路径
	LIB       += -ltaf -lutil 
	#-------------------------------------------------------------------------------
	#编译的目标
	TARGET  := ${NODEBIN}

	#-------------------------------------------------------------------------------
	#需要其他makefile的支持
	include ${TOPDIR}/node/Node.mk
	include ${TOPDIR}/inc.mk
