unit Demo1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Cs2, Cs2_Var, ComCtrls, ExtCtrls, StdCtrls, Menus;

type
  TMain = class(TForm)
    MainMenu1: TMainMenu;
    Memo1: TMemo;
    Splitter1: TSplitter;
    RichEdit1: TRichEdit;
    File1: TMenuItem;
    New1: TMenuItem;
    Open1: TMenuItem;
    Save1: TMenuItem;
    Saveas1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    N2: TMenuItem;
    Script1: TMenuItem;
    Run1: TMenuItem;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    procedure New1Click(Sender: TObject);
    procedure Open1Click(Sender: TObject);
    procedure Save1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure Saveas1Click(Sender: TObject);
    procedure Run1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure Memo1Change(Sender: TObject);
  Private
    { Private declarations }
  Public
    ps: TCs2PascalScript;
    fn: string;
    changed: boolean;
    function SaveTest: boolean;
    procedure AddLine(color: TColor; s: string);
    { Public declarations }
  end;

var
  Main: TMain;

implementation

{$R *.DFM}

procedure TMain.New1Click(SEnder: TObject);
begin
  if not SaveTest then Exit;
  Memo1.Lines.Text := 'Program CajScriptTest;'#13#10'Begin'#13#10'End.';
  RichEdit1.Lines.Clear;
  fn := '';
end;

procedure TMain.AddLine(color: TColor; s: string);
begin
  RichEdit1.SelStart := Length(RichEdit1.Text);
  RichEdit1.SelAttributes.Color := Color;
  RichEdit1.SelAttributes.Size := 8;
  RichEdit1.SelAttributes.Name := 'MS Sans Serif';
  RichEdit1.Lines.Add(S); 
end;

function TMain.SaveTest: boolean;
begin
  if changed then begin
    case MessageBox(handle, 'File is not saved, save now?',
      'CajScript2 Test Program', MB_YESNOCANCEL) of
      ID_YES: begin
          Save1Click(nil);
          Result := not Changed;
        end;
      ID_NO: Result := True;
    else
      Result := False;
    end;
  end else
    Result := True;
end;

procedure TMain.Open1Click(SEnder: TObject);
begin
  if not SaveTest then exit;
  if OpEndialog1.Execute then begin
    Memo1.Lines.LoadFromFile(Opendialog1.FileName);
    Changed := False;
    RichEdit1.Lines.Clear;
    fn := Opendialog1.FileName;
  end;
end;

procedure TMain.Save1Click(SEnder: TObject);
begin
  if Fn = '' then begin
    SaveAS1Click(nil);
  end else begin
    Memo1.Lines.SaveToFile(Fn);
    Changed := False;
  end;
end;

procedure TMain.Exit1Click(Sender: TObject);
begin
  Close;
end;

procedure TMain.Saveas1Click(Sender: TObject);
begin
  SaveDialog1.fileName := '';
  if SaveDialog1.Execute then begin
    fn := SaveDialog1.FileName;
    Memo1.Lines.SaveToFile(Fn);
    Changed := False;
  end;
end;

procedure TMain.Run1Click(Sender: TObject);
begin
  RichEdit1.Clear;
  ps.SetText(Pchar(Memo1.Text));
  AddLine(ClGreen, 'Script is running.');

  ps.RunScript;
  if ps.ErrorCode = ENoError then begin
    AddLine(ClGreen, 'Script finished, no errors.');
  end else begin
    AddLine(ClRed, ErrorToString(ps.ErrorCode));
    Memo1.SelStart := Ps.ErrorPos;
  end;
end;

function RegProc(ID: Pointer; const ProcName: string; Params: PVariableManager;
  res: PCajVariant): TCS2Error;
begin
  if ProcName = 'WRITELN' then begin
    Main.AddLine(ClBlack, GetStr(VM_Get(Params, 0)));
  end else if ProcName = 'READLN' then begin
    GetVarLink(VM_Get(Params, 0))^.Cv_Str := InputBox('Demo', 'Readln:', '');
  end else if ProcName = 'RANDOM' then begin
    SetInteger(Res, Random(GetInt(VM_GET(Params, 0))));
  end;
  Result := ENoError;
end;

function OnUses(Id: Pointer; Sender: PCs2PascalScript; Name: string): TCs2Error;
begin
  if Name = 'SYSTEM' then begin
    RegisterStdLib(Sender);
    PM_Add(Sender.Procedures, '0 WRITELN S 8', @RegProc);
    PM_Add(Sender.Procedures, '0 READLN !S 8', @RegProc);
    PM_Add(Sender.Procedures, '6 RANDOM I 6', @RegProc);
  end;
  Result := ENoError;
end;

function OnRunLine(Id: Pointer; Sender: PCs2PascalScript; Position: Longint):
TCs2Error;
begin
  Application.ProcessMessages;
  Result := ENoError;
end;


procedure TMain.FormCreate(Sender: TObject);
begin
  ps := TCs2PascalScript.Create(nil);
  ps.OnRunLine := OnRunLine;
  ps.OnUses := OnUses;
  fn := '';
  changed := false;
  randomize;
end;

procedure TMain.FormDestroy(Sender: TObject);
begin
  try
    ps.Free;
  except
    showmessage('Error ???');
  end;
end;




procedure TMain.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  CanClose := SaveTest;
end;

procedure TMain.Memo1Change(Sender: TObject);
begin
  changed := true;
  memo1.tag := 1;
end;

end.

