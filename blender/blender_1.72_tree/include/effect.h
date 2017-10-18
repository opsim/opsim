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



/* effect.h 
 * 
 * dec 95
 * jan feb 96
 * 
 */

#ifndef EFFECT_H
#define EFFECT_H

/* DENK ERAAN: NIEUWE EFFECTEN OOK IN DE WRITEFILE.C IVM DNA!!! */

#define PAF_MAXMULT		4

	/* paf->flag (bitje 0 vrij houden ivm compatibility) */
#define PAF_BSPLINE		2
#define PAF_STATIC		4
#define PAF_FACE		8

	/* eff->type */
#define EFF_BUILD		0
#define EFF_PARTICLE	1
#define EFF_WAVE		2

	/* eff->flag */
#define EFF_SELECT		1
#define EFF_CYCLIC		2

	/* paf->stype */
#define PAF_NORMAL		0
#define PAF_VECT		1

	/* paf->texmap */
#define PAF_TEXINT		0
#define PAF_TEXRGB		1
#define PAF_TEXGRAD		2

	/* wav->flag */
#define WAV_X			2
#define WAV_Y			4
#define WAV_CYCL		8


typedef struct Effect {
	struct Effect *next, *prev;
	short type, flag, buttype, rt;
	
} Effect;

typedef struct BuildEff {
	struct BuildEff *next, *prev;
	short type, flag, buttype, rt;
	
	float len, sfra;
	
} BuildEff;

#
#
typedef struct Particle {
	float co[3], no[3];
	float time, lifetime;
	short mat_nr, rt;
} Particle;


typedef struct PartEff {
	struct PartEff *next, *prev;
	short type, flag, buttype, stype;
	
	float sta, end, lifetime;
	int totpart, totkey, seed;
	
	float normfac, obfac, randfac, texfac, randlife;
	float force[3];
	float damp;
	
	float nabla, vectsize, defvec[3];
	
	float mult[4], life[4];
	short child[4], mat[4];
	short texmap, curmult;
	short staticstep, pad;
	
	Particle *keys;
	
} PartEff;


typedef struct WaveEff {
	struct WaveEff *next, *prev;
	short type, flag, buttype, stype;
	
	float startx, starty, height, width;
	float narrow, speed, minfac, damp;
	
	float timeoffs, lifetime;
	
} WaveEff;

	/* effect.c */
extern Effect *add_effect(int type);
extern PartEff *give_parteff(Object *ob);
extern void where_is_particle(PartEff *paf, Particle *pa, float ctime, float *vec);
extern void free_effect(Effect *eff);
extern void free_effects(ListBase *lb);
extern void copy_effects(ListBase *lbn, ListBase *lb);
extern void build_particle_system(Object *ob);



#endif

