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

#include <stdlib.h>
#include <jpeglib.h>

#define OUTPUT_BUF_SIZE  4096   /* choose an efficiently fwrite'able size */
#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

void my_jpeg_buffer_dest (j_compress_ptr cinfo, uchar *outbuffer);
void my_jpeg_buffer_src (j_decompress_ptr dinfo, uchar *outbuffer, int bufmax);

static int numbytes;

void add_huff_table (j_decompress_ptr dinfo, JHUFF_TBL **htblptr, const UINT8 *bits, const UINT8 *val) {
	if (*htblptr == NULL)
		*htblptr = jpeg_alloc_huff_table((j_common_ptr) dinfo);

	memcpy((*htblptr)->bits, bits, sizeof((*htblptr)->bits));
	memcpy((*htblptr)->huffval, val, sizeof((*htblptr)->huffval));

	/* Initialize sent_table FALSE so table will be written to JPEG file. */
	(*htblptr)->sent_table = FALSE;
}

/* Set up the standard Huffman tables (cf. JPEG standard section K.3) */
/* IMPORTANT: these are only valid for 8-bit data precision! */

void std_huff_tables (j_decompress_ptr dinfo) {
	static const UINT8 bits_dc_luminance[17] =
	{ /* 0-base */
		0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 	};
	static const UINT8 val_dc_luminance[] =
	{ 
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 	};

	static const UINT8 bits_dc_chrominance[17] =
	{ /* 0-base */
		0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 	};
	static const UINT8 val_dc_chrominance[] =
	{ 
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 	};

	static const UINT8 bits_ac_luminance[17] =
	{ /* 0-base */
		0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d 	};
	static const UINT8 val_ac_luminance[] =
	{ 
		0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
		0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
		0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
		0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
		0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
		0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
		0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
		0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
		0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
		0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
		0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
		0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
		0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
		0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
		0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
		0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
		0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
		0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
		0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
		0xf9, 0xfa 	};
	static const UINT8 bits_ac_chrominance[17] =
	{ /* 0-base */
		0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 	};
	static const UINT8 val_ac_chrominance[] =
	{ 
		0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
		0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
		0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
		0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
		0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
		0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
		0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
		0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
		0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
		0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
		0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
		0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
		0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
		0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
		0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
		0xf9, 0xfa 	};

	add_huff_table(dinfo, &dinfo->dc_huff_tbl_ptrs[0],
	    bits_dc_luminance, val_dc_luminance);
	add_huff_table(dinfo, &dinfo->ac_huff_tbl_ptrs[0],
	    bits_ac_luminance, val_ac_luminance);
	add_huff_table(dinfo, &dinfo->dc_huff_tbl_ptrs[1],
	    bits_dc_chrominance, val_dc_chrominance);
	add_huff_table(dinfo, &dinfo->ac_huff_tbl_ptrs[1],
	    bits_ac_chrominance, val_ac_chrominance);
}

int Decode_JPEG(uchar *inBuffer, uchar *outBuffer, int height, int bufsize){
	int row_stride, y;
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;

	dinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dinfo);
	my_jpeg_buffer_src(&dinfo, inBuffer, bufsize);
	jpeg_read_header(&dinfo, TRUE);
	if (dinfo.dc_huff_tbl_ptrs[0] == NULL){
		std_huff_tables(&dinfo);
	}
	dinfo.out_color_space = JCS_RGB;
	dinfo.dct_method = JDCT_IFAST;

	jpeg_start_decompress(&dinfo);
	row_stride= dinfo.output_width*dinfo.output_components;
	for (y = 0; y < dinfo.output_height; y++){
		jpeg_read_scanlines(&dinfo, (JSAMPARRAY) &outBuffer, 1);
		outBuffer += row_stride;
	}
	jpeg_finish_decompress(&dinfo);

	if (dinfo.output_height >= height) return 0;
	
	inBuffer+= numbytes;
	my_jpeg_buffer_src(&dinfo, inBuffer, bufsize-numbytes);

	numbytes= 0;
	jpeg_read_header(&dinfo, TRUE);
	if (dinfo.dc_huff_tbl_ptrs[0] == NULL){
		std_huff_tables(&dinfo);
	}

	jpeg_start_decompress(&dinfo);
	row_stride= dinfo.output_width*dinfo.output_components;
	for (y = 0; y < dinfo.output_height; y++){
		jpeg_read_scanlines(&dinfo, (JSAMPARRAY) &outBuffer, 1);
		outBuffer += row_stride;
	}
	jpeg_finish_decompress(&dinfo);
	jpeg_destroy_decompress(&dinfo);
	
	return 1;
}


