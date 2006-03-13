program OpSim;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms
  { add your units here }, MainU, AboutU, Unit1, PfdDesignerU, UnitopPalletU;

begin
  Application.Initialize;
  Application.CreateForm(TMain, Main);
  Application.CreateForm(TUnitopPallet, UnitopPallet);
  Application.CreateForm(TPfdDesigner, PfdDesigner);
  Application.Run;
end.

