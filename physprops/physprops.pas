unit PhysProps;

{$mode objfpc}{$H-}

interface

uses
  LinkedList, PP_components;

type
  ID = record
    next, prev : pointer;
  end;

  PPCoefficients = record
    data     : pdouble;
    totcoeff : integer;
  end;

  pp_T_callback = function(T     : double;
                           coeff : PPCoefficients): double;

  PPRange = record
    min : double;
    max : double;
  end;

  pPPModel = ^PPModel;
  PPModel = record
    id        : ID;

    name      : string;
    range     : PPRange;
    coeff     : PPCoefficients;
    method    : string;
    reference : string;
    callback  : pointer;
  end;

  PPModelData = record
    modelpath        : string;
    enthalpy_liq     : pListBase;
    vapor_pressure   : pListBase;
    conductivity_liq : pListBase;
    conductivity_vap : pListBase;
    density_liq      : pListBase;
    density_vap      : pListBase;
    enthalpy_vap     : pListBase;
    molweight        : pointer;
    surface_tension  : pListBase;
    viscosity_liq    : pListBase;
    viscosity_vap    : pListBase;
  end;
  pPPModelData = ^PPModelData;

var
  pp_model: pPPModelData;

{
  Display any error from the physical properties library

  @param(fmt is the formatting string for the error)
  @param(args is an array of arguments)
}
procedure PP_error(const fmt  : string;
                   const args : array of Const);

{
  Initialize the physical properties library
}
procedure PP_initialize(modelpath : string);

{
  Free all data from the physical properties library
}
procedure PP_free;

implementation

uses
  SysUtils,
  PP_vapor_pressure,
  PP_enthalpy_liquid;

procedure PP_error(const fmt  : string;
                   const args : array of Const);
begin
  writeln(Format(fmt, args));
end;

procedure PP_initialize(modelpath : string);
begin
  pp_model := callocN(sizeof(PPModelData));

  pp_model^.modelpath := IncludeTrailingPathDelimiter(modelpath);

  pp_model^.conductivity_liq := callocN(sizeof(ListBase));
  pp_model^.conductivity_vap := callocN(sizeof(ListBase));
  pp_model^.density_liq := callocN(sizeof(ListBase));
  pp_model^.density_vap := callocN(sizeof(ListBase));
  pp_model^.enthalpy_liq := callocN(sizeof(ListBase));
  pp_model^.enthalpy_vap := callocN(sizeof(ListBase));
  pp_model^.surface_tension := callocN(sizeof(ListBase));
  pp_model^.vapor_pressure := callocN(sizeof(ListBase));
  pp_model^.viscosity_liq := callocN(sizeof(ListBase));
  pp_model^.viscosity_vap := callocN(sizeof(ListBase));

  //this function loads all defined components
  PP_load_components;
end;

procedure PP_free;
begin
  PP_vapor_pressure_free(pp_model^.vapor_pressure^.first);
  PP_enthalpy_liq_free(pp_model^.enthalpy_liq^.first);

  //free all models from memory
  freelistN(pp_model^.conductivity_liq);
  freelistN(pp_model^.conductivity_vap);
  freelistN(pp_model^.density_liq);
  freelistN(pp_model^.density_vap);
  freelistN(pp_model^.enthalpy_liq);
  freelistN(pp_model^.enthalpy_vap);
  freelistN(pp_model^.surface_tension);
  freelistN(pp_model^.vapor_pressure);
  freelistN(pp_model^.viscosity_liq);
  freelistN(pp_model^.viscosity_vap);

  //free the model list variables
  freeN(pp_model^.conductivity_liq);
  freeN(pp_model^.conductivity_vap);
  freeN(pp_model^.density_liq);
  freeN(pp_model^.density_vap);
  freeN(pp_model^.enthalpy_liq);
  freeN(pp_model^.enthalpy_vap);
  freeN(pp_model^.surface_tension);
  freeN(pp_model^.vapor_pressure);
  freeN(pp_model^.viscosity_liq);
  freeN(pp_model^.viscosity_vap);

  freeN(pp_model);
end;

end.

