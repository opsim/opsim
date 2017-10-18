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

/*	formules voor blender  	*/

/*
	i_window(left, right, bottom, top, near, far,mat)

	Mat3Transp(mat);					Mat4Transp(mat);

 	Mat3Inv(mat1,mat2)					Mat4Inv(mat1,mat2)  (mat1= inv mat2, is wel met 3x3 berekend)
 	Mat4InvGG(mat1,mat2)				    (mat1= inv mat2 uit GraphicGems)
	Mat4Invert(mat1, mat2)		DEZE IS ECHT GOED
	Mat3Adj(mat1,mat2)

	Mat3MulMat3(mat1,mat2,mat3)			Mat4MulMat4(mat1,mat2,mat3)  (mat1=mat2*mat3)
	Mat3MulSerie(answ, m1, m2, m3....)  Mat4MulSerie(answ, m1, m2, m3....)
	
	Mat4CpyMat4(mat1,mat2)				    (mat1=mat2)
	Mat3CpyMat4(mat1,mat2)
	Mat4CpyMat3(mat1,mat2)				    (WEL clear)
	Mat3CpyMat3(mat1,mat2)

	Mat4SwapMat4(mat1,mat2);
	
	Mat3Clr(mat)						Mat4Clr(mat)
	Mat3One(mat)						Mat4One(mat)

	Mat3MulVecfl(mat,vec)				Mat4MulVecfl(mat,vec)   (vec is 3)
	Mat4MulVec4fl(mat,vec)					(vec is 4)
	Mat3MulVec(mat,vec)					Mat4MulVec(mat,vec)	(vec is 3)
	
	Mat3MulFloat(mat,f)					Mat4MulFloat(mat,f)
	Mat4MulFloat3(m,f)						(alleen de scale component )

	Mat3Ortho(mat)						Mat4Ortho(mat)

	VecStar(mat, vec)		matrix wordt Stermatrix van vec, soort uitprodukt met X-Y-Z-assen

short	EenheidsMat(mat)				    (mat3)

	printmatrix3(mat)						printmatrix4(mat)
	
	QuatToMat3(q,m)
	Mat3ToQuat(m, q)
	QuatMul(q1,q2,q3)						(q1=q2*q3)
	NormalQuat(q)

float	Normalise(n)
	VecCopyf(v1,v2)							(v1=v2)
void	VecLen(v1,v2)					    (afstand tussen twee punten)
float	VecLenf(v1,v2)
	VecAddf(v, v1,v2)
	VecSubf(v, v1,v2)
	VecMidf(v, v1,v2)
	VecMulf(v1,f)

	Crossf(vecout,vec1,vec2)
float	Inpf(vec1,vec2)
	CalcNormShort(v1,v2,v3,n)
	CalcNormLong(v1,v2,v3,n)
	CalcNormFloat(v1,v2,v3,n)
	

	CalcCent3f(cent, v1, v2, v3) CalcCent4f(cent, v1, v2, v3, v4)
	
float	Sqrt3f(f)			    (derdemachts wortel)
float	Sqrt3d(d)

float	DistVL2Dfl(v1,v2,v3)		(afstand v1 tot lijn v2-v3 :GEEN LIJNSTUK!)
float	PdistVL2Dfl(v1,v2,v3)		(pointdist v1 tot lijn v2-v3 :LIJNSTUK!)

float	AreaF2Dfl(v1,v2,v3)		    (oppervlakte van een 2D driehoek)
float	AreaQ3Dfl(v1, v2, v3, v4)	    (alleen bolle vierhoeken)
float	AreaT3Dfl(v1, v2, v3)		    (triangles)
float	AreaPoly3Dfl(nr, verts, normal)	    (met trapezium regel)

	MinMaxRGB(c)
	Spec(inpr,spec)	
*/

/* ************************ FUNKTIES **************************** */

#include <math.h>
#include <sys/types.h>

#ifndef __sgi
#include <sys/time.h>
#include <unistd.h>
#endif

#include "util.h"
#include "blender.h"

#define SMALL_NUMBER	1.e-8

float safacos(float fac)
{
	if(fac<= -1.0) return M_PI;
	else if(fac>=1.0) return 0.0;
	else return facos(fac);
}


float Normalise(float *n)
{
	register float d;

	d= n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
	if(d>0.0) {
		d= fsqrt(d);

		n[0]/=d; 
		n[1]/=d; 
		n[2]/=d;
	} else {
		n[0]=n[1]=n[2]= 0.0;
	}
	return d;
}

void Crossf(float *c, float *a, float *b)
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

float Inpf(float *v1, float *v2)
{
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}




void Mat3Transp(float mat[][3])
{
	float t;

	t = mat[0][1] ; 
	mat[0][1] = mat[1][0] ; 
	mat[1][0] = t;
	t = mat[0][2] ; 
	mat[0][2] = mat[2][0] ; 
	mat[2][0] = t;
	t = mat[1][2] ; 
	mat[1][2] = mat[2][1] ; 
	mat[2][1] = t;
}

void Mat4Transp(float mat[][4])
{
	float t;

	t = mat[0][1] ; 
	mat[0][1] = mat[1][0] ; 
	mat[1][0] = t;
	t = mat[0][2] ; 
	mat[0][2] = mat[2][0] ; 
	mat[2][0] = t;
	t = mat[0][3] ; 
	mat[0][3] = mat[3][0] ; 
	mat[3][0] = t;

	t = mat[1][2] ; 
	mat[1][2] = mat[2][1] ; 
	mat[2][1] = t;
	t = mat[1][3] ; 
	mat[1][3] = mat[3][1] ; 
	mat[3][1] = t;

	t = mat[2][3] ; 
	mat[2][3] = mat[3][2] ; 
	mat[3][2] = t;
}


/*
 * invertmat - 
 * 		computes the inverse of mat and puts it in inverse.  Returns 
 *	TRUE on success (i.e. can always find a pivot) and FALSE on failure.
 * 	Uses Gaussian Elimination with partial (maximal column) pivoting.
 *
 *					Mark Segal - 1992
 */

int Mat4Invert(float inverse[][4], float mat[][4])
{
	int i, j, k;
	double temp;
	float tempmat[4][4];
	float max;
	int maxj;

	/* Set inverse to identity */
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			inverse[i][j] = 0;
	for (i=0; i<4; i++)
		inverse[i][i] = 1;

	/* Copy original matrix so we don't mess it up */
	for(i = 0; i < 4; i++)
		for(j = 0; j <4; j++)
			tempmat[i][j] = mat[i][j];

	for(i = 0; i < 4; i++) {
		/* Look for row with max pivot */
		max = ABS(tempmat[i][i]);
		maxj = i;
		for(j = i + 1; j < 4; j++) {
			if(ABS(tempmat[j][i]) > max) {
				max = ABS(tempmat[j][i]);
				maxj = j;
			}
		}
		/* Swap rows if necessary */
		if (maxj != i) {
			for( k = 0; k < 4; k++) {
				SWAP(float, tempmat[i][k], tempmat[maxj][k]);
				SWAP(float, inverse[i][k], inverse[maxj][k]);
			}
		}

		temp = tempmat[i][i];
		if (temp == 0)
			return 0;  /* No non-zero pivot */
		for(k = 0; k < 4; k++) {
			tempmat[i][k] /= temp;
			inverse[i][k] /= temp;
		}
		for(j = 0; j < 4; j++) {
			if(j != i) {
				temp = tempmat[j][i];
				for(k = 0; k < 4; k++) {
					tempmat[j][k] -= tempmat[i][k]*temp;
					inverse[j][k] -= inverse[i][k]*temp;
				}
			}
		}
	}
	return 1;
}

void Mat4InvertSimp(float inverse[][4], float mat[][4])
{
	/* alleen HOEK bewarende Matrices */
	/* gebaseerd op GG IV pag 205 */
	float scale;
	
	scale= mat[0][0]*mat[0][0] + mat[1][0]*mat[1][0] + mat[2][0]*mat[2][0];
	if(scale==0.0) return;
	
	scale= 1.0/scale;
	
	/* transpose en scale */
	inverse[0][0]= scale*mat[0][0];
	inverse[1][0]= scale*mat[0][1];
	inverse[2][0]= scale*mat[0][2];
	inverse[0][1]= scale*mat[1][0];
	inverse[1][1]= scale*mat[1][1];
	inverse[2][1]= scale*mat[1][2];
	inverse[0][2]= scale*mat[2][0];
	inverse[1][2]= scale*mat[2][1];
	inverse[2][2]= scale*mat[2][2];

	inverse[3][0]= -(inverse[0][0]*mat[3][0] + inverse[1][0]*mat[3][1] + inverse[2][0]*mat[3][2]);
	inverse[3][1]= -(inverse[0][1]*mat[3][0] + inverse[1][1]*mat[3][1] + inverse[2][1]*mat[3][2]);
	inverse[3][2]= -(inverse[0][2]*mat[3][0] + inverse[1][2]*mat[3][1] + inverse[2][2]*mat[3][2]);
	
	inverse[0][3]= inverse[1][3]= inverse[2][3]= 0.0;
	inverse[3][3]= 1.0;
}


