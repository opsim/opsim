unit MixerSpecU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, DB,
  DBGrids;

type

  { TMixerSpec }

  TMixerSpec = class(TForm)
    dbGrid1: TdbGrid;
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  MixerSpec: TMixerSpec;

implementation

uses
  DMBaseU;

initialization
  {$I MixerSpecU.lrs}

end.

