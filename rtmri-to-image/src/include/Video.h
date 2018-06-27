#ifndef __VIDEO_H__
#define __VIDEO_H__

/*
  This code is based on http://dranger.com/ffmpeg
*/

#include <string>
#include <stdexcept>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "image/Image.h"

class Video{

private:

  const std::string fileName;

  Image image;


  AVCodecContext *pCodecCtx = nullptr;

public:

  Video(const std::string& fileName ) :
    fileName(fileName) {
  }


private:

  // opens the stream and sets the codec context
  void open_stream() {

    AVFormatContext *pFormatCtx = nullptr;

    // try to open the provided video file
    if(avformat_open_input(&pFormatCtx, this->fileName.c_str(), nullptr, 0, nullptr ) !=0 ) {

      throw std::runtime_error("Could not open video file.");

    }

    // find the first video stream
    int videoStream = -1;

    for(int i=0; i < pFormatCtx->nb_streams; ++i) {

      if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {

        videoStream = i;
        break;

      }

    }

    if( videoStream == -1) {

      throw std::runtime_error("Could not find any video stream.");

    }

    this->pCodecCtx = pFormatCtx->streams[videoStream]->codec;

  }

  void process_frames() {

    AVFrame *pFrame = NULL;

    // Allocate video frame
    pFrame=av_frame_alloc();

    // Allocate an AVFrame structure
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL)
      return -1;

    uint8_t *buffer = NULL;
    int numBytes;
    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                                pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);


    struct SwsContext *sws_ctx = NULL;
    int frameFinished;
    AVPacket packet;

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width,
                             pCodecCtx->height,
                             AV_PIX_FMT_GRAY8,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
                             );

    i=0;

    while( av_read_frame(pFormatCtx, &packet) >=0 ) {
      // Is this a packet from the video stream?
      if(packet.stream_index==videoStream) {
        // Decode video frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        // Did we get a video frame?
        if(frameFinished) {
          // Convert the image from its native format to RGB
          sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);

          // Save the frame to disk
          if(++i<=5)
            SaveFrame(pFrameRGB, pCodecCtx->width, 
                      pCodecCtx->height, i);
        }
      }
    
      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    }

  }

  void process_frame() {


  }
};


#endif
