unit UnitMain;

interface

uses
  Windows, Messages, SysUtils,  Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, Grids, StdCtrls, Spin, ComCtrls;

type
  TFormMain = class(TForm)
    PanelMng: TPanel;
    Button1: TButton;
    StringGridMatrix: TStringGrid;
    SpinEdit1: TSpinEdit;
    Label1: TLabel;
    StatusBar1: TStatusBar;
    Splitter1: TSplitter;
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure SpinEdit1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

    procedure ResetGrid;
  end;

var
  FormMain: TFormMain;

function precisesec( dt : TDateTime ) : string;

implementation

uses Sparxcpt, SparSolv;

{$R *.dfm}

procedure LoadLHS( const eq, vr : integer; const value : string );
var
  r : double;
begin
  if trim( value ) = '' then exit;
  try
    r := StrToFloat( value )
  except
    exit;
  end;

  AddLHS_A( eq, vr, r );
end;

procedure LoadRHS( const eq: integer; const value : string );
var
  r : double;
begin
  if trim( value ) = '' then exit;
  try
    r := StrToFloat( value )
  except
    exit;
  end;

  AddRHS_A( eq, r );
end;

procedure TFormMain.Button1Click(Sender: TObject);
var
  i, j : integer;
  t1 : TDateTime;
begin
  ResetGrid;

  InitStrucA( StringGridMatrix.RowCount-1 );
  with StringGridMatrix do
  try
    // load data to SparSolv
    for j := 1 to RowCount-1 do begin
      LoadRHS( j, Cells[0,j] );

      for i := 1 to ColCount-2 do
        LoadLHS( j, i, Cells[i, j] );
    end;

    t1 := Now;
    Solve1A;
    StatusBar1.Panels[1].Text := precisesec( Now - t1 ) + ' s';

    for j := 1 to RowCount-1 do
      Cells[ ColCount-1, j ] := FloatToStr( GetAnswerF(j) );
  finally
    ReleaseStruc;
  end;
end;

function precisesec( dt : TDateTime ) : string;
var
  Hour, Minute, Sec, MilSec : word;
  ss : double;
begin
  ss := trunc( dt ) * 24 * 60 * 60;
  DecodeTime( Dt, Hour, Minute, Sec, MilSec );
  ss := ss + Hour*3600 + Minute*60 + Sec + MilSec/1000;
  result := FloatToStr(ss);
end;

resourcestring
  rsRHS = 'RHS';
  rsResult = 'Result';

procedure TFormMain.ResetGrid;
var
  i : integer;
begin
  with StringGridMatrix do begin
    RowCount := SpinEdit1.Value + 1;
    FixedRows := 1;
    FixedCols := 0;
    ColCount := RowCount + 1;

    Cells[ 0,0 ] := rsRHS;
    Cells[ ColCount-1,0 ] := rsResult;

    for i := 1 to ColCount-2 do
      Cells[ i, 0 ] := IntToStr(i);
  end;
end;

procedure TFormMain.FormShow(Sender: TObject);
begin
  ResetGrid;
end;

procedure TFormMain.SpinEdit1Change(Sender: TObject);
begin
  ResetGrid;
end;

end.
