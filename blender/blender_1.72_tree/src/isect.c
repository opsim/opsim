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


/*  isect.c      GRAPHICS
 * 
 *  maart/april 1992 (traces)
 *  maart 95
 *  
 * 
 */


#include "blender.h"
#include "edit.h"

extern short cox, coy;	/* scanfill.c */
extern ListBase fillvertbase, filledgebase; 	/* scanfill.c */
extern EditVert *addvertlist();

typedef struct ISEdge {
	struct ISEdge *next,*prev;
	EditVert *v1,*v2;
	EditVlak *vl1,*vl2;
	short edflag1,edflag2;
} ISEdge;


ListBase isedgebase = {0,0};
ListBase isvertbase = {0,0};


/* ****  ARITH  *************************** */


short IsectFL(v1,v2,v3,v4,v5,vec)	/* Intersect Face and Linesegment */
float *v1,*v2,*v3,*v4,*v5,*vec;	/* 1,2,3=face 4,5=line vec=answer */
{
	/* return:
		-1: colliniar
		 0: no intersection
		 1: exact intersection of edge and line
		 2: cross-intersection
	*/

	double x0,x1,x2,t00,t01,t02,t10,t11,t12,t20,t21,t22;
	double m0,m1,m2,deeldet,det1,det2,det3;
	double rtu,rtv,rtlabda;

	t00= (double)v3[0]-(double)v1[0];
	t01= (double)v3[1]-(double)v1[1];
	t02= (double)v3[2]-(double)v1[2];
	t10= (double)v3[0]-(double)v2[0];
	t11= (double)v3[1]-(double)v2[1];
	t12= (double)v3[2]-(double)v2[2];
	t20= (double)v4[0]-(double)v5[0];
	t21= (double)v4[1]-(double)v5[1];
	t22= (double)v4[2]-(double)v5[2];

	x0=t11*t22-t12*t21;
	x1=t12*t20-t10*t22;
	x2=t10*t21-t11*t20;

	deeldet=t00*x0+t01*x1+t02*x2;
	if(deeldet==0.0) return -1;

	m0= (double)v4[0]-(double)v3[0];
	m1= (double)v4[1]-(double)v3[1];
	m2= (double)v4[2]-(double)v3[2];
	det1=m0*x0+m1*x1+m2*x2;
	rtu= det1/deeldet;
	if(rtu<=0.0) {
		det2=t00*(m1*t22-m2*t21);
		det2+=t01*(m2*t20-m0*t22);
		det2+=t02*(m0*t21-m1*t20);
		rtv= det2/deeldet;
		if(rtv<=0.0) {
			if(rtu+rtv>= -1.0) {
				det3=t00*(t11*m2-t12*m1);
				det3+=t01*(t12*m0-t10*m2);
				det3+=t02*(t10*m1-t11*m0);
				rtlabda=det3/deeldet;
				if(rtlabda>= 0.0 && rtlabda<=1.0) {
					vec[0]= v4[0]+ rtlabda*(v5[0]-v4[0]);
					vec[1]= v4[1]+ rtlabda*(v5[1]-v4[1]);
					vec[2]= v4[2]+ rtlabda*(v5[2]-v4[2]);
					if(rtu==0.0 || rtv==0.0 || (rtu+rtv== -1.0) || rtlabda==0.0 || rtlabda==1.0)
						return 1;
					return 2;
				}
			}
		}
	}
	return 0;
}


short IsectLL(v1,v2,v3,v4,cox,coy,labda,mu,vec)  /* intersect Line-Line */
float *v1,*v2,*v3,*v4;	/* vertices */
short cox,coy;			/* projection */
float *labda,*mu,*vec;		/* = answer */
{
	/* return:
		-1: colliniar
		 0: no intersection of segments
		 1: exact intersection of segments
		 2: cross-intersection of segments
	*/
	float deler;

	deler= (v1[cox]-v2[cox])*(v3[coy]-v4[coy])-(v3[cox]-v4[cox])*(v1[coy]-v2[coy]);
	if(deler==0.0) return -1;

	*labda= (v1[coy]-v3[coy])*(v3[cox]-v4[cox])-(v1[cox]-v3[cox])*(v3[coy]-v4[coy]);
	*labda= -(*labda/deler);

	deler= v3[coy]-v4[coy];
	if(deler==0) {
		deler=v3[cox]-v4[cox];
		*mu= -(*labda*(v2[cox]-v1[cox])+v1[cox]-v3[cox])/deler;
	} else {
		*mu= -(*labda*(v2[coy]-v1[coy])+v1[coy]-v3[coy])/deler;
	}
	vec[cox]= *labda*(v2[cox]-v1[cox])+v1[cox];
	vec[coy]= *labda*(v2[coy]-v1[coy])+v1[coy];

	if(*labda>=0.0 && *labda<=1.0 && *mu>=0.0 && *mu<=1.0) {
		if(*labda==0.0 || *labda==1.0 || *mu==0.0 || *mu==1.0) return 1;
		return 2;
	}
	return 0;
}



