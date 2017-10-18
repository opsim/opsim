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

#include <sys/stat.h>


#ifndef	NULL
#define NULL			0
#endif

#ifndef	FALSE
#define FALSE			0
#endif

#ifndef	TRUE
#define TRUE			1
#endif

#define HDRSIZE 512
#define NAMSIZE 200
#define RECURDIR 1
#define FASTDIR 2

struct header{
	char	name[NAMSIZE];
	uint	size;
	uint	chksum;
	char	fill[HDRSIZE-NAMSIZE-2*sizeof(uint)];
};


struct direntry{
	char	*string;
	mode_t	type;
	char	*relname;
	struct	stat s;
	uint	flags;
	char	size[16];
	char	mode1[4];
	char	mode2[4];
	char	mode3[4];
	char	owner[16];
	char	time[8];
	char	date[16];
	char	extra[16];
	void	*poin;
	int		nr;
};

struct dirlink
{
	struct dirlink *next,*prev;
	char *name;
};

/* ******** storage.c */

extern       void adddirstrings();
extern       void builddir(char *dirname, char *relname);
extern       int compare(struct direntry *entry1, struct direntry *entry2);
extern       short copy(char *cpy, char *org);
extern       double diskfree(char *dir);
extern       int exist(char *name);
extern       int existo(char *name);
extern       void fastdir(int on);
extern       int filesize(int file);
extern       void freedir(void);
extern       uint getdir(char *dirname,  struct direntry **filelist);
extern       char * getwdN(char * dir);
extern       int recurdir(char *name);