void Compress_JPEG(int quality, uchar *outbuffer, uchar *inBuffer, int width, int height) {
	int row_stride;
	int y;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char marker[60];
	int i;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	my_jpeg_buffer_dest (&cinfo, outbuffer);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_colorspace (&cinfo, JCS_YCbCr);
		
	jpeg_set_quality (&cinfo, quality, TRUE);

	cinfo.dc_huff_tbl_ptrs[0]->sent_table = TRUE;
	cinfo.dc_huff_tbl_ptrs[1]->sent_table = TRUE;
	cinfo.ac_huff_tbl_ptrs[0]->sent_table = TRUE;
	cinfo.ac_huff_tbl_ptrs[1]->sent_table = TRUE;

	cinfo.comp_info[0].component_id = 0;
	cinfo.comp_info[0].v_samp_factor = 1;
	cinfo.comp_info[1].component_id = 1;
	cinfo.comp_info[2].component_id = 2;

	cinfo.write_JFIF_header = FALSE;

	jpeg_start_compress(&cinfo, FALSE);

	i=0;
	marker[i++] = 'A';
	marker[i++] = 'V';
	marker[i++] = 'I';
	marker[i++] = '1';
	marker[i++] = 0;
	while (i<60)
		marker[i++] = 32;

	jpeg_write_marker (&cinfo, JPEG_APP0, marker, 60);

	i=0;
	while (i<60)
		marker[i++] = 0;

	jpeg_write_marker (&cinfo, JPEG_COM, marker, 60);

	row_stride= cinfo.image_width*cinfo.input_components;
	for (y = 0; y < cinfo.image_height; y++){
		jpeg_write_scanlines(&cinfo, (JSAMPARRAY) &inBuffer, 1);
		inBuffer += row_stride;
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
}

static void interlace (uchar *to, uchar *from, int width, int height) {
	int i;
	
	for (i=0; i<height; i++) {
		if (i%2)
			memcpy (&to[i*width*3], &from[(i/2 + height/2)*width*3], width * 3);
		else 
			memcpy (&to[i*width*3], &from[(i/2)*width*3], width * 3);
	}
}

static void deinterlace (int odd, uchar *to, uchar *from, int width, int height) {
	int i;
	
	for (i=0; i<height; i++) {
		if (i%2==odd)
			memcpy (&to[(i/2 + height/2)*width*3], &from[i*width*3], width * 3);
		else 
			memcpy (&to[(i/2)*width*3], &from[i*width*3], width * 3);
	}
}

void *avi_converter_from_mjpeg (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int y, deint, rowstride;
	uchar *buf;
	uchar *tmp;
	
	numbytes = 0;

	buf = mallocN (movie->header->Height * movie->header->Width * 3, "frommjpegbuf");

	deint= Decode_JPEG (buffer,  buf, movie->header->Height, *size);
	
	freeN (buffer);
	
	if (deint) {
		buffer = mallocN (movie->header->Height * movie->header->Width * 3, "frommjpegbuf");
		interlace (buffer, buf, movie->header->Width, movie->header->Height);
		freeN (buf);
	
		buf= buffer;
	}
		
	return buf;
}

void *avi_converter_to_mjpeg (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int y,i, rowstride;
	unsigned char *buf;
	int uninterlaced=0;
	
	numbytes = 0;
	*size= 0;

	buf = mallocN (movie->header->Height * movie->header->Width * 3, "tomjpegbuf");	
	if (!movie->interlace) {
		Compress_JPEG (movie->streams[stream].sh.Quality/100, buf, buffer,  movie->header->Width, movie->header->Height);
	} else {
		deinterlace (movie->odd_fields, buf, buffer, movie->header->Width, movie->header->Height);
		freeN (buffer);
	
		buffer= buf;

		buf = mallocN (movie->header->Height * movie->header->Width * 3, "tomjpegbuf");
	
		Compress_JPEG (movie->streams[stream].sh.Quality/100, buf, buffer,  movie->header->Width, movie->header->Height/2);
		*size += numbytes; numbytes=0;
		Compress_JPEG (movie->streams[stream].sh.Quality/100, buf+*size, buffer+(movie->header->Height/2)*movie->header->Width*3,  movie->header->Width, movie->header->Height/2);
	}
	*size += numbytes;	

	freeN (buffer);
	return buf;
}


/* Compression from memory */

typedef struct {
	struct jpeg_destination_mgr pub; /* public fields */

	uchar * outbuffer;               /* target stream */
	JOCTET * buffer;              /* start of buffer */
} my_destination_mgr;

typedef my_destination_mgr * my_dest_ptr;

void init_destination (j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

	/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)
	    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
	    OUTPUT_BUF_SIZE * sizeof(JOCTET));
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

