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



/*
 * exports.h
 * 
 * jan 95
 * 
 * 
 */

#ifdef WIN32
#include "winstuff.h"
#endif

	/* anim.c */
extern int where_on_path(Object *ob, float ctime, float *vec, float *dir);
extern void calc_curvepath(Object *ob);
extern void free_path(Path *path);
extern void make_duplilist(Scene *sce, Object *ob);
extern void free_duplilist(void);

	/* arithb.c */
#ifdef __WIN32
extern void srand48(uint );
extern float drand48(void);
#endif
extern void CalcCent3f(float *cent, float *v1, float *v2, float *v3);
extern void CalcCent4f(float *cent, float *v1, float *v2, float *v3, float *v4);
extern void Crossf(float *c, float *a, float *b);
extern void EulToMat3(float *eul, float mat[][3]);
extern void QuatToEul(float *quat, float *eul);
extern void Mat3Inv(float m1[][3], float m2[][3]);
/* extern int Mat4Invert(float inverse[][4], float mat[][4]); */
/* extern void Mat3CpyMat4(float *m1, float *m2); */
extern void Mat3ToEul(float tmat[][3], float *eul);
/* extern void Mat3MulMat3(float *m1, float *m3, float *m2); */
/* extern void Mat3MulVecfl(float mat[][3], float *vec); */
/* extern void Mat4MulVecfl(float mat[][4], float *vec); */
extern void Mat3MulFloat(float *m, float f);
extern void Mat4MulFloat(float *m, float f);
extern void Mat4MulFloat3(float *m, float f);
extern int FloatCompare(float *v1, float *v2, float limit);
extern float Normalise(float *n);
extern float CalcNormFloat(float *v1,float *v2,float *v3,float *n);
extern float CalcNormFloat4(float *v1,float *v2,float *v3,float *v4,float *n);
extern float VecLenf(float *v1, float *v2);
extern void VecMulf(float *v1, float f);
extern float Sqrt3f(float f);
extern double Sqrt3d(double d);
extern void euler_rot(float *beul, float ang, char axis);
extern    float safacos(float fac);
extern    float safsqrt(float fac);
extern float Inpf(float *v1, float *v2);
extern void VecSubf(float *v, float *v1, float *v2);
extern void VecAddf(float *v, float *v1, float *v2);
extern void VecUpMat3(float *vec, float mat[][3], short axis);

extern float DistVL2Dfl(float *v1,float *v2,float *v3);
extern float PdistVL2Dfl(float *v1,float *v2,float *v3);
extern float AreaF2Dfl(float *v1, float *v2, float *v3);
extern float AreaQ3Dfl(float *v1, float *v2, float *v3, float *v4);
extern float AreaT3Dfl(float *v1, float *v2, float *v3);
extern float AreaPoly3Dfl(int nr, float *verts, float *normal);
extern void VecRotToMat3(float *vec, float phi, float mat[][3]);
extern float Spec(float inp, int hard);
extern float *vectoquat(float *vec, short axis, short upflag);

extern void i_lookat(float vx, float vy, float vz, float px, float py, float pz, float twist, float mat[][4]);
extern void i_window(float left, float right, float bottom, float top, float near, float far, float mat[][4]);

extern void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b);
extern void rgb_to_hsv(float r, float g, float b, float *lh, float *ls, float *lv);
extern uint hsv_to_cpack(float h, float s, float v);
extern uint rgb_to_cpack(float r, float g, float b);
extern void cpack_to_rgb(uint col, float *r, float *g, float *b);

	/* blender.c */
extern struct Global G;
extern float matone[4][4];
extern char versionstr[];
extern void *dupallocN(void *mem);
extern int alloc_len(void *mem);
extern void addlisttolist(ListBase *list1, ListBase *list2);
extern void duplicatelist(ListBase *list1, ListBase *list2);  /* kopie van 2 naar 1 */

	/* bpaint.h */

	/* Button.h */
extern void drawcolorband(ColorBand *coba, float x1, float y1, float sizex, float sizey);

	/* buttons.c */
