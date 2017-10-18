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

#include <string.h>


#ifdef WIN32
#include "winstuff.h"
#else
#include <sys/param.h>
#endif

char *first_slash(char *string) {
	char *ffslash, *fbslash;
	
	ffslash= strchr(string, '/');	
	fbslash= strchr(string, '\\');
	
	if (!ffslash) return fbslash;
	else if (!fbslash) return ffslash;
	
	if ((int)ffslash < (int)fbslash) return ffslash;
	else return fbslash;
}

char *last_slash(char *string) {
	char *lfslash, *lbslash;
	
	lfslash= strrchr(string, '/');	
	lbslash= strrchr(string, '\\');

	if (!lfslash) return lbslash; 
	else if (!lbslash) return lfslash;
	
	if ((int)lfslash < (int)lbslash) return lbslash;
	else return lfslash;
}

#ifdef WIN32

static char str[MAXPATHLEN+12];

int fop_delete(char *file, int dir, int recursive) {
	int err;

	if (recursive) {
		error("Recursive delete is unsupported on Windows");
		err= 1;
	} else if (dir) {
		err= !RemoveDirectory(file);
		if (err) printf ("Unable to remove directory");
	} else {
		err= !DeleteFile(file);
		if (err) error ("Unable to delete file");
	}

	return err;
}

int fop_touch(char *file) {
	error("Touching files is unsupported on Windows");
	
	return 1;
}

int fop_move(char *file, char *to) {
	int err;

	err= !MoveFile(file,to);
	if (err) error("Unable to move file");

	return err;
}

int fop_copy(char *file, char *to) {
	int err;

	err= !CopyFile(file,to,FALSE);
	if (err) error("Unable to copy file!");

	return err;
}

int fop_link(char *file, char *to) {
	error("Linking files is unsupported on Windows");
	
	return 1;
}

int fop_backup(char *file, char *from, char *to) {
	error("Backing up files is unsupported on Windows");
	
	return 1;
}

int fop_exists(char *file) {
	return (GetFileAttributes(file) != 0xFFFFFFFF);
}

void fop_recurdir(char *dirname) {
	char *lslash;
	char tmp[MAXPATHLEN];
	int err;
	
	if (fop_exists(dirname)) return;

	strcpy(tmp, dirname);
		
	lslash= last_slash(tmp);
	if (!lslash) return;
	
	/* Split about the last slash and recurse */	
	*lslash = 0;

	fop_recurdir(tmp);
	if (!CreateDirectory(dirname, NULL))
		error ("Unable to create directory\n");
}

int fop_rename(char *from, char *to) {
	if (!fop_exists(from)) return 0;

	if (fop_exists(to))
		if(fop_delete(to, 0, 0)) return 1;
		
	return rename(from, to);
}

#else /* The sane UNIX world */

static char str[MAXPATHLEN+12];

int fop_delete(char *file, int dir, int recursive) {
	if (recursive) sprintf(str, "/bin/rm -rf %s", file);
	else if (dir) sprintf(str, "/bin/rmdir \"%s\"", file);
	else sprintf(str, "/bin/rm -f \"%s\"", file);

	return docmd(str);
}

int fop_touch(char *file) {
	sprintf(str, "/bin/touch %s", file);

	return docmd(str);
}

int fop_move(char *file, char *to) {
	sprintf(str, "/bin/mv -f %s %s", file, to);

	return docmd(str);
}

int fop_copy(char *file, char *to) {
	sprintf(str, "/bin/cp -rf \"%s\" %s", file, to);

	return docmd(str);
}

int fop_link(char *file, char *to) {
	sprintf(str, "/bin/ln -f %s %s", file, to);
	
	return docmd(str);
}

int fop_backup(char *file, char *from, char *to) {
	sprintf(str, "/bin/su root -c 'cd %s; /bin/tar cf - \"%s\" | (/bin/cd %s; /bin/tar xf -)'", from, file, to);
	
	return docmd(str);
}

int fop_exists(char *file) {
	return exist(file);
}

void fop_recurdir(char *dirname) {
	char *lslash;
	char tmp[MAXPATHLEN];
		
	if (fop_exists(dirname)) return;

	strcpy(tmp, dirname);
		
	lslash= last_slash(tmp);
	if (!lslash) return;
	
	/* Split about the last slash and recurse */	
	*lslash = 0;

	fop_recurdir(tmp);
	mkdir(dirname, 0777);
}

int fop_rename(char *from, char *to) {
	if (!fop_exists(from)) return 0;
	
	if (fop_exists(to))	if(fop_delete(to, 0, 0)) return 1;
		
	return rename(from, to);
}

#endif