void Mat4Inv(struct Matrix4 *m1, struct Matrix4 *m2)
{
	short a,b;
	float det,mat1[3][3],mat2[3][3];
	void Mat3Inv();
	void Mat3CpyMat4();
	void Mat4CpyMat3();

	Mat3CpyMat4(mat2,m2);
	Mat3Inv(mat1,mat2);
	Mat4CpyMat3(m1,mat1);

}

float Det2x2(float a, float b, float c, float d)
{

	return a*d - b*c;
}



float Det3x3(float a1,float a2,float a3,float b1,float b2,float b3,float c1,float c2,float c3)
{
	float ans;

	ans = a1 * Det2x2( b2, b3, c2, c3 )
	    - b1 * Det2x2( a2, a3, c2, c3 )
	    + c1 * Det2x2( a2, a3, b2, b3 );

	return ans;
}

float Det4x4(float m[][4])
{
	float ans;
	float a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4;

	a1= m[0][0]; 
	b1= m[0][1];
	c1= m[0][2]; 
	d1= m[0][3];

	a2= m[1][0]; 
	b2= m[1][1];
	c2= m[1][2]; 
	d2= m[1][3];

	a3= m[2][0]; 
	b3= m[2][1];
	c3= m[2][2]; 
	d3= m[2][3];

	a4= m[3][0]; 
	b4= m[3][1];
	c4= m[3][2]; 
	d4= m[3][3];

	ans = a1 * Det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4)
	    - b1 * Det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4)
	    + c1 * Det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4)
	    - d1 * Det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);

	return ans;
}


void Mat4Adj(float out[][4], float in[][4])	/* out = ADJ(in) */
{
	float a1, a2, a3, a4, b1, b2, b3, b4;
	float c1, c2, c3, c4, d1, d2, d3, d4;

	a1= in[0][0]; 
	b1= in[0][1];
	c1= in[0][2]; 
	d1= in[0][3];

	a2= in[1][0]; 
	b2= in[1][1];
	c2= in[1][2]; 
	d2= in[1][3];

	a3= in[2][0]; 
	b3= in[2][1];
	c3= in[2][2]; 
	d3= in[2][3];

	a4= in[3][0]; 
	b4= in[3][1];
	c4= in[3][2]; 
	d4= in[3][3];


	out[0][0]  =   Det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
	out[1][0]  = - Det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
	out[2][0]  =   Det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
	out[3][0]  = - Det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);

	out[0][1]  = - Det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
	out[1][1]  =   Det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
	out[2][1]  = - Det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
	out[3][1]  =   Det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);

	out[0][2]  =   Det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
	out[1][2]  = - Det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
	out[2][2]  =   Det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
	out[3][2]  = - Det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);

	out[0][3]  = - Det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
	out[1][3]  =   Det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
	out[2][3]  = - Det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
	out[3][3]  =   Det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

void Mat4InvGG(float out[][4], float in[][4])	/* van Graphic Gems I, out= INV(in)  */
{
	int i, j;
	float det, det4x4();

	/* calculate the adjoint matrix */

	Mat4Adj(out,in);

	det = Det4x4(out);

	if ( fabs( det ) < SMALL_NUMBER) {
		return;
	}

	/* scale the adjoint matrix to get the inverse */

	for (i=0; i<4; i++)
		for(j=0; j<4; j++)
			out[i][j] = out[i][j] / det;

	/* de laatste factor is niet altijd 1. Hierdoor moet eigenlijk nog gedeeld worden */
}


void Mat3Inv(float m1[][3], float m2[][3])
{
	short a,b;
	float det;
	void Mat3Adj();

	/* eerst adjoint */
	Mat3Adj(m1,m2);

	/* dan det oude mat! */
	det= m2[0][0]* (m2[1][1]*m2[2][2] - m2[1][2]*m2[2][1])
	    -m2[1][0]* (m2[0][1]*m2[2][2] - m2[0][2]*m2[2][1])
	    +m2[2][0]* (m2[0][1]*m2[1][2] - m2[0][2]*m2[1][1]);

	if(det==0) det=1;
	det= 1/det;
	for(a=0;a<3;a++) {
		for(b=0;b<3;b++) {
			m1[a][b]*=det;
		}
	}
}

void Mat3Adj(float m1[][3], float m[][3])
{
	m1[0][0]=m[1][1]*m[2][2]-m[1][2]*m[2][1];
	m1[0][1]= -m[0][1]*m[2][2]+m[0][2]*m[2][1];
	m1[0][2]=m[0][1]*m[1][2]-m[0][2]*m[1][1];

	m1[1][0]= -m[1][0]*m[2][2]+m[1][2]*m[2][0];
	m1[1][1]=m[0][0]*m[2][2]-m[0][2]*m[2][0];
	m1[1][2]= -m[0][0]*m[1][2]+m[0][2]*m[1][0];

	m1[2][0]=m[1][0]*m[2][1]-m[1][1]*m[2][0];
	m1[2][1]= -m[0][0]*m[2][1]+m[0][1]*m[2][0];
	m1[2][2]=m[0][0]*m[1][1]-m[0][1]*m[1][0];
}

void Mat4MulMat4(float *m1, float *m2, float *m3)
{

	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8] +m2[3]*m3[12];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9] +m2[3]*m3[13];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10] +m2[3]*m3[14];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] +m2[3]*m3[15];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8] +m2[3]*m3[12];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9] +m2[3]*m3[13];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10] +m2[3]*m3[14];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] +m2[3]*m3[15];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8] +m2[3]*m3[12];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9] +m2[3]*m3[13];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10] +m2[3]*m3[14];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] +m2[3]*m3[15];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8] +m2[3]*m3[12];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9] +m2[3]*m3[13];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10] +m2[3]*m3[14];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] +m2[3]*m3[15];
}

void subMat4MulMat4(float *m1, float *m2, float *m3)
{

	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] + m2[3];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] + m2[3];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
	m1[3]= m2[0]*m3[3] + m2[1]*m3[7] + m2[2]*m3[11] + m2[3];
}


void Mat3MulMat3(float *m1, float *m3, float *m2)
{

	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
	m1+=3;
	m2+=3;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
	m1+=3;
	m2+=3;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
}

void Mat4MulMat43(float *m1, float *m3, float *m2)	/* m1 en m3 zijn mat4, m2 is mat3 */
{

	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
	m1+=4;
	m2+=3;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
	m1+=4;
	m2+=3;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[4] + m2[2]*m3[8];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[5] + m2[2]*m3[9];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[6] + m2[2]*m3[10];
}

void Mat4MulMat34(float *m1, float *m3, float *m2)	/* m1 en m2 zijn mat4, m3 is mat3 */
{

	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
	m1+=4;
	m2+=4;
	m1[0]= m2[0]*m3[0] + m2[1]*m3[3] + m2[2]*m3[6];
	m1[1]= m2[0]*m3[1] + m2[1]*m3[4] + m2[2]*m3[7];
	m1[2]= m2[0]*m3[2] + m2[1]*m3[5] + m2[2]*m3[8];
}


void Mat4CpyMat4(float *m1, float *m2)
{
	bcopy(m2, m1, 64);
}

void Mat4SwapMat4(float *m1, float *m2)
{
	float t;
	int i;

	for(i=0;i<16;i++) {
		t= *m1;
		*m1= *m2;
		*m2= t;
		m1++; 
		m2++;
	}
}

void Mat3CpyMat4(float m1[][3], float m2[][4])
{
	m1[0][0]= m2[0][0];
	m1[0][1]= m2[0][1];
	m1[0][2]= m2[0][2];
	m1[1][0]= m2[1][0];
	m1[1][1]= m2[1][1];
	m1[1][2]= m2[1][2];
	m1[2][0]= m2[2][0];
	m1[2][1]= m2[2][1];
	m1[2][2]= m2[2][2];
}

void Mat4CpyMat3(float m1[][4], float m2[][3])		/* clear */
{
	m1[0][0]= m2[0][0];
	m1[0][1]= m2[0][1];
	m1[0][2]= m2[0][2];
	m1[0][3]= 0.0;
	m1[1][0]= m2[1][0];
	m1[1][1]= m2[1][1];
	m1[1][2]= m2[1][2];
	m1[1][3]= 0.0;
	m1[2][0]= m2[2][0];
	m1[2][1]= m2[2][1];
	m1[2][2]= m2[2][2];
	m1[2][3]=m1[3][0]=m1[3][1]= m1[3][2]= 0.0;
	m1[3][3]= 1.0;
}