/* **** INTERSECT  ******************** */

int count_comparevlak(vl1, vl2)
EditVlak *vl1, *vl2;
{
	EditVert *v1, *v2, *v3;
	int tot= 0;
	
	if(vl1->v4 && vl2->v4) {
		
	}
	else if(vl1->v4==0 && vl2->v4==0) {
		v1= vl2->v1;
		v2= vl2->v2;
		v3= vl2->v3;

		if(vl1->v1==v1 || vl1->v2==v1 || vl1->v3==v1) tot++;
		if(vl1->v1==v2 || vl1->v2==v2 || vl1->v3==v2) tot++;
		if(vl1->v1==v3 || vl1->v2==v3 || vl1->v3==v3) tot++;
	}

	return tot;
}



void empty() {
}

/* #define printf empty */

void addisedge(vec,edflag,vl1,vl2,tel)
float *vec;
short *edflag;
EditVlak *vl1,*vl2;
short tel;
{
	/* maakt edge en zo nodig nieuwe vertices */
	ISEdge *ise;
	EditVert *eve,*v1=0,*v2=0;
	float swapvec[3];
	short sw;


	/* test op volgorde snijpunten, dubbels swappen */
	if(tel>2) {
		if(FloatCompare(vec, vec+3,  COMPLIMIT)) {
			VECCOPY(swapvec,vec);
			VECCOPY(vec,vec+6);
			VECCOPY(vec+6,swapvec);
			sw= edflag[0];
			edflag[0]= edflag[2];
			edflag[2]= sw;
		}
	}
	if(FloatCompare(vec, vec+3,  COMPLIMIT)) return;

	/* test op dubbele vertices */
	eve= isvertbase.first;
	while(eve) {
		if(v1==0) if(FloatCompare(eve->co,vec,  COMPLIMIT)) v1= eve;
		if(v2==0) if(FloatCompare(eve->co,vec+3,  COMPLIMIT)) v2= eve;

		if(v1!=0 && v2!=0) break;
		eve= eve->next;
	}
	if(v1!=0 || v2!=0) if(v1==v2) return;

	/* if(v1!=0) printf("double removed\n"); */
	/* if(v2!=0) printf("double removed\n"); */

	/* nieuwe vertices? */
	if(v1==0) {
		v1= (EditVert *)calloc(sizeof(EditVert),1);
		addtail(&isvertbase,v1);
		VECCOPY(v1->co,vec);
	}
	if(v2==0) {
		v2= (EditVert *)calloc(sizeof(EditVert),1);
		addtail(&isvertbase,v2);
		VECCOPY(v2->co,vec+3);
	}

	/* de nieuwe ISEdge */
	ise= (ISEdge *)calloc(sizeof(ISEdge),1);
	addtail(&isedgebase,ise);
	ise->v1= v1;
	ise->v2= v2;
	ise->vl1= vl1;
	ise->vl2= vl2;
	ise->edflag1= edflag[0];
	ise->edflag2= edflag[1];

	/* printf("edflag: %d %d %d %d\n",edflag[0],edflag[1],edflag[2],edflag[3]); */

	if(tel>2) {
		if(FloatCompare(vec+6,vec, COMPLIMIT)) ise->edflag1|= edflag[2];
		else ise->edflag2|= edflag[2];
		if(tel>3) {
			if(FloatCompare(vec+9,vec, COMPLIMIT)) ise->edflag1|= edflag[3];
			else ise->edflag2|= edflag[3];
		}
	}
	/* printf("edflag: %d %d \n",ise->edflag1,ise->edflag2); */
}

