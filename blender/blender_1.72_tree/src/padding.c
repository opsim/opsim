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

#include "blender.h"

#include "util.h"
#include "screen.h"
#include "file.h"
#include "sequence.h"
#include "effect.h"
#include "sector.h"
#include "ika.h"
#include "oops.h"
#include "imasel.h"
#include "bpaint.h"
#include "psx.h"
#include "iff.h"


int main(argc,argv)
int argc;
char **argv;
{
printf("  Link 8 %d \n",  sizeof(struct Link) );
printf("  ListBase 8 %d \n",  sizeof(struct ListBase) );
printf("  MemHead 32 %d \n",  sizeof(struct MemHead) );
printf("  MemTail 8 %d \n",  sizeof(struct MemTail) );
printf("  vec2s 4 %d \n",  sizeof(struct vec2s) );
printf("  vec2i 8 %d \n",  sizeof(struct vec2i) );
printf("  vec2f 8 %d \n",  sizeof(struct vec2f) );
printf("  vec2d 16 %d \n",  sizeof(struct vec2d) );
printf("  vec3i 12 %d \n",  sizeof(struct vec3i) );
printf("  vec3f 12 %d \n",  sizeof(struct vec3f) );
printf("  vec3d 24 %d \n",  sizeof(struct vec3d) );
printf("  vec4i 16 %d \n",  sizeof(struct vec4i) );
printf("  vec4f 16 %d \n",  sizeof(struct vec4f) );
printf("  vec4d 32 %d \n",  sizeof(struct vec4d) );
printf("  rcti 16 %d \n",  sizeof(struct rcti) );
printf("  rctf 16 %d \n",  sizeof(struct rctf) );
printf("  ID 48 %d \n",  sizeof(struct ID) );
printf("  Library 144 %d \n",  sizeof(struct Library) );
printf("  Ipo 80 %d \n",  sizeof(struct Ipo) );
printf("  KeyBlock 28 %d \n",  sizeof(struct KeyBlock) );
printf("  Key 116 %d \n",  sizeof(struct Key) );
printf("  Camera 84 %d \n",  sizeof(struct Camera) );
printf("  Image 204 %d \n",  sizeof(struct Image) );
printf("  anim 0 %d \n",  sizeof(struct anim) );
printf("  ImBuf 0 %d \n",  sizeof(struct ImBuf) );
printf("  MTex 80 %d \n",  sizeof(struct MTex) );
printf("  Object 436 %d \n",  sizeof(struct Object) );
printf("  Tex 164 %d \n",  sizeof(struct Tex) );
printf("  PluginTex 240 %d \n",  sizeof(struct PluginTex) );
printf("  CBData 24 %d \n",  sizeof(struct CBData) );
printf("  ColorBand 392 %d \n",  sizeof(struct ColorBand) );
printf("  Lamp 164 %d \n",  sizeof(struct Lamp) );
printf("  Wave 52 %d \n",  sizeof(struct Wave) );
printf("  Material 216 %d \n",  sizeof(struct Material) );
printf("  VFont 316 %d \n",  sizeof(struct VFont) );
printf("  VFontData 0 %d \n",  sizeof(struct VFontData) );
printf("  MetaElem 72 %d \n",  sizeof(struct MetaElem) );
printf("  MetaBall 132 %d \n",  sizeof(struct MetaBall) );
printf("  BoundBox 0 %d \n",  sizeof(struct BoundBox) );
printf("  BezTriple 60 %d \n",  sizeof(struct BezTriple) );
printf("  BPoint 28 %d \n",  sizeof(struct BPoint) );
printf("  Nurb 48 %d \n",  sizeof(struct Nurb) );
printf("  Curve 232 %d \n",  sizeof(struct Curve) );
printf("  Path 0 %d \n",  sizeof(struct Path) );
printf("  IpoCurve 80 %d \n",  sizeof(struct IpoCurve) );
printf("  MFace 12 %d \n",  sizeof(struct MFace) );
printf("  MFaceInt 20 %d \n",  sizeof(struct MFaceInt) );
printf("  MVert 20 %d \n",  sizeof(struct MVert) );
printf("  MCol 4 %d \n",  sizeof(struct MCol) );
printf("  MSticky 8 %d \n",  sizeof(struct MSticky) );
printf("  Mesh 180 %d \n",  sizeof(struct Mesh) );
printf("  OcInfo 0 %d \n",  sizeof(struct OcInfo) );
printf("  Lattice 76 %d \n",  sizeof(struct Lattice) );
printf("  World 212 %d \n",  sizeof(struct World) );
printf("  RenderData 416 %d \n",  sizeof(struct RenderData) );
printf("  Base 28 %d \n",  sizeof(struct Base) );
printf("  Scene 528 %d \n",  sizeof(struct Scene) );
printf("  BGpic 36 %d \n",  sizeof(struct BGpic) );
printf("  View3D 396 %d \n",  sizeof(struct View3D) );
printf("  View2D 112 %d \n",  sizeof(struct View2D) );
printf("  SpaceIpo 180 %d \n",  sizeof(struct SpaceIpo) );
printf("  SpaceButs 160 %d \n",  sizeof(struct SpaceButs) );
printf("  SpaceSeq 136 %d \n",  sizeof(struct SpaceSeq) );
printf("  SpaceFile 276 %d \n",  sizeof(struct SpaceFile) );
printf("  direntry 0 %d \n",  sizeof(struct direntry) );
printf("  SpaceOops 148 %d \n",  sizeof(struct SpaceOops) );
printf("  SpaceImage 148 %d \n",  sizeof(struct SpaceImage) );
printf("  UserDef 172 %d \n",  sizeof(struct UserDef) );
printf("  bScreen 100 %d \n",  sizeof(struct bScreen) );
printf("  ScrVert 20 %d \n",  sizeof(struct ScrVert) );
printf("  ScrEdge 24 %d \n",  sizeof(struct ScrEdge) );
printf("  ScrArea 212 %d \n",  sizeof(struct ScrArea) );
printf("  FileGlobal 92 %d \n",  sizeof(struct FileGlobal) );
printf("  StripElem 64 %d \n",  sizeof(struct StripElem) );
printf("  Strip 108 %d \n",  sizeof(struct Strip) );
printf("  PluginSeq 240 %d \n",  sizeof(struct PluginSeq) );
printf("  Sequence 148 %d \n",  sizeof(struct Sequence) );
printf("  Editing 28 %d \n",  sizeof(struct Editing) );
printf("  Effect 16 %d \n",  sizeof(struct Effect) );
printf("  BuildEff 24 %d \n",  sizeof(struct BuildEff) );
printf("  PartEff 156 %d \n",  sizeof(struct PartEff) );
printf("  Particle 0 %d \n",  sizeof(struct Particle) );
printf("  Event 44 %d \n",  sizeof(struct Event) );
printf("  Action 60 %d \n",  sizeof(struct Action) );
printf("  Sensor 56 %d \n",  sizeof(struct Sensor) );
printf("  NetLink 20 %d \n",  sizeof(struct NetLink) );
printf("  LBuf 12 %d \n",  sizeof(struct LBuf) );
printf("  Sector 148 %d \n",  sizeof(struct Sector) );
printf("  Portal 32 %d \n",  sizeof(struct Portal) );
printf("  CamPos 0 %d \n",  sizeof(struct CamPos) );
printf("  CamFrame 0 %d \n",  sizeof(struct CamFrame) );
printf("  TFace 56 %d \n",  sizeof(struct TFace) );
printf("  Life 368 %d \n",  sizeof(struct Life) );
printf("  DFace 0 %d \n",  sizeof(struct DFace) );
printf("  Deform 228 %d \n",  sizeof(struct Deform) );
printf("  Limb 40 %d \n",  sizeof(struct Limb) );
printf("  Ika 140 %d \n",  sizeof(struct Ika) );
printf("  Oops 44 %d \n",  sizeof(struct Oops) );
printf("  SpaceImaSel 720 %d \n",  sizeof(struct SpaceImaSel) );
printf("  ImaDir 0 %d \n",  sizeof(struct ImaDir) );
printf("  OneSelectableIma 0 %d \n",  sizeof(struct OneSelectableIma) );
printf("  SpacePaint 72 %d \n",  sizeof(struct SpacePaint) );
printf("  IconButtonManager 0 %d \n",  sizeof(struct IconButtonManager) );
}