extern int do_clever_numbuts(char *name, int tot, int winevent);
extern void do_blenderbuttons(ushort event);
extern int movetolayer_buts(uint *lay);
extern void redraw_test_buttons(Base *new);
extern short button(short *var, short min, short max, char *str);
extern void add_numbut(int nr, int type, char *str, float min, float max, void *poin, char *tip);

	/* curve.c */
extern Curve *add_curve();
extern Curve *copy_curve(Curve *cu);
extern Nurb *duplicateNurb(Nurb *nu);
extern int count_curveverts(ListBase *nurb);
extern void autocalchandlesNurb(Nurb *nu, int flag);
extern void autocalchandlesNurb_all(int flag);
extern void calchandleNurb(BezTriple *bezt,BezTriple *prev, BezTriple *next, int mode);
extern void calchandlesNurb(Nurb *nu);
extern void freeNurblist(ListBase *lb);
extern void duplicateNurblist(ListBase *lb1, ListBase *lb2);
extern void freeNurb(Nurb *nu);
extern void free_curve(Curve *cu);
extern void maakbez(float q0, float q1, float q2, float q3, float *p, int it);
extern void makeBevelList(Object *ob);
extern void makeNurbcurve(Nurb *nu, float *data, int dim);
extern void makeNurbfaces(Nurb *nu, float *data) ;
extern void make_local_curve(Curve *cu);
extern void make_orco_surf(Curve *cu);
extern void makebevelcurve(Object *ob, ListBase *disp);
extern void makeknots(Nurb *nu, short uv, short type);
extern void minmaxNurb(Nurb *nu, float *min, float *max);
extern void sethandlesNurb(short code);
extern void switch_endian_knots(Nurb *nu);
extern void test2DNurb(Nurb *nu);
extern void test_curve_type(Object *ob);
extern void testhandlesNurb(Nurb *nu);
extern void tex_space_curve(Curve *cu);
extern void unlink_curve(Curve *cu);


	/* displist.c */
extern void copy_displist(ListBase *lbn, ListBase *lb);
extern void filldisplist(ListBase *dispbase, ListBase *to);
extern DispList *find_displist_create(ListBase *lb, int type);
extern DispList *find_displist(ListBase *lb, int type);
extern void addnormalsDispList(Object *ob, ListBase *lb);
extern void count_displist(ListBase *lb, int *totvert, int *totface);
extern void curve_to_filledpoly(Curve *cu, ListBase *dispbase);
extern void fastshade(float *co, float *nor, float *orco, Material *ma, char *col1, char *col2, char *vertcol);
extern void freedisplist(ListBase *lb);
extern void makeDispList(Object *ob);
extern void set_displist_onlyzero(int val);
extern void shadeDispList(Object *ob);


	/* drawimasel.c -> imsel.h */

	/* drawipo.c */
extern void areamouseco_to_ipoco(short *mval, float *x, float *y);
extern void draw_ipocurves(int sel);
extern void drawscroll(int disptype);
extern int in_ipo_buttons();
extern void ipoco_to_areaco(float *vec, short *mval);
extern void ipoco_to_areaco_noclip(float *vec, short *mval);
extern void view2dzoom();

	/* drawimage.c */
extern void rectwrite_part(int winxmin, int winymin, int winxmax, int winymax, int x1, int y1, int xim, int yim, float zoomx, float zoomy, uint *rect);
extern void calc_image_view(SpaceImage *sima, char mode);
extern void image_changed(SpaceImage *sima, int dotile);
extern void uvco_to_areaco(short *vec, short *mval);
extern void uvco_to_areaco_noclip(short *vec, short *mval);

	/* drawobject.c */
