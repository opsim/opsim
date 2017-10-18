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

/* sector.h    maart 96

 * 
 * 
 */

#ifndef SECTOR_H
#define SECTOR_H

typedef struct Event {
	short event, var, sfac, sfac1;	/* shortversies van fac en fac1 */
	short shiftval;
	char flag, pulse;
	short varnr, rt;
	float fac, fac1;
	void *poin;
	char name[16];
} Event;

typedef struct Action {
	short action, go, cur, var, flag, varnr;	/* varnr voor actvarar */
	short sta, end, butsta, butend;
	float force, fac, min, max;
	int pad;
	
	void *poin;
	char name[16];
} Action;

typedef struct Sensor {
	
	Event *events;
	Action *actions;

	short totaction, totevent;
	short event, evento;		/* voor 'hold' test */
	short flag, rt;
	
	/* hieronder is oud: laten staan tot '98 voor compatibility */
	/* daarna versions intelligent oplossen (met nepstruct?) */
	short event1, event2;
	int action;
	float force, sta, end, fac1, fac2, cur, dx;
} Sensor;

typedef struct NetLink {
	struct NetLink *next, *prev;
	Object *ob;
	short type, flag;
	short sfra, len;		/* sfra: offset in 'movie' */
	/* ipo-verwijzing? */
	
} NetLink;

#
#
typedef struct CamPos {
	Object *ob;
	float piramat[4][4];
	short axis[2];
	short visi, direction, dist, d_axis;	/* LOGICAL VALUES */
	short view, dura, flag, hold;	/* hold: minimum tijd, dura=teller */

	NetLink *nl;				/* geeft aan dat-e afspeelt */

	float lens, loc[3], rot[3];	/* oorspronkelijke */
	float sta, end;				/* net clip */
	float clipsta, clipend;		/* oorspronkelijke */
	
	short cfie, sfra;			/* cfie: tijdens nl afspelen van 0->tot, sfra: offset in 'movie' */
} CamPos;

#
#
typedef struct CamFrame {
	float rot[3], loc[3];
	float lens;
	float clipsta, clipend;
} CamFrame;

typedef struct LBuf {
	short tot, max;
	int pad;
	Object **ob;
} LBuf;

typedef struct Sector {
	ID id;
	
	Object *ob;
	short type, flag, totport, actport;
	
	struct Portal *portals;
	LBuf lbuf;
	CamPos *campos;
	CamFrame *camframe;
	
	ListBase ipo;
	
	Mesh *dynamesh;
	Mesh *texmesh;
	
	short totcam, cfra, totfra, sfra, depth, rt;	/* depth: van recursieve visible test */

		/* bbsize= boundbox size in wereld coords */
	float size[3], bbsize[3];
	float r, g, b;

} Sector;


typedef struct Portal {

	Sector *sector;
	Object *ob;
	
	short type, flag;
	int pad;
	float ofs[2], max[2];
	
} Portal;

/* TFace is een aanvulling op MFace */

typedef struct TFace {
	short uv[4][2];		/* als je dit wijzigt: ook fie set_correct_uv editmesh.c, ook andere plekken maken gebruik van de lengte van dit blok */
	unsigned int col[4];
	short no[3];
	char flag, transp;
	short mode, tile;
	
	int pad;
	
	void *tpage, *clut;
	
} TFace;

#
#
typedef struct DFace {
		/* alleen gedurende simulatie */
	float *v1, *v2, *v3, *v4;
	float dist;
	Material *ma;
	float no[3];
	short proj;
	char flag, edcode;
	ushort ocx, ocy, ocz, rt;
	
} DFace;

typedef struct Life {
	ID id;

	Sector *sector;
	
	float oldloc[3], loc[3], speed[3];		/* loc ook in object zetten */
	float oldloc1[3], loc1[3], speed1[3];	/* lokale sector co's */
	float startloc[3], startrot[3];
	float rot[3], rotspeed[3];
	float oldimat[4][4];
	float mass, frict, rotfrict, axsize, frictfac;
	float r, g, b;
	float aero, padf;
	
	Sensor *sensors;
	Material *contact;
	Object *collision, *from;
	DFace *floor;
	Mesh *dynamesh;
	Mesh *texmesh, *oldmesh;

	short totsens, actsens;
	char type, lay;
	short flag;
	
	short timer, sfra, cfra, dflag;		/* voor init/afhandeling ipoos, dflag: zit niet aan buttons */
	short state[4];						/* lokale variables */

	float colloc[3];					/* collision loc */
	float floorloc[3];
	
	LBuf links;							/* tijdens simul: de kinderen, in volgorde */
	
	ListBase ipo;
	
} Life;


#define TOLER 0.0000076

	/* drawsector.c */
extern int do_realtimelight(Object *ob, TFace *tface, float *col);
extern void draw_tface_mesh(Object *ob, Mesh *me, int dt);
extern void drawsector(Object *ob, int dt, uint col);
extern void get_co_portal(Sector *se, int type, float *ofs, float *cent);
extern void make_reprect(Image *ima);

	/* editsector.c */
extern void default_tface(TFace *tface);
extern void portal_detect(Object *ob);
extern void default_uv(TFace *tface, int size);

	/* initsector.c */
