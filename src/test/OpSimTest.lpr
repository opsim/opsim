program OpSimTest;

{$mode objfpc}{$H+}

uses
  Interfaces, Forms, GuiTestRunner, TCompoundsTestCaseU,
  TPropertyProviderTestCaseU, zcomponent, OpSimTestU;

begin
  Application.Initialize;
  Application.CreateForm(TGuiTestRunner, TestRunner);
  Application.Run;
end.

