unit PP_enthalpy_liquid;

{$mode objfpc}{$H-}

interface

uses
  fpjson, jsonparser,
  LinkedList, PhysProps;

{
  Calculate the liquid enthalpy from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(T is the temperature in degrees Celcius)
  @returns(the calculated liquid enhtalpy in kJ/kg)
}
function PP_enthalpy_liq_calculate(name : string;
                                   T    : double): double;

{
  Register a new model to calculate the liquid enthalpy from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(range is the temperature range the model is valid for in degrees Celcius)
  @param(coeff is the model coefficients)
  @param(method is the name of the model, internally the correct callback function is then found)
  @param(ref is the reference of the model parameters)
}
procedure PP_enthalpy_liq_register(name        : string;
                                   range       : PPRange;
                                   coeff       : PPCoefficients;
                                   method, ref : string);

{
  Free all allocated data.

  @param(vm is the first liquid enthalpy model from the list)
}
procedure PP_enthalpy_liq_free(vm : pPPModel);

procedure PP_enthalpy_liq_load_JSON(const component: string; const jArray: TJSONArray);

implementation

uses
  math, SysUtils, PP_models;

procedure PP_enthalpy_liq_register(name        : string;
                                   range       : PPRange;
                                   coeff       : PPCoefficients;
                                   method, ref : string);
var
  enth : pPPModel;
begin
  enth := callocN(sizeof(PPModel));

  enth^.name := name;

  //copy the contents from the coefficient record
  enth^.coeff.data := callocN(sizeof(double) * coeff.totcoeff);
  enth^.coeff.totcoeff := coeff.totcoeff;
  move(coeff.data^, enth^.coeff.data^, sizeof(double) * coeff.totcoeff);

  enth^.method := method;
  enth^.reference := ref;

  //determine which callback function to use
  enth^.callback := PP_model_find_callback(lowercase(method));

  enth^.range := range;

  addtail(pp_model^.enthalpy_liq, enth);
end;

function PP_enthalpy_liq_calculate(name : string;
                                   T    : double): double;
var
  enth  : pPPModel;
  lname : string;
  v     : SimVars;
begin
  if pp_model^.enthalpy_liq = nil then exit;

  lname := lowercase(name);
  enth := pp_model^.enthalpy_liq^.first;

  while enth <> nil do
  begin
    if enth^.name = lname then
      //check component name
      if (enth^.range.min <= T) and (enth^.range.max >= T) then
      begin
        v.value := callocN(sizeof(double));
        v.value^ := T;
        PP_enthalpy_liq_calculate := enth^.callback(v, enth^.coeff);
        freeN(v.value);
        exit;
      end;

    enth := enth^.id.next;
  end;
end;

procedure PP_enthalpy_liq_free(vm: pPPModel);
var
  link : pPPModel;
begin
  link := pPPModel(vm);

  //free all coefficients from memory
  while link <> nil do
  begin
    freeN(link^.coeff.data);
    link := link^.id.next;
  end;
end;

procedure PP_enthalpy_liq_load_JSON(const component: string; const jArray: TJSONArray);
var
  coeff   : PPCoefficients;
  i, j    : Integer;
  jArray2 : TJSONArray;
  jData   : TJSONData;
  jObject : TJSONObject;
  method  : string;
  range   : PPRange;
  ref     : string;
begin
  for i := 0 to jArray.Count - 1 do
  begin
    jData := jArray.Items[i];
    jObject := TJSONObject(jData);

    jArray2 := jObject.Get('coeff', TJSONArray(nil));
    coeff.totcoeff:= jArray2.Count;
    coeff.data := callocN(sizeof(double) * coeff.totcoeff);
    for j := 0 to coeff.totcoeff - 1 do
      coeff.data[j] := jArray2.Items[j].AsFloat;

    jArray2 := jObject.Get('range', TJSONArray(nil));
    range._unit := jArray2.Items[0].AsString;
    range.min := jArray2.Items[1].AsFloat;
    range.max := jArray2.Items[2].AsFloat;

    method := jObject.Get('method');
    ref := jObject.Get('ref');
    PP_enthalpy_liq_register(component, range, coeff, method, ref);
    freeN(coeff.data);
  end;
end;

end.
