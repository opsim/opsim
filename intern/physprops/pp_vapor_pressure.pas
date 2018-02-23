unit PP_vapor_pressure;

{$mode objfpc}{$H-}

interface

uses
  fpjson, jsonparser,
  MEM_guardedalloc, PhysProps;

{
  Calculate the vapor pressure from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(T is the temperature)
  @returns(the calculated vapor pressure)
}
function PP_vapor_pressure_calculate(name : string;
                                     T    : SimVars): SimVars;

{
  Register a new model to calculate the vapor pressure from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(range is the temperature range the model is valid for)
  @param(coeff is the model coefficients)
  @param(method is the name of the model, internally the correct callback function is then found)
  @param(ref is the reference of the model parameters)
}
procedure PP_vapor_pressure_register(name             : string;
                                     range            : PPRange;
                                     coeff            : PPCoefficients;
                                     method, ref, uom : string);

{
  Free all allocated data

  @param(vm is the first vapor pressure model from the list)
}
procedure PP_vapor_pressure_free(vm : pPPModel);

{
   Loads vapor pressure model data from a JSON file
   @param component: component name
   @param jArray: component data
}
procedure PP_vapor_pressure_load_JSON(const component: string; const jArray: TJSONArray);

implementation

uses
  SysUtils, PP_models, unit_conv;

procedure PP_vapor_pressure_register(name             : string;
                                     range            : PPRange;
                                     coeff            : PPCoefficients;
                                     method, ref, uom : string);
var
  pvap : pPPModel;
begin
  pvap := callocN(sizeof(PPModel), 'PP model');

  pvap^.name := name;

  //copy the contents from the coefficient record
  pvap^.coeff.data := callocN(sizeof(double) * coeff.totcoeff, 'coeff data');
  pvap^.coeff.totcoeff := coeff.totcoeff;
  move(coeff.data^, pvap^.coeff.data^, sizeof(double) * coeff.totcoeff);

  pvap^.uom := UNC_find_conversion(nil, uom);

  pvap^.method := method;
  pvap^.reference := ref;

  //determine which callback function to use
  pvap^.callback := PP_model_find_callback(lowercase(method));

  pvap^.range := range;

  addtail(pp_model^.vapor_pressure, pvap);
end;

function PP_vapor_pressure_calculate(name : string;
                                     T    : SimVars): SimVars;
var
  pvap  : pPPModel;
  lname : string;
  v     : SimVars;
  val   : SimVars;
begin
  if pp_model^.vapor_pressure = nil then exit;

  val.value := callocN(sizeof(double), 'SimVars value');

  lname := lowercase(name);
  pvap := pp_model^.vapor_pressure^.first;

  while pvap <> nil do
  begin
    //check component name
    if pvap^.name = lname then
    begin
      val.uom := pvap^.range.uom;
      val.value^ := UNC_convert_unit(T.value^, T.uom, pvap^.range.uom);

      //check if temperature within limits
      if (pvap^.range.min <= val.value^) and (pvap^.range.max >= val.value^) then
      begin
        v.value := callocN(sizeof(double), 'SimVars value');
        v.value^ := pvap^.callback(val, pvap^.coeff);
        v.uom := pvap^.uom;
        freeN(val.value);
        exit(v);
      end;
    end;

    pvap := pvap^.id.next;
  end;
  freeN(val.value);
end;

procedure PP_vapor_pressure_free(vm: pPPModel);
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

procedure PP_vapor_pressure_load_JSON(const component: string; const jArray: TJSONArray);
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
    coeff.data := callocN(sizeof(double) * coeff.totcoeff, 'coeff data');
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

    PP_vapor_pressure_register(component, range, coeff, method, ref, uom);
    freeN(coeff.data);
  end;
end;

end.