extern void tekenvertices_ext(int mode);	
extern void calc_meshverts(void);
extern void calc_meshverts_ext(void);
extern uint rectpurple[5][5];
extern uint rectyellow[5][5];
extern void tekenrect_col(short size, short sx, short sy, uint col);
extern void drawaxes(float size);
extern void drawcircball(float *cent, float rad, float tmat[][4]);
extern void calc_Nurbverts(Nurb *nurb);
extern void drawDispList(Object *ob, int dt);
extern void draw_object(Base *base);
extern void draw_object_ext(Base *base);
extern void drawboundbox(Object *ob);
extern void drawmeshwire(Object *ob);
extern void drawcamera(Object *ob);
extern void drawcube_size(float *size);
extern void drawmeshsolid(Object *ob, float *nors);
extern void tekenvertices(short sel);

	/* drawoops.c -> oops.h */
	
	/* drawscene */
extern void set_scene(Scene *sce);

	/* drawsector.c -> sector.h */

	/* drawseq.c -> sequence.h */

	/* drawview.c */
extern double speed_to_swaptime(int speed);
extern void circ(float x, float y, float rad);
extern void circf(float x, float y, float rad);
extern void backdrawview3d(int test);
extern void init_gl_materials(Object *ob);
extern void setalpha_bgpic(BGpic *bgpic);
extern void timestr(int time, char *str);

	/* edit.c */
extern void countall(void);
extern int get_border(rcti *rect, short col);
extern void popfirst(void *data);
extern void poplast(void *data);
extern void pushdata(void *data, int len);
extern void free_pushpop(void);

	/* editbpaint.c -> bpaint.h */

	/* editcurve.c */
extern ListBase editNurb;
extern void add_primitiveCurve(int stype);
extern void add_primitiveNurb(int type);
extern Nurb *addNurbprim(int type, int stype, int newname);
extern void findselectedNurbvert(Nurb **nu, BezTriple **bezt, BPoint **bp);
extern void addvert_Nurb(int mode);
extern void hideNurb(int swap);
extern short isNurbsel(Nurb *nu);
extern void join_curve(int type);
extern void weightflagNurb(short flag, float w, int mode);
extern void switchdirectionNurb(Nurb *nu);

	/* editfont.c */
extern void add_primitiveFont(void);
extern void do_textedit(ushort event, short val);

	/* editika.c */
extern void draw_ika(Object *ob, int sel);
extern void draw_ika_nrs(Object *ob, int type);
extern int extrude_ika(Object *ob, int add);

	/* editimasel.c */
extern void winqreadimasel(ushort event, short val);
extern void do_imasel_buttons(short event);
	
	
	/* editipo.c -> ipo.h */


	/* editkey.c */
extern Key *give_current_key(Object *ob);
extern void mesh_to_key(Mesh *me, KeyBlock *kb);
extern void key_to_mesh(KeyBlock *kb, Mesh *me);
extern void insert_meshkey(Mesh *me);
extern void key_to_latt(KeyBlock *kb, Lattice *lt);
extern void insert_lattkey(Lattice *lt);
extern void curve_to_key(Curve *cu, KeyBlock *kb, ListBase *nurb);
extern void key_to_curve(KeyBlock *kb, Curve  *cu, ListBase *nurb);
extern void insert_curvekey(Curve *cu);
extern void showkeypos(Key *key, KeyBlock *kb);



	/* editlattice.c */
extern Lattice *editLatt;
extern Lattice *add_lattice(void);
extern void free_lattice(Lattice *lt);
extern Lattice *copy_lattice(Lattice *lt);
extern void make_local_lattice(Lattice *lt);
extern void init_latt_deform(Object *oblatt, Object *ob);
extern void calc_latt_deform(float *co);
extern int object_deform(Object *ob);
extern void resizelattice(Lattice *lt);

	/* editmball.c */
extern ListBase editelems;
extern void add_primitiveMball(void);

	/* editmesh.c-> edit.h */

	/* editobject.c */
extern void transform(int mode);
extern void rightmouse_transform(void);
extern void enter_editmode(void);
extern void exit_editmode(int freedata);
extern void check_editmode(int type);
extern void docentre(void);
extern void docentre_new(void);
extern void docentre_cursor(void);
extern void add_object_draw(int type);
extern void apply_keyb_grid(float *val, float fac1, float fac2, float fac3, int invert);
extern Object *find_camera(void);
extern void headerprint(char *str);
extern void adduplicate(float *dtrans);
extern void make_trans_verts(float *min, float *max, int mode);

	/* editsector.c -> sector.h */

	/* editseq.c -> sequence.h */

	/* editview.c */
