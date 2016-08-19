unit PP_cp_gas;

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
function PP_cp_gas_calculate(name : string;
                                   T    : SimVars): SimVars;

{
  Register a new model to calculate the liquid enthalpy from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(range is the temperature range the model is valid for in degrees Celcius)
  @param(coeff is the model coefficients)
  @param(method is the name of the model, internally the correct callback function is then found)
  @param(ref is the reference of the model parameters)
}
procedure PP_cp_gas_register(name             : string;
                                   range            : PPRange;
                                   coeff            : PPCoefficients;
                                   method, ref, uom : string);

{
  Free all allocated data.

  @param(vm is the first liquid enthalpy model from the list)
}
procedure PP_cp_gas_free(vm : pPPModel);

procedure PP_cp_gas_load_JSON(const component: string; const jArray: TJSONArray);

implementation

uses
  SysUtils, PP_models, unit_conv;

procedure PP_cp_gas_register(name             : string;
                             range            : PPRange;
                             coeff            : PPCoefficients;
                             method, ref, uom : string);
var
  cp : pPPModel;
begin
  cp := callocN(sizeof(PPModel));

  cp^.name := name;

  //copy the contents from the coefficient record
  cp^.coeff.data := callocN(sizeof(double) * coeff.totcoeff);
  cp^.coeff.totcoeff := coeff.totcoeff;
  move(coeff.data^, cp^.coeff.data^, sizeof(double) * coeff.totcoeff);

  cp^.uom := UNC_find_conversion(nil, uom);

  cp^.method := method;
  cp^.reference := ref;

  //determine which callback function to use
  cp^.callback := PP_model_find_callback(lowercase(method));

  cp^.range := range;

  addtail(pp_model^.cp_gas, cp);
end;

function PP_cp_gas_calculate(name : string;
                                   T    : SimVars): SimVars;
var
  cp  : pPPModel;
  lname : string;
  v     : SimVars;
  val   : SimVars;
begin
  if pp_model^.cp_gas = nil then exit;

  val.value := callocN(sizeof(double));

  lname := lowercase(name);
  cp := pp_model^.cp_gas^.first;

  while cp <> nil do
  begin
    //check component name
    if cp^.name = lname then
    begin
      val.uom := cp^.range.uom;
      val.value^ := UNC_convert_unit(T.value^, T.uom, cp^.range.uom);

      //check if temperature within limits
      if (cp^.range.min <= val.value^) and (cp^.range.max >= val.value^) then
      begin
        v.value := callocN(sizeof(double));
        v.value^ := cp^.callback(val, cp^.coeff);
        v.uom := cp^.uom;
        freeN(val.value);
        exit(v);
      end;
    end;

    cp := cp^.id.next;
  end;
  freeN(val.value);
end;

procedure PP_cp_gas_free(vm: pPPModel);
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

procedure PP_cp_gas_load_JSON(const component: string; const jArray: TJSONArray);
var
  coeff   : PPCoefficients;
  i, j    : Integer;
  jArray2 : TJSONArray;
  jData   : TJSONData;
  jObject : TJSONObject;
  method  : string;
  range   : PPRange;
  ref     : string;
  uom     : TJSONStringType;
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
    uom := jArray2.Items[0].AsString;
    range.uom := UNC_find_conversion(nil, uom);
    range.min := jArray2.Items[1].AsFloat;
    range.max := jArray2.Items[2].AsFloat;

    uom := jObject.Get('result unit');
    method := jObject.Get('method');
    ref := jObject.Get('ref');
    PP_cp_gas_register(component, range, coeff, method, ref, uom);
    freeN(coeff.data);
  end;
end;

end.
