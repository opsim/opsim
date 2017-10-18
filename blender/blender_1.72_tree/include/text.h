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

/* text.h    april 99

 * 
 * 
 */

#ifndef TEXT_H
#define TEXT_H

#define TXT_OFFSET 35
#define TXT_TABSIZE	4
#define TXT_INIT_UNDO 1024
#define TXT_MAX_UNDO	(TXT_INIT_UNDO*TXT_INIT_UNDO)

/* text flags */
#define TXT_ISDIRTY		1
#define TXT_ISTMP		2
#define TXT_ISMEM		4
#define TXT_ISEXT		8
#define TXT_ISSCRIPT	16

/* spacetext flags */
#define ST_SCROLL_SELECT	1

/* Undo opcodes */

/* Simple main cursor movement */
#define UNDO_CLEFT		001
#define UNDO_CRIGHT		002
#define UNDO_CUP		003
#define UNDO_CDOWN		004

/* Simple selection cursor movement */
#define UNDO_SLEFT		005
#define UNDO_SRIGHT		006
#define UNDO_SUP		007
#define UNDO_SDOWN		021

/* Complex movement (opcode is followed
 * by 4 character line ID + a 2 character
 * position ID and opcode (repeat)) */
#define UNDO_CTO		022
#define UNDO_STO		023

/* Complex editing (opcode is followed
 * by 1 character ID and opcode (repeat)) */
#define UNDO_INSERT		024
#define UNDO_BS			025
#define UNDO_DEL		026

/* Text block (opcode is followed
 * by 4 character length ID + the text
 * block itself + the 4 character length
 * ID (repeat) and opcode (repeat)) */
#define UNDO_DBLOCK		027 /* Delete block */
#define UNDO_IBLOCK		030 /* Insert block */

/* Misc */
#define UNDO_SWAP		031	/* Swap cursors */

#endif /* TEXT_H */

