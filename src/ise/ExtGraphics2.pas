{ $Id$ }
{
 /***************************************************************************
                              ExtGraphics2.pas
                              ----------------
                  Abstract: Painting routines for special objects.
                  Initial Revision : 19/03/2006
                  Author: Seppo

                  ** Some routines here are from the Lazarus Component
                     Library (unit ExtGraphics.pas).
                  ** New routines were added by Seppo.
                  ** Since the new routines were not included  in the
                     Lazarus code base till now (current SVN checkout),
                     and to avoid conflicts with the original unit,
                     a different name was used.

 ***************************************************************************/

 *****************************************************************************
 *                                                                           *
 *  This file is part of the Lazarus Component Library (LCL)                 *
 *                                                                           *
 *  See the file COPYING.LCL, included in this distribution,                 *
 *  for details about the copyright.                                         *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 *****************************************************************************
}
unit ExtGraphics2;

{$MODE objfpc}{$H+}

interface

uses
  Classes, LCLProc, Graphics, math, GraphMath, ExtShapes, ExtGraphProc;

type
  TShapeDirection = (atUp, atDown, atLeft, atRight);

  TInitShapeProc = procedure(var P: array of TPoint;const R: TRect;var NumPts: Integer);

//General routines.
procedure InitPolygon(Canvas: TCanvas;PaintRect: TRect;RadAngle: Extended;Ptype: integer);overload;
var
  PR, vPR: TRect;
procedure InitPolygon(Canvas: TCanvas;PaintRect: TRect;RadAngle: Extended;
  InitShapeProc: TInitShapeProc);overload;
procedure PaintPolycon(Canvas: TCanvas;PR: TRect;fii: Extended;
P: array of TPoint;CountPts: Integer;cntPoint: TPoint);var
  N: array[0..3] of TPoint;
procedure PolycFixCenterpoint(var N: array of TPoint;cP: TPoint;var P: array of
  TPoint;CountPts: Integer);
procedure PolycMinMax(var N: array of TPoint;const P: array of TPoint;
  lkmPts: Integer);
procedure PolycNewPaintRect(var PR: TRect;cP: TPoint;wv, hv: Integer);
procedure PolycRotate(var Pts: array of TPoint;lkmPts: Integer;cntPoint: TPoint;
  fii: Extended);
procedure PolycScale(var P: array of TPoint;CountPts: Integer;const
  PaintRect: TRect;cntPoint: TPoint;N: array of TPoint);
procedure PolycSetHalfWidthAndHeight(const PR: TRect;var hv, wv: Integer;
  fii: Extended);

