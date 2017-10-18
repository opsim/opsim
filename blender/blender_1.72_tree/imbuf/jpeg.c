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

#include "iff.h"
#include "util.h"
#include "jpeglib.h"

/* strncasecmp hack */
#ifdef WIN32
#define strncasecmp(a, b, n) strncmp(a, b, n)
#endif

/*

zipfork "cc -g tojpeg.c /usr/people/frank/source/util.o -limbuf -limage libjpeg.a -lm -o tojpeg > /dev/console"                                                                  
zipfork "tojpeg > /dev/console"                                                                  

*/


/*
 * Er zijn in principe vier verschillende jpeg formaten.
 * 
 * 1. jpeg - standaard drukwerk, u & v op kwart van resolutie
 * 2. jvid - standaard video u & v halve resolutie, frame opengeklapt
 * 3. jstr - als 2, maar dan met twee losse fields weggeschreven
 *		moet baseline zijn
 *		moet rgb zijn
 *		moet samplingfactors goed hebben
 * 4. jmax - geen scaling in de componenten
 */

int jpeg_failed = FALSE;
int jpeg_default_quality;
int ibuf_ftype;

void jpeg_error (j_common_ptr cinfo)
{
	/* Always display the message */
	(*cinfo->err->output_message) (cinfo);

	/* Let the memory manager delete any temp files before we die */
	jpeg_destroy(cinfo);

	jpeg_failed = TRUE;
}

#define MAKESTMT(stuff)            do { stuff } while (0)

#define INPUT_VARS(cinfo)  \
	struct jpeg_source_mgr * datasrc = (cinfo)->src;  \
	const JOCTET * next_input_byte = datasrc->next_input_byte;  \
	size_t bytes_in_buffer = datasrc->bytes_in_buffer

/* Unload the local copies --- do this only at a restart boundary */
#define INPUT_SYNC(cinfo)  \
	( datasrc->next_input_byte = next_input_byte,  \
	  datasrc->bytes_in_buffer = bytes_in_buffer )

/* Reload the local copies --- seldom used except in MAKE_BYTE_AVAIL */
#define INPUT_RELOAD(cinfo)  \
	( next_input_byte = datasrc->next_input_byte,  \
	  bytes_in_buffer = datasrc->bytes_in_buffer )

/* Internal macro for INPUT_BYTE and INPUT_2BYTES: make a byte available.
 * Note we do *not* do INPUT_SYNC before calling fill_input_buffer,
 * but we must reload the local copies after a successful fill.
 */
#define MAKE_BYTE_AVAIL(cinfo,action)  \
	if (bytes_in_buffer == 0) {  \
	  if (! (*datasrc->fill_input_buffer) (cinfo))  \
	    { action; }  \
	  INPUT_RELOAD(cinfo);  \
	}  \
	bytes_in_buffer--

/* Read a byte into variable V.
 * If must suspend, take the specified action (typically "return FALSE").
 */
#define INPUT_BYTE(cinfo,V,action)  \
	MAKESTMT( MAKE_BYTE_AVAIL(cinfo,action); \
		  V = GETJOCTET(*next_input_byte++); )

/* As above, but read two bytes interpreted as an unsigned 16-bit integer.
 * V should be declared unsigned int or perhaps INT32.
 */
#define INPUT_2BYTES(cinfo,V,action)  \
	MAKESTMT( MAKE_BYTE_AVAIL(cinfo,action); \
		  V = ((unsigned int) GETJOCTET(*next_input_byte++)) << 8; \
		  MAKE_BYTE_AVAIL(cinfo,action); \
		  V += GETJOCTET(*next_input_byte++); )


