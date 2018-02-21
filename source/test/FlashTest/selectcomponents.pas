unit SelectComponents;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Buttons, DBCtrls;

type

  { TSelectComponentsForm }

  TSelectComponentsForm = class(TForm)
    AddComp: TBitBtn;
    RemoveComp: TBitBtn;
    MoveUpComp: TBitBtn;
    MoveDownComp: TBitBtn;
    BitBtn5: TBitBtn;
    BitBtn6: TBitBtn;
    Label1: TLabel;
    Label2: TLabel;
    AvailableComponents: TListBox;
    SelectedComponents: TListBox;
    procedure AddCompClick(Sender: TObject);
    procedure RemoveCompClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  SelectComponentsForm: TSelectComponentsForm;

implementation

{ TSelectComponentsForm }

procedure TSelectComponentsForm.AddCompClick(Sender: TObject);
var
  AddIndex:Integer;
  SelectIndex: Integer;
begin
  AddIndex:=AvailableComponents.ItemIndex;
  SelectIndex:=SelectedComponents.Items.Add(AvailableComponents.Items[AddIndex]);
  SelectedComponents.ItemIndex:=SelectIndex;
  AvailableComponents.Items.Delete(AddIndex);
end;

procedure TSelectComponentsForm.RemoveCompClick(Sender: TObject);
var
  DelIndex:Integer;
  SelectIndex: Integer;
begin
  DelIndex:=SelectedComponents.ItemIndex;
  SelectIndex:=AvailableComponents.Items.Add(SelectedComponents.Items[DelIndex]);
  AvailableComponents.ItemIndex:=SelectIndex;
  SelectedComponents.Items.Delete(DelIndex);
end;

initialization

  {$I selectcomponents.lrs}

end.

