//
// Created by bxc on 2023/7/20.
//

#include "AvPullStream.h"
#include "Common.h"

AvPullStream::AvPullStream() {
    pFormatCtx = avformat_alloc_context();
    pCodecCtx = nullptr;
}
AvPullStream::~AvPullStream() {
    if (pCodecCtx) {
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);
        pCodecCtx = nullptr;
    }
    if (pFormatCtx) {
        // 拉流不需要释放start
        //if (mFmtCtx && !(mFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        //    avio_close(mFmtCtx->pb);
        //}
        // 拉流不需要释放end
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}


void AvPullStream::start(int seq, const char* input, const char* rtsp_transport) {

    AVDictionary* format_dict = nullptr;
    av_dict_set(&format_dict, "rtsp_transport", rtsp_transport, 0); //设置rtsp底层网络协议 tcp or udp
    av_dict_set(&format_dict, "stimeout", "3000000", 0);   //设置rtsp连接超时（单位 us）
    av_dict_set(&format_dict, "rw_timeout", "3000000", 0); //设置rtmp/http-flv连接超时（单位 us）
    //av_dict_set(&format_dict, "timeout", "3000000", 0);//设置udp/http超时（单位 us）

    if (avformat_open_input(&pFormatCtx, input, nullptr, &format_dict) != 0)
    {
        LOGW("avformat_open_input error");
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0)
    {
        LOGW("avformat_find_stream_info error");
        return;
    }

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
        }
        else if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
        }

    }
    if (videoStreamIndex == -1)
    {
        LOGW("videoStreamIndex == -1");
        return;
    }
    AVStream *pStream = pFormatCtx->streams[videoStreamIndex];

    const AVCodec* pCodec = avcodec_find_decoder(pStream->codecpar->codec_id);

    //AVCodec* pCodec = avcodec_find_decoder_by_name("h264");
    //AVCodec* pCodec = avcodec_find_decoder_by_name("h264_cuvid");

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (0 != avcodec_parameters_to_context(pCodecCtx, pStream->codecpar)) {
        LOGW("avcodec_parameters_to_context error");
        return;
    }


    if (pCodec == nullptr)
    {
        LOGW("avcodec_find_decoder error");
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
    {
        LOGW( "avcodec_open2 error");
        return;
    }
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;
    int fps = pCodecCtx->framerate.num / pCodecCtx->framerate.den;


    char log[200];
#ifndef WIN32 // Linux系统
    snprintf(log, 200,"[线程-%d] 协议:%s，时长:%d，分辨率:%d*%d，解码器:%s，视频fps:%d", seq, pFormatCtx->iformat->name, (pFormatCtx->duration) / 1000000,
                      pCodecCtx->width, pCodecCtx->height, pCodec->name, fps);
#else
    sprintf_s(log, "[线程-%d] 协议:%s，时长:%d，分辨率:%d*%d，解码器:%s，视频fps:%d", seq, pFormatCtx->iformat->name, (pFormatCtx->duration) / 1000000,
        pCodecCtx->width, pCodecCtx->height, pCodec->name, fps);
#endif


    LOGW(log);

    AVPacket pkt;
    while (av_read_frame(pFormatCtx, &pkt) >= 0)
    {
        if (pkt.stream_index == videoStreamIndex)
        {

        }
        //av_free_packet(pkt);// 过时
        av_packet_unref(&pkt);
    }
    std::string log2 = "[线程-"+std::to_string(seq)+"%d] 退出拉流";
    LOGW(log2);

}