static boolean
handle_app1 (j_decompress_ptr cinfo)
{
	INT32 length, i;
	char neogeo[128];
	
	INPUT_VARS(cinfo);

	INPUT_2BYTES(cinfo, length, return FALSE);
	length -= 2;
	
	if (length < 16) {
		for (i = 0; i < length; i++) INPUT_BYTE(cinfo, neogeo[i], return FALSE);
		length = 0;
		if (strncmp(neogeo, "NeoGeo", 6) == 0) memcpy(&ibuf_ftype, neogeo + 6, 4);
		ibuf_ftype = BIG_LONG(ibuf_ftype);
	}
	INPUT_SYNC(cinfo);		/* do before skip_input_data */
	if (length > 0) (*cinfo->src->skip_input_data) (cinfo, length);
	return TRUE;
}


struct ImBuf * load_sub_jpeg (char * filename, int flags)
{
	struct jpeg_decompress_struct _cinfo, *cinfo = &_cinfo;
	struct jpeg_error_mgr jerr;
	FILE * infile;
	JSAMPARRAY row_pointer;
	JSAMPLE * buffer = 0;
	int row_stride, keep_yuv = FALSE;
	int x, y, depth, r, g, b, k, r2, g2, b2;
	struct ImBuf * ibuf = 0;
	uchar * rect;
	
	if ((infile = fopen(filename, "rb")) == NULL) {
		/*fprintf(stderr, "can't open %s\n", filename);*/
		return 0;
	}

	cinfo->err = jpeg_std_error(&jerr);
	jerr.error_exit = jpeg_error;

	jpeg_create_decompress(cinfo);
	jpeg_stdio_src(cinfo, infile);

	/* eigen app1 handler installeren */
	ibuf_ftype = 0;
	jpeg_set_marker_processor(cinfo, 0xe1, handle_app1);
	cinfo->dct_method = JDCT_FLOAT;

	if (jpeg_read_header(cinfo, FALSE) == JPEG_HEADER_OK) {
		x = cinfo->image_width;
		y = cinfo->image_height;
		depth = cinfo->num_components;
		
		if (flags & IB_yuv) {
			if (cinfo->jpeg_color_space == JCS_YCbCr) {
				cinfo->out_color_space = JCS_YCbCr;
				keep_yuv = TRUE;
			}
		}
		if (cinfo->jpeg_color_space == JCS_YCCK) cinfo->out_color_space = JCS_CMYK;

		jpeg_start_decompress(cinfo);

		if (ibuf_ftype == 0) {
			ibuf_ftype = JPG_STD;
			if (cinfo->max_v_samp_factor == 1) {
				if (cinfo->max_h_samp_factor == 1) ibuf_ftype = JPG_MAX;
				else ibuf_ftype = JPG_VID;
			}
		}

		if (flags & IB_test) {
			jpeg_abort_decompress(cinfo);
			ibuf = allocImBuf(x, y, 8 * depth, 0, 0);
		} else {
			ibuf = allocImBuf(x, y, 8 * depth, IB_rect, 0);

			row_stride = cinfo->output_width * depth;

			row_pointer = (*cinfo->mem->alloc_sarray) ((j_common_ptr) cinfo, JPOOL_IMAGE, row_stride, 1);
			
			for (y = ibuf->y - 1; y >= 0; y--) {
				jpeg_read_scanlines(cinfo, row_pointer, 1);
				if (flags & IB_ttob) {
					rect = (uchar *) (ibuf->rect + (ibuf->y - 1 - y) * ibuf->x);
				} else {
					rect = (uchar *) (ibuf->rect + y * ibuf->x);
				}
				buffer = row_pointer[0];
				
				switch(depth) {
					case 1:
						for (x = ibuf->x; x > 0; x--) {
							rect[3] = 255;
							rect[0] = rect[1] = rect[2] = *buffer++;
							rect += 4;
						}
						break;
					case 3:
						if (keep_yuv) {
							for (x = ibuf->x; x > 0; x--) {
								rect[0] = 255;
								rect[2] = *buffer++;
								rect[1] = *buffer++;
								rect[3] = *buffer++;
								rect += 4;
							}
						} else {
							for (x = ibuf->x; x > 0; x--) {
								rect[3] = 255;
								rect[0] = *buffer++;
								rect[1] = *buffer++;
								rect[2] = *buffer++;
								rect += 4;
							}
						}
						break;
					case 4:
						for (x = ibuf->x; x > 0; x--) {
							r = *buffer++;
							g = *buffer++;
							b = *buffer++;
							k = *buffer++;
							
							k = 255 - k;
							r -= k;
							if (r & 0xffffff00) {
								if (r < 0) r = 0;
								else r = 255;
							}
							g -= k;
							if (g & 0xffffff00) {
								if (g < 0) g = 0;
								else g = 255;
							}
							b -= k;
							if (b & 0xffffff00) {
								if (b < 0) b = 0;
								else b = 255;
							}							
							
							rect[3] = 255 - k;
							rect[2] = b;
							rect[1] = g;
							rect[0] = r;
							rect += 4;
						}
				}
			}
			jpeg_finish_decompress(cinfo);
		}
		ibuf->flags = flags & ~IB_yuv;
		if (keep_yuv) ibuf->flags |= IB_yuv;
		
		jpeg_destroy((j_common_ptr) cinfo);
		ibuf->ftype = ibuf_ftype;
   	}