extern void draw_sel_circle(short *mval, short *mvalo, float rad, float rado, int selecting);
extern void set_active_base(Base *base);
extern void set_active_object(Object *ob);
extern void circle_select (void);

	/* effect.c -> effect.h */

	/* envmap.c */
extern EnvMap *add_envmap(void);
extern EnvMap *copy_envmap(EnvMap *ema);
extern void free_envmap(EnvMap *ema);

	/* exotic.c */
extern void read_exotic(char *name);
extern void write_videoscape(char *str);
extern void dxf_read(char *str);
extern void write_videoscape_fs(void);
extern void write_vrml_fs(void);
extern void write_dxf_fs(void);

	/* filesel.c */
extern void addfilename_to_fsmenu(char *name);
extern void filesel_statistics(SpaceFile *sfile, int *totfile, int *selfile, float *totlen, float *sellen);
extern void sort_filelist(SpaceFile *sfile);
extern void split_dirfile(char *string, char *dir, char *file);
extern void read_dir(SpaceFile *sfile);
extern void freefilelist(SpaceFile *sfile);
extern void parent(SpaceFile *sfile);
extern void swapselect_file(SpaceFile *sfile);
extern void activate_fileselect(int type, char *title, char *file, void (*func)(char *));
extern void activate_imageselect(int type, char *title, char *file, void (*func)(char *));
extern void activate_databrowse(ID *id, int idcode, int fromcode, int retval, void (*func)(ushort));
extern void free_filesel_spec(char *dir);
extern void winqreadfilespace(ushort event, short val);
extern int groupname_to_code(char *group);
extern char *code_to_groupname(int code);
extern int is_a_library(SpaceFile *sfile, char *dir, char *group);
extern void make_file_string (char *string,  char *dir,  char *file);

	/* fileops.c */
extern int fop_delete(char *file, int dir, int recursive);
extern int fop_touch(char *file);
extern int fop_move(char *file, char *to);
extern int fop_copy(char *file, char *to);
extern int fop_link(char *file, char *to);
extern int fop_backup(char *file, char *from, char *to);
extern int fop_exists(char *file);
extern void fop_recurdir(char *dirname);
extern int fop_rename(char *from, char *to);
extern char *first_slash(char *string);
extern char *last_slash(char *string);

	/* font.c */
extern VFont *load_vfont(char *name);
extern void free_vfont(struct VFont *vf);
extern void reload_vfont(VFont *vfont);
extern struct chartrans *text_to_curve(Object *ob, int mode);


	/* genfile.c -> file.h */

	/* headerbuttons.c */
	
extern void do_global_buttons(ushort event);	

	/* iconbut.c -> .h */
	

	/* ika.c -> .h */

	/* image.c */
extern void free_image(Image *ima);
extern void makepicstring(char *string, int frame);
extern void write_ibuf(ImBuf *ibuf, char *name);
extern int calcimanr(int cfra, Tex *tex);
extern void ima_ibuf_is_nul(Tex *tex);
extern int imagewrap(Tex *tex, float *texvec);
extern int imagewraposa(Tex *tex, float *texvec, float *dxt, float *dyt);
extern Image *add_image(char *str);
extern struct anim *openanim(char * name, int flags);

	/* imageprocess.c */
extern void scalefastrect(uint *recto, uint *rectn, int oldx, int oldy, int newx, int newy);
extern void addalphaAdd(char *doel, char *bron);
extern void addalphaAddfac(char *doel, char *bron, char addfac);
extern void addalphaOver(char *doel, char *bron);
extern void addalphaUnder(char *doel, char *bron);
extern void addalphaUnderGamma(char *doel, char *bron);
extern void keyalpha (char *doel);
extern void addalphaAddshort(ushort *doel, ushort *bron); 
	
	/* imasel.c -> .h */

	/* ipo.c -> .h */

	/* isect.c */
