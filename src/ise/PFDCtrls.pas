{
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
unit PFDCtrls;

interface

uses
  SysUtils, Windows, Forms, Classes, Graphics, Controls, ExtCtrls, StdCtrls,
  Types, Math, extgraphics, LCLProc;

type

  TPFDWorkplace = class;

  TPFDFrame = class (TGraphicControl)
  protected
    procedure Paint; override;
  end;
  
  TPFDControl = class (TGraphicControl)
  private
    DefaultHeight: Integer;
    DefaultWidth: Integer;
    FDragFrame: TPFDFrame;
    FScale: Double;
    FSelected: Boolean;
    StartPoint: TPoint;
    function GetPFDWorkplace: TPFDWorkplace;
    procedure SetScale(Value: Double);
    procedure SetSelected(Value: Boolean);
  protected
    procedure ChangeScale(M, D: Integer); override;
    procedure DoMouseDown(Sender: TObject; Button: TMouseButton ; Shift: 
            TShiftState; X, Y: Integer); virtual;
    procedure DoMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer); 
            virtual;
    procedure DoMouseUp(Sender: TObject; Button: TMouseButton ; Shift: 
            TShiftState; X, Y: Integer); virtual;
    procedure Paint; override;
  public
    constructor Create(AOwner: TPFDWorkplace); reintroduce; virtual;
    property Canvas;
    property PFDWorkplace: TPFDWorkplace read GetPFDWorkplace;
    property Scale: Double read FScale write SetScale;
    property Selected: Boolean read FSelected write SetSelected;
  published
    property Align;
    property Anchors;
    property Color;
    property Constraints;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
    property ParentColor;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Visible;
  end;
  
  TPFDMixer = class (TPFDControl)
  protected
    procedure Paint; override;
  public
    constructor Create(AOwner: TPFDWorkplace); override;
  end;
  
  TPFDWorkplace = class (TScrollBox)
  protected
    procedure DoMouseDown(Sender: TObject; Button: TMouseButton ; Shift: 
            TShiftState; X, Y: Integer);
  public
    constructor Create(AOwner: TComponent); override;
  end;
  
implementation

type
  TGradientStyles = (gsCenter, gsHorizontal, gsVertical, gsTopToBottom, gsBottomToTop, gsLeftToRight, gsRightToLeft);
  TGradientStyle = set of TGradientStyles;

function HeightOf(ARect: TRect): Integer;
begin
  Result := ARect.Bottom - ARect.Top;
end;

function WidthOf(ARect: TRect): Integer;
begin
  Result := ARect.Right - ARect.Left;
end;

procedure GradientFillRect(Canvas: TCanvas; ARect: TRect; Origin: TPoint; StartColor,
  EndColor: TColor; Style: TGradientStyles; Colors: Byte);
var
  StartRGB: array [0..2] of Byte; { Start RGB values }
  RGBDelta: array [0..2] of Integer; { Difference between start and end RGB values }
  ColorBand: TRect; { Color band rectangular coordinates }
  I, Delta: Integer;