	fclose(infile);
	return(ibuf);
}


struct ImBuf * loadjpeg (char * filename, int flags)
{
	struct ImBuf *ibuf, *jf0 = 0, *jf1 = 0;
	char newname[1024];
	int len;
	extern rectcpy();
	
	ibuf = load_sub_jpeg(filename, flags);
	if (ibuf == 0) return (0);

	if (IS_jstjpg(ibuf) | IS_vidjpg(ibuf)){
		len = strlen(filename);
		if (len > 4) {
			if (strncasecmp(filename + len - 4, ".jf", 3) == 0) {
				strcpy(newname, filename);
				newname[len - 1] ^= 1; /* 0 -> 1 en 1 -> 0 */
				
				if (filename[len - 1] == '0') {
					jf0 = ibuf;
					jf1 = load_sub_jpeg(newname, flags);
				} else {
					jf1 = ibuf;
					jf0 = load_sub_jpeg(newname, flags);
				}
				
				if ((jf1 != 0) && (jf0 != 0)) {
					if (flags & IB_test) {
						ibuf = allocImBuf(jf0->x, jf0->y * 2, jf1->depth, 0, 0);
						ibuf->ftype = jf0->ftype;
						ibuf->flags = jf0->flags;
					} else {
						ibuf = allocImBuf(jf0->x * 2, jf0->y, jf1->depth, IB_rect, 0);
						if (flags & IB_ttob) {
							rectop(ibuf, jf1, 0, 0, 0, 0, 32767, 32767, rectcpy);
							rectop(ibuf, jf0, jf1->x, 0, 0, 0, 32767, 32767, rectcpy);
						}else{
							rectop(ibuf, jf0, 0, 0, 0, 0, 32767, 32767, rectcpy);
							rectop(ibuf, jf1, jf0->x, 0, 0, 0, 32767, 32767, rectcpy);
						}
						ibuf->x /= 2;
						ibuf->y *= 2;
						ibuf->ftype = jf0->ftype;
						ibuf->flags = jf0->flags;
					}
					freeImBuf(jf0); freeImBuf(jf1);
					if (IS_vidjpg(ibuf)) ibuf->ftype = JPG_JST | (ibuf->ftype & JPG_MSK);
				}
			}
		}
	}

/*
	if (ibuf) {
		if (ibuf->rect) {
			convert_rgba_to_abgr((ibuf->x+ibuf->skipx)*ibuf->y, ibuf->rect);
		}
	}
*/

	return(ibuf);
}


