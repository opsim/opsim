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

 * jchuff.h
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains declarations for Huffman entropy encoding routines
 * that are shared between the sequential encoder (jchuff.c) and the
 * progressive encoder (jcphuff.c).  No other modules need to see these.
 */

/* Derived data constructed for each Huffman table */

typedef struct {
  unsigned int ehufco[256];	/* code for each symbol */
  char ehufsi[256];		/* length of code for each symbol */
  /* If no code has been allocated for a symbol S, ehufsi[S] contains 0 */
} c_derived_tbl;

/* Short forms of external names for systems with brain-damaged linkers. */

#ifdef NEED_SHORT_EXTERNAL_NAMES
#define jpeg_make_c_derived_tbl	jMkCDerived
#define jpeg_gen_optimal_table	jGenOptTbl
#endif /* NEED_SHORT_EXTERNAL_NAMES */

/* Expand a Huffman table definition into the derived format */
EXTERN(void) jpeg_make_c_derived_tbl
	JPP((j_compress_ptr cinfo, JHUFF_TBL * htbl, c_derived_tbl ** pdtbl));

/* Generate an optimal table definition given the specified counts */
EXTERN(void) jpeg_gen_optimal_table
	JPP((j_compress_ptr cinfo, JHUFF_TBL * htbl, long freq[]));

