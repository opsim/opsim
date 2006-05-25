unit FlashTestMainForm;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Buttons,SelectComponents, ZConnection, ZDataset;

type

  { TMainForm }

  TMainForm = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    ToggleBox1: TToggleBox;
    DBConnection: TZConnection;
    DBTable: TZTable;
    procedure Button1Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure ToggleBox1Click(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  MainForm: TMainForm;

implementation


{ TMainForm }

procedure TMainForm.ToggleBox1Click(Sender: TObject);
var
  TempString: String;
begin
  If ToggleBox1.State = cbChecked then
      begin
       DBConnection.Connected := True;
       DBTable.Active:=True;
       SelectComponentsForm.AvailableComponents.Clear;
       SelectComponentsForm.AvailableComponents.Sorted:=True;
       DBTable.First;
       while not DBTable.EOF do
        begin
         TempString:=DBTable['COMPONENT'];
         SelectComponentsForm.AvailableComponents.Items.Add(TempString);
         DBTable.Next;
        end;
      end
    else
      begin
       DBTable.Active:=False;
       DBConnection.Connected := False;
      end;
end;

procedure TMainForm.Button3Click(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.Button1Click(Sender: TObject);
begin
  SelectComponentsForm.ShowModal;
end;

initialization
  {$I flashtestmainform.lrs}

end.

