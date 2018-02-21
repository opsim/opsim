unit PP_components;

{$mode objfpc}{$H-}

interface

{
   Seaches and loads physical property models from file
}
procedure PP_load_components;

implementation

uses
  SysUtils, Classes,
  fpjson, jsonparser,
  PhysProps,
  PP_vapor_pressure,
  PP_cp_gas;

procedure PP_load_components;
var
  component : string;
  count     : integer = 0;
  fs        : TFileStream;
  info      : TSearchRec;
  jArray    : TJSONArray;
  jData     : TJSONData;
  jObject   : TJSONObject;
begin
  writeln('loading physical property models');
  if FindFirst(pp_model^.modelpath + '*.prop', faAnyFile and faDirectory, info) = 0 then
  begin
    repeat
      inc(count);
      with info do
      begin
        writeln('  ', pp_model^.modelpath + name);
        fs := TFileStream.Create(pp_model^.modelpath + name, fmOpenRead);

        jData := GetJSON(fs);
        jObject := TJSONObject(jData);
        component := lowercase(jObject.Get('component'));

        jArray := jObject.Get('vapor pressure', TJSONArray(nil));
        if jArray <> nil then
          PP_vapor_pressure_load_JSON(component, jArray);

        jArray := jObject.Get('molar heat capacity gas', TJSONArray(nil));
        if jArray <> nil then
          PP_cp_gas_load_JSON(component, jArray);

        jData.Free;

        fs.Free;
      end;
    until FindNext(info) <> 0;
  end;
  FindClose(info);
  writeln(count, ' components loaded');
end;

end.