void Mat4CpyMat3nc(float m1[][4], float m2[][3])	/* no clear */
{
	m1[0][0]= m2[0][0];
	m1[0][1]= m2[0][1];
	m1[0][2]= m2[0][2];

	m1[1][0]= m2[1][0];
	m1[1][1]= m2[1][1];
	m1[1][2]= m2[1][2];

	m1[2][0]= m2[2][0];
	m1[2][1]= m2[2][1];
	m1[2][2]= m2[2][2];
}

void Mat3CpyMat3(float *m1, float *m2)
{

	bcopy(m2, m1, 36);
}

void Mat3MulSerie(float *answ, float *m1, float *m2, float *m3, float *m4, float *m5, float *m6, float *m7, float *m8)
{
	float temp[3][3];
	
	if(m1==0 || m2==0) return;
	
	Mat3MulMat3(answ, m2, m1);
	if(m3) {
		Mat3MulMat3(temp, m3, answ);
		if(m4) {
			Mat3MulMat3(answ, m4, temp);
			if(m5) {
				Mat3MulMat3(temp, m5, answ);
				if(m6) {
					Mat3MulMat3(answ, m6, temp);
					if(m7) {
						Mat3MulMat3(temp, m7, answ);
						if(m8) {
							Mat3MulMat3(answ, m8, temp);
						}
						else Mat3CpyMat3(answ, temp);
					}
				}
				else Mat3CpyMat3(answ, temp);
			}
		}
		else Mat3CpyMat3(answ, temp);
	}
}

void Mat4MulSerie(float *answ, float *m1, float *m2, float *m3, float *m4, float *m5, float *m6, float *m7, float *m8)
{
	float temp[4][4];
	
	if(m1==0 || m2==0) return;
	
	Mat4MulMat4(answ, m2, m1);
	if(m3) {
		Mat4MulMat4(temp, m3, answ);
		if(m4) {
			Mat4MulMat4(answ, m4, temp);
			if(m5) {
				Mat4MulMat4(temp, m5, answ);
				if(m6) {
					Mat4MulMat4(answ, m6, temp);
					if(m7) {
						Mat4MulMat4(temp, m7, answ);
						if(m8) {
							Mat4MulMat4(answ, m8, temp);
						}
						else Mat4CpyMat4(answ, temp);
					}
				}
				else Mat4CpyMat4(answ, temp);
			}
		}
		else Mat4CpyMat4(answ, temp);
	}
}



void Mat4Clr(float *m)
{

	bzero(m, 64);
}

void Mat3Clr(float *m)
{
	bzero(m, 36);
}

void Mat4One(float m[][4])
{

	m[0][0]= m[1][1]= m[2][2]= m[3][3]= 1.0;
	m[0][1]= m[0][2]= m[0][3]= 0.0;
	m[1][0]= m[1][2]= m[1][3]= 0.0;
	m[2][0]= m[2][1]= m[2][3]= 0.0;
	m[3][0]= m[3][1]= m[3][2]= 0.0;
}

void Mat3One(float m[][3])
{

	m[0][0]= m[1][1]= m[2][2]= 1.0;
	m[0][1]= m[0][2]= 0.0;
	m[1][0]= m[1][2]= 0.0;
	m[2][0]= m[2][1]= 0.0;
}

void Mat4MulVec(float mat[][4], int *vec)
{
	int x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]=x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2] + mat[3][0];
	vec[1]=x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2] + mat[3][1];
	vec[2]=x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2] + mat[3][2];
}

void Mat4MulVecfl(float mat[][4], float *vec)
{
	float x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]=x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2] + mat[3][0];
	vec[1]=x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2] + mat[3][1];
	vec[2]=x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2] + mat[3][2];
}

void VecMat4MulVecfl(float *in, float mat[][4], float *vec)
{
	float x,y;

	x=vec[0]; 
	y=vec[1];
	in[0]= x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2] + mat[3][0];
	in[1]= x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2] + mat[3][1];
	in[2]= x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2] + mat[3][2];
}

void Mat4Mul3Vecfl(float mat[][4], float *vec)
{
	float x,y;

	x= vec[0]; 
	y= vec[1];
	vec[0]= x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2];
	vec[1]= x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2];
	vec[2]= x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2];
}

void Mat4MulVec4fl(float mat[][4], float *vec)
{
	float x,y,z;

	x=vec[0]; 
	y=vec[1]; 
	z= vec[2];
	vec[0]=x*mat[0][0] + y*mat[1][0] + z*mat[2][0] + mat[3][0]*vec[3];
	vec[1]=x*mat[0][1] + y*mat[1][1] + z*mat[2][1] + mat[3][1]*vec[3];
	vec[2]=x*mat[0][2] + y*mat[1][2] + z*mat[2][2] + mat[3][2]*vec[3];
	vec[3]=x*mat[0][3] + y*mat[1][3] + z*mat[2][3] + mat[3][3]*vec[3];
}

void Mat3MulVec(float mat[][3], int *vec)
{
	int x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]= x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2];
	vec[1]= x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2];
	vec[2]= x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2];
}

void Mat3MulVecfl(float mat[][3], float*vec)
{
	float x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]= x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2];
	vec[1]= x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2];
	vec[2]= x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2];
}

void Mat3MulVecd(float mat[][3], double *vec)
{
	double x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]= x*mat[0][0] + y*mat[1][0] + mat[2][0]*vec[2];
	vec[1]= x*mat[0][1] + y*mat[1][1] + mat[2][1]*vec[2];
	vec[2]= x*mat[0][2] + y*mat[1][2] + mat[2][2]*vec[2];
}

void Mat3TransMulVecfl(float mat[][3], float *vec)
{
	float x,y;

	x=vec[0]; 
	y=vec[1];
	vec[0]= x*mat[0][0] + y*mat[0][1] + mat[0][2]*vec[2];
	vec[1]= x*mat[1][0] + y*mat[1][1] + mat[1][2]*vec[2];
	vec[2]= x*mat[2][0] + y*mat[2][1] + mat[2][2]*vec[2];
}

void Mat3MulFloat(float *m, float f)
{
	int i;

	for(i=0;i<9;i++) m[i]*=f;
}

void Mat4MulFloat(float *m, float f)
{
	int i;

	for(i=0;i<12;i++) m[i]*=f;	/* tot 12 tellen: zonder vector */
}


void Mat4MulFloat3(float *m, float f)		/* alleen de scale component */
{
	int i,j;

	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			
			m[4*i+j] *= f;
		}
	}
}

void VecStar(float mat[][3], float *vec)
{

	mat[0][0]= mat[1][1]= mat[2][2]= 0.0;
	mat[0][1]= -vec[2];	
	mat[0][2]= vec[1];
	mat[1][0]= vec[2];	
	mat[1][2]= -vec[0];
	mat[2][0]= -vec[1];	
	mat[2][1]= vec[0];
	
}

short EenheidsMat(float mat[][3])
{

	if(mat[0][0]==1.0 && mat[0][1]==0.0 && mat[0][2]==0.0)
		if(mat[1][0]==0.0 && mat[1][1]==1.0 && mat[1][2]==0.0)
			if(mat[2][0]==0.0 && mat[2][1]==0.0 && mat[2][2]==1.0)
				return 1;
	return 0;
}

int FloatCompare(float *v1, float *v2, float limit)
{

	if( fabs(v1[0]-v2[0])<limit ) {
		if( fabs(v1[1]-v2[1])<limit ) {
			if( fabs(v1[2]-v2[2])<limit ) return 1;
		}
	}
	return 0;
}

void printmatrix4(char *str, float m[][4])
{
	printf("%s\n", str);
	printf("%f %f %f %f\n",m[0][0],m[0][1],m[0][2],m[0][3]);
	printf("%f %f %f %f\n",m[1][0],m[1][1],m[1][2],m[1][3]);
	printf("%f %f %f %f\n",m[2][0],m[2][1],m[2][2],m[2][3]);
	printf("%f %f %f %f\n",m[3][0],m[3][1],m[3][2],m[3][3]);
	printf("\n");

}

void printmatrix3(char *str, float m[][3])
{
	printf("%s\n", str);
	printf("%f %f %f\n",m[0][0],m[0][1],m[0][2]);
	printf("%f %f %f\n",m[1][0],m[1][1],m[1][2]);
	printf("%f %f %f\n",m[2][0],m[2][1],m[2][2]);
	printf("\n");

}

/* **************** QUATERNIONS ********** */

void QuatMul(float *q, float *q1, float *q2)
{
	float t0,t1,t2;

	t0=   q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3];
	t1=   q1[0]*q2[1]+q1[1]*q2[0]+q1[2]*q2[3]-q1[3]*q2[2];
	t2=   q1[0]*q2[2]+q1[2]*q2[0]+q1[3]*q2[1]-q1[1]*q2[3];
	q[3]= q1[0]*q2[3]+q1[3]*q2[0]+q1[1]*q2[2]-q1[2]*q2[1];
	q[0]=t0; 
	q[1]=t1; 
	q[2]=t2;
}


