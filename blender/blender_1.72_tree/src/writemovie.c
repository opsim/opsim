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


/*  writemovie.c        RENDER
 * 
 *  juni 95
 * 
 */


#ifdef __sgi

#include <movie.h>
#include <cdaudio.h>
#include <dmedia/cl.h>
#include <dmedia/cl_cosmo.h>
#include <sys/file.h>			/* flock */

#include "blender.h"	/* NA de movie-includes ivm X11.h patch */
#include "edit.h"
#include "render.h"

extern rectcpy();
extern rectfill();

#define error(str) {perror(str) ; error(str); G.afbreek= 1;}
#define QUIT(str) {error(str); return;}

#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_BOTH (DIR_UP | DIR_DOWN)

#define MAXQUAL R.r.quality
#define MINQUAL 30

#ifdef MIPS1
#define mvDestroyMovie(x) mvClose(x)
#endif

/* globals */

/* index is used in strings.h */
#define index	indexx

static CL_Handle	compr, soft_compr;
static MVid		movie, image; 
static DMparams	*movie_params, *image_params;
static int			compr_params[64];
static int			index, qualindex, qualnow, mv_outx, mv_outy, numfields= 2;
static char		*comp_buf;
static int			sfra, efra, first = TRUE, maxbufsize;
static int			ntsc = FALSE;

#define FIRST_IMAGE "FIRST_IMAGE"
#define BLENDER_FIRST_IMAGE "BLENDER_1ST_IMG"


void report_flock()
{
	static int flock_reported = FALSE;
	
	if (flock_reported) return;
	flock_reported = TRUE;
	
	error("WriteMovie: couldn't flock() moviefile. Ignoring.");
}


void make_movie_name(char *string)
{
	int len;
	char txt[64];

	if (string==0) return;

	strcpy(string, G.scene->r.pic);
	convertstringcode(string);
	len= strlen(string);

	make_existing_file(string);

	if (strcasecmp(string + len - 3, ".mv")) {
		sprintf(txt, "%04d_%04d.mv", sfra, efra);
		strcat(string, txt);
	}
}

int my_Compress(uint * rect, int *bufsize)
{
	int err = 0;
	
	compr_params[qualindex] = qualnow;
	clSetParams(compr, compr_params, index);

	while (clCompress(compr, numfields, rect, bufsize, comp_buf) != numfields) {
		if (compr == soft_compr) QUIT("clCompress (software)");
		
		/* hardware opnieuw initialiseren */
		clCloseCompressor(compr);
		clOpenCompressor(CL_JPEG_COSMO, &compr);

		qualnow--;
		compr_params[qualindex] = qualnow;
		clSetParams(compr, compr_params, index);
		printf("retrying at quality %d\n", qualnow);
		
		err= TRUE;
	}
	
	return (err);
}

void set_sfra_efra()
{
	if (G.background) {
		sfra = G.real_sfra;
		efra = G.real_efra;
	} else {
		sfra = SFRA;
		efra = EFRA;
	}
}

void open_compressor()
{
	int cosmo = FAILURE, bufsize;
	
	/* initialiseren van de compressor */
	
	if (clOpenCompressor(CL_JPEG_SOFTWARE, &soft_compr) != SUCCESS) QUIT("clOpenCompressor");
	
	if (G.scene->r.mode & R_COSMO) {
		cosmo = clOpenCompressor(CL_JPEG_COSMO, &compr);
		if (cosmo != SUCCESS && first) error("warning: using software compression");
		first = FALSE;
	}
	
	if (cosmo != SUCCESS) compr = soft_compr;
	
	index = 0;

	compr_params[index++]= CL_IMAGE_WIDTH;
	compr_params[index++]= mv_outx;

	compr_params[index++]= CL_IMAGE_HEIGHT;
	compr_params[index++]= mv_outy / numfields;
	
	compr_params[index++]= CL_JPEG_QUALITY_FACTOR;
	qualindex = index;
	compr_params[index++]= R.r.quality;

	compr_params[index++]= CL_ORIGINAL_FORMAT;
	compr_params[index++]= CL_RGBX;

	compr_params[index++]= CL_ORIENTATION;
	compr_params[index++]= CL_TOP_DOWN;

	compr_params[index++]= CL_INTERNAL_FORMAT;
	compr_params[index++]= CL_YUV422;

	/* this parameter must be set for non-queueing mode */
	compr_params[index++]= CL_ENABLE_IMAGEINFO;
	compr_params[index++]= 1;

	/* enable stream headers */
	compr_params[index++]= CL_STREAM_HEADERS;
	compr_params[index++]= TRUE;

	clSetParams(compr, compr_params, index);
	if (compr != soft_compr) clSetParams(soft_compr, compr_params, index);
	
	maxbufsize = 2 * clGetParam(compr, CL_COMPRESSED_BUFFER_SIZE);
	comp_buf = mallocN(maxbufsize, "cosmo_buffer");
}

