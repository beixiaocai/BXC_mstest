# BXC_mstest

* 作者：北小菜 
* 个人网站：http://www.any12345.com
* 邮箱：bilibili_bxc@126.com
* QQ：1402990689
* 微信：bilibili_bxc
* 作者-哔哩哔哩主页：https://space.bilibili.com/487906612
* 作者-头条西瓜主页：https://www.ixigua.com/home/4171970536803763
* github开源地址：https://github.com/any12345com/BXC_mstest
* gitee开源地址：https://gitee.com/Vanishi/BXC_mstest


## 项目介绍
1. 这是一个基于C++开发的流媒体服务器并发测试工具，支持Windows/Linux，可以用来测试流媒体服务器的并发性能，并实时记录测试数据

### windows系统编译运行
~~~
visual studio 打开 BXC_mstest.sln （作者使用的是vs2019）

选择 x64/Debug 或 x64/Release都能够直接运行，依赖的第三方库ffmpeg 已经配置好相对路径

~~~

### linux系统编译运行
~~~
依赖的第三方库ffmpeg，需要自行编译安装，我用的版本是：ffmpeg-6.0

//编译
mkdir build
cd build
cmake ..
make

//运行
./BXC_mstest -h
~~~

### 测试工具使用介绍

~~~
命令行参数介绍

    -h 查看帮助文档
    -c 拉流并发数   如：-c 2
    -i 拉流地址（也可以是视频流文件）  如：-i rtsp://192.168.1.3:554/live/test
    -r rtsp拉流时传输层协议(udp或tcp)  如：-r udp
    -s 记录日志的间隔时间，休眠单位为毫秒  如：-s 1000

注意：运行过程中产生的日志文件，保存在当前运行程序的同级目录中 tssc.log
~~~
### ZLMediaKit流媒体服务器（作为流媒体服务器用来测试）

* [ZLMediaKit开源地址](https://gitee.com/xia-chu/ZLMediaKit)
* [ZLMediaKit直接可用无需编译的程序 ](https://gitee.com/Vanishi/zlm)


### 附带常用ffmpeg推流命令

~~~

# 文件rtsp推流
ffmpeg -re -i test.mp4 -rtsp_transport tcp -c copy -f rtsp rtsp://127.0.0.1:554/live/test

# 文件（循环）rtsp推流  [-stream_loop  -1]
ffmpeg -re -stream_loop  -1  -i test.mp4 -rtsp_transport tcp -c copy -f rtsp rtsp://127.0.0.1:554/live/test

# 文件rtmp推流
ffmpeg -re -i test.mp4 -vcodec h264_nvenc  -acodec aac -f flv  rtmp://127.0.0.11935/live/test

# 文件（循环）rtmp推流  [-stream_loop  -1]
ffmpeg -re -stream_loop  -1 -i test.mp4 -vcodec h264_nvenc  -acodec aac -f flv  rtmp://127.0.0.1:1935/live/test
ffmpeg -re -stream_loop  -1 -i test.mp4 -vcodec h264  -acodec aac -f flv  rtmp://127.0.0.1:1935/live/test

~~~

### 附带ZLMediaKit 常用转发协议拉流地址

~~~

//rtsp播放
rtsp://127.0.0.1:554/live/test

//rtmp播放
rtmp://127.0.0.1:1935/live/test

//hls播放
http://127.0.0.1:80/live/test/hls.m3u8

//http-flv播放
http://127.0.0.1:80/live/test.live.flv

//http-ts播放
http://127.0.0.1:80/live/test.live.ts

~~~

### 附带ZLMediaKit 常用HTTP-API入口地址
~~~
// 获取实时流在线情况
http://127.0.0.1/index/api/getMediaList

~~~


