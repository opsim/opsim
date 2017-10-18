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

#include "avi.h"

#include "endian.h"

/* avi_set_compress_options gets its own file... now don't WE feel important? */

AviError avi_set_compress_option (AviMovie *movie, int option_type, int stream, AviOption option, void *opt_data) {
	int i;

	if (movie->header->TotalFrames != 0)  /* Can't change params after we have already started writing frames */
		return AVI_ERROR_OPTION;

	switch (option_type) {
	case AVI_OPTION_TYPE_MAIN:
		switch (option) {
			case AVI_OPTION_WIDTH:
				movie->header->Width = *((int *) opt_data);
				movie->header->SuggestedBufferSize = movie->header->Width*movie->header->Height*3;				

				for (i=0; i < movie->header->Streams; i++) {
					if (avi_get_format_type(movie->streams[i].format) == FCC("vids")) {
						((AviBitmapInfoHeader *) movie->streams[i].sf)->Width = *((int *) opt_data);
						movie->streams[i].sh.SuggestedBufferSize = movie->header->SuggestedBufferSize;
						movie->streams[i].sh.right = *((int *) opt_data);
						((AviBitmapInfoHeader *) movie->streams[i].sf)->SizeImage = movie->header->SuggestedBufferSize;
						fseek (movie->fp, movie->offset_table[1+i*2+1], SEEK_SET);
						awrite (movie, movie->streams[i].sf, 1, movie->streams[i].sf_size, movie->fp, AVI_BITMAPH);
					}
				}

				break;
				
			case AVI_OPTION_HEIGHT:
				movie->header->Height = *((int *) opt_data);
				movie->header->SuggestedBufferSize = movie->header->Width*movie->header->Height*3;
				
				for (i=0; i < movie->header->Streams; i++) {
					if (avi_get_format_type(movie->streams[i].format) == FCC("vids")) {
						((AviBitmapInfoHeader *) movie->streams[i].sf)->Height = *((int *) opt_data);
						movie->streams[i].sh.SuggestedBufferSize = movie->header->SuggestedBufferSize;
						movie->streams[i].sh.bottom = *((int *) opt_data);
						((AviBitmapInfoHeader *) movie->streams[i].sf)->SizeImage = movie->header->SuggestedBufferSize;
						fseek (movie->fp, movie->offset_table[1+i*2+1], SEEK_SET);
						awrite (movie, movie->streams[i].sf, 1, movie->streams[i].sf_size, movie->fp, AVI_BITMAPH);
					}
				}

				break;
				
			case AVI_OPTION_QUALITY:
				for (i=0; i < movie->header->Streams; i++) {
					if (avi_get_format_type(movie->streams[i].format) == FCC("vids")) {
						movie->streams[i].sh.Quality = (*((int *) opt_data))*100;
						fseek (movie->fp, movie->offset_table[1+i*2+1], SEEK_SET);
						awrite (movie, movie->streams[i].sf, 1, movie->streams[i].sf_size, movie->fp, AVI_BITMAPH);						
					}
				}
				break;
				
			case AVI_OPTION_FRAMERATE:
				if (1000000/(*((int *) opt_data)))
					movie->header->MicroSecPerFrame = 1000000/(*((int *) opt_data));					

				for (i=0; i < movie->header->Streams; i++) {
					if (avi_get_format_type(movie->streams[i].format) == FCC("vids")) {
						movie->streams[i].sh.Scale = movie->header->MicroSecPerFrame;
						fseek (movie->fp, movie->offset_table[1+i*2+1], SEEK_SET);
						awrite (movie, movie->streams[i].sf, 1, movie->streams[i].sf_size, movie->fp, AVI_BITMAPH);
					}
				}
				
		}

    fseek (movie->fp, movie->offset_table[0], SEEK_SET);
    awrite (movie, movie->header, 1, sizeof(AviMainHeader), movie->fp, AVI_MAINH);

    break;
  case AVI_OPTION_TYPE_STRH:
    break;
  case AVI_OPTION_TYPE_STRF:
    break;
  default:
    return AVI_ERROR_OPTION;
    break;
  }

  return AVI_ERROR_NONE;
}


