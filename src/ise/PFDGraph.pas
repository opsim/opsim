{ $Id$ }
{
 /***************************************************************************

                  Abstract: Groups routines for rendering the PFD objects.
                  Initial Revision : 20/03/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
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
unit PFDGraph;

{$MODE objfpc}{$H+}

interface

uses
  Classes, SysUtils, Graphics, GraphMath, ExtGraphics2;

procedure PaintValve(Canvas: TCanvas; const PaintRect: TRect;
  RadAngle: Extended = 0.0);
procedure PaintMixer(Canvas: TCanvas; const PaintRect: TRect;
  RadAngle: Extended = 0.0);

implementation

//Author: Seppo.
procedure InitValve(var P: array of TPoint; const R: TRect; var NumPts: Integer);
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

//Based on Seppo's code.
procedure PaintValve(Canvas: TCanvas; const PaintRect: TRect; RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, @InitValve);
end;

procedure InitMixer(var P: array of TPoint; const R: TRect; var NumPts: Integer);
var
  cntPoint: TPoint;
begin
  cntPoint := CenterPoint(R);
  with R do begin
    P[0].x := Left;
    P[0].y := Top;
    P[1].x := Round(0.8 * cntPoint.x);
    P[1].y := P[0].y;
    P[2].x := Right;
    P[2].y := cntPoint.y;
    P[3].x := P[1].x;
    P[3].y := Bottom;
    P[4] := P[1];
    P[5] := P[3];
    P[6].x := P[0].x;
    P[6].y := P[3].y;
  end;
  NumPts := 7;
end;

procedure PaintMixer(Canvas: TCanvas; const PaintRect: TRect; RadAngle: Extended = 0.0);
begin
  InitPolygon(Canvas, PaintRect, RadAngle, @InitMixer);
end;

end.