void QuatToMat3(float *q, float *m)
{
	float q0, q1, q2, q3, qda,qdb,qdc,qaa,qab,qac,qbb,qbc,qcc;

	q0= M_SQRT2 * q[0];
	q1= M_SQRT2 * q[1];
	q2= M_SQRT2 * q[2];
	q3= M_SQRT2 * q[3];

	qda= q0*q1;
	qdb= q0*q2;
	qdc= q0*q3;
	qaa= q1*q1;
	qab= q1*q2;
	qac= q1*q3;
	qbb= q2*q2;
	qbc= q2*q3;
	qcc= q3*q3;

	m[0]= 1.0-qbb-qcc;
	m[1]= qdc+qab;
	m[2]= -qdb+qac;

	m[3]= -qdc+qab;
	m[4]= 1.0-qaa-qcc;
	m[5]= qda+qbc;

	m[6]= qdb+qac;
	m[7]= -qda+qbc;
	m[8]= 1.0-qaa-qbb;
}

void QuatToMat4(float *q, float *m)
{
	float q0, q1, q2, q3, qda,qdb,qdc,qaa,qab,qac,qbb,qbc,qcc;

	q0= M_SQRT2 * q[0];
	q1= M_SQRT2 * q[1];
	q2= M_SQRT2 * q[2];
	q3= M_SQRT2 * q[3];

	qda= q0*q1;
	qdb= q0*q2;
	qdc= q0*q3;
	qaa= q1*q1;
	qab= q1*q2;
	qac= q1*q3;
	qbb= q2*q2;
	qbc= q2*q3;
	qcc= q3*q3;

	m[0]= 1.0-qbb-qcc;
	m[1]= qdc+qab;
	m[2]= -qdb+qac;
	m[3]= 0.0;

	m[4]= -qdc+qab;
	m[5]= 1.0-qaa-qcc;
	m[6]= qda+qbc;
	m[7]= 0.0;

	m[8]= qdb+qac;
	m[9]= -qda+qbc;
	m[10]= 1.0-qaa-qbb;
	m[11]= m[12]= m[13]= m[14]= 0.0;
	
	m[15]= 1.0;
}

void Mat3ToQuat(float wmat[][3], float *q)		/* uit Sig.Proc.85 pag 253 */
{
	void Mat3Ortho(), NormalQuat();
	double tr, s;
	float mat[3][3];

	/* voor de netheid: op kopie werken */
	Mat3CpyMat3(mat, wmat);
	Mat3Ortho(mat);			/* dit moet EN op eind NormalQuat */
	
	tr= 0.25*(1.0+mat[0][0]+mat[1][1]+mat[2][2]);
	
	if(tr>0.00001) {
		s= sqrt( tr);
		q[0]= s;
		s*= 4.0;
		q[1]= (mat[1][2]-mat[2][1])/s;
		q[2]= (mat[2][0]-mat[0][2])/s;
		q[3]= (mat[0][1]-mat[1][0])/s;
	}
	else {
		q[0]= 0.0;
		s= -0.5*(mat[1][1]+mat[2][2]);
		
		if(s>0.00001) {
			s= sqrt(s);
			q[1]= s;
			q[2]= mat[0][1]/(2*s);
			q[3]= mat[0][2]/(2*s);
		}
		else {
			q[1]= 0.0;
			s= 0.5*(1.0-mat[2][2]);
			
			if(s>0.00001) {
				s= sqrt(s);
				q[2]= s;
				q[3]= mat[1][2]/(2*s);
			}
			else {
				q[2]= 0.0;
				q[3]= 1.0;
			}
		}
	}
	NormalQuat(q);
}

void Mat3ToQuat_is_ok(float wmat[][3], float *q)
{
	void Mat3Ortho();
	float mat[3][3], matr[3][3], matn[3][3], q1[4], q2[4], hoek, si, co, nor[3];

	/* voor de netheid: op kopie werken */
	Mat3CpyMat3(mat, wmat);
	Mat3Ortho(mat);
	
	/* roteer z-as matrix op z-as */

	nor[0] = mat[2][1];		/* uitprodukt met (0,0,1) */
	nor[1] =  -mat[2][0];
	nor[2] = 0.0;
	Normalise(nor);
	
	co= mat[2][2];
	hoek= 0.5*facos(co);
	
	co= fcos(hoek);
	si= fsin(hoek);
	q1[0]= co;
	q1[1]= -nor[0]*si;		/* hier negatief, waarom is een raadsel */
	q1[2]= -nor[1]*si;
	q1[3]= -nor[2]*si;

	/* roteer x-as van mat terug volgens inverse q1 */
	QuatToMat3(q1, matr);
	Mat3Inv(matn, matr);
	Mat3MulVecfl(matn, mat[0]);
	
	/* en zet de x-asssen gelijk */
	hoek= 0.5*fatan2(mat[0][1], mat[0][0]);
	
	co= fcos(hoek);
	si= fsin(hoek);
	q2[0]= co;
	q2[1]= 0.0;
	q2[2]= 0.0;
	q2[3]= si;
	
	QuatMul(q, q1, q2);
}


void Mat4ToQuat(float *m, float *q)
{
	float mat[3][3];
	
	Mat3CpyMat4(mat, m);
	Mat3ToQuat(mat, q);
	
}

void NormalQuat(float *q)
{
	float len;
	
	len= fsqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
	if(len!=0.0) {
		q[0]/= len;
		q[1]/= len;
		q[2]/= len;
		q[3]/= len;
	}
	
}

float *vectoquat(float *vec, short axis, short upflag)
{
	static float q1[4];
	float up[3], q2[4], nor[3], *fp, mat[3][3], hoek, si, co, x2, y2, z2, len1;
	
	/* eerst roteer naar as */
	if(axis>2) {	
		x2= vec[0] ; y2= vec[1] ; z2= vec[2];
		axis-= 3;
	}
	else {
		x2= -vec[0] ; y2= -vec[1] ; z2= -vec[2];
	}
	
	q1[0]=1.0; 
	q1[1]=q1[2]=q1[3]= 0.0;

	len1= fsqrt(x2*x2+y2*y2+z2*z2);
	if(len1 == 0.0) return(q1);

	if(axis==0) {	/* x-as */
		nor[0]= 0.0;
		nor[1]= -z2;
		nor[2]= y2;
		co= x2;
	}
	else if(axis==1) {	/* y-as */
		nor[0]= z2;
		nor[1]= 0.0;
		nor[2]= -x2;
		co= y2;
	}
	else {			/* z-as */
		nor[0]= -y2;
		nor[1]= x2;
		nor[2]= 0.0;
		co= z2;
	}
	co/= len1;
	Normalise(nor);
	
	hoek= 0.5*facos(co);
	co= fcos(hoek);
	si= fsin(hoek);
	q1[0]= co;
	q1[1]= nor[0]*si;
	q1[2]= nor[1]*si;
	q1[3]= nor[2]*si;

	if(axis!=upflag) {
		QuatToMat3(q1, mat);

		fp= mat[2];
		if(axis==0) {
			if(upflag==1) hoek= 0.5*fatan2(fp[2], fp[1]);
			else hoek= -0.5*fatan2(fp[1], fp[2]);
		}
		else if(axis==1) {
			if(upflag==0) hoek= -0.5*fatan2(fp[2], fp[0]);
			else hoek= 0.5*fatan2(fp[0], fp[2]);
		}
		else {
			if(upflag==0) hoek= 0.5*fatan2(-fp[1], -fp[0]);
			else hoek= -0.5*fatan2(-fp[0], -fp[1]);
		}
				
		co= fcos(hoek);
		si= fsin(hoek)/len1;
		q2[0]= co;
		q2[1]= x2*si;
		q2[2]= y2*si;
		q2[3]= z2*si;
			
		QuatMul(q1,q2,q1);
	}

	return(q1);
}

void VecUpMat3old(float *vec, float mat[][3], short axis)
{
	float inp, up[3];
	short cox, coy, coz;
	
	/* up varieeren heeft geen zin, is eigenlijk helemaal geen up!
	 */

	up[0]= 0.0;
	up[1]= 0.0;
	up[2]= 1.0;

	if(axis==0) {
		cox= 0; coy= 1; coz= 2;		/* Y up Z tr */
	}
	if(axis==1) {
		cox= 1; coy= 2; coz= 0;		/* Z up X tr */
	}
	if(axis==2) {
		cox= 2; coy= 0; coz= 1;		/* X up Y tr */
	}
	if(axis==3) {
		cox= 0; coy= 2; coz= 1;		/*  */
	}
	if(axis==4) {
		cox= 1; coy= 0; coz= 2;		/*  */
	}
	if(axis==5) {
		cox= 2; coy= 1; coz= 0;		/* Y up X tr */
	}

	mat[coz][0]= vec[0];
	mat[coz][1]= vec[1];
	mat[coz][2]= vec[2];
	Normalise((float *)mat[coz]);
	
	inp= mat[coz][0]*up[0] + mat[coz][1]*up[1] + mat[coz][2]*up[2];
	mat[coy][0]= up[0] - inp*mat[coz][0];
	mat[coy][1]= up[1] - inp*mat[coz][1];
	mat[coy][2]= up[2] - inp*mat[coz][2];

	Normalise((float *)mat[coy]);
	
	Crossf(mat[cox], mat[coy], mat[coz]);
	
}