void close_compressor()
{
	freeN(comp_buf);
	comp_buf = 0;

	clCloseCompressor(compr);
	if (soft_compr != compr) clCloseCompressor(soft_compr);
}

void end_movie()
{
}

int empty_image()
{
	int size;
	ImBuf * ibuf;
	
	ibuf = allocImBuf(mv_outx, mv_outy, 32, IB_rect, 0);
	rectoptot(ibuf, 0, rectfill, 0xc0c0);
	my_Compress(ibuf->rect, &size);
	
	freeImBuf(ibuf);
	
	return(size);
}

void new_movie(int fd)
{
	int		cur_image, bufsize;
	char	string[120];

	if (dmParamsCreate(&movie_params) != DM_SUCCESS) QUIT("dmParamsCreate");
	if (dmParamsCreate(&image_params) != DM_SUCCESS) QUIT("dmParamsCreate");
		
		if (mvSetMovieDefaults(movie_params, MV_FORMAT_SGI_3) != DM_SUCCESS) QUIT("mvSetMovieDefaults");
		if (dmSetImageDefaults(image_params, mv_outx, mv_outy, DM_PACKING_RGBX) != DM_SUCCESS) QUIT("dmSetImageDefaults");
			
		mvAddUserParam(BLENDER_FIRST_IMAGE);
		sprintf(string, "%04d", sfra);
		dmParamsSetString(image_params, BLENDER_FIRST_IMAGE, string);
	
		if (ntsc) dmParamsSetFloat(image_params, DM_IMAGE_RATE, 29.97);
		else dmParamsSetFloat(image_params, DM_IMAGE_RATE, 25.0);
		
		if (numfields == 2) {
			if (ntsc) dmParamsSetEnum(image_params, DM_IMAGE_INTERLACING, DM_IMAGE_INTERLACED_ODD);
			else dmParamsSetEnum(image_params, DM_IMAGE_INTERLACING, DM_IMAGE_INTERLACED_EVEN);
		} else dmParamsSetEnum(image_params, DM_IMAGE_INTERLACING, DM_IMAGE_NONINTERLACED);
	
		dmParamsSetEnum(image_params, DM_IMAGE_ORIENTATION, DM_TOP_TO_BOTTOM);
		dmParamsSetString(image_params, DM_IMAGE_COMPRESSION, DM_IMAGE_JPEG);
	
		if (mvCreateFD(fd, movie_params, NULL, &movie) != DM_SUCCESS) QUIT("mvCreateFile");
		if (mvAddTrack(movie, DM_IMAGE, image_params, NULL, &image)) QUIT("mvAddTrack");;
		if (mvSetLoopMode(movie, MV_LOOP_CONTINUOUSLY) != DM_SUCCESS) QUIT("mvSetMovieDefaults");
						
		if (mvWrite(movie) != DM_SUCCESS) QUIT("mvWrite");
		if (mvClose(movie) != DM_SUCCESS) QUIT("mvClose");
	
	dmParamsDestroy(image_params);
	dmParamsDestroy(movie_params);
}


void extend_movie(int start, int end, int curlen)
{
	int		cur_image, bufsize, i;
	char * temp_buf;
	
	
	if (start <= 0 && end <= 0) return;
	
	temp_buf = comp_buf;
	comp_buf = mallocN(maxbufsize, "extend_movie");
	bufsize = empty_image();
	
	if (start > 0) {		
		for (i = 0; i < start; i++) {
			if (mvInsertCompressedImage(image, 0, bufsize, comp_buf) != DM_SUCCESS) {
				mvDestroyMovie(movie);
				error("mvInsertCompressedImage");
				G.afbreek = 1;
				break;
			}
		}
		printf("movie: added %d frames at start\n", start);
	}
	
	if (end > 0) {
		for (i = 0; i < end; i++) {
			if (mvInsertCompressedImage(image, curlen + i, bufsize, comp_buf) != DM_SUCCESS) {
				mvDestroyMovie(movie);
				error("mvInsertCompressedImage");
				G.afbreek = 1;
				break;
			}
		}
		printf("movie: added %d frames at end\n", end);
	}
	
	freeN(comp_buf);
	comp_buf = temp_buf;
}

