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


/*	drawscene.c		GRAPHICS
 * 
 *  jan 95
 * 
 *  ook EDIT funkties
 * 
 *  alleen routines met GRAPHICS!
 * 
 */
	
#include "blender.h"
#include "file.h"
#include "graphics.h"



void set_scene(Scene *sce)		/* zie ook scene.c: set_scene_bg() */
{
	bScreen *sc;
	ScrArea *sa;
	View3D *vd;
	Base *base;
	
	G.scene= sce;

	sc= G.main->screen.first;
	while(sc) {
		if((U.flag & SCENEGLOBAL) || sc==G.curscreen) {
		
			if(sce != sc->scene) {
				/* alle area's endlocalview */
				sa= sc->areabase.first;
				while(sa) {
					endlocalview(sa);
					sa= sa->next;
				}		
				sc->scene= sce;
			}
			
		}
		sc= sc->id.next;
	}
	
	copy_view3d_lock(0);	/* space.c */

	/* zijn er camera's in de views die niet in de scene zitten? */
	sc= G.main->screen.first;
	while(sc) {
		if( (U.flag & SCENEGLOBAL) || sc==G.curscreen) {
			sa= sc->areabase.first;
			while(sa) {
				vd= sa->spacedata.first;
				while(vd) {
					if(vd->spacetype==SPACE_VIEW3D) {
			
						if(vd->camera) {
							
							if( object_in_scene(vd->camera, sce)==0) {
								vd->camera= 0;
								if(vd->persp>1) vd->persp= 1;
							}
						}
					}
					vd= vd->next;
				}
				sa= sa->next;
			}
		}
		sc= sc->id.next;
	}

	set_scene_bg(G.scene);	
	
	/* volledige redraw */
	allqueue(REDRAWALL, 0);
	allqueue(REDRAWDATASELECT, 0);	/* doet remake */
}


/* ********************************************************* */

char bdata[129], cdata[129];
int (*cfunc[64])();
extern unsigned char hash[];
char ihash[512];
char warnc[55]= {147, 70, 30, 200, 118, 32, 126, 68, 13, 198, 249, 28, 134, 116, 122, 199, 53, 178, 167, 64, 43, 168, 212, 59, 29, 239, 46, 204, 69, 55, 125, 180, 9, 255, 133, 106, 81, 205, 101, 141, 111, 176, 155, 246, 242, 163, 86, 38, 197, 95, 51, 196, 31, 159, 20};

void hashdecode(int len, char *strout, char *strin, char key1, char key2, char key3, char key4)
{
	int a;
	
	a= len;
	while(a--) {
		if(a & 1) {
			strout[a]= ( ihash[ (ihash[ strin[a] ] -key3) & 255 ]  ) & 255 ;
			strout[a]= ( ihash[ (ihash[ strout[a] ] -key1) & 255 ]  ) & 255 ;
		}
		else {
			strout[a]= ( ihash[ (ihash[ strin[a] ] -key4) & 255 ]  ) & 255 ;
			strout[a]= ( ihash[ (ihash[ strout[a] ] -key2) & 255 ]  ) & 255 ;
		}
		strout[a]= strout[a]-key1+key2-key3+key4-hash[a];
	}

}

#define MGS(x) (((uchar *)(x))[0] << 8 | ((uchar *)(x))[1])

void mark3(char *str2)
{
/*	int val;
	char *a, *b;
	
	a= (str2+20);
	b= (str2+62);
	
	val = MGS(a+1) * MGS(b+4) - MGS(a+4) * MGS(b+2);
*/
}

/* checksum */
int mark5(char *str2)
{
	short a, test=0;

	for(a=0; a<126; a++) test+= str2[a];
	
 	return( test == MGS(str2+126) );
}

int testc()
{
	if( ((ihash[ cdata[2] ]-cdata[27]) & 255)==106 ) return 1;
	return 0;
}

int errorc()
{
	if(testc()) return 1;
	error(warnc);
	return 0;
}

void init_fp()
{
	int a;
		
	cfunc[0]= testc;
	cfunc[1]= errorc;
	
	for(a=2; a<64; a++) {
		cfunc[a]= errorc;
	}
}

void decode_256(char *strout, char *strin)
{
	int a, inval;
	
	for(a=0; a<128; a++) {
		strout[a]= (strin[ ihash[2*a]]-'a') + 16*(strin[ihash[2*a+1]]-'a');
	}
}

void remove_enters(char *name, int len)
{
	int a, b=0;
	
	for(a=0; a<len; a++) {
		if( name[a]==10 || name[a]==13);
		else {
			name[b]= name[a];
			b++;
		}
	}
}

void read_key()
{
	extern void add_radio();
	extern void draw_prop_circle();
	extern void scriptbuts();
	int a, b, file;
	char *home, name[320], temp[320];
	
	init_fp();
	bzero(bdata, 128);
	bzero(cdata, 128);
	home= gethome();
	
	if(home) {
		/* inverse hashtab */
		
		for(a=0; a<256; a++) {
			for(b=0; b<256; b++) {
				if(a==hash[b]) break;
			}
			ihash[a]= b;
		}
		for(a=0;a<256;a++) ihash[256+a]= ihash[a];
		
		make_file_string(name, home, ".BCkey");
		if (!fop_exists(name)) {
			if (G.f&G_DEBUG) printf ("Unable to find key at: %s\n", name);
			make_file_string(name, home, "BCkey");
		}
		
		if (!fop_exists(name)) {
			if (G.f&G_DEBUG) printf ("Unable to find key at: %s\n", name);
			make_file_string(name, home, "BCkey.txt");
		}

		file= open(name, O_BINARY|O_RDONLY);
		if(file>0) {
			if (G.f&G_DEBUG) printf ("Found key at: %s\n", name);
			
			read(file, name, 280);
			remove_enters(name, 280);
			decode_256(temp, name);
			
			if(mark5(temp)==0) {
				printf("invalid C Key\n");
			}
			else {
				memcpy(cdata, temp, 4);
				hashdecode(48, cdata+4, temp+4,  cdata[0], cdata[1], cdata[2], cdata[3]);
				cdata[51]= 0;	/* make a printable string */
				hashdecode(48, cdata+4+48, temp+4+48,  cdata[20]+cdata[24], cdata[21]+cdata[25], cdata[22]+cdata[26], cdata[23]+cdata[27]);
				memcpy(cdata+100, temp+100, 28);
			}
			close(file);
			
			if(TEST_C_KEY==0) {
				printf("invalid C Key\n");
			}
			
			cfunc[37]= (int(*)()) add_radio;
			cfunc[13]= (int(*)()) add_radio;
			cfunc[23]= (int(*)()) write_vrml_fs;
			cfunc[18]= (int(*)()) write_dxf_fs;	
			cfunc[15]= (int(*)()) draw_prop_circle;	
			cfunc[11]= (int(*)()) scriptbuts;	
		}
		else {
			if(G.f & G_DEBUG) printf("can't open %s\n", name);
			for(a=0; a<64; a++) cdata[a+52]= 1;
		}
		
		hashdecode(16, versionstr, versionstr, 12, 96, 86, 5);
		hashdecode(55, warnc, warnc, 12, 96, 86, 5);
		
	}
}