//Painting routines.
procedure Paint2HeadArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintBarbadosTrident(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintBigI(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintBoldArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintCanadianMaple(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintChevronArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintFivePointStar(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintHexagon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintNotchedArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintOctogon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintPentagon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintPlus(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintQuadrangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintRightTriangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintSwastika(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintTriangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintTriangular(Canvas: TCanvas;const PaintRect: TRect;
  RadAngle: Extended = 0.0;RightLeftFactor: extended = 0.5);
procedure PaintValve(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
procedure PaintVArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);

procedure PaintCross(Canvas: TCanvas;XLeft, YUp, XRight, YLow,
  CrossX1, CrossX2, CrossY1, CrossY2: integer);
procedure PaintHalfEllipse(Canvas: TCanvas;const PaintRect: TRect;
  AHalfEllipseDirection: TShapeDirection);
procedure PaintFivePointLineStar(Canvas: TCanvas;const PaintRect: TRect);
procedure PaintStarN(Canvas: TCanvas;cx, cy, r, n, a: Integer);

implementation

procedure PaintCross(Canvas: TCanvas;XLeft, YUp, XRight, YLow,
  CrossX1, CrossX2, CrossY1, CrossY2: integer);
var
  P: array[0..12] of TPoint;
begin
  P[0].x := XLeft;P[0].y := CrossY1;
  P[1].x := CrossX1;P[1].y := P[0].y;
  P[2].x := P[1].x;P[2].y := YUp;
  P[3].x := CrossX2;P[3].y := P[2].y;
  P[4].x := P[3].x;P[4].y := CrossY1;
  P[5].x := XRight;P[5].y := P[4].y;
  P[6].x := P[5].x;P[6].y := CrossY2;
  P[7].x := CrossX2;P[7].y := P[6].y;
  P[8].x := P[7].x;P[8].y := YLow;
  P[9].x := CrossX1;P[9].y := P[8].y;
  P[10].x := P[9].x;P[10].y := CrossY2;
  P[11].x := XLeft;P[11].y := P[10].y;
  P[12].x := P[11].x;P[12].y := CrossY1;
  Canvas.Polygon(P);
end;

procedure PolycRotate(var Pts: array of TPoint;lkmPts: Integer;
  cntPoint: TPoint;fii: Extended);
var
  i, dx, dy: Integer;
  x, y: Extended;
begin
  for i := 0 to lkmPts - 1 do begin
    dx := Pts[i].x - cntPoint.x;
    dy := Pts[i].y - cntPoint.y;
    x := dx * cos(fii) + dy * sin(fii);
    y := dy * cos(fii) - dx * sin(fii);
    Pts[i].x := cntPoint.x + Round(x);
    Pts[i].y := cntPoint.y + Round(y);
  end;
end;

procedure PolycMinMax(var N: array of TPoint;const P: array of TPoint;lkmPts: Integer);
var
  i, Xmin, Xmax, Ymin, Ymax: Integer;
begin
  Xmin := P[0].x;Xmax := P[0].x;Ymin := P[0].y;Ymax := P[0].y;
  for i := 0 to lkmPts - 1 do begin
    if P[i].x < Xmin then Xmin := P[i].x;
    if P[i].x > Xmax then Xmax := P[i].x;
    if P[i].y < Ymin then Ymin := P[i].y;
    if P[i].y > Ymax then Ymax := P[i].y;
  end;
  N[0] := Point(Xmin, Ymin);N[1] := Point(Xmin, Ymax);
  N[2] := Point(Xmax, Ymax);N[3] := Point(Xmax, Ymin);
end;

procedure PolycNewPaintRect(var PR: TRect;cP: TPoint;wv, hv: Integer);
begin
  with PR do begin
    Left := cP.x - wv;
    Right := cP.x + wv;
    Top := cP.y - hv;
    Bottom := cP.y + hv;
  end;
end;

procedure PolycFixCenterpoint(var N: array of TPoint;cP: TPoint;
  var P: array of TPoint;CountPts: Integer);
var
  i, nx, ny, dx, dy: Integer;
begin
  nx := (N[0].x + N[2].x) div 2;
  ny := (N[0].y + N[2].y) div 2;
  dx := cP.x - nx;
  dy := cP.y - ny;
  for i := 0 to 3 do begin
    N[i].x := N[i].x + dx;
    N[i].y := N[i].y + dy;
  end;
  for i := 0 to CountPts - 1 do begin
    P[i].x := P[i].x + dx;
    P[i].y := P[i].y + dy;
  end;
end;

procedure PolycSetHalfWidthAndHeight(const PR: TRect;var hv, wv: Integer;fii: Extended);
var
  h, w: Integer;
begin
  h := PR.Bottom - PR.Top;
  w := PR.Right - PR.Left;
  hv := Round(h * abs(cos(fii)) + w * abs(sin(fii))) div 2;
  wv := Round(h * abs(sin(fii)) + w * abs(cos(fii))) div 2;
end;

procedure PolycScale(var P: array of TPoint;CountPts: Integer;
  const PaintRect: TRect;cntPoint: TPoint;
  N: array of TPoint);
var
  k, kx, ky: Extended;
  i: Integer;
begin
  kx := (PaintRect.Right - PaintRect.Left) / (N[2].x - N[0].x);
  ky := (PaintRect.Bottom - PaintRect.Top) / (N[2].y - N[0].y);
  k := min(kx, ky);
  for i := 0 to CountPts - 1 do begin
    P[i].x := cntPoint.x + Round(k * (P[i].x - cntPoint.x));
    P[i].y := cntPoint.y + Round(k * (P[i].y - cntPoint.y));
  end;
end;

procedure PaintPolycon(Canvas: TCanvas;PR: TRect;fii: Extended;
  P: array of TPoint;CountPts: Integer;cntPoint: TPoint);
var
  N: array[0..3] of TPoint;

begin
  PolycRotate(P, CountPts, cntPoint, fii);
  PolycMinMax(N, P, CountPts);
  PolycFixCenterpoint(N, cntPoint, P, CountPts);
  PolycScale(P, CountPts, PR, cntPoint, N);
  case CountPts of
    3: Canvas.Polygon([P[0], P[1], P[2]]);
    4: Canvas.Polygon([P[0], P[1], P[2], P[3]]);
    5: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4]]);
    6: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5]]);
    7: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6]]);
    8: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7]]);

    10: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9]]);
    13: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9],
        P[10], P[11], P[12]]);

    20: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9],
        P[10], P[11], P[12], P[13], P[14], P[15], P[16], P[17], P[18], P[19]]);
    33: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9],
        P[10], P[11], P[12], P[13], P[14], P[15], P[16], P[17], P[18], P[19],
          P[20], P[21], P[22], P[23], P[24], P[25], P[26], P[27], P[28], P[29],
          P[30], P[31], P[32]]);
    35: Canvas.Polygon([P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9],
        P[10], P[11], P[12], P[13], P[14], P[15], P[16], P[17], P[18], P[19],
          P[20], P[21], P[22], P[23], P[24], P[25], P[26], P[27], P[28], P[29],
          P[30], P[31], P[32], P[33], P[34]]);

  end;