void start_movie()
{
	char	name[FILE_MAXDIR+FILE_MAXFILE];
	char	bak[sizeof(name) + 4];
	int		fd, first_image = -1, num_images;
	const char	* string;
	
	first = TRUE;
	
	set_sfra_efra();
	
	/* naam bedenken voor de movie */
	make_movie_name(name);
	
	ntsc = FALSE;
	
	switch (R.recty) {
		case 480: case 360: case 240: case 120:
			ntsc = TRUE;
	}
	
	if (ntsc) {
		switch (R.rectx) {
		case 360: case 320: case 720: case 640:
			mv_outx = R.rectx;
			break;
		default:
			if (R.rectx <= 320) mv_outx = 320;
			else if (R.rectx <= 640) mv_outx = 640;
			else mv_outx = 720;
		}
	} else {
		switch (R.rectx) {
		case 360: case 384: case 720: case 768:
			mv_outx = R.rectx;
			break;
		default:
			if (R.rectx < 384) mv_outx = 384;
			else mv_outx = 768;
		}
	}
	
	if (ntsc) {
		if (R.recty <= 240) {
			mv_outy = 240;
			numfields = 1;
		} else {
			mv_outy = 480;
			numfields = 2;
		}
	} else {
		if (R.recty <= 288) {
			mv_outy = 288;
			numfields = 1;
		} else {
			mv_outy = 576;
			numfields = 2;
		}
	}
	
	if(R.r.mode & R_MOVIECROP) {
		if (ntsc) {
			if (R.rectx > 640) mv_outx = 720;
			else mv_outx = 640;
			mv_outy = 480;
			numfields = 2;
		} else {
			if (R.rectx > 720) mv_outx = 768;
			else mv_outx = 720;
			mv_outy = 576;
			numfields = 2;
		}
	}
	
	qualnow = R.r.quality;

	
	fd = open(name, O_BINARY|O_RDWR);
	if (fd != -1) {
		if (flock(fd, LOCK_EX) == -1) report_flock();
		
			if (mvOpenFD(fd, &movie) == DM_SUCCESS) {
				if (mvFindTrackByMedium(movie, DM_IMAGE, &image) == DM_SUCCESS) {
					if (mvGetImageWidth(image) == mv_outx) {
						if (mvGetImageHeight(image) == mv_outy) {
							mvClose(movie);
							close(fd);
							return;
						}
					}
				}
				strcpy(bak, name);
				strcat(bak, ".bak");
				fop_rename(name, bak);
				mvClose(movie);
			}
		
		close(fd);
	}
	fd = open(name, O_BINARY|O_RDWR | O_CREAT | O_EXCL, 0664);
	if (fd != -1) {
		if (flock(fd, LOCK_EX) == -1) report_flock();
			new_movie(fd);
			printf("Created movie: %s\n", name);
		close(fd);
	}
}

