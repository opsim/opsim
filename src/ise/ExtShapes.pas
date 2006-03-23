{ $Id$ }
{
 /***************************************************************************

                  Abstract: Implements special shapes.
                  Initial Revision : 19/03/2006
                  Authors:
                    - Seppo
                    - Additional contributors...

 ***************************************************************************/

 *****************************************************************************
 *                                                                           *
 *  This file is part of the OpSim - OPEN SOURCE PROCESS SIMULATOR           *
 *                                                                           *
 *  See the file COPYING.GPL, included in this distribution,                 *
 *  for details about the copyright.                                         *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 *****************************************************************************
}
unit ExtShapes;

{$MODE objfpc}{$H+}

interface

uses
  Classes, SysUtils, lclproc, math, GraphMath, ExtGraphProc;

procedure Init2HeadArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitBarbadosTrident(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitBigI(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitBoldArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitChevronArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitCanadianMaple(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitFivePointStar(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitHexagon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitNotchedArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitOctogon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitPentagon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitPlus(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitQuadrangle(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitRightTriangle(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitSwastika(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitTriangle(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitValve(var P: array of TPoint;const R: TRect;var NumPts: Integer);
procedure InitVArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);

implementation

procedure Init2HeadArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dx, dy: Integer;
begin
  with R do begin
    dx := (Right - Left) div 4;
    dy := (Bottom - Top) div 4;
    P[0].x := Left;P[0].y := Top + (Bottom - Top) div 2;
    P[1].x := Left + dx;P[1].y := Top;
    P[2].x := P[1].x;P[2].y := Top + dy;
    P[3].x := Right - dx;P[3].y := P[2].y;
    P[4].x := P[3].x;P[4].y := Top;
    P[5].x := Right;P[5].y := P[0].y;
    P[6].x := P[3].x;P[6].y := Bottom;
    P[7].x := P[3].x;P[7].y := Bottom - dy;
    P[8].x := P[1].x;P[8].y := P[7].y;
    P[9].x := P[1].x;P[9].y := Bottom;
  end;
  NumPts := 10;
end;

procedure InitBarbadosTrident(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  RmLpW, BmTpH: extended;cntPoint: TPoint;
begin
  cntPoint := CenterPoint(R);
  with R do begin
    RmLpW := (Right - Left) / 140;
    BmTpH := (Bottom - Top) / 160;
    P[0].x := cntPoint.x - round(RmLpW * 10);P[0].y := Bottom;
    P[34].x := cntPoint.x + round(RmLpW * 10);P[34].y := P[0].y;
    P[1].x := P[0].x;P[1].y := Bottom - round(BmTpH * 50);
    P[33].x := P[34].x;P[33].y := P[1].y;
    P[2].x := cntPoint.x - round(RmLpW * 35);P[2].y := P[1].y;
    P[32].x := cntPoint.x + round(RmLpW * 35);P[32].y := P[2].y;
    P[3].x := cntPoint.x - round(RmLpW * 48);P[3].y := Bottom - round(BmTpH * 98);
    P[31].x := cntPoint.x + round(RmLpW * 48);P[31].y := P[3].y;
    P[4].x := left;P[4].y := top;
    P[30].x := Right;P[30].y := P[4].y;
    P[5].x := cntPoint.x - round(RmLpW * 42);P[5].y := Top + round(BmTpH * 4);
    P[29].x := cntPoint.x + round(RmLpW * 42);P[29].y := P[5].y;
    P[6].x := cntPoint.x - round(RmLpW * 40);P[6].y := Top + round(BmTpH * 6);
    P[28].x := cntPoint.x + round(RmLpW * 40);P[28].y := P[6].y;
    P[7].x := cntPoint.x - round(RmLpW * 39);P[7].y := Top + round(BmTpH * 11);
    P[27].x := cntPoint.x + round(RmLpW * 39);P[27].y := P[7].y;
    P[8].x := cntPoint.x - round(RmLpW * 45);P[8].y := Top + round(BmTpH * 16);
    P[26].x := cntPoint.x + round(RmLpW * 45);P[26].y := P[8].y;
    P[9].x := cntPoint.x - round(RmLpW * 45);P[9].y := Top + round(BmTpH * 21);
    P[25].x := cntPoint.x + round(RmLpW * 45);P[25].y := P[9].y;
    P[10].x := cntPoint.x - round(RmLpW * 32);P[10].y := Top + round(BmTpH * 47);
    P[24].x := cntPoint.x + round(RmLpW * 32);P[24].y := P[10].y;
    P[11].x := cntPoint.x - round(RmLpW * 28);P[11].y := Top + round(BmTpH * 70);
    P[23].x := cntPoint.x + round(RmLpW * 28);P[23].y := P[11].y;
    P[12].x := cntPoint.x - round(RmLpW * 22);P[12].y := Top + round(BmTpH * 92);
    P[22].x := cntPoint.x + round(RmLpW * 22);P[22].y := P[12].y;
    P[13].x := P[0].x;P[13].y := P[12].y;
    P[21].x := P[34].x;P[21].y := P[13].y;
    P[14].x := P[0].x;P[14].y := Top + round(BmTpH * 30);
    P[20].x := P[34].x;P[20].y := P[14].y;
    P[15].x := cntPoint.x - round(RmLpW * 22);P[15].y := Top + round(BmTpH * 22);
    P[19].x := cntPoint.x + round(RmLpW * 22);P[19].y := P[15].y;
    P[16].x := cntPoint.x - round(RmLpW * 9);P[16].y := Top + round(BmTpH * 12);
    P[18].x := cntPoint.x + round(RmLpW * 9);P[18].y := P[16].y;
    P[17].x := cntPoint.x;P[17].y := Top;

  end;
  NumPts := 35;
end;

procedure InitBigI(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dx, dy: Integer;
begin
  with R do begin
    dx := (Right - Left) div 4;
    dy := (Bottom - Top) div 18;
    P[0].x := Left;P[0].y := Top;
    P[1].x := Right;P[1].y := TOP;
    P[2].x := Right - dx;P[2].y := Top + dy;
    P[3].x := P[2].x;P[3].y := Bottom - dy;
    P[4].x := Right;P[4].y := Bottom;
    P[5].x := Left;P[5].y := Bottom;
    P[6].x := Left + dx;P[6].y := P[3].y;
    P[7].x := P[6].x;P[7].y := P[2].y;
  end;
  NumPts := 8;
end;

procedure InitBoldArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dy: Integer;cntPoint: TPoint;
begin
  cntPoint := CenterPoint(R);
  with R do begin
    dy := (Bottom - Top) div 4;
    P[0].x := Left;
    P[0].y := Top + dy;
    P[1].x := cntPoint.x;
    P[1].y := P[0].y;
    P[2].x := cntPoint.x;
    P[2].y := Top;
    P[3].x := Right;
    P[3].y := cntPoint.y;
    P[4].x := cntPoint.x;
    P[4].y := Bottom;
    P[5].x := cntPoint.x;
    P[5].y := Bottom - dy;
    P[6].x := Left;
    P[6].y := P[5].y;
  end;
  NumPts := 7;
end;

procedure InitCanadianMaple(var P: array of TPoint;const R: TRect;var NumPts: Integer);
const
  leafheight = 54;leafwidth = 50;
var
  xcenter, x2: integer;
  RmLpLW, //  (Right - Left)/LeafWidth;
    BmTpLH //(Bottom-Top)/ LeafHeight
    : extended;
begin
  with R do begin
    xcenter := Left + (Right - Left) div 2;
    p[0].y := Top;
    p[0].x := xcenter;
    RmLpLW := (Right - Left) / LeafWidth;
    BmTpLH := (Bottom - Top) / LeafHeight;
    x2 := RoundToInt(RmLpLW * 5);
    P[1].x := xcenter - x2;P[1].y := RoundToInt(BmTpLH * 9 + Top);
    P[32].x := xcenter + x2;P[32].y := P[1].y;
    x2 := RoundToInt(RmLpLW * 10);
    P[2].x := xcenter - x2;P[2].y := RoundToInt(BmTpLH * 7 + Top);
    P[31].x := xcenter + x2;P[31].y := P[2].y;
    x2 := RoundToInt(RmLpLW * 7);
    P[3].x := xcenter - x2;P[3].y := RoundToInt(BmTpLH * 21 + Top);
    P[30].x := xcenter + x2;P[30].y := P[3].y;
    x2 := RoundToInt(RmLpLW * 9);
    P[4].x := xcenter - x2;P[4].y := P[3].y;
    P[29].x := xcenter + x2;P[29].y := P[3].y;
    x2 := RoundtoInt(RmLpLW * 15);
    P[5].x := xcenter - x2;P[5].y := RoundtoInt(BmTpLH * 15 + Top);
    P[28].x := xcenter + x2;P[28].y := P[5].y;
    x2 := RoundtoInt(RmLpLW * 17);
    P[6].x := xcenter - x2;P[6].y := RoundtoInt(BmTpLH * 19 + Top);
    P[27].x := xcenter + x2;P[27].y := P[6].y;
    x2 := RoundtoInt(RmLpLW * 24);
    P[7].x := xcenter - x2;P[7].y := RoundtoInt(BmTpLH * 17 + Top);
    P[26].x := xcenter + x2;P[26].y := P[7].y;
    x2 := RoundtoInt(RmLpLW * 22);
    P[8].x := xcenter - x2;P[8].y := RoundtoInt(BmTpLH * 26 + Top);
    P[25].x := xcenter + x2;P[25].y := P[8].y;
    x2 := RoundtoInt(RmLpLW * 25);
    P[9].x := xcenter - x2;P[9].y := RoundtoInt(BmTpLH * 28 + Top);
    P[24].x := xcenter + x2;P[24].y := P[9].y;
    x2 := RoundtoInt(RmLpLW * 14);
    P[10].x := xcenter - x2;P[10].y := RoundtoInt(BmTpLH * 38 + Top);
    P[23].x := xcenter + x2;P[23].y := P[10].y;
    x2 := RoundtoInt(RmLpLW * 15);
    P[11].x := xcenter - x2;P[11].y := RoundtoInt(BmTpLH * 43 + Top);
    P[22].x := xcenter + x2;P[22].y := P[11].y;
    x2 := RoundtoInt(RmLpLW);
    P[12].x := xcenter - x2;P[12].y := RoundtoInt(BmTpLH * 41 + Top);
    P[21].x := xcenter + x2;P[21].y := P[12].y;
    x2 := RoundtoInt(RmLpLW / 2);
    P[13].x := xcenter - x2;P[13].y := RoundtoInt(BmTpLH * 42 + Top);
    P[20].x := xcenter + x2;P[20].y := P[13].y;
    P[14].x := P[13].x;P[14].y := RoundtoInt(BmTpLH * 47 + Top);
    P[19].x := P[20].x;P[19].y := P[14].y;
    x2 := RoundtoInt(RmLpLW);
    P[15].x := xcenter - x2;P[15].y := P[14].y;
    P[18].x := xcenter + x2;P[18].y := P[14].y;
    P[16].x := P[15].x;P[16].y := bottom;
    P[17].x := P[18].x;P[17].y := bottom;
  end;
  NumPts := 33;
end;

procedure InitChevronArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dx: Integer;
begin
  with R do begin
    dx := (Right - Left) div 3;
    P[0].x := Left;
    P[0].y := Top;
    P[1].x := Right - dx;
    P[1].y := Top;
    P[2].x := Right;
    P[2].y := (Top + Bottom) div 2;
    P[3].x := P[1].x;
    P[3].y := Bottom;
    P[4].x := Left;
    P[4].y := Bottom;
    P[5].x := Left + dx;
    P[5].y := P[2].y;
  end;
  NumPts := 6;
end;

procedure InitFivePointStar(var P: array of TPoint;const R: TRect;var NumPts: Integer);
begin
  CalculatePentagonPoints(R, P[0], P[2], P[4], P[6], P[8]);
  P[1] := LinesPointOfIntersection(P[0], P[4], P[2], P[8]);
  P[3] := LinesPointOfIntersection(P[0], P[4], P[2], P[6]);
  P[5] := LinesPointOfIntersection(P[8], P[4], P[2], P[6]);
  P[7] := LinesPointOfIntersection(P[8], P[4], P[0], P[6]);
  P[9] := LinesPointOfIntersection(P[8], P[2], P[0], P[6]);
  NumPts := 10;
end;

procedure InitHexagon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dx: Integer;
begin
  with R do begin
    dx := round(((Right - Left) / 2 * cos(DegToRad(15))) / 2);
    P[0].x := Left + dx;P[0].y := Top;
    P[1].x := Left;P[1].y := (Top + Bottom) div 2;
    P[2].x := P[0].x;P[2].y := Bottom;
    P[3].x := Right - dx;P[3].y := Bottom;
    P[4].x := Right;P[4].y := P[1].y;
    P[5].x := Right - dx;P[5].y := Top;
  end;
  NumPts := 6;
end;

procedure InitNotchedArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
begin
  InitBoldArrow(P, R, NumPts);
  with R do begin
    P[7].x := Left + (Right - Left) div 4;
    P[7].y := P[3].y; // centerpoint y
  end;
  NumPts := 8;
end;

procedure InitOctogon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  dx, dy: Integer;
begin
  with R do begin
    dx := Right - Left;
    dx := round((dx - dx / (sqrt(2) + 1)) / 2);
    dy := Bottom - Top;
    dy := round((dy - dy / (sqrt(2) + 1)) / 2);
    P[0].x := Left + dx;P[0].y := Top;
    P[1].x := Right - dx;P[1].y := Top;
    P[2].x := Right;P[2].y := Top + dy;
    P[3].x := Right;P[3].y := Bottom - dy;
    P[4].x := P[1].x;P[4].y := Bottom;
    P[5].x := P[0].x;P[5].y := Bottom;
    P[6].x := Left;P[6].y := P[3].y;
    P[7].x := Left;P[7].y := P[2].y;
  end;
  NumPts := 8;
end;

procedure InitPentagon(var P: array of TPoint;const R: TRect;var NumPts: Integer);
begin
  CalculatePentagonPoints(R, P[0], P[1], P[2], P[3], P[4]);
  NumPts := 5;
end;

procedure InitPlus(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  CrossX, Crossy: integer;
begin
  with R do begin
    CrossX := (Right - Left) div 3;
    CrossY := (Bottom - Top) div 3;
    P[0].x := Left;P[0].y := Top + CrossY;
    P[1].x := Left + CrossX;P[1].y := P[0].y;
    P[2].x := P[1].x;P[2].y := Top;
    P[3].x := Right - CrossX;P[3].y := P[2].y;
    P[4].x := P[3].x;P[4].y := P[0].y;
    P[5].x := Right;P[5].y := P[4].y;
    P[6].x := P[5].x;P[6].y := Bottom - CrossY;
    P[7].x := P[3].x;P[7].y := P[6].y;
    P[8].x := P[7].x;P[8].y := Bottom;
    P[9].x := P[1].x;P[9].y := P[8].y;
    P[10].x := P[9].x;P[10].y := P[6].y;
    P[11].x := Left;P[11].y := P[10].y;
    P[12].x := P[11].x;P[12].y := P[0].y;
  end;
  NumPts := 13;
end;

procedure InitQuadrangle(var P: array of TPoint;const R: TRect;var NumPts: Integer);
begin
  with R do begin
    P[0].x := Left;P[0].y := Top;
    P[1].x := Left;P[1].y := Bottom;
    P[2].x := Right;P[2].y := Bottom;
    P[3].x := Right;P[3].y := Top;
  end;
  NumPts := 4;
end;

procedure InitRightTriangle(var P: array of TPoint;const R: TRect;
  var NumPts: Integer);
begin
  with R do begin
    P[0].x := Left;P[0].y := Top;
    P[1].x := Right;P[1].y := Bottom;
    P[2].x := P[0].x;P[2].y := Bottom;
  end;
  NumPts := 3;
end;


procedure InitSwastika(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  x1, x2, y1, y2: Integer;
begin
  with r do begin
    x1 := (Right - Left) div 5;
    y1 := (Bottom - Top) div 5;
    x2 := (Right - Left) * 2 div 5;
    y2 := (Bottom - Top) * 2 div 5;
    P[0].x := Left;P[0].y := Top;P[1].x := Left + x1;P[1].y := Top;
    P[2].x := P[1].x;P[2].y := Top + y2;P[3].x := Left + x2;P[3].y := P[2].y;
    P[4].x := P[3].x;P[4].y := Top;P[5].x := Right;P[5].y := P[4].y;
    P[6].x := P[5].x;P[6].y := Top + y1;P[7].x := Right - x2;P[7].y := P[6].y;
    P[8].x := P[7].x;P[8].y := p[2].y;P[9].x := Right;P[9].y := P[8].y;
    P[10].x := P[9].x;P[10].y := Bottom;P[11].x := Right - x1;P[11].y := P[10].y;
    P[12].x := P[11].x;P[12].y := Bottom - y2;P[13].x := P[7].x;P[13].y := P[12].y;
    P[14].x := P[13].x;P[14].y := Bottom;P[15].x := Left;P[15].y := P[14].y;
    P[16].x := P[15].x;P[16].y := Bottom - y1;P[17].x := Left + x2;P[17].y := P[16].y;
    P[18].x := P[17].x;P[18].y := Bottom - y2;P[19].x := Left;P[19].y := P[18].y;
  end;
  NumPts := 20;
end;

procedure InitTriangle(var P: array of TPoint;const R: TRect;
  var NumPts: Integer);
begin
  with R do begin
    P[0].x := Left;P[0].y := Top;
    P[1].x := Right;P[1].y := Top + (Bottom - Top) div 2;
    P[2].x := P[0].x;P[2].y := Bottom;
  end;
  NumPts := 3;
end;

procedure InitValve(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  cntPoint: TPoint;
begin
  cntPoint := CenterPoint(R);
  with R do begin
    P[0].x := Left;
    P[0].y := Top;
    P[1].x := cntPoint.x;
    P[1].y := cntPoint.y;
    P[2].x := Right;
    P[2].y := Top;
    P[3].x := Right;
    P[3].y := Bottom;
    P[4].x := cntPoint.x;
    P[4].y := cntPoint.y;
    P[5].x := Left;
    P[5].y := Bottom;
  end;
  NumPts := 6;
end;

procedure InitVArrow(var P: array of TPoint;const R: TRect;var NumPts: Integer);
var
  cntPoint: TPoint;
begin
  cntPoint := CenterPoint(R);
  with R do begin
    P[0].x := Left;
    P[0].y := Top;
    P[1].x := Right;
    P[1].y := cntPoint.y;
    P[2].x := Left;
    P[2].y := Bottom;
    P[3].x := cntPoint.x;
    P[3].y := cntPoint.y;
  end;
  NumPts := 4;
end;

end.
