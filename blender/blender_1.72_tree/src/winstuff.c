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

#include "blender.h"
#include "winstuff.h"

/*
void strlower (char *str) {
	while (*str) {
		*str= tolower(*str);
		str++;
	}
}

void strlower (char *str, n) {
	while (*str && n>0) {
		*str= tolower(*str);
		str++;
		n--;
	}
}

int strcasecmp (char *s1, char *s2) {
	char *st1, *st2;
	int r;
	
	st1= mallocN(strlen(s1)+1, "temp string");
	strcpy(st1, s1);

	st2= mallocN(strlen(s1)+1, "temp string");
	strcpy(st2, s1);

	strlower(st1);
	strlower(st2);
	r= strcmp (st1, st2);
	
	freeN(st1);
	freeN(st2);

	return r;
}

int strncasecmp (char *s1, char *s2, int n) {
	char *st1, *st2;
	int r;
	
	st1= mallocN(n, "temp string");
	memcpy(st1, s1, n);

	st2= mallocN(n, "temp string");
	memcpy(st2, s2, n);

	strnlower(st1, n);
	strnlower(st2, n);

	r= strcmp (st1, st2);
	
	freeN(st1);
	freeN(st2);

	return r;	
}
*/

void bzero(void *buf, int size) {
	memset (buf, 0, size);
}

void bcopy(void *src, void *dst, int size) {
	memcpy (dst, src, size);
}

void *dlopen (const char *name, int mode) {
	return LoadLibrary(name);
}

void *dlsym (void *handle, const char *symname) {
	return GetProcAddress(handle, symname);
}

int dlclose (void *handle) {
	return FreeLibrary(handle);
}

char *dlerror(void) {
	return NULL;
}

int srandom(unsigned int seed) {
	srand(seed);

	return 0;
}

long random(void) {
	return rand();
}

static int first;

DIR *opendir (const char *path) {
	DIR *newd;

	if (GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY) {
		newd= (DIR *) malloc(sizeof(DIR));

		sprintf (newd->path, "%s/*.*",path);

		first=1;
		
		if (newd->handle == 0) {
			free (newd);
			return NULL;
		} else {
			return newd;
		}
		return NULL;
	} else { return NULL; }
}

struct dirent *readdir(DIR *dp) {
	struct dirent *ret;

	if (first) {
		first=0;

		dp->handle= FindFirstFile (dp->path, &(dp->data));
		ret= (struct dirent *) malloc (sizeof(struct dirent));

		ret->d_name= (char*) malloc (strlen(dp->data.cFileName)+1);
		strcpy (ret->d_name, dp->data.cFileName);

		return ret;
	} else if (FindNextFile (dp->handle, &(dp->data))) {
		ret= (struct dirent *) malloc (sizeof(struct dirent));

		ret->d_name= (char*) malloc (strlen(dp->data.cFileName)+1);
		strcpy (ret->d_name, dp->data.cFileName);

		return ret;
	} else {
		return NULL;
	}

	return NULL;
}

int closedir (DIR *dp) {
	FindClose (dp->handle);

	return 0;
}

int times(struct tms *buf) {
	/* Blender never uses the data in the buf,
		just the return value */

	return GetTickCount();
}

int __errno;

