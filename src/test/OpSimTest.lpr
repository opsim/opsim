program OpSimTest;

{$mode objfpc}{$H+}

uses
  Interfaces, Forms, GuiTestRunner, TCompoundsTestCaseU;

begin
  Application.Initialize;
  Application.CreateForm(TGuiTestRunner, TestRunner);
  Application.Run;
end.

