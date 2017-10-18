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

#include <stdlib.h>

#include <stdio.h> 
#include "avi.h"
#include "endian.h"

static void invert (int *num) {
	int new=0,i,j;

	for (j=0; j < 4; j++) {
		for (i=0; i<8; i++) {
			new |= ((*num>>(j*8+i))&1)<<((3-j)*8+i);
		}
	}
	
	*num = new;
}

static void sinvert (short int *num) {
	short int new=0;
	int i,j;

	for (j=0; j < 2; j++) {
		for (i=0; i<8; i++) {
			new |= ((*num>>(j*8+i))&1)<<((1-j)*8+i);
		}
	}

	*num = new;
}

static void Ichunk (AviChunk *chunk) {
	invert (&chunk->fcc);
	invert (&chunk->size);
}

static void Ilist (AviList *list){
	invert (&list->fcc);
	invert (&list->size);
	invert (&list->ids);
}

static void Imainh (AviMainHeader *mainh) {
	invert (&mainh->fcc);
	invert (&mainh->size);
	invert (&mainh->MicroSecPerFrame);
	invert (&mainh->MaxBytesPerSec);
	invert (&mainh->PaddingGranularity);
	invert (&mainh->Flags);
	invert (&mainh->TotalFrames);
	invert (&mainh->InitialFrames);
	invert (&mainh->Streams);
	invert (&mainh->SuggestedBufferSize);
	invert (&mainh->Width);
	invert (&mainh->Height);
	invert (&mainh->Reserved[0]);
	invert (&mainh->Reserved[1]);
	invert (&mainh->Reserved[2]);
	invert (&mainh->Reserved[3]);
}

static void Istreamh (AviStreamHeader *streamh) {
	invert (&streamh->fcc);
	invert (&streamh->size);
	invert (&streamh->Type);
	invert (&streamh->Handler);
	invert (&streamh->Flags);
	sinvert (&streamh->Priority);
	sinvert (&streamh->Language);
	invert (&streamh->InitialFrames);
	invert (&streamh->Scale);
	invert (&streamh->Rate);
	invert (&streamh->Start);
	invert (&streamh->Length);
	invert (&streamh->SuggestedBufferSize);
	invert (&streamh->Quality);
	invert (&streamh->SampleSize);
	sinvert (&streamh->left);
	sinvert (&streamh->right);
	sinvert (&streamh->top);
	sinvert (&streamh->bottom);
}

static void Ibitmaph (AviBitmapInfoHeader *bitmaph) {
	invert (&bitmaph->fcc);
	invert (&bitmaph->size);
	invert (&bitmaph->Size);
	invert (&bitmaph->Width);
	invert (&bitmaph->Height);
	sinvert (&bitmaph->Planes);
	sinvert (&bitmaph->BitCount);
	invert (&bitmaph->Compression);
	invert (&bitmaph->SizeImage);
	invert (&bitmaph->XPelsPerMeter);
	invert (&bitmaph->YPelsPerMeter);
	invert (&bitmaph->ClrUsed);
	invert (&bitmaph->ClrImportant);
}

static void Imjpegu (AviMJPEGUnknown *mjpgu) {
	invert (&mjpgu->a);
	invert (&mjpgu->b);
	invert (&mjpgu->c);
	invert (&mjpgu->d);
	invert (&mjpgu->e);
	invert (&mjpgu->f);
	invert (&mjpgu->g);
}

static void Iindexe (AviIndexEntry *indexe) {
	invert (&indexe->ChunkId);
	invert (&indexe->Flags);
	invert (&indexe->Offset);
	invert (&indexe->Size);
}

void awrite (AviMovie *movie, void *datain, int block, int size, FILE *fp, int type) {
#ifdef WORDS_BIGENDIAN
	void *data;

	data = mallocN (size, "awritedata");

	memcpy (data, datain, size);

	switch (type) {
	case AVI_RAW:
		fwrite (data, block, size, fp);
		break;
	case AVI_CHUNK:
		Ichunk ((AviChunk *) data);
		fwrite (data, block, size, fp);
		break;
	case AVI_LIST:
		Ilist ((AviList *) data);
		fwrite (data, block, size, fp);
		break;
	case AVI_MAINH:
		Imainh ((AviMainHeader *) data);
		fwrite (data, block, size, fp);
		break;
	case AVI_STREAMH:
		Istreamh ((AviStreamHeader *) data);
		fwrite (data, block, size, fp);
		break;
	case AVI_BITMAPH:
		Ibitmaph ((AviBitmapInfoHeader *) data);
		if (size==sizeof(AviBitmapInfoHeader) + sizeof(AviMJPEGUnknown)) {
			Imjpegu((AviMJPEGUnknown*)((char*)data+sizeof(AviBitmapInfoHeader)));
		}
		fwrite (data, block, size, fp);
		break;
	case AVI_MJPEGU:
		Imjpegu ((AviMJPEGUnknown *) data);
		fwrite (data, block, size, fp);
		break;
	case AVI_INDEXE:
		Iindexe ((AviIndexEntry *) data);
		fwrite (data, block, size, fp);
		break;
	default:
		break;
	}

	freeN (data);
#else /* WORDS_BIGENDIAN */
	fwrite (datain, block, size, fp);
#endif
}