end;

procedure InitPolygon(Canvas: TCanvas;PaintRect: TRect;RadAngle: Extended;Ptype: integer); overload;
var
  PR, vPR: TRect;
  P: array[0..35] of TPoint;
  CountPts, hv, wv: Integer;
  cntPoint: TPoint;
begin
  PR := PaintRect;
  cntPoint := CenterPoint(PR);
  PolycSetHalfWidthAndHeight(PR, hv, wv, RadAngle);
  PolycNewPaintRect(vPR, cntPoint, wv, hv);
  case Ptype of
    1: Init2HeadArrow(P, vPR, CountPts);
    2: InitBarbadosTrident(P, vPR, CountPts);
    3: InitBigI(P, vPR, CountPts);
    4: InitBoldArrow(P, vPR, CountPts);
    5: InitCanadianMaple(P, vPR, CountPts);
    6: InitChevronArrow(P, vPR, CountPts);
    7: InitFivePointStar(P, vPR, CountPts);
    8: InitHexagon(P, vPR, CountPts);
    9: InitNotchedArrow(P, vPR, CountPts);
    10: InitOctogon(P, vPR, CountPts);
    11: InitPentagon(P, vPR, CountPts);
    12: InitPlus(P, vPR, CountPts);
    13: InitQuadrangle(P, vPR, CountPts);
    14: InitRightTriangle(P, vPR, CountPts);
    15: InitSwastika(P, vPR, CountPts);
    16: InitTriangle(P, vPR, CountPts);
    17: InitValve(P, vPR, CountPts);
    18: InitVArrow(P, vPR, CountPts);
  end;
  PaintPolycon(Canvas, PR, RadAngle, P, CountPts, cntPoint);
end;

procedure InitPolygon(Canvas: TCanvas;PaintRect: TRect;RadAngle: Extended;
  InitShapeProc: TInitShapeProc); overload;
var
  PR, vPR: TRect;
  P: array[0..35] of TPoint;
  CountPts, hv, wv: Integer;
  cntPoint: TPoint;
begin
  PR := PaintRect;
  cntPoint := CenterPoint(PR);
  PolycSetHalfWidthAndHeight(PR, hv, wv, RadAngle);
  PolycNewPaintRect(vPR, cntPoint, wv, hv);
  InitShapeProc(P, vPR, CountPts);
  PaintPolycon(Canvas, PR, RadAngle, P, CountPts, cntPoint);
end;

procedure Paint2HeadArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 1);
end;

procedure PaintBarbadosTrident(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 2);
end;

procedure PaintBigI(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 3);
end;

procedure PaintBoldArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 4);
end;

procedure PaintCanadianMaple(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 5);
end;

procedure PaintChevronArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 6);
end;

procedure PaintFivePointStar(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 7);
end;

procedure PaintHexagon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 8);
end;

procedure PaintNotchedArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 9);
end;

procedure PaintOctogon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 10);
end;

procedure PaintPentagon(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 11);
end;

procedure PaintPlus(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 12);
end;

procedure PaintQuadrangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 13);
end;

procedure PaintRightTriangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 14);
end;

