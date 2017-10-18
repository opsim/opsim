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

#include "imbuf.h"


#define OBJECTBLOK "delta"


/************************************************************************/
/*				DELTA'S					*/
/************************************************************************/


void addbpl(struct ImBuf *ibuf1, struct ImBuf *ibuf2)
{
	int size,d;
	uint **list1,**list2;

	size = ibuf1->y * ibuf1->skipx;
	list1 = ibuf1->planes;
	list2 = ibuf2->planes;

	for (d = ibuf1->depth;d>0;d-=8){
		int offset;

		for (offset=0; offset<size; offset++){
			int i;
			register int rest;
			rest=0;

			for (i=0;i<8;i++){
				register int data1,data2,restt;

				data1=(list1[i])[offset];
				data2=(list2[i])[offset];

				restt = data1 & rest;
				data1 = data1 ^ rest;

				rest = data1 & data2;
				(list1[i])[offset] = data1 ^ data2;

				rest |= restt;
			}
		}
		list1 +=8;
		list2 +=8;
	}
}

void subbpl(struct ImBuf *ibuf1, struct ImBuf *ibuf2)
{
	int size,d;
	uint **list1,**list2;

	size = ibuf1->y * ibuf1->skipx;
	list1 = ibuf1->planes;
	list2 = ibuf2->planes;

	for (d = ibuf1->depth;d>0;d-=8){
		int offset;

		for (offset=0; offset<size; offset++){
			int i;
			register int rest;
			rest=0;

			for (i=0;i<8;i++){
				register int data1,data2,restt;

				data1=(list1[i])[offset];
				data2=(list2[i])[offset];

				restt = ~(data1) & rest;
				data1 = data1 ^ rest;

				rest = ~(data1) & data2;
				(list1[i])[offset] = data1 ^ data2;

				rest |= restt;
			}
		}
		list1 +=8;
		list2 +=8;
	}
}


void swapbitsbpl(struct ImBuf *ibuf)
{
	int size,d;
	uint **list;

	size = ibuf->y * ibuf->skipx;
	list = ibuf->planes;

	for (d = ibuf->depth;d>0;d-=8){
		int offset;

		for (offset=0; offset<size; offset++){
			int i,data1,data2;

			data1=(list[0])[offset];
			for (i=0;i<7;i++){
				data2=(list[i+1])[offset];
				(list[i])[offset] = data2 ^ data1;
				data1=data2;
			}
		}
		list +=8;
	}
}


void deswapbitsbpl(struct ImBuf *ibuf)
{
	int size,d;
	uint **list;

	size = ibuf->y * ibuf->skipx;
	list = ibuf->planes;

	for (d = ibuf->depth;d>0;d-=8){
		int offset;

		for (offset=0; offset<size; offset++){
			int i,data1,data2;

			data1=(list[7])[offset];
			for (i=6;i>=0;i--){
				data2=(list[i])[offset];
				data2 ^= data1;
				(list[i])[offset] = data2;
				data1=data2;
			}
		}
		list +=8;
	}
}

void rectxorn(int *drect, int *srect, int x)
{
	for(;x>0;x--){
		*drect++ ^= *srect++;
	}
}

short makedelta(char *pic1,char *pic2,char *delta)
{
	struct ImBuf *ibuf1,*ibuf2;
	extern void rectxor();
	extern void rectsub();
	short ok;

	ok=FALSE;

	ibuf1=loadiffname(pic1,IB_cmap);
	if (ibuf1){
		ibuf2=loadiffname(pic2,IB_cmap);
		if (ibuf2){
			if ((ibuf1->x == ibuf2->x) && (ibuf1->y == ibuf2->y) && (ibuf1->depth == ibuf2->depth)){
				if ((ibuf1->planes != 0) && (ibuf2->planes !=0)){
					subbpl(ibuf1,ibuf2);
					swapbitsbpl(ibuf1);
					if (saveiff(ibuf1,delta,IB_vert | IB_cmap)==0){
						fprintf(stderr,"Makedelta: Couldn't save delta:%s\n",delta);
					} else ok=TRUE;
				} else if ((ibuf1->rect != 0) && (ibuf2->rect != 0)){
					rectoptot(ibuf1,ibuf2,rectsub);
					if (saveiff(ibuf1,delta,IB_rect | IB_cmap)==0){
						fprintf(stderr,"Makedelta: Couldn't save delta:%s\n",delta);
					} else ok=TRUE;
				} else{
					fprintf(stderr,"Makedelta: %s, %s incompatible types\n",pic1,pic2);
				}
			} else{
				fprintf(stderr,"Makedelta: %s, %s different sizes\n",pic1,pic2);
			}
			freeImBuf(ibuf2);
		} else{
			fprintf(stderr,"Makedelta: Couldn't load pic: %s\n",pic2);
		}
		freeImBuf(ibuf1);
	} else{
		fprintf(stderr,"Makedelta: Couldn't load pic: %s\n",pic1);
	}

	return (ok);
}

