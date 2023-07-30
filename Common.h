//
// Created by bxc on 2023/7/20.
//


#ifndef BXC_COMMON_H
#define BXC_COMMON_H

#include <time.h>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>

#pragma warning(disable: 4996)

static std::string getCurTimeStr() {
    const char* time_fmt = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);
    char time_str[64];
    strftime(time_str, sizeof(time_str), time_fmt, localtime(&t));

    return time_str;
}

static int64_t getCurTime()// 获取当前系统启动以来的毫秒数
{
    long long now = std::chrono::steady_clock::now().time_since_epoch().count();
    return now / 1000000;
}
static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).
        count();

}

extern std::mutex LOGW_mtx;
static void LOGW(std::string log) {
   
    LOGW_mtx.lock();
    std::ofstream fs;
    fs.open("log.log", std::ofstream::app);
    fs << "[W]" << getCurTimeStr() << " " << log << "\n";
    std::cout << "[W]" <<getCurTimeStr()<<" "<< log << "\n";
    fs.close();
    LOGW_mtx.unlock();
}
#endif //BXC_COMMON_H