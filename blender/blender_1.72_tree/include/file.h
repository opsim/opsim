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

/* file.h juli 94 */


/*
 * 
 * 
 * 
 */

#ifndef FILE_H
#define FILE_H

/* flags voor wegschrijven/lezen scene */

#include <fcntl.h>
#include "blender.h"
#include "storage.h"

#ifndef WIN32
#ifdef __FreeBSD__
  #include <sys/param.h>
  #include <sys/stat.h>
  #include <sys/mount.h>
#else   
  #if defined(linux)
    #include <sys/vfs.h>
  #else
	#ifndef __BeOS
	    #include <sys/statfs.h>  
	#endif
  #endif
#endif
#endif

/* INTEGER CODES */

#define FORM MAKE_ID('F','O','R','M')
#define DDG1 MAKE_ID('3','D','G','1')
#define DDG2 MAKE_ID('3','D','G','2')
#define DDG3 MAKE_ID('3','D','G','3')
#define DDG4 MAKE_ID('3','D','G','4')

#define GOUR MAKE_ID('G','O','U','R')

#define BLEN MAKE_ID('B','L','E','N')
#define DER_ MAKE_ID('D','E','R','_')
#define V100 MAKE_ID('V','1','0','0')

#define DATA MAKE_ID('D','A','T','A')
#define GLOB MAKE_ID('G','L','O','B')
#define IMAG MAKE_ID('I','M','A','G')

#define DNA1 MAKE_ID('D','N','A','1')
#define TEST MAKE_ID('T','E','S','T')
#define REND MAKE_ID('R','E','N','D')
#define USER MAKE_ID('U','S','E','R')

#define ENDB MAKE_ID('E','N','D','B')

#
#
struct SDNA {
	char *data;
	int datalen, nr_names;
	char **names;
	int nr_types, pointerlen;
	char **types;
	short *typelens;
	int nr_structs;
	short **structs;
};

#
#
typedef struct BHead {
	int code, len;
	void *old;
	int SDNAnr, nr;
} BHead;
#
#
typedef struct BHead4 {
	int code, len;
	int old;
	int SDNAnr, nr;
} BHead4;
#
#
typedef struct BHead8 {
	int code, len;
#ifdef WIN32
	DWORD old;
#else
	long long old;
#endif	
	int SDNAnr, nr;
} BHead8;


/* structs alleen voor files: (moeten WEL in SDNA) */

typedef struct FileGlobal {

	void *curscreen;
	short displaymode, winpos;
	int pad;
	char psx[80];
	
} FileGlobal;

	/* genfile.c */
extern void make_structDNA(void);
extern short *findstruct_name(struct SDNA *sdna, char *str);
extern int findstruct_nr(struct SDNA *sdna, char *str);
extern int struct_compare(char *stype, void *one, void *two);
extern struct SDNA cur_sdna, old_sdna;	
extern struct chartrans *text_to_curve(Object *ob, int mode) ;
extern void freestructDNA(struct SDNA *sdna);
extern void freestructDNA_all();
extern void init_structDNA(struct SDNA *sdna);
extern void reconstruct(int oldSDNAnr, int blocks, void **data);
extern void switch_endian_struct(int oldSDNAnr, char *data);

	/* readfile.c */
extern	  char* gethome(void);
extern    void add_data_adr(void *old, void *new);
extern    void add_lib_adr(void *old, void *new);
extern    void append_id_part(char *filedata, Main *main, ID *id);
extern    void append_named_part(SpaceFile *sfile, Main *main, char *name, int idcode);
extern    void change_libadr(void *old, void *new);
extern    int convertstringcode(char *str);
extern    void do_versions(Main *main);
extern    BHead *find_bhead(void *old, char *filedata);
extern    void give_base_to_objects(Scene *sce, ListBase *lb);
extern    void inst_file(char *filename, char *data, int size);
extern    ID *is_yet_read(Main *main, BHead *bhead);
extern    void library_append(SpaceFile *sfile);	/* append aan G
extern    void link_global(FileGlobal *fg);
extern    void link_list(ListBase *lb);		/* alleen direkte data */
extern    void link_recurs_seq(ListBase *lb);
extern    void makestringcode(char *str);
extern    void *mallocNN(int len, char *str);
extern    void *newadr(void *adr);		/* alleen direkte datablokken */
extern    void *newlibadr(void *lib, void *adr);		/* alleen Lib datablokken */
extern    void *newlibadr_us(void *lib, void *adr);		/* hoogt usernummer op */
extern    void *newlibadr_us_type(short type, void *adr);		/* alleen Lib datablokken */
extern    void newstructname(BHead *bh);
extern    char *openblenderfile(char *name, int *filelen);
extern    void read_autosavefile(void);
extern    void read_file(char *dir);
extern    int read_file_dna(char *filedata, int filelen);
extern    int read_homefile(void);
extern    int read_libblock(Main *main, BHead *bhead, int flag);
extern    void read_libraries(void);
extern    void *read_libstruct(BHead *bh);
extern    void read_struct(BHead *bh);

extern    void splitdirstring(char *di,char *fi);
extern    void switch_endian_bheads(char *filedata, int filelen);
extern    void switch_endian_structs(BHead *bhead);
extern    int testextensie(char *str, char *ext);
extern    void vcol_to_fcol(Mesh *me);

/* ******* writefile.c */
extern   void add_cam_info(short mode);	/* alleen current camera */
extern   void add_network_info(short mode);		/* alleen met CFRA */
extern   int add_scal_info(Object *ob, short mode);			/* return OK */
extern   void delete_autosave(void);
extern   void do_history(char *name);
extern   void bgnwrite(int file);
extern   void endwrite(void);
extern   void flushwrite(void);
extern   void initrender_infofiles(void);
extern   void make_mdec_movies(void);
extern   void mdec_to_pupstring(char **str);
extern   void readBlog(void);
extern   void remove_mdec_chunks(int id);
extern   void test_Mdec_movie(char *name);
extern   void writeBlog(void);
extern   void write_file(char *dir);
extern   int write_homefile(void);
extern   void writedata(int filecode, int len, void *adr);	/* geen struct */
extern   void writestruct(int filecode, char *structname, int nr, void *adr);


#endif /* FILE_H */

