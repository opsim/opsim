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

#include <stdlib.h>

#include <be/kernel/image.h>

void *dlopen (const char *name, int mode) {
	image_id handle;
	
	printf ("Loading image <%s>\n", name);
	handle= load_add_on(name);
	printf ("handle is %d\n", handle);
	
	return (void*) handle;
}

void *dlsym (void *vhandle, const char *symname) {
	void *sym;
	image_id handle= (image_id) vhandle;
	status_t err;
	
	printf ("handle is %d\n", handle);
	
	printf ("Getting symbol <%s>\n", symname);
	err= get_image_symbol(handle, symname, B_SYMBOL_TYPE_TEXT, &sym);
	
	printf ("err is %d\n", err);
	printf ("sym is %x\n", sym);

	err= get_image_symbol(handle, "plugin_func", B_SYMBOL_TYPE_TEXT, &sym);
	
	printf ("err is %d\n", err);
	printf ("sym is %x\n", sym);
	
	return sym;
}

int dlclose (void *vhandle) {
	int handle= (int) vhandle;

	return unload_add_on(handle);
}

char *dlerror(void) {
	return NULL;
}


