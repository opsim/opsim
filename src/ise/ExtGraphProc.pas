{ $Id$ }
{
 /***************************************************************************
                              ExtGraphProc.pas
                              ----------------
                  Abstract: Implements special graphic routines.
                  Initial Revision : 19/03/2006
                  Author: Seppo


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
unit ExtGraphProc;

{$MODE objfpc}{$H+}

interface

uses Classes, lclproc, math;

procedure CalculatePentagonPoints(const PentagonRect: TRect;var P1, P2, P3, P4, P5: TPoint);
function LinesPointOfIntersection(const Line1a, Line1b, Line2a, line2b: TPoint): TPoint; 

implementation

procedure CalculatePentagonPoints(const PentagonRect: TRect;
  var P1, P2, P3, P4, P5: TPoint);
var
  cx, cy, dy, dx: Integer;r: real;
begin
  P1.y := PentagonRect.Top;
  P2.x := PentagonRect.Left;
  P3.y := PentagonRect.Bottom;
  P4.y := PentagonRect.Bottom;
  P5.x := PentagonRect.Right;
  P1.x := (PentagonRect.Right + PentagonRect.Left) div 2;
  dy := RoundToInt((P1.x - P2.x) * tan(Pi / 10));
  r := sqrt(dy * dy + (P1.x - P2.x) * (P1.x - P2.x));
  cx := P1.x;
  cy := P1.y + round(r);
  P2.y := cy - dy;
  P5.y := P2.y;
  dx := RoundToInt(r * sin(Pi / 5));
  P3.x := cx - dx;
  P4.x := cx + dx;
end;               

function LinesPointOfIntersection(const Line1a, Line1b, Line2a, line2b: TPoint): TPoint;
var
  k1, k2, b1, b2, x, x1, x2, x3, x4, y, y1, y2, y3, y4: real;
  p: TPoint;
begin
  x1 := Line1a.x;y1 := Line1a.y;
  x2 := Line1b.x;y2 := Line1b.y;
  x3 := Line2a.x;y3 := Line2a.y;
  x4 := Line2b.x;y4 := Line2b.y;
  k1 := (y2 - y1) / (x2 - x1);
  k2 := (y4 - y3) / (x4 - x3);
  b1 := -k1 * x1 + y1;
  b2 := -k2 * x3 + y3;
  x := (b1 - b2) / (k2 - k1);
  y := (k2 * b1 - k1 * b2) / (k2 - k1);
  p.x := RoundToInt(x);
  p.y := RoundToInt(y);
  LinesPointOfIntersection := p;
end;

end.