void oldedsort_andmake(olded, edcount, proj)
EditVert **olded;
int edcount, proj;
{
	EditVert *eve;
	int a, ok= 1;

	if(edcount==2) {
		ok= 0;
	}
	while(ok) {
		ok= 0;
		for(a=1; a<edcount; a++) {
			if(olded[a-1]->co[proj] > olded[a]->co[proj]) {
				eve= olded[a-1];
				olded[a-1]= olded[a];
				olded[a]= eve;
				ok= 1;
			}
		}
	}
	for(a=1;a<edcount;a++) {
		addfilledge(olded[a-1], olded[a]);
	}
}

short maxco(v1, v2)
float *v1, *v2;
{
	float x, y, z;

	x= fabs(v1[0]-v2[0]);
	y= fabs(v1[1]-v2[1]);
	z= fabs(v1[2]-v2[2]);

	if(x>=y && x>=z) return 0;
	else if(y>=x && y>=z) return 1;
	return 2;
	
}


void newfillvert(v1)
EditVert *v1;
{
	/* v1 is vert uit ISEdge struct
	   bestaat deze al in fill lijst ?
	*/
	EditVert *eve,*vn;

	eve= fillvertbase.first;
	while(eve) {
		if(eve->vn==v1) return;
		eve= eve->next;
	}

	eve= addfillvert(v1->co);
	v1->vn= eve;

	eve->vn= v1;
}

void addisfaces(evl)
EditVlak *evl;
{
	/* -pak alle ISEdges bij elkaar en genereer filledge lijst
	   -genereer fillvert lijst (geen dubbels)
	   
	ise->v1,v2:   nieuwe vertices
	ise->vl1,vl2: vlakken waarin de voornoemde edges liggen.
	ise->edflag:  code welke edges zijn gesneden
	*/
	
	EditVlak *evln;
	ISEdge *ise;
	EditVert *eve,*v1,*v2,*v3,*nextve;
	EditEdge *eed;
	EditVert **olded1,**olded2,**olded3;
	int edcount1=2, edcount2=2, edcount3=2, edcount=0;
	int a, proj, ok=1, bitmask;

	/* printf("\n in addisfaces \n"); */
	/* hoeveel edges doen mee? kun je oldedgeblock size berekenen */
	ise= isedgebase.first;
	while(ise) {
		if(ise->vl1==evl || ise->vl2==evl) edcount++;
		ise= ise->next;
	}
	if(edcount==0) return;
	/* printf("edcount: %d\n",edcount); */

	olded1= (EditVert **)mallocN(8+2*sizeof(void *)*edcount,"addisfaces1");
	olded2= (EditVert **)mallocN(8+2*sizeof(void *)*edcount,"addisfaces2");
	olded3= (EditVert **)mallocN(8+2*sizeof(void *)*edcount,"addisfaces3");
	olded1[0]= evl->v1;
	olded1[1]= evl->v2;
	olded2[0]= evl->v2;
	olded2[1]= evl->v3;
	olded3[0]= evl->v3;
	olded3[1]= evl->v1;

	/* fill edges maken en oldedge arrays maken*/

	ise= isedgebase.first;
	while(ise) {
		if(ise->vl1==evl || ise->vl2==evl) {
			eed= addfilledge(ise->v1, ise->v2);
			
			bitmask= 1;
			if(evl== ise->vl2) bitmask= 8;

			/* moet olded1 doormidden? */
			if(ise->edflag1 & bitmask) olded1[edcount1++]= ise->v1;
			if(ise->edflag2 & bitmask) olded1[edcount1++]= ise->v2;
			bitmask*=2;

			/* moet olded2 doormidden? */
			if(ise->edflag1 & bitmask) olded2[edcount2++]= ise->v1;
			if(ise->edflag2 & bitmask) olded2[edcount2++]= ise->v2;
			bitmask*=2;

			/* moet olded3 doormidden? */
			if(ise->edflag1 & bitmask) olded3[edcount3++]= ise->v1;
			if(ise->edflag2 & bitmask) olded3[edcount3++]= ise->v2;
		}
		ise= ise->next;
	}

	/* van oude edges vertices sorteren en filledges maken */

	/* printf("edcount123 %d %d %d\n", edcount1, edcount2, edcount3); */

	proj= maxco(evl->v1->co, evl->v2->co);
	oldedsort_andmake(olded1, edcount1, proj);

	proj= maxco(evl->v2->co, evl->v3->co);
	oldedsort_andmake(olded2, edcount2, proj);

	proj= maxco(evl->v3->co, evl->v1->co);
	oldedsort_andmake(olded3, edcount3, proj);

	freeN(olded1); 
	freeN(olded2); 
	freeN(olded3);

	/* - de filledges verwijzen naar oude(Edit) en nieuwe(Isect) vertices
	     met deze verts een tijdelijke fill lijst maken
	   - als een fillvert uiteindelijk een ->vn heeft is het een 'oude' vertex
	*/

	ise= isedgebase.first;
	while(ise) {
		if(ise->vl1==evl || ise->vl2==evl) {
			newfillvert(ise->v1);
			newfillvert(ise->v2);
			/* printf("verts uit ISEdge %x %x\n",ise->v1->vn, ise->v2->vn); */
			
		}
		ise= ise->next;
	}

	/* oude vertices kopieeren, flag zetten */
	v1= addfillvert(evl->v1->co);
	evl->v1->vn= v1;
	v1->vn= evl->v1;
	v1->f= 1;
	/* printf("oude vert %x\n",v1); */

	v1= addfillvert(evl->v2->co);
	evl->v2->vn= v1; 
	v1->vn= evl->v2;
	v1->f= 1;
	/* printf("oude vert %x\n",v1); */

	v1= addfillvert(evl->v3->co);
	evl->v3->vn= v1;
	v1->vn= evl->v3;
	v1->f= 1;
	/* printf("oude vert %x\n",v1); */

	/* nieuwe pointers in edges schrijven */
	eed= filledgebase.first;
	edcount= 0;
	while(eed) {
		edcount++;
		eed->v1= eed->v1->vn;
		eed->v2= eed->v2->vn;
		/* printf("edge: %x %x\n",eed->v1,eed->v2); */
		eed= eed->next;
	}

	/* printf("voor fill: %d edges\n",edcount); */
	/* de ->vn pointers van nieuwe vertices wissen */
	eve= fillvertbase.first;
	while(eve) {
		/* printf("vert: %x\n",eve); */
		if(eve->f==0) eve->vn=0;
		eve= eve->next;
	}

	/*	FILL */
	ok= edgefill(0);	/* (1) is kruispunten testen */
	/* printf("FILL\n"); */

	if(ok) {
		/* De nieuwe vlakken */
		edcount= 0;
		
		evl= fillvlakbase.first;
		while(evl) {
			edcount++;
			v1= evl->v1->vn;
			if(v1==0) {
				v1= addvertlist(evl->v1->co);
				/* evl->v1->vn= v1; */
			}
			v2= evl->v2->vn;
			if(v2==0) {
				v2= addvertlist(evl->v2->co);
				/* evl->v2->vn= v2; */
			}
			v3= evl->v3->vn;
			if(v3==0) {
				v3= addvertlist(evl->v3->co);
				/* evl->v3->vn= v3; */
			}

			evln= addvlaklist(v1, v2, v3, 0, evl->mat_nr, evl->flag, evl->tface);
			evln->f= 3;
			
			/* printf("EdVl %x %x %x\n",v1,v2,v3); */
			evl= evl->next;
		}
		/* printf("Na fill: %d vlakken\n", edcount); */
	}
	else printf("error in fill\n");

	end_edgefill();	
}