void VecUpMat3(float *vec, float mat[][3], short axis)
{
	float inp;
	short cox, coy, coz;
	
	/* up varieeren heeft geen zin, is eigenlijk helemaal geen up!
	 * zie VecUpMat3old
	 */

	if(axis==0) {
		cox= 0; coy= 1; coz= 2;		/* Y up Z tr */
	}
	if(axis==1) {
		cox= 1; coy= 2; coz= 0;		/* Z up X tr */
	}
	if(axis==2) {
		cox= 2; coy= 0; coz= 1;		/* X up Y tr */
	}
	if(axis==3) {
		cox= 0; coy= 2; coz= 1;		/*  */
	}
	if(axis==4) {
		cox= 1; coy= 0; coz= 2;		/*  */
	}
	if(axis==5) {
		cox= 2; coy= 1; coz= 0;		/* Y up X tr */
	}

	mat[coz][0]= vec[0];
	mat[coz][1]= vec[1];
	mat[coz][2]= vec[2];
	Normalise((float *)mat[coz]);
	
	inp= mat[coz][2];
	mat[coy][0]= - inp*mat[coz][0];
	mat[coy][1]= - inp*mat[coz][1];
	mat[coy][2]= 1.0 - inp*mat[coz][2];

	Normalise((float *)mat[coy]);
	
	Crossf(mat[cox], mat[coy], mat[coz]);
	
}


/* **************** VIEW / PROJEKTIE ********************************  */


i_ortho(float left, float right, float bottom, float top, float near, float far, float mat[][4])
{
    float Xdelta, Ydelta, Zdelta;
 
    Xdelta = right - left;
    Ydelta = top - bottom;
    Zdelta = far - near;
    if (Xdelta == 0.0 || Ydelta == 0.0 || Zdelta == 0.0) {
		/* fprintf(stderr,"ortho: window width, height, or depth is 0.0\n"); */
		return;
    }
    Mat4One(mat);
    mat[0][0] = 2.0/Xdelta;
    mat[3][0] = -(right + left)/Xdelta;
    mat[1][1] = 2.0/Ydelta;
    mat[3][1] = -(top + bottom)/Ydelta;
    mat[2][2] = -2.0/Zdelta;		/* note: negate Z	*/
    mat[3][2] = -(far + near)/Zdelta;
}

i_window(float left, float right, float bottom, float top, float near, float far, float mat[][4])
{
	register float Xdelta, Ydelta, Zdelta;

	Xdelta = right - left;
	Ydelta = top - bottom;
	Zdelta = far - near;
	if (Xdelta == 0.0 || Ydelta == 0.0 || Zdelta == 0.0) {
		return;
	}
	mat[0][0] = near * 2.0/Xdelta;
	mat[1][1] = near * 2.0/Ydelta;
	mat[2][0] = (right + left)/Xdelta;		/* note: negate Z	*/
	mat[2][1] = (top + bottom)/Ydelta;
	mat[2][2] = -(far + near)/Zdelta;
	mat[2][3] = -1.0;
	mat[3][2] = (-2.0 * near * far)/Zdelta;
	mat[0][1] = mat[0][2] = mat[0][3] =
	    mat[1][0] = mat[1][2] = mat[1][3] =
	    mat[3][0] = mat[3][1] = mat[3][3] = 0.0;

}

i_translate(float Tx, float Ty, float Tz, float mat[][4])
{
    mat[3][0] += (Tx*mat[0][0] + Ty*mat[1][0] + Tz*mat[2][0]);
    mat[3][1] += (Tx*mat[0][1] + Ty*mat[1][1] + Tz*mat[2][1]);
    mat[3][2] += (Tx*mat[0][2] + Ty*mat[1][2] + Tz*mat[2][2]);
}

i_multmatrix(float icand[][4], float Vm[][4])
{
    register int row, col;
    float temp[4][4];

    for(row=0 ; row<4 ; row++) 
        for(col=0 ; col<4 ; col++)
            temp[row][col] = icand[row][0] * Vm[0][col]
                           + icand[row][1] * Vm[1][col]
                           + icand[row][2] * Vm[2][col]
                           + icand[row][3] * Vm[3][col];
	Mat4CpyMat4(Vm, temp);
}

i_rotate(float angle, char axis, float mat[][4])
{
    register int row,col;
    float temp[4];
    float cosine, sine;

    for(col=0; col<4 ; col++)	/* init temp to zero matrix */
        temp[col] = 0;

    angle = angle*(3.1415926535/180.0);
    cosine = fcos(angle);
    sine = fsin(angle);
    switch(axis){
    case 'x':    
    case 'X':    
        for(col=0 ; col<4 ; col++)
            temp[col] = cosine*mat[1][col] + sine*mat[2][col];
        for(col=0 ; col<4 ; col++) {
	    mat[2][col] = - sine*mat[1][col] + cosine*mat[2][col];
            mat[1][col] = temp[col];
	}
        break;

    case 'y':
    case 'Y':
        for(col=0 ; col<4 ; col++)
            temp[col] = cosine*mat[0][col] - sine*mat[2][col];
        for(col=0 ; col<4 ; col++) {
            mat[2][col] = sine*mat[0][col] + cosine*mat[2][col];
            mat[0][col] = temp[col];
        }
	break;

    case 'z':
    case 'Z':
        for(col=0 ; col<4 ; col++)
            temp[col] = cosine*mat[0][col] + sine*mat[1][col];
        for(col=0 ; col<4 ; col++) {
            mat[1][col] = - sine*mat[0][col] + cosine*mat[1][col];
            mat[0][col] = temp[col];
        }
	break;
    }
}

i_polarview(float dist, float azimuth, float incidence, float twist, float Vm[][4])
{

	Mat4One(Vm);

    i_translate(0.0, 0.0, -dist, Vm);
    i_rotate(-twist,'z', Vm);	
    i_rotate(-incidence,'x', Vm);
    i_rotate(-azimuth,'z', Vm);
}

i_lookat(float vx, float vy, float vz, float px, float py, float pz, float twist, float mat[][4])
{
	register float sine, cosine, hyp, hyp1, dx, dy, dz;
	float mat1[4][4];
	
	Mat4One(mat);
	Mat4One(mat1);

	i_rotate(-twist,'z', mat);

	dx = px - vx;
	dy = py - vy;
	dz = pz - vz;
	hyp = dx * dx + dz * dz;	/* hyp squared	*/
	hyp1 = sqrt(dy*dy + hyp);
	hyp = sqrt(hyp);		/* the real hyp	*/
	
	if (hyp1 != 0.0) {		/* rotate X	*/
		sine = -dy / hyp1;
		cosine = hyp /hyp1;
	} else {
		sine = 0;
		cosine = 1.0;
	}
	mat1[1][1] = cosine;
	mat1[1][2] = sine;
	mat1[2][1] = -sine;
	mat1[2][2] = cosine;
	
	i_multmatrix(mat1, mat);

    mat1[1][1] = mat1[2][2] = 1.0;	/* be careful here to reinit	*/
    mat1[1][2] = mat1[2][1] = 0.0;	/* those modified by the last	*/
	
	/* paragraph	*/
	if (hyp != 0.0) {			/* rotate Y	*/
		sine = dx / hyp;
		cosine = -dz / hyp;
	} else {
		sine = 0;
		cosine = 1.0;
	}
	mat1[0][0] = cosine;
	mat1[0][2] = -sine;
	mat1[2][0] = sine;
	mat1[2][2] = cosine;
	
	i_multmatrix(mat1, mat);
	i_translate(-vx,-vy,-vz, mat);	/* translate viewpoint to origin */
}





/* ************************************************  */

void Mat3Ortho(float mat[][3])
{	
	Normalise(mat[0]);
	Normalise(mat[1]);
	Normalise(mat[2]);
}

void Mat4Ortho(float mat[][4])
{
	float len;
	
	len= Normalise(mat[0]);
	if(len!=0.0) mat[0][3]/= len;
	len= Normalise(mat[1]);
	if(len!=0.0) mat[1][3]/= len;
	len= Normalise(mat[2]);
	if(len!=0.0) mat[2][3]/= len;
}

