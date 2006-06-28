unit ActualFlash;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, Grids,
  StdCtrls, Buttons, Thermo, Equilibrium, Eos_Srk;

type

  { TFlashForm }

  TFlashForm = class(TForm)
    BitBtn1: TBitBtn;
    FlashBtn: TButton;
    NormalizeBtn: TButton;
    Label3: TLabel;
    Flowrate: TEdit;
    Temperature: TEdit;
    Pressure: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    FlashGrid: TStringGrid;
    procedure FlashBtnClick(Sender: TObject);
    procedure FlashGridClick(Sender: TObject);
    procedure FlashGridEditingDone(Sender: TObject);
    procedure FlashGridSelectEditor(Sender: TObject; Col, Row: Integer;
      var Editor: TWinControl);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure NormalizeBtnClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
    FlashStrm:TMaterial;
    TestFlash:TThreePhaseFlash;
    TestEOS:TSrkEos;
  end; 

var
  FlashForm: TFlashForm;
  TotalRow: Integer;
  Total: Real;

implementation

{ TFlashForm }

procedure TFlashForm.FlashGridEditingDone(Sender: TObject);
var
  i : integer;
begin
  Total:=0.0;
  i := FlashGrid.Row;
  If i>FlashStrm.Compositions.Count then exit;
  FlashStrm.Compositions[i-1].MoleFraction.Value:=StrtoFloat(FlashGrid.Cells[1,i]);
  for i := 1 to FlashStrm.Compounds.Count do
      Total := Total + FlashStrm.Compositions[i-1].MoleFraction.Value;
  FlashGrid.Cells[1,TotalRow]:= FloattoStrF(Total,ffFixed,5,4);

end;

procedure TFlashForm.FlashGridClick(Sender: TObject);
begin

end;

procedure TFlashForm.FlashBtnClick(Sender: TObject);
begin

end;

procedure TFlashForm.FlashGridSelectEditor(Sender: TObject; Col, Row: Integer;
  var Editor: TWinControl);
begin
  if Col<>1 then Editor:=nil;
  if Row=TotalRow then Editor:=nil;
end;

procedure TFlashForm.FormCreate(Sender: TObject);
begin
  FlashStrm:=TMaterial.Create;
  TestFlash:=TThreePhaseFlash.Create;
  TestEOS:=TSrkEos.Create;
  TestFlash.Eos:=TestEOS;
end;

procedure TFlashForm.FormShow(Sender: TObject);
var
  i,NumComp : integer;
  temp: real;

begin
  Total := 0;
  NumComp:= FlashStrm.Compositions.Count;
  NumComp:= FlashStrm.Compounds.Count;
  TotalRow:= NumComp + 1;

  // If a composition exists, use it
  for i := 0 to NumComp - 1 do
    begin
      temp:= FlashStrm.Compositions[i].MoleFraction.Value;
      FlashGrid.Cells[1,i+1]:=FloattoStrF(temp,ffFixed,5,4);
      Total:= Total + temp;
    end;
   FlashGrid.Cells[1,TotalRow]:= FloattoStrF(Total,ffFixed,5,4);
end;

procedure TFlashForm.NormalizeBtnClick(Sender: TObject);
var
  i : integer;
  temp: real;

begin
  TestFlash.Normalize(FlashStrm.Compositions);
  Total := 0;
  for i := 1 to FlashStrm.Compounds.Count do
    begin
      temp:= FlashStrm.Compositions[i-1].MoleFraction.Value;
      FlashGrid.Cells[1,i]:=FloattoStrF(temp,ffFixed,5,4);
      Total:= Total + temp;
    end;
   FlashGrid.Cells[1,TotalRow]:= FloattoStrF(Total,ffFixed,5,4);
end;

initialization
  {$I actualflash.lrs}

end.

