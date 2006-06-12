unit ActualFlash;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, Grids,
  StdCtrls, Buttons;

type

  { TFlashForm }

  TFlashForm = class(TForm)
    BitBtn1: TBitBtn;
    Button1: TButton;
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    StringGrid1: TStringGrid;
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  FlashForm: TFlashForm;

implementation

initialization
  {$I actualflash.lrs}

end.