void VecCopyf(register float *v1, register float *v2)
{

	v1[0]= v2[0];
	v1[1]= v2[1];
	v1[2]= v2[2];
}

int VecLen(int *v1, int *v2)
{
	float x,y,z;

	x=v1[0]-v2[0];
	y=v1[1]-v2[1];
	z=v1[2]-v2[2];
	return ffloor(fsqrt(x*x+y*y+z*z));
}

float VecLenf(float *v1, float *v2)
{
	float x,y,z;

	x=v1[0]-v2[0];
	y=v1[1]-v2[1];
	z=v1[2]-v2[2];
	return fsqrt(x*x+y*y+z*z);
}

void VecAddf(register float *v, register float *v1, register float *v2)
{

	v[0]= v1[0]+ v2[0];
	v[1]= v1[1]+ v2[1];
	v[2]= v1[2]+ v2[2];
}

void VecSubf(register float *v, register float *v1, register float *v2)
{

	v[0]= v1[0]- v2[0];
	v[1]= v1[1]- v2[1];
	v[2]= v1[2]- v2[2];
}

void VecMidf(register float *v, register float *v1, register float *v2)
{

	v[0]= 0.5*(v1[0]+ v2[0]);
	v[1]= 0.5*(v1[1]+ v2[1]);
	v[2]= 0.5*(v1[2]+ v2[2]);
}

void VecMulf(float *v1, float f)
{

	v1[0]*= f;
	v1[1]*= f;
	v1[2]*= f;
}

void CalcNormShort(short *v1, short *v2, short *v3, float *n) /* is ook uitprodukt */
{
	float n1[3],n2[3];

	n1[0]= v1[0]-v2[0];
	n2[0]= v2[0]-v3[0];
	n1[1]= v1[1]-v2[1];
	n2[1]= v2[1]-v3[1];
	n1[2]= v1[2]-v2[2];
	n2[2]= v2[2]-v3[2];
	n[0]= n1[1]*n2[2]-n1[2]*n2[1];
	n[1]= n1[2]*n2[0]-n1[0]*n2[2];
	n[2]= n1[0]*n2[1]-n1[1]*n2[0];
	Normalise(n);
}

void CalcNormLong(int *v1, int *v2, int *v3, float *n)
{
	float n1[3],n2[3];

	n1[0]= v1[0]-v2[0];
	n2[0]= v2[0]-v3[0];
	n1[1]= v1[1]-v2[1];
	n2[1]= v2[1]-v3[1];
	n1[2]= v1[2]-v2[2];
	n2[2]= v2[2]-v3[2];
	n[0]= n1[1]*n2[2]-n1[2]*n2[1];
	n[1]= n1[2]*n2[0]-n1[0]*n2[2];
	n[2]= n1[0]*n2[1]-n1[1]*n2[0];
	Normalise(n);
}

float CalcNormFloat(float *v1, float *v2, float *v3, float *n)
{
	float n1[3],n2[3];

	n1[0]= v1[0]-v2[0];
	n2[0]= v2[0]-v3[0];
	n1[1]= v1[1]-v2[1];
	n2[1]= v2[1]-v3[1];
	n1[2]= v1[2]-v2[2];
	n2[2]= v2[2]-v3[2];
	n[0]= n1[1]*n2[2]-n1[2]*n2[1];
	n[1]= n1[2]*n2[0]-n1[0]*n2[2];
	n[2]= n1[0]*n2[1]-n1[1]*n2[0];
	return Normalise(n);
}

float CalcNormFloat4(float *v1, float *v2, float *v3, float *v4, float *n)
{
	/* real cross! */
	float n1[3],n2[3];

	n1[0]= v1[0]-v3[0];
	n1[1]= v1[1]-v3[1];
	n1[2]= v1[2]-v3[2];

	n2[0]= v2[0]-v4[0];
	n2[1]= v2[1]-v4[1];
	n2[2]= v2[2]-v4[2];

	n[0]= n1[1]*n2[2]-n1[2]*n2[1];
	n[1]= n1[2]*n2[0]-n1[0]*n2[2];
	n[2]= n1[0]*n2[1]-n1[1]*n2[0];

	return Normalise(n);
}


void CalcCent3f(float *cent, float *v1, float *v2, float *v3)
{

	cent[0]= 0.33333*(v1[0]+v2[0]+v3[0]);
	cent[1]= 0.33333*(v1[1]+v2[1]+v3[1]);
	cent[2]= 0.33333*(v1[2]+v2[2]+v3[2]);
}

void CalcCent4f(float *cent, float *v1, float *v2, float *v3, float *v4)
{

	cent[0]= 0.25*(v1[0]+v2[0]+v3[0]+v4[0]);
	cent[1]= 0.25*(v1[1]+v2[1]+v3[1]+v4[1]);
	cent[2]= 0.25*(v1[2]+v2[2]+v3[2]+v4[2]);
}

float Sqrt3f(float f)
{
	if(f==0.0) return 0;
	if(f<0) return -fexp(flog(-f)/3);
	else return fexp(flog(f)/3);
}

double Sqrt3d(double d)
{
	if(d==0.0) return 0;
	if(d<0) return -exp(log(-d)/3);
	else return exp(log(d)/3);
}
							 /* afstand v1 tot lijn v2-v3 */
float DistVL2Dfl(float *v1,float *v2,float *v3)   /* met formule van Hesse :GEEN LIJNSTUK! */
{
	float a[2],deler;

	a[0]= v2[1]-v3[1];
	a[1]= v3[0]-v2[0];
	deler= fsqrt(a[0]*a[0]+a[1]*a[1]);
	if(deler== 0.0) return 0;

	return fabs((v1[0]-v2[0])*a[0]+(v1[1]-v2[1])*a[1])/deler;

}

float PdistVL2Dfl(float *v1,float *v2,float *v3)  /* PointDist: WEL LIJNSTUK */
{
	float labda, rc[2], pt[2], len;
	
	rc[0]= v3[0]-v2[0];
	rc[1]= v3[1]-v2[1];
	len= rc[0]*rc[0]+ rc[1]*rc[1];
	if(len==0.0) {
		rc[0]= v1[0]-v2[0];
		rc[1]= v1[1]-v2[1];
		return fsqrt(rc[0]*rc[0]+ rc[1]*rc[1]);
	}
	
	labda= ( rc[0]*(v1[0]-v2[0]) + rc[1]*(v1[1]-v2[1]) )/len;
	if(labda<=0.0) {
		pt[0]= v2[0];
		pt[1]= v2[1];
	}
	else if(labda>=1.0) {
		pt[0]= v3[0];
		pt[1]= v3[1];
	}
	else {
		pt[0]= labda*rc[0]+v2[0];
		pt[1]= labda*rc[1]+v2[1];
	}

	rc[0]= pt[0]-v1[0];
	rc[1]= pt[1]-v1[1];
	return fsqrt(rc[0]*rc[0]+ rc[1]*rc[1]);
}

float AreaF2Dfl(float *v1,float *v2,float *v3)
{
	return .5*fabs( (v1[0]-v2[0])*(v2[1]-v3[1]) + (v1[1]-v2[1])*(v3[0]-v2[0]) );
}


float AreaQ3Dfl(float *v1,float *v2,float *v3, float *v4)  /* only convex Quadrilaterals */
{
	float len, vec1[3], vec2[3], n[3];

	VecSubf(vec1, v2, v1);
	VecSubf(vec2, v4, v1);
	Crossf(n, vec1, vec2);
	len= Normalise(n);

	VecSubf(vec1, v4, v3);
	VecSubf(vec2, v2, v3);
	Crossf(n, vec1, vec2);
	len+= Normalise(n);

	return (len/2.0);
}

float AreaT3Dfl(float *v1,float *v2,float *v3)  /* Triangles */
{
	float len, vec1[3], vec2[3], n[3];

	VecSubf(vec1, v3, v2);
	VecSubf(vec2, v1, v2);
	Crossf(n, vec1, vec2);
	len= Normalise(n);

	return (len/2.0);
}

#define MAX2(x,y)		( (x)>(y) ? (x) : (y) )
#define MAX3(x,y,z)		MAX2( MAX2((x),(y)) , (z) )

float AreaPoly3Dfl(int nr, float *verts, float *normal)
{
	float x, y, z, area, max, *cur, *prev;
	int a, px=0, py=1;

	/* first: find dominant axis: 0==X, 1==Y, 2==Z */
	x= fabs(normal[0]);
	y= fabs(normal[1]);
	z= fabs(normal[2]);
	max = MAX3(x, y, z);
	if(max==y) py=2;
	else if(max==x) {
		px=1; 
		py= 2;
	}

	/* The Trapezium Area Rule */
	prev= verts+3*(nr-1);
	cur= verts;
	area= 0;
	for(a=0; a<nr; a++) {
		area+= (cur[px]-prev[px])*(cur[py]+prev[py]);
		prev= cur;
		cur+=3;
	}

	return fabs(0.5*area/max);
}

