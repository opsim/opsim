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


#define OBJECTBLOK "writeimage"

short saveiff(struct ImBuf *ibuf,char *naam,int flags)
{	
	short ok=TRUE,delpl=FALSE;
	int file;

	if (ibuf==0) return (FALSE);
	ibuf->flags = flags;

	if (IS_jpg(ibuf)) {
		if (save_jpeg(naam, ibuf, flags)) return (0);
		else return (TRUE);
	}
	file = open(naam, O_BINARY | O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (file<=0) return (FALSE);

	if (flags & IB_rect){
		if (ibuf->cmap){
			checkncols(ibuf);
		}
	}

	if (IS_tga(ibuf)) {
		ok = savetarga(ibuf,file,flags);
		if (ok) {
			close (file);
			return (ok);
		}
	}
	
	if (IS_iris(ibuf)) {
		ok = saveiris(ibuf,file,flags);
		if (ok) {
			close (file);
			return (ok);
		}
	}
	
	if (IS_tim(ibuf)) {
		ok = savetim(ibuf,file,flags);
		if (ok) {
			close (file);
			return (ok);
		}
	}

	if (ok) ok=start_iff(ibuf,file,flags);

	if (IS_amiga(ibuf)){
		flipy(ibuf);
		if (flags & IB_rect){
			if ((flags & IB_cmap) == 0) {
				if (IS_ham(ibuf)){
					if (ok) ok=converttoham(ibuf);
				}else if (ibuf->cmap){
					if (ok) ok=converttocmap(ibuf);
				}
			}
			if (ok){
				if (ibuf->planes==0){
					delpl=TRUE;
					ok=addplanesImBuf(ibuf);
				}
				longtobp(ibuf);
			}
		}

		if (flags & IB_vert){
			if (ok) ok=encodebodyv(ibuf,file);
		}
		else{
			if (ok) ok=encodebodyh(ibuf,file);
		}
		if (ok) ok=update_iff(file,BODY);
	}else if (IS_cdi(ibuf)){
		flipy(ibuf);
		if (IS_rl7(ibuf)){
			if ((flags & IB_cmap)==0){
				if (ok) ok=converttocmap(ibuf);
			}
			if (ok) ok=enc_rl7(ibuf,file);
		} else if (IS_rl3(ibuf)){
			if ((flags & IB_cmap)==0){
				if (ok) ok=converttocmap(ibuf);
			}
			if (ok) ok=enc_rl3(ibuf,file);
		} else if (IS_dyuv(ibuf)){
			if ((flags & IB_cmap)){
				if (ok) applycmap(ibuf);
			}
			if (ok) ok=enc_dyuv(ibuf,file);
		}
#ifndef CODIM
		if (ok) ok=update_iff(file,IDAT);
#endif
	}else if (IS_anim(ibuf)) {
		if (ok) ok = enc_anim(ibuf, file);
		if (ok) ok = update_iff(file, BODY);
	}
	close(file);
	if (ok==FALSE) {
		/*remove(naam);*/
		fprintf(stderr,"Couldn't save picture.\n");
		/*fprintf(stderr,"Destination file removed.\n");*/
	}	
	if (delpl) freeplanesImBuf(ibuf);

	return (ok);
}


