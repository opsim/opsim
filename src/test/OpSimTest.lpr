program OpSimTest;

{$mode objfpc}{$H+}

uses
  Interfaces, Forms, GuiTestRunner, TThermoTestCaseU,
  TPropertyProviderTestCaseU, zcomponent, OpSimTestU;

begin
  Application.Initialize;
  Application.CreateForm(TGuiTestRunner, TestRunner);
  Application.Run;
end.

