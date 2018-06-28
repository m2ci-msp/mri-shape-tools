#ifndef __VIDEO_H__
#define __VIDEO_H__

/*
  This code is based on http://dranger.com/ffmpeg
*/

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "image/Image.h"

class Video{

private:

  const std::string fileName;

  Image image;

  std::vector<double> values;

  int frameAmount;

  int videoStream = -1;

  AVCodecContext *pCodecCtx = nullptr;
  AVFormatContext *pFormatCtx = nullptr;
public:

  Video(const std::string& fileName ) :

    fileName(fileName) {

  }

  void process() {

    open_stream();

    process_frames();


  }


private:

  // opens the stream and sets the codec context
  void open_stream() {

    // try to open the provided video file
    if(avformat_open_input(&pFormatCtx, this->fileName.c_str(), nullptr, nullptr ) != 0 ) {

      throw std::runtime_error("Could not open video file.");

    }

    // find the first video stream
    for(int i = 0; i < pFormatCtx->nb_streams; ++i) {

      if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {

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

    AVFrame *pFrame = nullptr;
    AVFrame *pFrameRGB = nullptr;

    // Allocate video frame
    pFrame=av_frame_alloc();

    // Allocate an AVFrame structure
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB == nullptr) {

      throw std::runtime_error("Could not allocate AVFrame.");

    }

    uint8_t *buffer = nullptr;
    int numBytes;

    // Determine required buffer size and allocate buffer
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
                                  pCodecCtx->height);

    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    int height = pCodecCtx->height;
    int width = pCodecCtx->width;

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *) pFrameRGB, buffer, AV_PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);


    struct SwsContext *sws_ctx = nullptr;
    int frameFinished;
    AVPacket packet;

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(width,
                             height,
                             pCodecCtx->pix_fmt,
                             width,
                             height,
                             AV_PIX_FMT_GRAY8,
                             SWS_BILINEAR,
                             nullptr,
                             nullptr,
                             nullptr
                             );

    while( av_read_frame(pFormatCtx, &packet) >=0 ) {

      // Is this a packet from the video stream?
      if(packet.stream_index==videoStream) {

        // Decode video frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        // Did we get a video frame?
        if(frameFinished) {

          // convert the image from its native format to gray scale
          sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
                    pFrame->linesize, 0, height,
                    pFrameRGB->data, pFrameRGB->linesize);

          for(int i = 0; i < width * height; ++i) {

            const double value = *(pFrame->data[0] + i);

            this->values.push_back(value);

          } // end for

        } // end if frameFinished

      } // end packet.stream_index

      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);

    } // end while

    this->frameAmount = this->values.size() / ( width * height );

    std::cout << this->frameAmount << std::endl;

  } // end process_frames

};
#endif