boolean empty_output_buffer (j_compress_ptr cinfo) {
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

	memcpy ((dest->outbuffer+numbytes),dest->buffer,OUTPUT_BUF_SIZE);

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	numbytes = numbytes + OUTPUT_BUF_SIZE;
	return TRUE;
}


void term_destination (j_compress_ptr cinfo) {
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

	if (datacount > 0) {
		memcpy ((dest->outbuffer+numbytes),dest->buffer,datacount);
		/*    if (JFWRITE(dest->outfile, dest->buffer, datacount) != datacount);*/
	}
	numbytes = numbytes + datacount;
}

void my_jpeg_buffer_dest (j_compress_ptr cinfo, uchar *outbuffer) {
	my_dest_ptr dest;

	/* The destination object is made permanent so that multiple JPEG images
   * can be written to the same file without re-executing jpeg_buffer_dest.
   * This makes it dangerous to use this manager and a different destination
   * manager serially with the same JPEG object, because their private object
   * sizes may be different.  Caveat programmer.
   */
	if (cinfo->dest == NULL) {    /* first time for this JPEG object? */
		cinfo->dest = (struct jpeg_destination_mgr *)
		    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		    sizeof(my_destination_mgr));
	}

	dest = (my_dest_ptr) cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	dest->outbuffer = outbuffer;
}


/* Decompression from memory */

typedef struct {
	struct jpeg_source_mgr pub;	/* public fields */

	uchar * inbuffer;		/* source buffer */
	JOCTET * buffer;		/* start of buffer */
	
	int bufmax; /* maximum number of bytes we can read */
} my_source_mgr;

typedef my_source_mgr* my_src_ptr;

void init_source (j_decompress_ptr dinfo) {
	my_src_ptr src = (my_src_ptr) dinfo->src;
}

boolean fill_input_buffer (j_decompress_ptr dinfo) {
	my_src_ptr src = (my_src_ptr) dinfo->src;
	int bytes_in_buf;
	
	bytes_in_buf= src->bufmax - numbytes;

	if (bytes_in_buf> INPUT_BUF_SIZE)
		bytes_in_buf= INPUT_BUF_SIZE;

	memcpy (src->buffer,(src->inbuffer+numbytes), bytes_in_buf);

	src->pub.next_input_byte = src->inbuffer+numbytes;
	src->pub.bytes_in_buffer = bytes_in_buf;

	numbytes = numbytes + bytes_in_buf;

	return TRUE;
}

void skip_input_data (j_decompress_ptr cinfo, long num_bytes) {
	my_src_ptr src = (my_src_ptr) cinfo->src;

	/* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
	if (num_bytes > 0) {
		while (num_bytes > (long) src->pub.bytes_in_buffer) {
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

void term_source (j_decompress_ptr dinfo) {
	my_src_ptr src = (my_src_ptr) dinfo->src;
	
	numbytes -= src->pub.bytes_in_buffer;
}

void my_jpeg_buffer_src (j_decompress_ptr dinfo, uchar *inbuffer, int bufmax) {
	my_src_ptr src;

	/* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
	if (dinfo->src == NULL) {	/* first time for this JPEG object? */
		dinfo->src = (struct jpeg_source_mgr *)
		    (*dinfo->mem->alloc_small) ((j_common_ptr) dinfo, JPOOL_PERMANENT,
		    sizeof(my_source_mgr));
		src = (my_src_ptr) dinfo->src;
		src->buffer = (JOCTET *)
		    (*dinfo->mem->alloc_small) ((j_common_ptr) dinfo, JPOOL_PERMANENT,
		    INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	src = (my_src_ptr) dinfo->src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = term_source;
	src->inbuffer = inbuffer;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
	src->bufmax= bufmax;
}