void MinMax3(float *min, float *max, float *vec)
{
	if(min[0]>vec[0]) min[0]= vec[0];
	if(min[1]>vec[1]) min[1]= vec[1];
	if(min[2]>vec[2]) min[2]= vec[2];

	if(max[0]<vec[0]) max[0]= vec[0];
	if(max[1]<vec[1]) max[1]= vec[1];
	if(max[2]<vec[2]) max[2]= vec[2];
}

/* ************ EULER *************** */

void EulToMat3(float *eul, float mat[][3])
{
	float ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	
	ci = fcos(eul[0]); 
	cj = fcos(eul[1]); 
	ch = fcos(eul[2]);
	si = fsin(eul[0]); 
	sj = fsin(eul[1]); 
	sh = fsin(eul[2]);
	cc = ci*ch; 
	cs = ci*sh; 
	sc = si*ch; 
	ss = si*sh;

	mat[0][0] = cj*ch; 
	mat[1][0] = sj*sc-cs; 
	mat[2][0] = sj*cc+ss;
	mat[0][1] = cj*sh; 
	mat[1][1] = sj*ss+cc; 
	mat[2][1] = sj*cs-sc;
	mat[0][2] = -sj;	 
	mat[1][2] = cj*si;    
	mat[2][2] = cj*ci;

}

void EulToMat4(float *eul, float mat[][4])
{
	float ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	
	ci = fcos(eul[0]); 
	cj = fcos(eul[1]); 
	ch = fcos(eul[2]);
	si = fsin(eul[0]); 
	sj = fsin(eul[1]); 
	sh = fsin(eul[2]);
	cc = ci*ch; 
	cs = ci*sh; 
	sc = si*ch; 
	ss = si*sh;

	mat[0][0] = cj*ch; 
	mat[1][0] = sj*sc-cs; 
	mat[2][0] = sj*cc+ss;
	mat[0][1] = cj*sh; 
	mat[1][1] = sj*ss+cc; 
	mat[2][1] = sj*cs-sc;
	mat[0][2] = -sj;	 
	mat[1][2] = cj*si;    
	mat[2][2] = cj*ci;

	mat[3][0]= mat[3][1]= mat[3][2]= mat[0][3]= mat[1][3]= mat[2][3]= 0.0;
	mat[3][3]= 1.0;
}


void Mat3ToEul(float tmat[][3], float *eul)
{
	float cy, quat[4], mat[3][3];
	
	Mat3ToQuat(tmat, quat);
	QuatToMat3(quat, mat);
	Mat3CpyMat3(mat, tmat);
	Mat3Ortho(mat);
	
	cy = fsqrt(mat[0][0]*mat[0][0] + mat[0][1]*mat[0][1]);

	if (cy > 16.0*FLT_EPSILON) {
		eul[0] = fatan2(mat[1][2], mat[2][2]);
		eul[1] = fatan2(-mat[0][2], cy);
		eul[2] = fatan2(mat[0][1], mat[0][0]);
	} else {
		eul[0] = fatan2(-mat[2][1], mat[1][1]);
		eul[1] = fatan2(-mat[0][2], cy);
		eul[2] = 0.0;
	}
}

void Mat3ToEuln(float tmat[][3], float *eul)
{
	float sin1, cos1, sin2, cos2, sin3, cos3;
	
	sin1 = -tmat[2][0];
	cos1 = fsqrt(1 - sin1*sin1);

	if ( fabs(cos1) > FLT_EPSILON ) {
		sin2 = tmat[2][1] / cos1;
		cos2 = tmat[2][2] / cos1;
		sin3 = tmat[1][0] / cos1;
		cos3 = tmat[0][0] / cos1;
    } 
	else {
		sin2 = -tmat[1][2];
		cos2 = tmat[1][1];
		sin3 = 0.0;
		cos3 = 1.0;
    }
	
	eul[0] = fatan2(sin3, cos3);
	eul[1] = fatan2(sin1, cos1);
	eul[2] = fatan2(sin2, cos2);

PRINT3(f, f, f, eul[0], eul[1], eul[2]);
} 


void QuatToEul(float *quat, float *eul)
{
	float mat[3][3];
	
	QuatToMat3(quat, mat);
	Mat3ToEul(mat, eul);
}

void EulToQuat(float *eul, float *quat)
{
    float ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
 
    ti = eul[0]*0.5; tj = eul[1]*0.5; th = eul[2]*0.5;
    ci = fcos(ti);  cj = fcos(tj);  ch = fcos(th);
    si = fsin(ti);  sj = fsin(tj);  sh = fsin(th);
    cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
	
	quat[0] = cj*cc + sj*ss;
	quat[1] = cj*sc - sj*cs;
	quat[2] = cj*ss + sj*cc;
	quat[3] = cj*cs - sj*sc;
}

void VecRotToMat3(float *vec, float phi, float mat[][3])
{
	/* rotatie van phi radialen rond vec */
	float vx, vx2, vy, vy2, vz, vz2, co, si;
	
	vx= vec[0];
	vy= vec[1];
	vz= vec[2];
	vx2= vx*vx;
	vy2= vy*vy;
	vz2= vz*vz;
	co= fcos(phi);
	si= fsin(phi);
	
	mat[0][0]= vx2+co*(1.0-vx2);
	mat[0][1]= vx*vy*(1.0-co)+vz*si;
	mat[0][2]= vz*vx*(1.0-co)-vy*si;
	mat[1][0]= vx*vy*(1.0-co)-vz*si;
	mat[1][1]= vy2+co*(1.0-vy2);
	mat[1][2]= vy*vz*(1.0-co)+vx*si;
	mat[2][0]= vz*vx*(1.0-co)+vy*si;
	mat[2][1]= vy*vz*(1.0-co)-vx*si;
	mat[2][2]= vz2+co*(1.0-vz2);
	
}

void euler_rot(float *beul, float ang, char axis)
{
	float eul[3], mat1[3][3], mat2[3][3], totmat[3][3];
	
	eul[0]= eul[1]= eul[2]= 0.0;
	if(axis=='x') eul[0]= ang;
	else if(axis=='y') eul[1]= ang;
	else eul[2]= ang;
	
	EulToMat3(eul, mat1);
	EulToMat3(beul, mat2);
	
	Mat3MulMat3(totmat, mat2, mat1);
	
	Mat3ToEul(totmat, beul);
	
}



void SizeToMat3(float *size, float mat[][3])
{
	mat[0][0]= size[0];
	mat[0][1]= 0.0;
	mat[0][2]= 0.0;
	mat[1][1]= size[1];
	mat[1][0]= 0.0;
	mat[1][2]= 0.0;
	mat[2][2]= size[2];
	mat[2][1]= 0.0;
	mat[2][0]= 0.0;
}

void Mat3ToSize(float mat[][3], float *size)
{
	float vec[3];


	VecCopyf(vec, mat[0]);
	size[0]= Normalise(vec);
	VecCopyf(vec, mat[1]);
	size[1]= Normalise(vec);
	VecCopyf(vec, mat[2]);
	size[2]= Normalise(vec);
}

void Mat4ToSize(float mat[][4], float *size)
{
	float vec[3];
	

	VecCopyf(vec, mat[0]);
	size[0]= Normalise(vec);
	VecCopyf(vec, mat[1]);
	size[1]= Normalise(vec);
	VecCopyf(vec, mat[2]);
	size[2]= Normalise(vec);
}

/* ************* SPECIALS ******************* */

void triatoquat(float *v1, float *v2, float *v3, float *quat)
{
	/* denkbeeldige x-as, y-as driehoek wordt geroteerd */
	float vec[3], q1[4], q2[4], n[3], si, co, hoek, mat[3][3], imat[3][3];
	
	/* eerst z-as op vlaknormaal */
	CalcNormFloat(v1, v2, v3, vec);

	n[0]= vec[1];
	n[1]= -vec[0];
	n[2]= 0.0;
	Normalise(n);
	
	if(n[0]==0.0 && n[1]==0.0) n[0]= 1.0;
	
	hoek= -0.5*facos(vec[2]);
	co= fcos(hoek);
	si= fsin(hoek);
	q1[0]= co;
	q1[1]= n[0]*si;
	q1[2]= n[1]*si;
	q1[3]= 0.0;
	
	/* v1-v2 lijn terug roteren */
	QuatToMat3(q1, mat);
	Mat3Inv(imat, mat);
	VecSubf(vec, v2, v1);
	Mat3MulVecfl(imat, vec);

	/* welke hoek maakt deze lijn met x-as? */
	vec[2]= 0.0;
	Normalise(vec);

	hoek= 0.5*fatan2(vec[1], vec[0]);
	co= fcos(hoek);
	si= fsin(hoek);
	q2[0]= co;
	q2[1]= 0.0;
	q2[2]= 0.0;
	q2[3]= si;
	
	QuatMul(quat, q1, q2);
}

