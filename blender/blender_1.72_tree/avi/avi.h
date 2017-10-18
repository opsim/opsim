/**
 * $Id:$
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * The contents of this file may be used under the terms of either the GNU
 * General Public License Version 2 or later (the "GPL", see
 * http://www.gnu.org/licenses/gpl.html ), or the Blender License 1.0 or
 * later (the "BL", see http://www.blender.org/BL/ ) which has to be
 * bought from the Blender Foundation to become active, in which case the
 * above mentioned GPL option does not apply.
 *
 * The Original Code is Copyright (C) 2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 */

#include <stdio.h>

#include "util.h"

#ifndef __AVI_H__
#define __AVI_H__

/* An (IMHO) admirable attempt has been/is being made to keep these
   structures and defines more unmudelled than the format itself has
   become.  I failed miserably, of course.  To give credit where
   credit is due, I must admit most of the information herein comes
   from aviriff.h, part of the windows AVI implementation. */

#define FCC(ch4) (ch4[0] | ch4[1]<<8 | ch4[2]<<16 | ch4[3] << 24)

unsigned int GET_FCC (FILE *fp);
unsigned int GET_TCC (FILE *fp);
/*#define GET_FCC(fp) (getc(fp) << 0 | getc(fp) << 8 | getc(fp) << 16 | getc(fp) << 24) */
/*#define GET_TCC(fp) (getc(fp) << 0 | getc(fp) << 8)*/

#define PUT_FCC(ch4, fp) putc(ch4[0],fp); putc(ch4[1],fp); putc(ch4[2],fp); putc(ch4[3],fp)
#define PUT_FCCN(num, fp) putc((num>>0)&0377,fp); putc((num>>8)&0377,fp); putc((num>>16)&0377,fp); putc((num>>24)&0377,fp)
#define PUT_TCC(ch2, fp) putc(ch2[0],fp); putc(ch2[1],fp)

typedef struct _AviChunk {
  int fcc;
  int size;
} AviChunk;

typedef struct _AviList {
  int fcc;
  int size;
  int ids;
} AviList;

typedef struct _AviMainHeader {
  int fcc;
  int size;  
  int MicroSecPerFrame;       /* MicroSecPerFrame - timing between frames */
  int MaxBytesPerSec;        /* MaxBytesPerSec - approx bps system must handle */
  int PaddingGranularity;
  int Flags;
#define AVIF_HASINDEX           0x00000010        /* had idx1 chunk */
#define AVIF_MUSTUSEINDEX       0x00000020        /* must use idx1 chunk to determine order */
#define AVIF_ISINTERLEAVED      0x00000100        /* AVI file is interleaved */
#define AVIF_TRUSTCKTYPE        0x00000800
#define AVIF_WASCAPTUREFILE     0x00010000        /* specially allocated used for capturing real time video */
#define AVIF_COPYRIGHTED        0x00020000        /* contains copyrighted data */

  int TotalFrames;
  int InitialFrames;    /* InitialFrames - initial frame before interleaving */
  int Streams;
  int SuggestedBufferSize;
  int Width;
  int Height;
  int Reserved[4];
} AviMainHeader;

typedef struct _AviStreamHeader {
  int fcc;
  int size;  
  int Type;
#define AVIST_VIDEO FCC("vids")
#define AVIST_AUDIO FCC("auds")
#define AVIST_MIDI  FCC("mids")
#define AVIST_TEXT  FCC("txts")
  
  int Handler;
  int Flags;
#define AVISF_DISABLED 0x00000001
#define AVISF_VIDEO_PALCHANGES 0x00010000

  short Priority;
  short Language;
  int InitialFrames;
  int Scale;
  int Rate;
  int Start;
  int Length;
  int SuggestedBufferSize;
  int Quality;
  int SampleSize;
  short left;
  short top;
  short right;
  short bottom;
} AviStreamHeader;

