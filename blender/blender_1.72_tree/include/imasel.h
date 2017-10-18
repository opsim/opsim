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

/* imasel.h    ma 97

 * 
 * 
 */

#ifndef IMASEL_H
#define IMASEL_H


/* headerbuttons: 450-499 */

#define B_IMASELHOME		451
#define B_IMASELREMOVEBIP	452

#define C_BACK  0xBAAAAA
#define C_DARK  0x665656
#define C_DERK  0x766666
#define C_HI	0xCBBBBB
#define C_LO	0x544444

/* queue settings */
#define IMS_KNOW_WIN        1
#define IMS_KNOW_BIP        2
#define IMS_KNOW_DIR        4
#define IMS_DOTHE_INF		8
#define IMS_KNOW_INF	   16
#define IMS_DOTHE_IMA	   32
#define IMS_KNOW_IMA	   64
#define IMS_FOUND_BIP	  128
#define IMS_DOTHE_BIP	  256
#define IMS_WRITE_NO_BIP  512

/* imasel->mode */
#define IMS_NOIMA			0
#define IMS_IMA				1
#define IMS_ANIM			2
#define IMS_DIR				4
#define IMS_FILE			8
#define IMS_STRINGCODE		16

#define IMS_INDIR			1
#define IMS_INDIRSLI		2
#define IMS_INFILE			3
#define IMS_INFILESLI		4

#
#
typedef struct OneSelectableIma {
	int   header;						
	int   ibuf_type;
	struct ImBuf *pict;					
	struct OneSelectableIma *next;		
	struct OneSelectableIma *prev;		
	
	short  cmap, image, draw_me, rt;
	short  sx, sy, ex, ey, dw, dh;				
	short  selectable, selected;		
	int   mtime, disksize;				
	char   file_name[64];
	
	short  orgx, orgy, orgd, anim;		/* same as ibuf->x...*/
	char   dummy[4];					/* 128 */

	char   pict_rect[3968];				/* 4096   (RECT = 64 * 62) */
	
} OneSelectableIma;

#
#
typedef struct ImaDir {
	struct ImaDir *next, *prev;
	int  selected, hilite; 
	int  type,  size;
	int mtime;
	char name[100];
} ImaDir;



typedef struct SpaceImaSel {
	struct SpaceImaSel *next, *prev;	/* verplicht */
	int    spacetype;				    /* verplicht */
	
	char   title[24];
	
	int   fase; 
	short  mode, subfase;
	short  mouse_move_redraw, imafase;
	short  mx, my;
	
	short  dirsli, dirsli_lines;
	short  dirsli_sx, dirsli_ey , dirsli_ex, dirsli_h;
	short  imasli, fileselmenuitem;
	short  imasli_sx, imasli_ey , imasli_ex, imasli_h;
	
	short  dssx, dssy, dsex, dsey; 
	short  desx, desy, deex, deey; 
	short  fssx, fssy, fsex, fsey; 
	short  dsdh, fsdh; 
	short  fesx, fesy, feex, feey; 
	short  infsx, infsy, infex, infey; 
	short  dnsx, dnsy, dnw, dnh;
	short  fnsx, fnsy, fnw, fnh;

	
	char   fole[128], dor[128];
	char   file[128], dir[128];
	ImaDir *firstdir, *firstfile;
	int    topdir,  totaldirs,  hilite; 
	int    topfile, totalfiles;
	
	float  image_slider;
	float  slider_height;
	float  slider_space;
	short  topima,  totalima;
	short  curimax, curimay;
	OneSelectableIma *first_sel_ima;
	OneSelectableIma *hilite_ima;
	short  total_selected, ima_redraw;
	int pad;
	
	ImBuf  *cmap;
	
	void (*returnfunc)();
	
} SpaceImaSel;

	/* drawimsel.c */
extern void calc_hilite(SpaceImaSel *simasel);
extern void ima_select_all(SpaceImaSel *simasel);
extern void move_imadir_sli(SpaceImaSel *simasel);
extern void move_imafile_sli(SpaceImaSel *simasel);
extern void pibplay(SpaceImaSel *simasel);
extern void select_ima_files(SpaceImaSel *simasel);

	/* imasel.c */
extern void write_new_pib(SpaceImaSel *simasel);
extern void free_ima_dir(ImaDir *firstdir);
extern check_for_pib(SpaceImaSel *simasel);
extern void clear_ima_dir(SpaceImaSel *simasel);
extern void check_ima_dir_name(char *dir);
extern int get_ima_dir(char *dirname, int dtype, int *td, ImaDir **first);
extern void get_next_image(SpaceImaSel *simasel);
extern void get_file_info(SpaceImaSel *simasel);
extern void get_pib_file(SpaceImaSel *simasel);
extern void change_imadir(SpaceImaSel *simasel);
extern void init_imaselspace(ScrArea *sa);
extern void check_imasel_copy(SpaceImaSel *simasel);
extern void free_imasel(SpaceImaSel *simasel);



#endif /* IMASEL_H */