void write_jpeg(struct jpeg_compress_struct * cinfo, struct ImBuf * ibuf)
{
	JSAMPLE * buffer = 0;
	JSAMPROW row_pointer[1];
	uchar * rect;
	int x, y;
	char neogeo[128];


	jpeg_start_compress(cinfo, TRUE);

	strcpy(neogeo, "NeoGeo");
	ibuf_ftype = BIG_LONG(ibuf->ftype);
	
	memcpy(neogeo + 6, &ibuf_ftype, 4);
	jpeg_write_marker(cinfo, 0xe1, neogeo, 10);

	row_pointer[0] = mallocstruct(JSAMPLE, cinfo->input_components * cinfo->image_width);

	for(y = ibuf->y - 1; y >= 0; y--){
		rect = (uchar *) (ibuf->rect + y * ibuf->x);
		buffer = row_pointer[0];

		switch(cinfo->in_color_space){
		case JCS_RGB:
			for (x = 0; x < ibuf->x; x++) {
				*buffer++ = rect[0];
				*buffer++ = rect[1];
				*buffer++ = rect[2];
				rect += 4;
			}
			break;
		case JCS_YCbCr:
			for (x = 0; x < ibuf->x; x++) {
				*buffer++ = rect[1];
				*buffer++ = rect[2];
				*buffer++ = rect[0];
				rect += 4;
			}
			break;
		case JCS_GRAYSCALE:
			for (x = 0; x < ibuf->x; x++) {
				*buffer++ = rect[0];
				rect += 4;
			}
			break;
		case JCS_UNKNOWN:
			memcpy(buffer, rect, 4 * ibuf->x);
			break;
		}

		jpeg_write_scanlines(cinfo, row_pointer, 1);

		if (jpeg_failed) break;
	}

	if (jpeg_failed == FALSE) jpeg_finish_compress(cinfo);
	free(row_pointer[0]);
}


int init_jpeg(FILE * outfile, struct jpeg_compress_struct * cinfo, struct ImBuf *ibuf)
{
	int quality;

	quality = ibuf->ftype & 0xff;
	if (quality <= 0) quality = jpeg_default_quality;
	if (quality > 100) quality = 100;

	jpeg_create_compress(cinfo);
	jpeg_stdio_dest(cinfo, outfile);

	cinfo->image_width = ibuf->x;
	cinfo->image_height = ibuf->y;

	cinfo->in_color_space = JCS_RGB;
	if (ibuf->flags & IB_yuv) cinfo->in_color_space = JCS_YCbCr;
	if (ibuf->depth == 8 && ibuf->cmap == 0) cinfo->in_color_space = JCS_GRAYSCALE;
	if (ibuf->depth == 32) cinfo->in_color_space = JCS_UNKNOWN;
	
	switch(cinfo->in_color_space){
	case JCS_RGB:
	case JCS_YCbCr:
		cinfo->input_components = 3;
		break;
	case JCS_GRAYSCALE:
		cinfo->input_components = 1;
		break;
	case JCS_UNKNOWN:
		cinfo->input_components = 4;
		break;
	}
	jpeg_set_defaults(cinfo);
	
	/* eigen instellingen */

	cinfo->dct_method = JDCT_FLOAT;
	jpeg_set_quality(cinfo, quality, TRUE);

	return(0);
}


int save_stdjpeg(char * name, struct ImBuf * ibuf)
{
	FILE * outfile;
	struct jpeg_compress_struct _cinfo, *cinfo = &_cinfo;
	struct jpeg_error_mgr jerr;

	if ((outfile = fopen(name, "wb")) == NULL) return(-1);
	jpeg_default_quality = 75;

	cinfo->err = jpeg_std_error(&jerr);
	jerr.error_exit = jpeg_error;

	init_jpeg(outfile, cinfo, ibuf);

	write_jpeg(cinfo, ibuf);

	fclose(outfile);
	jpeg_destroy_compress(cinfo);

	if (jpeg_failed) remove(name);
	return(jpeg_failed);
}


