unit PfdWorkSpaceFormU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls;

type

  { TPfdWorkSpaceForm }

  TPfdWorkSpaceForm = class(TForm)
    Panel1: TPanel;
    ScrollBox1: TScrollBox;
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  PfdWorkSpaceForm: TPfdWorkSpaceForm;

implementation

initialization
  {$I pfdworkspaceformu.lrs}

end.