void intersect_mesh()
{
	static long numberofcalls=0;
	ISEdge *ise;
	EditVlak *evl,*evl1,*evl2,**eld,**evlist,*nextvl;
	EditVert *eve,*v1,*v2;
	EditEdge *eed, *nexted, *edar[8];
	float min[3], max[3], afm[3], vec[15], *fp;
	uint *octlist, *oct, *oct1;
	int a, b, c, totvlak;
	short ok, tel, oc1, oc2, oc3, ocmin, ocmax, edflag[8];

	if(G.obedit==0 || G.obedit->type!=OB_MESH) return;

	/* (voorlopig) alles naar driehoeken omzetten */
	convert_to_triface(0);	/* 0=alleen selected */

	isedgebase.first=isedgebase.last= 0;
	isvertbase.first=isvertbase.last= 0;

	/* array van vlakpointers maken */
	evl= G.edvl.first;
	totvlak= 0;
	while(evl) {
		if( (evl->v1->f & 1) && (evl->v2->f & 1) && (evl->v3->f & 1) ) {
			totvlak++;
			evl->f= 1;
		}
		else evl->f= 0;
		evl= evl->next;
	}
	if(totvlak<2) return;

	waitcursor(1);
	eld=evlist= (EditVlak **)mallocN(totvlak*sizeof(void *),"intersectVV");
	evl= G.edvl.first;
	while(evl) {
		if(evl->f==1) {
			*(eld++)= evl;
		}
		evl= evl->next;
	}

	/* min en max bepalen */
	eld= evlist;
	min[0]=min[1]=min[2]= 1.0e20;
	max[0]=max[1]=max[2]= -1.0e20;
	for(a=0;a<totvlak;a++) {
		evl= *(eld++);
		for(c=0;c<3;c++) {
			min[c]= MIN4(min[c],evl->v1->co[c],evl->v2->co[c],evl->v3->co[c]);
			max[c]= MAX4(max[c],evl->v1->co[c],evl->v2->co[c],evl->v3->co[c]);
		}
	}

	/* array octreegetallen maken en invullen */
	oct=octlist= (uint *)callocN(totvlak*3*sizeof(int),"intersectVV2");
	afm[0]= max[0]-min[0]+0.0001;
	afm[1]= max[1]-min[1]+0.0001;
	afm[2]= max[2]-min[2]+0.0001;
	eld= evlist;
	for(a=0;a<totvlak;a++) {
		evl= *(eld++);
		for(c=0;c<3;c++) {
			oc1= (short)(31.9*(evl->v1->co[c]-min[c])/afm[c]);
			oc2= (short)(31.9*(evl->v2->co[c]-min[c])/afm[c]);
			oc3= (short)(31.9*(evl->v3->co[c]-min[c])/afm[c]);
			
			ocmin= MIN3(oc1, oc2, oc3);
			ocmax= MAX3(oc1, oc2, oc3);
			
			for(b=ocmin; b<=ocmax; b++) {
				*oct= BSET(*oct, b);
			}
			oct++;
		}
	}

	eed= G.eded.first;
	while(eed) {
		eed->f= 0;
		eed= eed->next;
	}

	/* snijlus */
	eld= evlist;
	oct= octlist;
	for(a=0;a<totvlak;a++) {
		evl= *(eld++);
		oct1= oct+3;
		for(b=a+1; b<totvlak; b++) {
			if((oct[0] & oct1[0]) && (oct[1] & oct1[1]) && (oct[2] & oct1[2]) ) {
				evl1= evlist[b];
				if(count_comparevlak(evl, evl1)==0) {
					tel= 0;
					fp= vec;
					edflag[0]=edflag[1]=edflag[2]=edflag[3]= 0;
					/* edflag: van rechts naar links bitjes gezet als edge gesneden */
					if(IsectFL(evl->v1->co,evl->v2->co,evl->v3->co,evl1->v1->co,evl1->v2->co,fp)>0) {
						edflag[tel]= 1;
						edar[tel]= evl1->e1;
						tel++; 
						fp+=3;
					}
					if(IsectFL(evl->v1->co,evl->v2->co,evl->v3->co,evl1->v2->co,evl1->v3->co,fp)>0) {
						edflag[tel]= 2;
						edar[tel]= evl1->e2;
						tel++; 
						fp+=3;
					}
					if(IsectFL(evl->v1->co,evl->v2->co,evl->v3->co,evl1->v3->co,evl1->v1->co,fp)>0) {
						edflag[tel]= 4;
						edar[tel]= evl1->e3;
						tel++; 
						fp+=3;
					}
					if(IsectFL(evl1->v1->co,evl1->v2->co,evl1->v3->co,evl->v1->co,evl->v2->co,fp)>0) {
						edflag[tel]= 8;
						edar[tel]= evl->e1;
						tel++; 
						fp+=3;
					}
					if(IsectFL(evl1->v1->co,evl1->v2->co,evl1->v3->co,evl->v2->co,evl->v3->co,fp)>0) {
						edflag[tel]= 16;
						edar[tel]= evl->e2;
						tel++; 
						fp+=3;
					}
					if(IsectFL(evl1->v1->co,evl1->v2->co,evl1->v3->co,evl->v3->co,evl->v1->co,fp)>0) {
						edflag[tel]= 32;
						edar[tel]= evl->e3;
						tel++; 
						fp+=3;
					}
					/* printf("aantal snijps: %d\n",tel); */
					/* intersect edge gevonden? */
					if(tel>=2) {
						addisedge(vec, edflag, evl1, evl, tel);
						for(tel--;tel>=0;tel--) {
							if(edar[tel]) edar[tel]->f= 1;
						}
						evl->f= 2;
						evl1->f= 2;
					}
				}
			}
			oct1+=3;
		}
		oct+=3;
	}

		/* DEBUG: nu even tijdelijk de snijedges wegschrijven?
		
		ise= isedgebase.first;
		while(ise) {
			v1= addvertlist(ise->v1->co);
			v2= addvertlist(ise->v2->co);
			addedgelist(v1,v2);

			ise= ise->next;
		}
		*/

	/* gesneden edges verwijderen  */
	eed= G.eded.first;
	while(eed) {
		nexted= eed->next;
		if(eed->f) {
			/* printf("edge verwijderd %x %x\n",eed->v1,eed->v2); */
			remedge(eed);
		}
		eed= nexted;
	}

	/* nieuwe vlakken maken met fill en verwijderen */
	eld= evlist;
	for(a=0; a<totvlak; a++) {
		evl= *(eld++);
		if(evl->f==2) {
			addisfaces(evl);
			remlink(&G.edvl, evl);
			/* printf("vlak verwijderd %x %x %x\n",evl->v1,evl->v2,evl->v3); */
		}
	}

	freeN(evlist); 
	freeN(octlist);
	freelist(&isedgebase);
	freelist(&isvertbase);

	/* BEVEILIGING */

/* #undef printf */

	/* loop edgelijst af en controleer of alle vertices bestaan */
	eve= G.edve.first;

	v1= (EditVert *)numberofcalls;
	numberofcalls++;
	while(eve) {
		eve->vn= v1;
		eve= eve->next;
	}
	a= 0;
	eed= G.eded.first;
	while(eed) {
		nexted= eed->next;
		if(eed->v1->vn!=v1 || eed->v2->vn!=v1) {
			a++;
			remlink(&G.eded, eed);
		}
		eed= nexted;
	}
	if(a) printf("error after intersect: %d edges pointing to non-existant vertices\n", a);

	/* loop vlakkenlijst af en controleer of alle edges bestaan */
	a= 0;

	evl= G.edvl.first;
	while(evl) {
		/* nieuw vlak */
		if(evl->f==3) {
			evl->e1= findedgelist(evl->v1,evl->v2);
			if(evl->e1==0) {
				a++;
				evl->e1= addedgelist(evl->v1,evl->v2);
			}
			evl->e2= findedgelist(evl->v2,evl->v3);
			if(evl->e2==0) {
				a++;
				evl->e2= addedgelist(evl->v2,evl->v3);
			}
			evl->e3= findedgelist(evl->v3,evl->v1);
			if(evl->e3==0) {
				a++;
				evl->e3= addedgelist(evl->v3,evl->v1);
			}
		}
		evl= evl->next;
	}
	if(a) printf("error after intersect: %d edges in faces didn't exist\n", a);


/* HET UIT ELKAAR PLUKKEN VAN DE ONDERDELEN */
	/* nu zijn alle oude select, zet f1 flag */
	eve= G.edve.first;
	while(eve) {
		if(eve->h==0) {
			if(eve->f & 1) {
				eve->f1= 1;
			}
			eve->f= 0;
		}
		eve= eve->next;
	}

	/* begin met een oude vertex, selecteer connected zonder nieuwe vertices
	 * te passeren,  removedoubles en clear f1 vlag.
	 */
	eve= G.edve.first;
	while(eve) {
		if(eve->f1==1) {
			ok= 1;
			while(ok) {
				ok= 0;
				eve->f= 1;
				eed= G.eded.first;
				while(eed) {
					if(eed->h==0) {
						if( (eed->v1->f & 1) && (eed->v2->f & 1)==0) {
							eed->v2->f= 1;
							ok= 1;
						}
						else if( (eed->v2->f & 1) && (eed->v1->f & 1)==0) {
							eed->v1->f= 1;
							ok= 1;
						}
					}
					eed= eed->next;
				}
			}
			
			removedoublesflag(1, (float)0.0003);

			/* en flags wissen */
			v1= G.edve.first;
			while(v1) {
				if(v1->f & 1) {
					v1->f= 0;
					v1->f1= 0;
				}
				v1= v1->next;
			}
			
			eve= G.edve.first;	/* ivm removedoubles */
		}
		eve= eve->next;
	}

	waitcursor(0);
	countall();
	allqueue(REDRAWVIEW3D, 0);
}