int save_vidjpeg(char * name, struct ImBuf * ibuf)
{
	FILE * outfile;
	struct jpeg_compress_struct _cinfo, *cinfo = &_cinfo;
	struct jpeg_error_mgr jerr;
	int i;

	if ((outfile = fopen(name, "wb")) == NULL) return(-1);
	jpeg_default_quality = 90;

	cinfo->err = jpeg_std_error(&jerr);
	jerr.error_exit = jpeg_error;

	init_jpeg(outfile, cinfo, ibuf);

	/* scalings factoren goedzetten */
	if ((cinfo->in_color_space == JCS_RGB) || (cinfo->in_color_space == JCS_YCbCr)) {
		cinfo->comp_info[0].h_samp_factor = 2;
		cinfo->comp_info[0].v_samp_factor = 1;
	}

	write_jpeg(cinfo, ibuf);

	fclose(outfile);
	jpeg_destroy_compress(cinfo);

	if (jpeg_failed) remove(name);
	return(jpeg_failed);
}

int save_jstjpeg(char * name, struct ImBuf * ibuf)
{
	char fieldname[1024];
	struct ImBuf * tbuf;
	int oldy;
	extern rectcpy();

	tbuf = allocImBuf(ibuf->x, ibuf->y / 2, 24, IB_rect, 0);
	tbuf->ftype = ibuf->ftype;
	tbuf->flags = ibuf->flags;
	
	oldy = ibuf->y;
	ibuf->x *= 2;
	ibuf->y /= 2;

	rectop(tbuf, ibuf, 0, 0, 0, 0, 32767, 32767, rectcpy);
	sprintf(fieldname, "%s.jf0", name);

	if (save_vidjpeg(fieldname, tbuf) == 0) {
		rectop(tbuf, ibuf, 0, 0, tbuf->x, 0, 32767, 32767, rectcpy);
		sprintf(fieldname, "%s.jf1", name);
		save_vidjpeg(fieldname, tbuf);
	}

	ibuf->y = oldy;
	ibuf->x /= 2;
	freeImBuf(tbuf);

}


int save_maxjpeg(char * name, struct ImBuf * ibuf)
{
	FILE * outfile;
	struct jpeg_compress_struct _cinfo, *cinfo = &_cinfo;
	struct jpeg_error_mgr jerr;
	int i;

	if ((outfile = fopen(name, "wb")) == NULL) return(-1);
	jpeg_default_quality = 100;

	cinfo->err = jpeg_std_error(&jerr);
	jerr.error_exit = jpeg_error;

	init_jpeg(outfile, cinfo, ibuf);

	/* scalings factoren goedzetten */
	if (cinfo->in_color_space == JCS_RGB) {
		cinfo->comp_info[0].h_samp_factor = 1;
		cinfo->comp_info[0].v_samp_factor = 1;
	}

	write_jpeg(cinfo, ibuf);

	fclose(outfile);
	jpeg_destroy_compress(cinfo);

	if (jpeg_failed) remove(name);
	return(jpeg_failed);
}


int save_jpeg(char * name, struct ImBuf * ibuf, int flags)
{
	int ret;
	
	ibuf->flags = flags;
	if (IS_stdjpg(ibuf)) ret = save_stdjpeg(name, ibuf);
	else if (IS_jstjpg(ibuf)) ret = save_jstjpeg(name, ibuf);
	else if (IS_maxjpg(ibuf)) ret = save_maxjpeg(name, ibuf);
	else ret = save_vidjpeg(name, ibuf);
	return(ret);
}


main___(int argc, char **argv)
{
	struct ImBuf * ibuf;

	if (argc < 3) {
		printf("Need input, and output\n");
		exit(1);
	}
	
	jpeg_failed = FALSE;
	if (strcmp(argv[0], "tojpeg") == 0) {
		ibuf = loadiffname(argv[1], IB_rect);
		if (ibuf==0) {
			printf("Can't load image\n");
			exit(1);
		}
	
		ibuf->ftype = JPG_MAX | 75;
		save_maxjpeg(argv[2], ibuf);
	} else {
		ibuf = loadjpeg(argv[1], IB_rect);
		if (ibuf==0) {
			printf("Can't load image\n");
			exit(1);
		}
		ibuf->ftype = TGA;
		saveiff(ibuf, argv[2], IB_rect);
	}

	if (jpeg_failed) {
		perror("jpeg failed");
	}
}