extern void intersect_mesh();

	/* initrender.c -> render.h */

	/* initsector.c */

	/* key.c */
extern         Key *add_key(ID *id);	/* algemeen */
extern         Key *copy_key(Key *key);
extern         void cp_cu_key(Curve *cu, KeyBlock *kb, int start, int end);
extern         void cp_key(int start, int end, int tot, char *poin, Key *key, KeyBlock *k, int mode);
extern         void do_all_keys();
extern         void do_cu_key(Curve *cu, KeyBlock **k, float *t);
extern         void do_curve_key(Curve *cu);
extern         void do_key(int start, int end, int tot, char *poin, Key *key, KeyBlock **k, float *t, int mode);
extern         void do_latt_key(Lattice *lt);
extern         void do_mesh_key(Mesh *me);
extern         void do_ob_key(Object *ob);
extern         void do_spec_key(Key *key);
extern         void free_key(Key *key);
extern         void make_local_key(Key *key);
extern         void set_afgeleide_four_ipo(float d, float *data, int type);
extern         void set_four_ipo(float d, float *data, int type);
extern         int setkeys(float fac, ListBase *lb, KeyBlock *k[], float *t, int cycl);
extern         void sort_keys(Key *key);
extern         void switch_endian_keyblock(Key *key, KeyBlock *kb);

	/* library.c */
extern     void IDnames_to_pupstring(char **str, ListBase *lb, ID *link, short *nr);
extern     void IDnames_to_pupstring_nr(char **str, ListBase *lb, ID *link, short *nr, short maxpup);
extern     void IPOnames_to_pupstring(char **str, ListBase *lb, ID *link, short *nr, int blocktype);
extern     void add_main_to_main(Main *main, Main *from);
extern     void all_local();
extern     void *alloc_libblock(ListBase *lb, short type, char *name);
extern     ID *alloc_libblock_notest(short type);
extern     void clear_id_newpoins();
extern     void *copy_libblock(void *rt);
extern     int count_mainblocks_flag(Main *main, int flag);
extern     ID *find_id(char *type, char *name);		
extern     Main *find_main(char *dir);
extern     void free_libblock(ListBase *lb, void *idv);
extern     void free_libblock_us(ListBase *lb, void *idv);		
extern     void free_liblist(ListBase *lb);
extern     void free_library(Library *lib);
extern     void free_main(ListBase *lb, Main *main);
extern     void free_mainlist();
extern     int has_id_number(ID *id);
extern     void id_lib_extern(ID *id);
extern     void id_us_plus(ID *id);
extern     void join_main();
extern     int new_id(ListBase *lb, ID *id, char *tname);
extern     void rename_id(ID *id, char *name);
extern     int set_listbasepointers(Main *main, ListBase **lb);
extern     void sort_alpha_id(ListBase *lb,  ID *id);
extern     void splitIDname(char *name, char *left, int *nr);
extern     void split_libdata(ListBase *lb, Main *first);
extern     void split_main();
extern     void test_idbutton(char *name);
extern     ListBase *wich_libbase(Main *main, short type);

	/* life.c -> sector.h */

	/* makesdna.c */
extern    short le_short(short temp);
extern    int le_int(int temp);

	/* material.c */
extern    void init_material(Material *ma);
extern    Material *add_material(char *name);
extern    void assign_material(Object *ob, Material *ma, int act);
extern    void automatname(Material *ma);
extern    Material *copy_material(Material *ma);
extern    void delete_material_index();
extern    void end_render_material(Material *ma);
extern    void end_render_materials();
extern    void free_material(Material *ma);
extern    Material *give_current_material(Object *ob, int act);
extern    Material ***give_matarar(Object *ob);
extern    short *give_totcolp(Object *ob);
extern    void init_render_material(Material *ma);
extern    void init_render_materials();
extern    void make_local_material(Material *ma);
extern    ID *material_from(Object *ob, int act);
extern    void new_material_to_objectdata(Object *ob);
extern    void replace_material(Material *old, Material *new);
extern    void test_object_materials(ID *id);

	/* mball.c */
