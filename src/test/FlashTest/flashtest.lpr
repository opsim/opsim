program flashtest;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms
  { add your units here }, FlashTestMainForm, zcomponent, SelectComponents,
  Equilibrium, Eos, Eos_Srk, Thermo, Entities;

begin
  Application.Title:='FlashTest';
  Application.Initialize;
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TSelectComponentsForm, SelectComponentsForm);
  Application.Run;
end.

