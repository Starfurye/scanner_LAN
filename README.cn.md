# 简易局域网端口扫描器

该扫描器只能在Linux平台上使用，不要尝试在Windows和WSL上使用，WSL无法操作物理网卡。

不建议在其他Unix发行版上使用，他们对于struct handler的定义可能与Linux不同。


## 运行

1. 使用makefile编译

```bash
$ make
```

2. 清理

```bash
$ make clean
```

## 用法


1. 扫描局域网中的所有主机：

```bash
$ sudo ./scanner
```

该扫描器使用`fping`来搜索局域网指定网段的存活主机，搜索结果将会存放在`hosts.log`里。

2. 指定ip地址，扫描特定主机：

```bash
$ sudo ./scanner -i [ip address]
$ sudo ./scanner -i 192.168.1.102
```

3. 给定端口区间和ip地址，扫描特定主机的特定端口：

```bash
$ sudo ./scanner -i [ip address] [start port] [end port]
$ sudo ./scanner -i 192.168.1.102 251 996
```


## 程序中的默认值

可以自由更改以下的默认值。

`common.c > getHosts(), 第 30 行`:


fping指定的网段：`192.168.1.0/24`;


`common.h, 第 26, 27 行`:


默认起始端口为`1`，默认结束端口为`1024`。