begin
  StartColor := ColorToRGB(StartColor);
  EndColor := ColorToRGB(EndColor);
  case Style of
    gsTopToBottom, gsLeftToRight, gsCenter:
      begin
        { Set the Red, Green and Blue colors }
        StartRGB[0] := GetRValue(StartColor);
        StartRGB[1] := GetGValue(StartColor);
        StartRGB[2] := GetBValue(StartColor);
        { Calculate the difference between begin and end RGB values }
        RGBDelta[0] := GetRValue(EndColor) - StartRGB[0];
        RGBDelta[1] := GetGValue(EndColor) - StartRGB[1];
        RGBDelta[2] := GetBValue(EndColor) - StartRGB[2];
      end;
    gsBottomToTop, gsRightToLeft:
      begin
        { Set the Red, Green and Blue colors }
        { Reverse of TopToBottom and LeftToRight directions }
        StartRGB[0] := GetRValue(EndColor);
        StartRGB[1] := GetGValue(EndColor);
        StartRGB[2] := GetBValue(EndColor);
        { Calculate the difference between begin and end RGB values }
        { Reverse of TopToBottom and LeftToRight directions }
        RGBDelta[0] := GetRValue(StartColor) - StartRGB[0];
        RGBDelta[1] := GetGValue(StartColor) - StartRGB[1];
        RGBDelta[2] := GetBValue(StartColor) - StartRGB[2];
      end;
  end;//case

  { Calculate the color band's coordinates }
  ColorBand := ARect;
  if Style in [gsTopToBottom, gsBottomToTop, gsCenter] then
  begin
    Colors := Max(2, Min(Colors, HeightOf(ARect)));
    Delta := HeightOf(ARect) div Colors;
  end
  else
  begin
    Colors := Max(2, Min(Colors, WidthOf(ARect)));
    Delta := WidthOf(ARect) div Colors;
  end;

  with Canvas.Pen do
  begin { Set the pen style and mode }
    Style := psSolid;
    Mode := pmCopy;
  end;

  with Canvas do begin

  end;//with
end;

procedure GradientFillPolygon(Canvas: TCanvas; Points: array of TPoint; StartColor,
  EndColor: TColor; Colors: Byte);
var
  StartRGB: array [0..2] of Byte; { Start RGB values }
  RGBDelta: array [0..2] of Integer; { Difference between start and end RGB values }
  ColorBand, BoundRect: TRect; { Color band rectangular coordinates }
  I, Delta: Integer;
begin
  //Calculate a bound rectangle.
  BoundRect := Rect(MaxInt,MaxInt,-MaxInt,-MaxInt);
  for I := Low(Points) to High(Points) do begin
    if Points[I].X < BoundRect.Left then
      BoundRect.Left := Points[I].X;
    if Points[I].Y < BoundRect.Top then
      BoundRect.Top := Points[I].Y;
    if Points[I].X > BoundRect.Right then
      BoundRect.Right := Points[I].X;
    if Points[I].Y > BoundRect.Bottom then
      BoundRect.Bottom := Points[I].Y;
  end;//for
end;

procedure GradientFillGuadrilateral(Canvas: TCanvas; Points: array of TPoint; StartColor,
  EndColor: TColor; Colors: Byte);
var
  StartRGB: array [0..2] of Byte; { Start RGB values }
  RGBDelta: array [0..2] of Integer; { Difference between start and end RGB values }
  ColorBand, BoundRect: TRect; { Color band rectangular coordinates }
  I, Delta, DV1, DV2: Integer;
  V1, V2: TPoint; //Vectors for advancing the band.
begin
  //Takes four points. The first two in clockwise direction
  //are the start side, the last two are the end side.
  V1.X := Points[0].X-Points[3].X;
  V1.Y := Points[0].Y-Points[3].Y;
  DV1 := Round(Norm([V1.X,V1.Y])/Colors);
  V2.X := Points[0].X-Points[3].X;
  V2.Y := Points[0].Y-Points[3].Y;
  DV2 := Round(Norm([V2.X,V2.Y])/Colors);

  //Unit vectors
  {V1.X := V1.X/DV1;
  V1.Y := V1.Y/DV1;
  V2.X := V2.X/DV2;
  V2.Y := V2.Y/DV2;}
end;

{
********************************** TPFDFrame ***********************************
}
procedure TPFDFrame.Paint;
begin
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Pen.Color := clRed;
  Canvas.Brush.Style := bsClear;
  Canvas.Rectangle(Rect(0,0,Width-1,Height-1));
end;

{
********************************* TPFDControl **********************************
}
constructor TPFDControl.Create(AOwner: TPFDWorkplace);
begin
  inherited Create(AOwner);
  
  Parent := TPFDWorkplace(Owner);
  
  //Each descendent should define its default dimensions, for when scale factor
  //is one.
  DefaultWidth := 105;
  DefaultHeight := 105;
  
  //Initializes size.
  Scale := 1;
  
  OnMouseDown := DoMouseDown;
  OnMouseMove := DoMouseMove;
  OnMouseUp := DoMouseUp;
end;

procedure TPFDControl.ChangeScale(M, D: Integer);
begin
  inherited ChangeScale(M, D);
  //Reserved to scale other associated controls, e.g. label tags.
end;

procedure TPFDControl.DoMouseDown(Sender: TObject; Button: TMouseButton ; 
        Shift: TShiftState; X, Y: Integer);
begin
  Selected := True;
  StartPoint := Point(X,Y);
end;

procedure TPFDControl.DoMouseMove(Sender: TObject; Shift: TShiftState; X, Y: 
        Integer);
begin
  if MouseCapture then begin
    //Create drag border.
    if not Assigned(FDragFrame) then begin
      FDragFrame := TPFDFrame.Create(Self);
      FDragFrame.Parent := Parent;
      FDragFrame.SetBounds(Left, Top, Width, Height);
    end;//if
    //Move the border frame.
    with FDragFrame do begin
      Left := Left + X - StartPoint.X;
      Top := Top + Y - StartPoint.Y;
      StartPoint := Point(X,Y);
    end;//with
  end;//if
end;

procedure TPFDControl.DoMouseUp(Sender: TObject; Button: TMouseButton ; Shift: 
        TShiftState; X, Y: Integer);
begin
  if Assigned(FDragFrame) then begin
    Left := FDragFrame.Left;
    Top := FDragFrame.Top;
    FreeAndNil(FDragFrame);
  end;//if
end;

function TPFDControl.GetPFDWorkplace: TPFDWorkplace;
begin
  Result := TPFDWorkplace(Owner);
end;

procedure TPFDControl.Paint;
begin
  if Selected then begin
    with Canvas do begin
      Pen.Style := psSolid;
      Pen.Mode := pmCopy;
      Pen.Color := clWhite;
      Pen.Width := 1;
      Brush.Style := bsClear;
      //Lazarus needs a correction of one pixel.
      Rectangle(Rect(0,0,Self.Width-1,Self.Height-1));
      //Debugln(Format('TPFDControl Width: %s', [IntToStr(Self.Width)]));
      //Debugln(Format('TPFDControl Height: %s', [IntToStr(Self.Height)]));
    end;//with
  end;//if
  
  //if MouseCapture then
    //Canvas.DrawFocusRect(Rect(0,0,Width,Height));
  //  Canvas.Rectangle(Rect(0,0,Width,Height));
    //Canvas.DrawFocusRect(BoundsRect);
  
  {Canvas.Font := Font;
  Canvas.Brush.Color := Color;
  if csDesigning in ComponentState then
    with Canvas do
    begin
      Pen.Style := psDash;
      Brush.Style := bsClear;
      Rectangle(0, 0, Width, Height);
    end;}
end;

procedure TPFDControl.SetScale(Value: Double);
begin
  FScale := Value;
  Width := Round(Scale * DefaultWidth);
  Height := Round(Scale * DefaultHeight);
end;

procedure TPFDControl.SetSelected(Value: Boolean);
begin
  FSelected := Value;
  Invalidate;
end;

{
********************************** TPFDMixer ***********************************
}
constructor TPFDMixer.Create(AOwner: TPFDWorkplace);
begin
  inherited Create(AOwner);
  DefaultWidth := 30;
  DefaultHeight := 45;
  Scale := 1;
end;

procedure TPFDMixer.Paint;
var
  LX, LY, OffsetX, OffsetY: Integer;
  P1, P2, P3, P4, P5: TPoint;
begin
  inherited Paint;
  LX := Width;
  LY := Height;
  OffsetX := LX div 5;
  OffsetY := LX div 5;
  with Canvas do begin
    //Pen.Width := Round(Max(1, Width/50));
    Pen.Width := 1;
    Pen.Color := clBlack;
    //Pen.Style := psSolid;
    //Pen.Mode := pmBlack;
    Brush.Color := clBlue;
    Brush.Style := bsSolid;
  
    //For testing.
    Ellipse(Rect(1,1,Self.Width-1,Self.Height-1));
  
    //Draw the contour lines.
    {P1 := Point(OffsetX, OffsetY);
    P2 := Point(Round(P1.X+LX/5), P1.Y);
    P3 := Point(Round(P2.X+2*LX/5), Round(LY/2));
    P4 := Point(P2.X,	Round(LY-OffsetY));
    P5 := Point(P1.X,	P4.Y);
    Polyline([P1, P2, P3, P4, P5, P1]);
    Polyline([P2, P4 ]);}
  
    //Paint the gradient.
  
  
  end;//with
end;

{
******************************** TPFDWorkplace *********************************
}
constructor TPFDWorkplace.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  {OnMouseDown := DoMouseDown;
  OnMouseMove := DoMouseMove;
  OnMouseUp := DoMouseUp;}
  Color := $00808040;
  Align := alClient;
  OnMouseDown := DoMouseDown;
end;

procedure TPFDWorkplace.DoMouseDown(Sender: TObject; Button: TMouseButton ; 
        Shift: TShiftState; X, Y: Integer);
var
  I: Integer;
begin
  for I := 0 to ControlCount - 1 do
    if Controls[I] is TPFDControl then
      TPFDControl(Controls[I]).Selected := False;
end;

end.