float Spec(float inp, int spec)
{
	register float b1;

	if(inp>=1.0) return 1.0;
	
	b1=inp*inp;
	if((spec & 1)==0)  inp= 1.0;
	if(spec & 2)  inp*=b1;
	b1*=b1;
	if(spec & 4)  inp*=b1;
	b1*=b1;
	if(spec & 8)  inp*=b1;
	b1*=b1;
	if(spec & 16) inp*=b1;
	b1*=b1;
	if(spec & 32) inp*=b1;
	if(spec & 64) {
		b1*=b1;
		inp*=b1;
	}
	return inp;
}

void MinMaxRGB(c)
short c[];
{
	if(c[0]>255) c[0]=255;
	else if(c[0]<0) c[0]=0;
	if(c[1]>255) c[1]=255;
	else if(c[1]<0) c[1]=0;
	if(c[2]>255) c[2]=255;
	else if(c[2]<0) c[2]=0;
}

void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b)
{
	int i;
	float f, p, q, t;

	h *= 360.0;
	
	if(s==0) {
		*r = v;
		*g = v;
		*b = v;
	}
	else {
		if(h==360) h = 0;
		
		h /= 60;
		i = ffloor(h);
		f = h - i;
		p = v*(1.0-s);
		q = v*(1.0-(s*f));
		t = v*(1.0-(s*(1.0-f)));
		
		switch (i) {
		case 0 :
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1 :
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2 :
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3 :
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4 :
			*r = t;
			*g = p;
			*b = v;
			break;
		case 5 :
			*r = v;
			*g = p;
			*b = q;
			break;
		}
	}
}

void rgb_to_hsv(float r, float g, float b, float *lh, float *ls, float *lv)
{
	float h, s, v;
	float cmax, cmin, cdelta;
	float rc, gc, bc;

	cmax = r;
	cmin = r;
	cmax = (g>cmax ? g:cmax);
	cmin = (g<cmin ? g:cmin);
	cmax = (b>cmax ? b:cmax);
	cmin = (b<cmin ? b:cmin);

	v = cmax;		/* value */
	if (cmax!=0.0)
		s = (cmax - cmin)/cmax;
	else {
		s = 0.0;
		h = 0.0;
	}
	if (s == 0.0)
		h = -1.0;
	else {
		cdelta = cmax-cmin;
		rc = (cmax-r)/cdelta;
		gc = (cmax-g)/cdelta;
		bc = (cmax-b)/cdelta;
		if (r==cmax)
			h = bc-gc;
		else
			if (g==cmax)
				h = 2.0+rc-bc;
			else
				h = 4.0+gc-rc;
		h = h*60.0;
		if (h<0.0)
			h += 360.0;
	}
	
	*ls = s;
	*lh = h/360.0;
	if( *lh < 0.0) *lh= 0.0;
	*lv = v;
}

/* Bij afspraak is cpack een getal dat als 0xFFaa66 of zo kan worden uitgedrukt. Is dus gevoelig voor endian. 
 * Met deze define wordt het altijd goed afgebeeld
 */

uint hsv_to_cpack(float h, float s, float v)
{
	short r, g, b;
	float rf, gf, bf;
	uint col;
	
	hsv_to_rgb(h, s, v, &rf, &gf, &bf);
	
	r= rf*255.0;
	g= gf*255.0;
	b= bf*255.0;
	
	col= ( r + (g*256) + (b*256*256) );
	return col;
}

uint rgb_to_cpack(float r, float g, float b)
{
	int ir, ig, ib;
	
	ir= ffloor(255.0*r);
	if(ir<0) ir= 0; else if(ir>255) ir= 255;
	ig= ffloor(255.0*g);
	if(ig<0) ig= 0; else if(ig>255) ig= 255;
	ib= ffloor(255.0*b);
	if(ib<0) ib= 0; else if(ib>255) ib= 255;
	
	return (ir+ (ig*256) + (ib*256*256));
	
}

void cpack_to_rgb(uint col, float *r, float *g, float *b)
{
	
	*r= ((col)&0xFF);
	*r /= 255.0;

	*g= (((col)>>8)&0xFF);
	*g /= 255.0;

	*b= (((col)>>16)&0xFF);
	*b /= 255.0;
}



/* ************ NIEUWE QSORT ********************** */

int (*vergfunc)();
int vergsize, temppivot[40];


int partitie(int *poin, int n, int *pivot)
{
	int i=0, j= n-1, k, t1;
	int *next;
	
	k= 0;
	while(k<vergsize) {
		temppivot[k]= pivot[k];
		k++;
	}
	
	next= poin+ vergsize*(n-1);
	
	while(i<=j) {
		while( vergfunc(poin, temppivot)== -1 ) {
			i++;
			poin+= vergsize;
		}
		while( vergfunc(next, temppivot)!= -1 ) {
			j--;
			next-= vergsize;
		}
		if(i<j) {
			/* wissel */
			if(vergsize==1) {
				t1= *poin;
				*poin= *next;
				*next= t1;
				poin++;
				next--;
			}
			else if(vergsize==2) {
				t1= poin[0];
				poin[0]= next[0];
				next[0]= t1;
				t1= poin[1];
				poin[1]= next[1];
				next[1]= t1;
				poin+= 2;
				next-= 2;
			}
			else {
				k= 0;
				while(k<vergsize) {
					t1= poin[k];
					poin[k]= next[k];
					next[k]= t1;
					k++;
				}
				poin+=vergsize;
				next-=vergsize;
			}
			i++;
			j--;
		}
	}
	return i;
}

void qsortNN(int *poin, int n)
{
	int i, k, ok=0;
	int *pivot, *next;
	
	/* zoek pivot */
	if(n<3) {
		if(n==2) {
			next= poin+vergsize;
			k= vergfunc(poin, next);
			if(k==1) {
				k= 0;
				while(k<vergsize) {
					i= poin[k];
					poin[k]= next[k];
					next[k]= i;
					k++;
				}
			}
		}
		return;
	}
	else {
		next= poin+vergsize;
		for(i=1; i<n; ++i, next+=vergsize) {
			k= vergfunc(poin, next);
			if(k== 1) {
				pivot= poin;
				ok= 1;
				break;
			}
			else if(k== -1) {
				pivot= next;
				ok= 1;
				break;
			}
		}

		if( ok ) {
			k= partitie(poin, n, pivot);
			qsortNN(poin, k);
			qsortNN(poin+vergsize*k, n-k);
		}
	}
}

void qsortN(void *poin, int n, int size, int (*func)())
{

	vergfunc= func;
	vergsize= size/4;

	if(4*vergsize!=size) {
		printf("wrong size in qsortN\n");
		return;
	}

	qsortNN(poin, n);
}

/* ********** PREFAB QSORT: qsortN2(int *data, int nr) , sorteert setjes van 2 ints (bv. edges) ********* */


#define VERGLONG2(e1, e2)	( (e1[0] > e2[0])? 1 : ( (e1[0] < e2[0]) ? -1 : ( (e1[1] > e2[1]) ? 1 : ( (e1[1] < e2[1]) ? -1 : 0 ) ) ) )


int partitie2(int *poin, int n, int *pivot)
{
	int i=0, j= n-1, k, t1;
	int *next;
	
	temppivot[0]= pivot[0];
	temppivot[1]= pivot[1];
	
	next= poin+ 2*(n-1);
	
	while(i<=j) {
		while( VERGLONG2(poin, temppivot)== -1 ) {
			i++;
			poin+= 2;
		}
		while( VERGLONG2(next, temppivot)!= -1 ) {
			j--;
			next-= 2;
		}
		if(i<j) {
			t1= poin[0];
			poin[0]= next[0];
			next[0]= t1;
			t1= poin[1];
			poin[1]= next[1];
			next[1]= t1;
			poin+= 2;
			next-= 2;
			i++;
			j--;
		}
	}
	return i;
}

void qsortN2(int *poin, int n)
{
	int i, k, ok=0;
	int *pivot, *next;
	
	/* zoek pivot */
	if(n<3) {
		if(n==2) {
			next= poin+2;
			k= VERGLONG2(poin, next);
			if(k==1) {
				i= poin[0];
				poin[0]= next[0];
				next[0]= i;
				i= poin[1];
				poin[1]= next[1];
				next[1]= i;
			}
		}
	}
	else {
		next= poin+2;
		for(i=1; i<n; ++i, next+=2) {
			k= VERGLONG2(poin, next);
			if(k== 1) {
				pivot= poin;
				ok= 1;
				break;
			}
			else if(k== -1) {
				pivot= next;
				ok= 1;
				break;
			}
		}

		if( ok ) {
			k= partitie2(poin, n, pivot);
			qsortN2(poin, k);
			qsortN2(poin+2*k, n-k);
		}
	}
}