typedef struct _AviBitmapInfoHeader {
  int fcc;
  int size;  
  int Size;
  int Width;
  int Height;
  short Planes;
  short BitCount;
  int Compression;
  int SizeImage;
  int XPelsPerMeter;
  int YPelsPerMeter;
  int ClrUsed;
  int ClrImportant;
} AviBitmapInfoHeader;

typedef struct _AviMJPEGUnknown {
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
} AviMJPEGUnknown;

typedef struct _AviIndexEntry {
  int ChunkId;
  int Flags;
#define AVIIF_LIST       0x00000001
#define AVIIF_KEYFRAME   0x00000010 
#define AVIIF_NO_TIME    0x00000100
#define AVIIF_COMPRESSOR 0x0FFF0000
  int Offset;
  int Size;
} AviIndexEntry;

typedef struct _AviIndex {
  int fcc;
  int size;
  AviIndexEntry *entrys;
} AviIndex;

typedef enum {
  AVI_FORMAT_RGB24,  /* The most basic of forms, 3 bytes per pixel, 1 per r, g, b */
  AVI_FORMAT_RGB32,  /* The second most basic of forms, 4 bytes per pixel, 1 per r, g, b, alpha */
  AVI_FORMAT_AVI_RGB, /* Same as above, but is in the wierd AVI order (bottom to top, left to right) */
  AVI_FORMAT_JMF, /* A lossless compression codec */
  AVI_FORMAT_MJPEG /* Motion-JPEG */
} AviFormat;

typedef struct _AviStreamRec {
  AviStreamHeader sh;
  void *sf;
  int sf_size;
  AviFormat format;
} AviStreamRec;

typedef struct _AviMovie {
  FILE *fp;

  int type;
#define AVI_MOVIE_READ  0
#define AVI_MOVIE_WRITE 1

  unsigned long size;

  AviMainHeader *header;
  AviStreamRec *streams;
  AviIndexEntry *entries;
  int index_entries;

  int movi_offset;
  int read_offset;
  long *offset_table;
  
  /* Local data goes here */
  int interlace;
  int odd_fields;
} AviMovie;

typedef enum {
  AVI_ERROR_NONE=0,
  AVI_ERROR_COMPRESSION,
  AVI_ERROR_OPEN,
  AVI_ERROR_READING,
  AVI_ERROR_WRITING,
  AVI_ERROR_FORMAT,
  AVI_ERROR_ALLOC,
  AVI_ERROR_FOUND,
  AVI_ERROR_OPTION
} AviError;

/* The offsets that will always stay the same in AVI files we write... used to seek around to the
   places where we need to write the sizes */

#define AVI_RIFF_SOFF 4L
#define AVI_HDRL_SOFF 16L

int avi_is_avi (char *name);

void avi_set_debug (int mode);
int avi_get_stream (AviMovie *movie, int avist_type, int stream_num);

AviError avi_print_error (AviError error);

int avi_get_data_id (AviFormat format, int stream);
int avi_get_format_type (AviFormat format);
int avi_get_format_fcc (AviFormat format);
int avi_get_format_compression (AviFormat format);
void *avi_format_convert (AviMovie *movie, int stream, void *buffer, AviFormat from, AviFormat to, int *size);

AviError avi_open_movie (char *name, AviMovie *movie);
void *avi_read_frame (AviMovie *movie, AviFormat format, int frame, int stream);
AviError avi_close (AviMovie *movie);

AviError avi_open_compress (char *name, AviMovie *movie, int streams, ...);
AviError avi_write_frame (AviMovie *movie, int frame_num, ...);
AviError avi_close_compress (AviMovie *movie);

typedef enum {
  AVI_OPTION_WIDTH=0,
  AVI_OPTION_HEIGHT, 
  AVI_OPTION_QUALITY,
  AVI_OPTION_FRAMERATE
} AviOption;

AviError avi_set_compress_option (AviMovie *movie, int option_type, int stream, AviOption option, void *opt_data);
#define AVI_OPTION_TYPE_MAIN 0
#define AVI_OPTION_TYPE_STRH 1
#define AVI_OPTION_TYPE_STRF 2

#endif /* __AVI_H__ */

