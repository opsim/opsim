program Demo;

uses
  Forms,
  Demo1 in 'Demo1.pas' {Main};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TMain, Main);
  Application.Run;
end.