void append_movie(int cfra)
{
	ImBuf		*ibuf, *tbuf;
	int			err, ofsx, ofsy, bufsize, rate, lastqual, qualstep, direction, first_image, num_images;
	char		name[FILE_MAXDIR+FILE_MAXFILE];
	char		new_string[FILE_MAXDIR+FILE_MAXFILE];
	const char	*string, *first_image_string;
	int			fd;
	
	set_sfra_efra();
	make_movie_name(name);
	open_compressor();
	
	rate = 1024 * R.r.maximsize;
	
	/* veranderd: kopie van rectot maken */
	ibuf= allocImBuf(R.rectx, R.recty, 32, IB_rect, 0);
	memcpy(ibuf->rect, R.rectot, 4*R.rectx*R.recty);
	
	if (ibuf->x != mv_outx || ibuf->y != mv_outy) {
		tbuf = allocImBuf(mv_outx, mv_outy, 32, IB_rect, 0);
		rectoptot(tbuf, 0, rectfill, 0x00);
		
		ofsx = (tbuf->x - ibuf->x) / 2;
		ofsy = (tbuf->y - ibuf->y) / 2;
		if (numfields == 2) ofsy &= ~1;
		
		rectop(tbuf, ibuf, ofsx, ofsy, 0, 0, 32767, 32767, rectcpy);
		freeImBuf(ibuf);
		strcpy(tbuf->name, ibuf->name);
		ibuf = tbuf;
	}
	convert_rgba_to_abgr(ibuf->x*ibuf->y, ibuf->rect);
	
	if (numfields == 2) {
		if (ntsc) {
			rectop(ibuf, ibuf, 0, 0, 0, 1, 32767, 32767, rectcpy);
			flipy(ibuf);
			de_interlace(ibuf);
			if (ntsc) rectop(ibuf, ibuf, 0, 0, 0, 1, 32767, 32767, rectcpy);
		} else {
			flipy(ibuf);
			rectop(ibuf, ibuf, 0, 0, 0, 1, 32767, 32767, rectcpy);
			de_interlace(ibuf);
		}
	}
	else {
		/* kleine movies anders op de kop */
		flipy(ibuf);
	}
	
	if (rate == 0) {
		qualnow = R.r.quality;
		my_Compress(ibuf->rect, &bufsize);
	} else {
		qualstep = 4;
		direction = 0;
		
		do {
			if (qualnow > MAXQUAL) qualnow = MAXQUAL;
			if (qualnow < MINQUAL) qualnow = MINQUAL;

			compr_params[qualindex] = qualnow;
			clSetParams(compr, compr_params, index);

			lastqual = qualnow;
			err = my_Compress(ibuf->rect, &bufsize);
			
			printf(" tried quality: %d, size %d\n", qualnow, bufsize);
			
			if (bufsize < 0.9 * rate) {
				if (err) {
					/* forget about this frame, retry next frame at old quality settting */
					qualnow = lastqual;
					break;
				}
				if (qualnow == MAXQUAL) break;
				direction |= DIR_UP;
				if (direction == DIR_BOTH) qualstep /= 2;
				qualnow += qualstep;
			} else if (bufsize > 1.1 * rate) {
				if (qualnow == MINQUAL) break;
				direction |= DIR_DOWN;
				if (direction == DIR_BOTH) qualstep /= 2;
				qualnow -= qualstep;
			} else break;
									
			if (qualstep == 0) {
				/* this was the last iteration. Make sure that the buffer isn't to big */
				if (bufsize < 1.1 * rate) break;
				else qualnow--;
			}
		} while (1);
		
		printf("used quality: %d\n", qualnow);
		
		if (bufsize < rate) qualnow++;
		else qualnow--;
		
	}
	
	fd = open(name, O_BINARY|O_RDWR);

	if (fd != -1) {
		if (flock(fd, LOCK_EX) == -1) report_flock();
			if (mvOpenFD(fd, &movie) == DM_SUCCESS){
				if (mvFindTrackByMedium(movie, DM_IMAGE, &image) == DM_SUCCESS) {
					image_params = mvGetParams(image);
					
					first_image = 1;
					first_image_string = 0;
					
					string = dmParamsGetString(image_params, FIRST_IMAGE);
					if (string) {
						first_image = atoi(string);
						first_image_string = FIRST_IMAGE;
					}
					string = dmParamsGetString(image_params, BLENDER_FIRST_IMAGE);
					if (string) {
						first_image = atoi(string);
						first_image_string = BLENDER_FIRST_IMAGE;
					}
					
					num_images = mvGetTrackLength(image);
					
					if (cfra >= first_image && cfra <= (first_image + num_images - 1)) {
						if (mvDeleteFrames(image, cfra - first_image, 1) != DM_SUCCESS) {
							mvDestroyMovie(movie);
							error("mvDeleteFrames");
							G.afbreek = 1;
						}
					} else {
						/* Zr - This seems to be some patch from frank to make sure
						 * the frame was actually written... Problem was that SkyWriter
						 * reported wierd effects at his machine with 6.2 and not 5.3
						 * if this code is in. Perhaps there is some buffering scheme
						 * that is causing the code to fail on the higher-performance OS.
						 * 
						 * Perhaps I have no clue what the code is doing! 
						 */
#if 0					
						if (cfra < first_image) {
							/* een minder extenden dan strikt noodzakelijk is ! Insert voegt er weer een toe */
							extend_movie(first_image - cfra - 1, 0, num_images);
							sprintf(new_string, "%04d", cfra);
							if (first_image_string) dmParamsSetString(image_params, first_image_string, new_string);
							first_image = cfra;
						} else {
							extend_movie(0, cfra - (first_image + num_images), num_images);
						}
#endif
					}
					
					if (G.afbreek != 1) {
						if (mvInsertCompressedImage(image, cfra - first_image, bufsize, comp_buf) == DM_SUCCESS) {
							printf("added frame %3d (frame %3d in movie): length %6d: ", cfra, cfra - first_image + 1, bufsize);
							mvClose(movie);
						} else {
							mvDestroyMovie(movie);
							error("mvInsertCompressedImage");
							G.afbreek = 1;
						}
					}
				} else {
					mvDestroyMovie(movie);
					error("mvFindTrackByMedium");
					G.afbreek = 1;
				}
			}else {
				error("mvOpenFD");
				G.afbreek = 1;
			}
		close(fd);
	} else {
		error("open movie");
		G.afbreek = 1;
	}
	
	freeImBuf(ibuf);
	
	close_compressor();	
}

#endif	/* __sgi */

