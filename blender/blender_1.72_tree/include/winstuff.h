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

#include <windows.h>


#undef near
#undef far
#undef rad
#undef rad1
#undef rad2
#undef rad3
#undef vec
#undef rect
#undef rct1
#undef rct2

#define near bnearsub
#define far bfarsub

#undef small

#ifndef __WINSTUFF_H__
#define __WINSTUFF_H__

#define PROT_READ 0
#define MAP_SHARED 0

#define M_PI 3.1415926535
#define M_PI_2 1.5707963267
#define M_SQRT2 1.414
#define M_SQRT1_2 0.707

#define MAXPATHLEN MAX_PATH

#define S_ISREG(x) ((x&S_IFMT) == S_IFREG)
#define S_ISDIR(x) ((x&S_IFMT) == S_IFDIR)

#define strcasecmp(a, b) strcmp(a, b)
#define strncasecmp(a, b, n) strncmp(a, b, n)

#define RTLD_NOW 0
#define RTLD_LAZY 1

typedef long pid_t;
typedef unsigned int mode_t;

typedef struct _DIR {
	HANDLE handle;
	WIN32_FIND_DATA data;
	char path[MAX_PATH];
	long dd_loc;
	long dd_size;
	char dd_buf[4096];
	void *dd_direct;
} DIR;

struct dirent {
	int d_ino;
	int d_off;
	unsigned short d_reclen;
	char *d_name;
};

struct tms {
	long tms_utime;
	long tms_stime;
	long tms_cutime;
	long tms_cstime;
};

#endif /* __WINSTUFF_H__ */