procedure PaintSwastika(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 15);
end;

procedure PaintTriangle(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 16);
end;

procedure PaintValve(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 17);
end;

procedure PaintVArrow(Canvas: TCanvas;const PaintRect: TRect;RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, 18);
end;

procedure PaintTriangular(Canvas: TCanvas;const PaintRect: TRect;
  RadAngle: Extended = 0.0;RightLeftFactor: Extended = 0.5);
var
  PR, vPR: TRect;
  P: array[0..35] of TPoint;
  CountPts, hv, wv: Integer;
  cntPoint: TPoint;
begin
  PR := PaintRect;
  cntPoint := CenterPoint(PR);
  PolycSetHalfWidthAndHeight(PR, hv, wv, RadAngle);
  PolycNewPaintRect(vPR, cntPoint, wv, hv);

  with vPR do begin
    P[0].x := Left;P[0].y := Bottom;
    P[1].x := Left + round((Right - left) * RightLeftFactor);P[1].y := Top;
    P[2].x := Right;P[2].y := Bottom;
  end;
  CountPts := 3;

  PaintPolycon(Canvas, PR, RadAngle, P, CountPts, cntPoint);
end;

procedure PaintHalfEllipse(Canvas: TCanvas;const PaintRect: TRect;
  AHalfEllipseDirection: TShapeDirection);
var
  Ex1, Ex2, Ey1, Ey2, Sx, Sy, Ex, Ey, i: integer;
begin
  case AHalfEllipseDirection of
    atUp: with PaintRect do begin
        Ex1 := Left;Ex2 := Right;
        Ex := Left;Sx := Right;
        i := Bottom - Top;
        Ey1 := Top;Ey2 := Bottom + i;
        Sy := Top + i;Ey := Top + i;
      end;
    atDown: with PaintRect do begin
        Ex1 := Left;Ex2 := Right;
        Sx := Left;Ex := Right;
        i := Bottom - Top;
        Ey1 := Top - i;Ey2 := Bottom;
        Sy := Top;Ey := Top;
      end;
    atRight: with PaintRect do begin
        Ey1 := Top;Ey2 := Bottom;
        Ey := Top;Sy := Bottom;
        i := Right - Left;
        Ex1 := Left - i;Ex2 := Right;
        Sx := Left;Ex := Left;
      end;
    atLeft: with PaintRect do begin
        Ey1 := Top;Ey2 := Bottom;
        Sy := Top;Ey := Bottom;
        i := Right - Left;
        Ex1 := Left;Ex2 := Right + i;
        Sx := Left + i;Ex := Left + i;
      end;
  end;;
  Canvas.Pie(Ex1, Ey1, Ex2, Ey2, Sx, Sy, Ex, Ey);
end;

procedure PaintFivePointLineStar(Canvas: TCanvas;const PaintRect: TRect);
var
  P: array[0..4] of TPoint;
begin
  CalculatePentagonPoints(PaintRect, P[0], P[1], P[2], P[3], P[4]);
  Canvas.Line(P[0].x, P[0].y, P[2].x, P[2].y);
  Canvas.Line(P[0].x, P[0].y, P[3].x, P[3].y);
  Canvas.Line(P[1].x, P[1].y, P[3].x, P[3].y);
  Canvas.Line(P[1].x, P[1].y, P[4].x, P[4].y);
  Canvas.Line(P[2].x, P[2].y, P[4].x, P[4].y);
end;

procedure PaintStarN(Canvas: TCanvas;cx, cy, r, n, a: Integer);
const
  MaxStarPoint = 36;
var
  r1, r0, alpha: double;
  P: array[0..MaxStarPoint * 2 - 1] of TPoint;
  i, cs: Integer;
begin
  r1 := r / 2;
  for i := 0 to 2 * n do begin
    if (i mod 2) = 0 then r0 := r else r0 := r1;
    alpha := a + (0.5 + i / n) * Pi;
    cs := RoundToInt(r0 * cos(alpha));
    P[i].x := cx + cs;
    P[i].y := cy - Round(r0 * sin(alpha));
  end;
  for i := 2 * n to MaxStarPoint * 2 - 1 do begin
    P[i].x := P[2 * n - 1].x;
    P[i].y := P[2 * n - 1].y;
  end;
  Canvas.Polygon(P);
end;

end.

