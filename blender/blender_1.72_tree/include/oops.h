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

/* oops.h    feb 97

 * 
 * 
 */

#ifndef OOPS_H
#define OOPS_H

#define OOPSX	5.0
#define OOPSY	1.8

typedef struct Oops {
	struct Oops *next, *prev;
	short type, flag, dt, hide;
	float x, y;		/* linksonder */
	float dx, dy;	/* shuffle */
	ID *id;
	ListBase link;
} Oops;

#
#
typedef struct OopsLink {
	struct OopsLink *next, *prev;
	short type, flag;
	ID **idfrom;
	Oops *to, *from;	/* from is voor temp */
	float xof, yof;
	char name[12];
} OopsLink;


/* ********** */

/* oops->flag  (1==SELECT) */
#define OOPS_DOSELECT	2
#define OOPS_REFER		4

	/* drawoops.c */
extern void give_oopslink_line(Oops *oops, OopsLink *ol, float *v1, float *v2);

/* ******* oops.c */
extern        void add_curve_oopslinks(Curve *cu, Oops *oops, short flag);
extern        void add_from_link(Oops *from, Oops *oops);
extern        void add_material_oopslinks(Material *ma, Oops *oops, short flag);
extern        void add_mball_oopslinks(MetaBall *mb, Oops *oops, short flag);
extern        void add_mesh_oopslinks(Mesh *me, Oops *oops, short flag);
extern        void add_object_oopslinks(Object *ob, Oops *oops, short flag);
extern        Oops *add_oops(void *id);
extern        OopsLink *add_oopslink(char *name, Oops *oops, short type, void *from, float xof, float yof);
extern        Oops *add_test_oops(void *id);	/* incl links */
extern        void add_texture_oops(Material *ma);
extern        void build_oops();
extern        int correct_oops_y(Oops *oops);
extern        Oops *find_oops(ID *id);
extern        void free_oops(Oops *oops);	/* ook oops zelf */
extern        void free_oopspace(SpaceOops *so);
extern        void new_oops_location(Oops *new);
extern        int oops_test_overlap(Oops *test);
extern        int oops_test_overlaphide(Oops *test);
extern        float oopslink_totlen(Oops *oops);
extern        void shrink_oops();
extern        void shuffle_oops();
extern        int test_oops(Oops *oops);
extern        void test_oopslink(OopsLink *ol);
extern        void test_oopslinko(OopsLink *ol);


#endif /* OOPS_H */

