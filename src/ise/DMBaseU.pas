unit DMBaseU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Dialogs;

type

  { TDMBase }

  TDMBase = class (TDataModule)
  end;
  
var
  DMBase: TDMBase;

implementation

initialization

  {$I DMBaseU.lrs}

end.

