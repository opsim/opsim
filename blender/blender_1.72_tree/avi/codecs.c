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


#include "avirgb.h"
#include "jmf.h"
#include "mjpeg.h"
#include "rgb32.h"

void *avi_format_convert (AviMovie *movie, int stream, void *buffer, AviFormat from, AviFormat to, int *size) {
  if (from == to)
    return buffer;
    
  if (from != AVI_FORMAT_RGB24 &&
      to != AVI_FORMAT_RGB24)
    return avi_format_convert(movie, stream, 
			      avi_format_convert (movie, stream, buffer, from, AVI_FORMAT_RGB24, size), 
			      AVI_FORMAT_RGB24, to, size);
  
  switch (to) {
  case AVI_FORMAT_RGB24:
    switch (from) {
    case AVI_FORMAT_AVI_RGB:
      buffer = avi_converter_from_avi_rgb (movie, stream, buffer, size);
      break;
    case AVI_FORMAT_JMF:
      buffer = avi_converter_from_jmf (movie, stream, buffer, size);
      break;
    case AVI_FORMAT_MJPEG:
      buffer = avi_converter_from_mjpeg (movie, stream, buffer, size);
      break;
    case AVI_FORMAT_RGB32:
      buffer = avi_converter_from_rgb32 (movie, stream, buffer, size);
      break;
    default:
      break;
    }
    break;
  case AVI_FORMAT_AVI_RGB:
    buffer = avi_converter_to_avi_rgb (movie, stream, buffer, size);
    break;
  case AVI_FORMAT_JMF:
    buffer = avi_converter_to_jmf (movie, stream, buffer, size);
    break;
  case AVI_FORMAT_MJPEG:
    buffer = avi_converter_to_mjpeg (movie, stream, buffer, size);
    break;
  case AVI_FORMAT_RGB32:
    buffer = avi_converter_to_rgb32 (movie, stream, buffer, size);
    break;
  default:
    break;
  }

  return buffer;
}

int avi_get_data_id (AviFormat format, int stream) {
  char fcc[5];

  if (avi_get_format_type (format) == FCC("vids"))
    sprintf (fcc,"%2.2ddc",stream);
  else if (avi_get_format_type (format) == FCC("auds"))
    sprintf (fcc,"%2.2ddc",stream);
  else
    return 0;

  return FCC(fcc);
}

int avi_get_format_type (AviFormat format) {
  switch (format) {
  case AVI_FORMAT_RGB24:
  case AVI_FORMAT_RGB32:
  case AVI_FORMAT_AVI_RGB:
  case AVI_FORMAT_JMF:
  case AVI_FORMAT_MJPEG:
    return FCC("vids");
    break;
  default:
    return 0;
    break;
  }
}

int avi_get_format_fcc (AviFormat format) {
  switch (format) {
  case AVI_FORMAT_RGB24:
  case AVI_FORMAT_RGB32:
  case AVI_FORMAT_AVI_RGB:
    return FCC("DIB ");
    break;
  case AVI_FORMAT_JMF:
    return FCC("JMF ");
    break;
  case AVI_FORMAT_MJPEG:
    return FCC("MJPG");
    break;
  default:
    return 0;
    break;
  }
}

int avi_get_format_compression (AviFormat format) {
  switch (format) {
  case AVI_FORMAT_RGB24:
  case AVI_FORMAT_RGB32:
  case AVI_FORMAT_AVI_RGB:
    return 0;
    break;
  case AVI_FORMAT_JMF:
    return FCC("JMF ");
    break;
  case AVI_FORMAT_MJPEG:
    return FCC("MJPG");
    break;
  default:
    return 0;
    break;
  }
}

