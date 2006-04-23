program OpSimTest;

{$mode objfpc}{$H+}

uses
  Interfaces, Forms, GuiTestRunner, TCompoundsTestCaseU,
  TPropertyProviderTestCaseU;

begin
  Application.Initialize;
  Application.CreateForm(TGuiTestRunner, TestRunner);
  Application.Run;
end.

