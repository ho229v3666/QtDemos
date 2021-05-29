﻿/**
 * @brief FFmpeg Library
 * @anchor Ho 229
 * @date 2021/4/10
 */

#ifndef FFMPEG_H
#define FFMPEG_H

extern "C"
{
#ifdef __cplusplus
# define __STDC_CONSTANT_MACROS
# ifdef _STDINT_H
#  undef _STDINT_H
# endif
# include "stdint.h"
#endif

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#endif // FFMPEG_H
