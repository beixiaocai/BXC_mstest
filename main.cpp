//
// Created by bxc on 2023/7/20.
//

#include <vector>
#ifndef WIN32 // Linux系统
#include <unistd.h>
#else
#include <Windows.h>
#endif // !WIN32

#include <thread>
#include "Common.h"
#include "AvPullStream.h"

std::mutex LOGW_mtx;// 全局变量
std::mutex mtx;
int        online_count;

void run(int interval, int cc, const char* input,const char* rtsp_transport);
static void pullStreamThread(int seq, const char* input, const char* rtsp_transport);


void printHelp() {
    //打印help信息
    printf("-h 查看帮助文档\n");
    printf("-c 拉流并发数   如：-c 2 \n");
    printf("-i 拉流地址（也可以是视频流文件）  如：-i rtsp://127.0.0.1:554/live/test \n");
    printf("-r rtsp拉流时传输层协议(udp或tcp)  如：-r udp   \n");
    printf("-s 记录日志的间隔时间，休眠单位为毫秒  如：-s 1000   \n");
    
}

int main(int argc, char* argv[]) {

    if (argc <= 1)
    {
        printHelp();
        system("pause\n");
        exit(0);
        return -1;
    }
    int  cc = 0;
    std::string input;//rtmp://192.168.1.3:1935/live/test
    std::string rtsp_transport = "udp";// rtsp类型的请求，传输协议，默认udp
    int  interval = 1000;//间隔时间单位为毫秒 取值范围[1000,1000000]

    for (int i = 1; i < argc; i += 2)
    {
        if (argv[i][0] != '-')
        {
            printf("参数有误:%s\n", argv[i]);
            return false;
        }
        switch (argv[i][1])
        {
        case 'h': {
            printHelp();
            system("pause\n");
            exit(0);
            return -1;
        }
        case 'i':input = argv[i + 1]; break;
        case 's':interval = atoi(argv[i + 1]);  break;
        case 'r':rtsp_transport = argv[i + 1]; break;
        case 'c':cc = atoi(argv[i + 1]);  break;
        default:printf("参数配置有误:%s\n", argv[i]); return false;
        }
    }
    if ("tcp" == rtsp_transport || "udp" == rtsp_transport) {
    }
    else {
        LOGW("-r rtsp拉流时传输层协议！");
        return 0;
    }
    if (interval >= 1000 && interval <= 1000000) {
    
    }
    else {
        LOGW("-s 记录日志的间隔时间，休眠单位为毫秒，取值范围[1000,1000000]！");
        return 0;
    }

    LOGW("start");
    std::string log = "拉流并发:"+std::to_string(cc);
    log+="，拉流地址:"+input;
    log+="，记录日志休眠间隔:"+std::to_string(interval)+"毫秒";
    log+="，rtsp传输层协议:"+rtsp_transport;
    LOGW(log);

    if (cc > 0) {
        run(interval,cc, input.data(), rtsp_transport.data());

    }

    LOGW("end");
    return 0;
}
void run(int interval,int cc, const char* input, const char* rtsp_transport) {

    std::vector<std::thread*> threads;

    for (int i = 0; i < cc; i++)
    {
        std::thread* t = new std::thread(pullStreamThread, i,input,rtsp_transport);
        threads.push_back(t);
    }
    std::thread([&]() {
 
        while (true) {
#ifndef WIN32
            usleep(interval * 1000);//函数的休眠单位是微秒
#else
            Sleep(interval);//函数的休眠单位是毫秒
#endif // !WIN32

            mtx.lock();
            std::string log = "实时拉流并发在线数:"+std::to_string(online_count);
            mtx.unlock();
            LOGW(log);
          
        }
    }).detach();

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->join();
    }

    for (int i = 0; i < threads.size(); i++)
    {
        delete threads[i];
    }

    threads.clear();

}


static void pullStreamThread(int seq, const char* input, const char* rtsp_transport) {
    mtx.lock();
    online_count++;
    mtx.unlock();

    AvPullStream pullStream;
    pullStream.start(seq, input, rtsp_transport);

    mtx.lock();
    online_count--;
    mtx.unlock();

}