extern Sector *add_sector();
extern Sector *copy_sector(Sector *se);
extern void make_local_sector(Sector *se);
extern void add_array_element(void **base, short *act, short *tot, int size);
extern void delete_array_element(void **base, short *act, short *tot, int size);
extern void add_portal(Sector *se);
extern void delete_portal(Sector *se);
extern void testportals(Sector *se);
extern void init_dynamesh(Object *ob, Mesh *me);
extern void end_dynamesh(Mesh *me);

	/* life.c */
extern        void actionnames_to_pupstring(char **str);
extern        void actvars_to_vars(short *vars);	/* voor writeblendpsx */
extern        void add_action_debug(Action *ac, int timer);
extern        void add_action_var(Object *ob, Action *ac);
extern        Life *add_life();
extern        void add_sensor(Life *lf);
extern        void aerodynamics(Object *ob);
extern        void camera_behaviour(Object *cam, Life *lfcam, Action *ac);
extern        void collision_sensor_input(Object *ob, Life *lf);
extern        void compatible_eulFast(float *eul, float *oldrot);
extern        void convert_ipo(Object *ob);
extern        void copy_action_var(Object *ob, Action *actest);
extern        Life *copy_life(Life *lf);
extern        void delete_sensor(Life *lf);
extern        void do_force_obipo(Object *ob, short cur, float *force, float *omega);
extern        void do_obipo(Object *ob, short cur, short delta, float *speed);	/* delta is ook flag! */
extern        void draw_gamedebug_info();
extern        int dyna_near_life(Object *prob, float mindist, Object *actor);
extern        void end_lifes(int restore);
extern        void eventnames_to_pupstring(char **str);
extern        void free_life(Life *lf);
extern        void init_devs();
extern        void init_lifes();		/* bij start simulation */
extern        void life_from_inv_sector_co(Life *lf);
extern        void life_to_local_sector_co(Life *lf);
extern        void make_local_life(Life *lf);

extern        short pad_read();
extern        int sensor_input(Object *ob, Life *lf, float *forcetot, float *omegatot);
extern        int set_k2k_interval(short mode, Action *ac, Life *lf);
extern        void test_action_var(Object *ob, Action *actest);
extern        void track_life_to_life(Life *this, Life *to, float fac, short mode);
extern        void unlink_life(Life *lf);

	/* sector.c */
extern ushort simuldevs[32];
extern short simulvals[32];

extern      void ApplyMatrix(void *mat, float *old, float *new);
extern      int Normalises(short *vec);	/* 12 bits */
extern      int SquareRoot0(int num);
extern      void add_dupli_life(Object *ob, Object *from, int time);
extern      void add_dyna_life(Object *ob);		/* uitzondering: camera zit er ook in */
extern      void add_to_lbuf(LBuf *lbuf, Object *ob);
extern      void build_sectorlist(Object *cam);
extern      int cliptest_sector(float *vec, float *size, float *mat, short round);
extern      void collision_detect(Object *ob, Life *lf);

extern      void del_dupli_life(Object *ob);
extern      void del_dupli_lifes();
extern      void del_from_lbuf(LBuf *lbuf, Object *ob);
extern      void end_sectors();
extern      Sector *find_sector(float *vec, float *local);			/* algemene find */
extern      void fix_coordvec(int *ip, float *fp);
extern      void fix_coordvecs(short *ip, float *fp);
extern      void free_lbuf(LBuf *lbuf);
extern      void init_sectors();
extern      void life_in_sector_test(Object *ob);
extern      void mainactor_in_sector_test();
extern      int passed_portal(int type, float *size, float *vec);
extern      int passed_portal_safe(int type, float *size, float *vec, float safe);
extern      int sector_cliptest(Sector *se, float *vec);
extern      int sector_cliptest_sure(Sector *se, float *vec, float sure);
extern      void sector_go();
extern      int sector_inside(Sector *se, float *vec, float *local);		/* veexternis globaal */
extern      int sector_intersect(Sector *se, Object *ob);

extern      int test_visibility(float *lookat, float *from, Life *lf, Sector *se);
extern      void update_Fheight(Life *lf, float *force);
extern      void update_dynamics(Object *ob);
extern      void update_lifes();
extern      void update_sector_lifes(Sector *se);
extern      void view_to_piramat(float mat[][4], float lens, float far);

/* ******* sectormv.c */
extern    void add_camera_to_sector(Sector *se, Object *ob);
extern    void add_networklink();
extern    void end_camera_network();
extern    void end_render_camera_network();
extern    void evaluate_camera(CamPos *capo);
extern    void evaluate_camera_network(Object *set, short mode);
extern    CamPos *find_best_campos1(CamPos *curcapo);		/* leuk voor stills */
extern    CamPos *find_best_campos2(CamPos *curcapo);		/* aktie */
extern    CamPos *find_best_campos3(CamPos *curcapo);		/* aktie + back */
extern    Sector *find_sector_lay(float *vec, float *local, uint lay);			/* algemene find */
extern    void init_camera_network();
extern    void init_render_camera_network();
extern    short netlink_flag(Object *to, Object *from, Sector *se);
extern    short netlink_sfra(Object *to, Object *from, Sector *se);
extern    void next_camera(int prev);
extern    void render_frame_to_camera();
extern    void render_frame_to_info(float *loc, float *rot, float *lens);
extern    void sector_frame_to_camera(Sector *se);
extern    void set_netclipsta(CamPos *capo);



#endif /* SECTOR_H */