extern       void accum_mballfaces(int i1, int i2, int i3, int i4);
extern       MetaBall *add_mball();
extern       void calc_mballco(MetaElem *ml, float *vec);
extern       MetaBall *copy_mball(MetaBall *mb);
extern       Object *find_basis_mball(Object *ob);
extern       void free_mball(MetaBall *mb);
extern       float init_meta(Object *ob);
extern       void make_local_mball(MetaBall *mb);
extern       void make_orco_mball(Object *ob);
extern       void metaball_polygonize(Object *ob);
extern       void tex_space_mball(Object *ob);
extern       void unlink_mball(MetaBall *mb);

	/* mesh.c */
extern        Mesh *add_mesh();
extern        void boundbox_mesh(Mesh *me, float *loc, float *size);
extern        Mesh *copy_mesh(Mesh *me);
extern        void flipnorm_mesh(Mesh *me);
extern        void free_mesh(Mesh *me);
extern        Mesh *get_mesh(Object *ob);
extern        Mesh *get_other_mesh(Object *ob);
extern        void make_local_mesh(Mesh *me);
extern        void make_local_tface(Mesh *me);
extern        void make_orco_mesh(Mesh *me);
extern        void mball_to_mesh(ListBase *lb, Mesh *me);
extern        void nurbs_to_mesh(Object *ob);
extern        void set_mesh(Object *ob, Mesh *me);
extern        void test_index_mface(MFace *mface, int nr);
extern        void tex_space_mesh(Mesh *me);
extern        void unlink_mesh(Mesh *me);
extern		  void make_s_editmesh(Object *);
extern		  void make_s_mesh(Object *);
void		  vertexnormals_mesh(Mesh *me, float *extverts);
extern		  void edge_drawflags_mesh(Mesh *me);

	/* noise.c */
extern float hnoise(float noisesize, float x, float y, float z);
extern float hnoisep(float noisesize, float x, float y, float z);
extern float turbulence(float noisesize, float x, float y, float z, int nr);
extern float turbulence1(float noisesize, float x, float y, float z, int nr);

	/* py_main.c */
extern void txt_do_python (Text *text);
extern void txt_do_python (Text *text);
extern void txt_do_python (Text *text);
extern void do_all_scripts(short event);

	/* py_draw.c */
/* extern void do_py_event(SpaceText *forspace, ushort event, short val); */
/* extern void do_py_buttons(SpaceText *forspace, ushort event); */
/* extern void do_py_draw(SpaceText *forspace); */
/* extern int py_is_regged(SpaceText *forspace); */
/* extern void exit_pydraw(SpaceText *st); */

	/* object.c */
extern Object workob;
extern      void *add_camera();
extern      void *add_lamp();
extern      Object *add_object(int type);
extern		int exist_object(Object *ob);
extern      void *add_wave();
extern      void clear_obact_names();
extern      void clear_workob();
extern      void copy_baseflags();
extern      Camera *copy_camera(Camera *cam);
extern      Lamp *copy_lamp(Lamp *la);
extern      Object *copy_object(Object *ob);
extern      void copy_objectflags();
extern      void expand_local_object(Object *ob);
extern      void free_lamp(Lamp *la);
extern      void free_object(Object *ob);
extern      void free_object_fromscene(Object *ob);
extern      void give_parvert(Object *par, int nr, float *vec);
extern      void make_local_camera(Camera *cam);
extern      void make_local_lamp(Lamp *la);
extern      void make_local_object(Object *ob);
extern      void minmax_object(Object *ob, float *min, float *max);
extern      void ob_parcurve(Object *ob, Object *par, float mat[][4]);
extern      void ob_parlimb(Object *ob, Object *par, float mat[][4]);
extern      void ob_parvert3(Object *ob, Object *par, float mat[][4]);
extern      void object_to_mat3(Object *ob, float mat[][3]);	
extern      void object_to_mat4(Object *ob, float mat[][4]);
extern      void set_dtime(int dtime);
extern      void set_mblur_offs(int blur);
extern      void set_no_parent_ipo(int val);
extern      void set_obact_names(Object *ob);
extern      float bsystem_time(Object *ob, Object *par, float cfra, float ofs);
extern      BoundBox *unit_boundbox();
extern      void what_does_parent(Object *ob);
extern      void what_does_parent1(Object *par, int partype, int par1, int par2, int par3);
extern		void disable_where_script(short on);
extern      void where_is_object(Object *ob);
extern      void where_is_object_simul(Object *ob);
extern      void where_is_object_time(Object *ob, float ctime);



	/* rct.c */
