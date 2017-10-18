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

/* sequence.h    juni 95

 * 
 * 
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "plugin.h"

#define WHILE_SEQ(base)	{											\
							int totseq_, seq_; Sequence **seqar;	\
							build_seqar( base,  &seqar, &totseq_);	\
							for(seq_ = 0; seq_ < totseq_; seq_++) {	\
									seq= seqar[seq_];
								

#define END_SEQ					}						\
							if(seqar) freeN(seqar);		\
							}


typedef struct StripElem {
	char name[40];
	struct ImBuf *ibuf;
	struct StripElem *se1, *se2, *se3;
	short ok, nr;
	int pad;
		
} StripElem;

typedef struct Strip {
	struct Strip *next, *prev;
	short rt, len, us, done;
	StripElem *stripdata;
	char dir[80];
	short orx, ory;
	int pad;
		
} Strip;


typedef struct PluginSeq {
	char name[80];
	void *handle;
	
	char *pname;
	
	int vars, version;

	void *varstr;
	float *cfra;
	
	float data[32];

	void (*doit)();

	void (*callback)();
} PluginSeq;


/* LET OP: eerste stuk identiek aan ID (ivm ipo's) */

typedef struct Sequence {

	struct Sequence *next, *prev, *new;
	void *lib;
	char name[24];
	
	short flag, type;
	int len;
	int start, startofs, endofs;
	int startstill, endstill;
	int machine, depth;
	int startdisp, enddisp;
	float mul, handsize;
	int sfra;
	
	Strip *strip;
	StripElem *curelem;
	
	Ipo *ipo;
	Scene *scene;
	struct anim *anim;
	float facf0, facf1;
	
	PluginSeq *plugin;

	/* pointers voor effecten: */
	struct Sequence *seq1, *seq2, *seq3;
	
	/* meta */
	ListBase seqbase;
	
} Sequence;


#
#
typedef struct MetaStack {
	struct MetaStack *next, *prev;
	ListBase *oldbasep;
	Sequence *parseq;
} MetaStack;

typedef struct Editing {
	ListBase *seqbasep;
	ListBase seqbase;
	ListBase metastack;
	short flag, rt;
	int pad;
} Editing;

	/* drawseq.c */
extern void drawseq(Sequence *seq);
extern void set_special_seq_update(int val);

	/* editseq.c */
extern void change_plugin_seq(char *str);
extern Sequence *find_nearest_seq(int *hand);
extern int test_overlap_seq(Sequence *test);
extern void shuffle_seq(Sequence *test);
extern Sequence *sfile_to_sequence(SpaceFile *sfile, int cfra, int machine, int last);
extern void sfile_to_mv_sequence(SpaceFile *sfile, int cfra, int machine);
extern void reload_image_strip(char *name);
extern void load_plugin_seq(char *str);
extern int is_a_sequence(Sequence *test);

	/* sequence.c */
extern    PluginSeq *add_plugin_seq(char *str, char *seqname);
extern    void build_seqar(ListBase *seqbase, Sequence  ***seqar, int *totseq);
extern    void calc_sequence(Sequence *seq);
extern    void clear_scene_in_allseqs(Scene *sce);
extern    void do_add_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    void do_alphaover_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    void do_alphaunder_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    int do_build_seqar(ListBase *seqbase, Sequence ***seqar, int depth);
extern    int do_build_seqar_cfra(ListBase *seqbase, Sequence ***seqar, int cfra);
extern    void do_cross_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    void do_drop_effect(float facf0, float facf1, int x, int y, uint *rect2, uint *rect1, uint *out);
extern    void do_effect(int cfra, Sequence *seq, StripElem *se);
extern    void do_gammacross_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    void do_mul_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    void do_render_seq();
extern    int do_seq_count(ListBase *seqbase, int *totseq);
extern    int do_seq_count_cfra(ListBase *seqbase, int *totseq, int cfra);
extern    void do_sub_effect(float facf0, float facf1, int x, int y, uint *rect1, uint *rect2, uint *out);
extern    int evaluate_seq_frame(int cfra);
extern    void free_editing(Editing *ed);
extern    void free_imbuf_seq_except(int cfra);
extern    void free_plugin_seq(PluginSeq *pis);
extern    void free_sequence(Sequence *seq);
extern    void free_strip(Strip *strip);
extern    void free_stripdata(int len, StripElem *se);
extern    ImBuf *give_ibuf_seq(int cfra);
extern    StripElem *give_stripelem(Sequence *seq, int cfra);
extern    void make_black_ibuf(ImBuf *ibuf);
extern    void multibuf(ImBuf *ibuf, float fmul);
extern    void new_stripdata(Sequence *seq);
extern    void open_plugin_seq(PluginSeq *pis, char *seqname);
extern    void set_meta_stripdata(Sequence *seqm);
extern    void sort_seq();






#endif /* SEQUENCE_H */

