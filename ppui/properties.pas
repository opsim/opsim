unit Properties;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, INIFiles;

var
  AppProps: TINIFile;

function PropertiesINIReadString(const Section, Ident, Default: string): string;
procedure PropertiesINIWriteString(const Section, Ident, Value: string);

implementation

function PropertiesINIReadString(const Section, Ident, Default: string): string;
begin
  Result := AppProps.ReadString(Section, Ident, Default);
end;

procedure PropertiesINIWriteString(const Section, Ident, Value: string);
begin
  AppProps.WriteString(Section, Ident, Value);
  AppProps.UpdateFile;
end;

initialization
  if not Assigned(AppProps) then
    AppProps := TINIFile.Create(ExtractFilePath(ParamStr(0)));

finalization
  AppProps.Free;

end.