extern         int in_rctf(rctf *rect, float x, float y);
extern         void init_rctf(rctf *rect, float xmin, float xmax, float ymin, float ymax);
extern         int isect_rctf(rctf *src1, rctf *src2, rctf *dest);
extern         void union_rctf(rctf *rct1, rctf *rct2);

	/* renderfg.c */
extern    void bgntimer(void);
extern    void clear_render_display(void);
extern    void display_scanline(View3D *vd, uint *rect, int starty, int endy);
extern    int do_renderfg(int anim);
extern    void endtimer(void);
extern    void init_render_display();
extern    void printrenderinfo(int mode);
extern    void redraw_render_display();
extern    void redraw_render_win(int val);
extern    void render_display(int starty, int endy);
extern    void set_timecursor(int nr);
extern    int test_break(void);
extern    void toggle_render_display(void);
extern    void zoomwin(void);

	/* scene.c */
extern       Scene *add_scene(char *name);
extern       Scene *copy_scene(Scene *sce, int level);
extern       void free_scene(Scene *sce);
extern       int next_object(int val, Base **base, Object **ob);
extern       int object_in_scene(Object *ob, Scene *sce);
extern       void set_scene_bg(Scene *sce);
extern       void set_scene_name(char *name);
extern       void sort_baselist(Scene *sce);

	/* scanfill.c -> edit.h */

	/* screen.c -> .h */

	/* sector.c -> .h */

	/* space.c -> screen.h */

	/* texture.c -> render.h */
	
	/* text.c */
	
extern		char *txt_cut_buffer;

extern		void free_text(Text *text);
extern		void add_text_fs(char *file);
extern		Text *add_empty_text(void);
extern		Text *add_text(char *file);
extern		void txt_pop_first (Text *text);
extern		void txt_pop_last (Text *text);
extern		void txt_pop_sel (Text *text);
extern		void make_local_text(Text *text);
extern		Text *copy_text(Text *ta);

extern		TextLine *txt_new_line(char *str);
extern		TextLine *txt_new_linen(char *str, int n);
extern		void txt_clean_text (Text *text);
extern		int txt_get_span (TextLine *from, TextLine *to);
extern		int txt_illegal_char (char c);
extern		void txt_make_dirty (Text *text);
extern		void txt_move_up (Text *text, short sel);
extern		void txt_move_down (Text *text, short sel);
extern		void txt_move_left (Text *text, short sel);
extern		void txt_move_right (Text *text, short sel);
extern		void txt_move_bol (Text *text, short sel);
extern		void txt_move_eol (Text *text, short sel);
extern		void txt_move_bof (Text *text, short sel);
extern		void txt_move_eof (Text *text, short sel);
extern		void txt_move_toline (Text *text, unsigned int line, short sel);

extern		void txt_sel_all (Text *text);
extern		void txt_sel_line (Text *text);
extern		void txt_sel_par (Text *text);

extern		char *txt_sel_to_buf (Text *text);
extern		void txt_print_cutbuffer (void);
extern		void txt_cut_sel (Text *text);
extern		void txt_copy_sel (Text *text);
extern		void txt_paste_buf (Text *text, char *buf);
extern		void txt_delete_sel (Text *text);
extern		char *txt_to_buf (Text *text);

