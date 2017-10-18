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


/* cc perftest.c -o perftest */

int main(argc,argv)
int argc;
char **argv;
{
	int a=5000000;
	
	if(argc==2 && argv[1][0]=='f') {
		float val1=2.0, val2=1.3, val3=1230.4;
		
		while(a--) {
			val1*= (val2*val2 + val3)/(val2);
			val1/= (val2*val2 + val3)/(val2);
		}
		printf("float\n");
		
	}
	if(argc==2 && argv[1][0]=='i') {
		float val1=2;
		int val2=13, val3=1230;
		
		while(a--) {
			val1*= (val2*val2 + val3)/(val2);
			val1/= (val2*val2 + val3)/(val2);
		}
		printf("mixed int\n");
		
	}
	if(argc==2 && argv[1][0]=='d') {
		double val1=2.0, val2=1.3, val3=1230.4;
		
		while(a--) {
			val1*= (val2*val2 + val3)/(val2);
			val1/= (val2*val2 + val3)/(val2);
		}
		printf("double\n");
		
	}
}



