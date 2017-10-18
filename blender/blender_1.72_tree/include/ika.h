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


/* ika.h 
 * 
 * april 96
 * 
 */

#ifndef IKA_H
#define IKA_H


typedef struct Deform {
	Object *ob;
	short flag, partype;
	int par1, par2, par3;	/* kunnen vertexnrs zijn */
	
	float imat[4][4], premat[4][4], postmat[4][4];
	float vec[3];	/* als partype==LIMB, voor distfunc */
	float fac, dist, pad;
	
} Deform;

typedef struct Limb {
	struct Limb *next, *prev;
	
	float len, leno, fac, alpha, alphao, pad;
	float eff[2];
	
} Limb;

typedef struct Ika {
	ID id;
	
	short partype, flag, iter, lastfra;
	
	ListBase limbbase;
	float eff[3], effg[3], effn[3];	/* current, global en (local)wanted */
	float mem, slow, toty, totx, xyconstraint;
	
	Ipo *ipo;
	Object *parent;
	int par1, par2, par3;	/* kunnen vertexnrs zijn */

	int totdef;
	Deform *def;
	
} Ika;


	/* ika.c */
extern Ika *add_ika();
extern Ika *copy_ika(Ika *ika);
extern void free_ika(Ika *ika);
extern void make_local_ika(Ika *ika);
extern int count_limbs(Object *ob);
extern void calc_limb(Limb *li);
extern void calc_ika(Ika *ika, Limb *li);
extern void init_defstate_ika(Object *ob);
extern void itterate_ika(Object *ob);
extern void init_skel_deform(Object *par, Object *ob);
extern void calc_skel_deform(Ika *ika, float *co);


#endif /* IKA_H */