extern		void txt_undo_add_op(Text *text, int op);
extern		void txt_undo_add_block(Text *text, int op, char *buf);
extern		void txt_undo_add_toop(Text *text, int op, unsigned int froml, unsigned short fromc, unsigned int tol, unsigned short toc);
extern		void txt_undo_add_char(Text *text, char c);
extern		void txt_do_undo(Text *text);
extern		void txt_do_redo(Text *text);

extern		void txt_move_pgdn (SpaceText *st);
extern		void txt_move_pgup (SpaceText *st);
extern		void save_mem_text (char *str);
extern		void txt_write_file (Text *text);
extern		void txt_split_curline (Text *text);
extern		void txt_go_somewhere (TextLine **line);
extern		void txt_delete_line (Text *text, TextLine *line);
extern		void txt_combine_lines (Text *text, TextLine *linea, TextLine *lineb);
extern		void txt_delete_char (Text *text);
extern		void txt_backspace_char (Text *text);
extern		void txt_add_char (Text *text, char add);

	/* toets.c */
extern       int blenderqread(ushort event, short val);
extern       void persptoetsen(ushort event);
extern       int untitled(char * name);	

	/* toolbox.c */
extern     void ColorFunc(int i);
extern     void asciitoraw(int ch, ushort *event, ushort *qual);
extern     void bgnpupdraw(int startx, int starty, int endx, int endy);
extern     void bgntoolbox();
extern     short confirm(char *title, char *item);
extern     void drawtoolbox();
extern     void dummy();
extern     void endpupdraw();
extern     void endtoolbox();
extern     void error(char *str);
extern     void errorstr(char *str1, char *str2, char *str3);
extern     void mygetcursor(short *index);
extern     void notice(char *str);
extern     short okee(char *str);
extern     short pupmenu(char *instr);	
extern     int saveover(char *str);
extern     void tbox_embossbox(short x1, short y1, short x2, short y2, short type);	
extern     void tbox_embossbox_patch(short x1, short y1, short x2, short y2, short type);
extern     void tbox_setmain(int val);
extern     void toolbox();

	/* usiblender.c */
extern UserDef U;
extern		void exit_usiblender(void);

	/* view.c -> graphics.h */

	/* view2d.c */
extern      void init_view2d_calc();
extern      void view2d_to_window(float *x, float *y);	
extern      void window_to_view2d(float *x, float *y);	

/* ******* vpaint.c */
extern      int calc_vp_alpha(MVert *mvert, short *mval);
extern      void clear_vpaint();
extern      void copy_vpaint_undo(uint *mcol, int tot);
extern      void do_shared_vertexcol(Mesh *me);
extern      void free_vertexpaint();
extern      void init_vertexpaint();
extern      uint mcol_add(uint col1, uint col2, int fac);
extern      uint mcol_blend(uint col1, uint col2, int fac);
extern      uint mcol_mul(uint col1, uint col2, int fac);
extern      uint mcol_sub(uint col1, uint col2, int fac);
extern      void mcol_to_rgb(uint col, float *r, float *g, float *b);
extern      void mcol_to_tface(Mesh *me, int freedata);
extern      uint rgb_to_mcol(float r, float g, float b);
extern      uint sample_backbuf(int x, int y);
extern      int sample_backbuf_area(int x, int y);
extern      void sample_vpaint();	/* frontbuf */
extern      void set_faceselect();	/* toggle */
extern      void set_vpaint();		/* toggle */
extern      void tface_to_mcol(Mesh *me);
extern      void vertex_paint();
extern      void vpaint_blend( uint *col, uint paintcol, int alpha);
extern      void vpaint_dogamma();
extern      void vpaint_undo();


	/* world.c */
extern       World *add_world(char *name);
extern       World *copy_world(World *wrld);
extern       void free_world(World *wrld);
extern       void init_render_world();
extern       void make_local_world(World *wrld);

	/* writeblendpsx.c */
extern int le_coordint(float ftemp);
extern short le_coordshort(float ftemp);
extern short le_float_dangshort(float ftemp);
extern short le_floatangshort(float ftemp);
extern int le_dyna_int(float ftemp);
extern short le_dyna_short(float ftemp);
extern short le_floatshort(float ftemp);


