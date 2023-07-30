//
// Created by bxc on 2023/7/20.
//

#ifndef BXC_AVPULLSTREAM_H
#define BXC_AVPULLSTREAM_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

class AvPullStream {
    public:
        AvPullStream();
        ~AvPullStream();
    public:
        void start(int seq, const char* input, const char* rtsp_transport);
    private:
        AVFormatContext* pFormatCtx;
        AVCodecContext* pCodecCtx;
};

#endif //BXC_AVPULLSTREAM_H