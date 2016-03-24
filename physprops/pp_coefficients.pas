unit PP_coefficients;

{$mode objfpc}{$H+}

interface

uses
  util;

type
  ID = record
    next, prev : pointer;
  end;


  PPRange = record
    min : double;
    max : double;
  end;

  PPCoefficients = record
    data     : pdouble;
    totcoeff : integer;
  end;

  PPModelData = record
    //conductivity_liq: pListBase;
    //conductivity_vap: pListBase;
    //density_liq: pListBase;
    //density_vap: pListBase;
    //enthalpy_liq: pListBase;
    //enthalpy_vap: pListBase;
    //molweight: pointer;
    vapor_pressure : pListBase;
    //surface_tension: pListBase;
    //viscosity_liq: pListBase;
    //viscosity_vap: pListBase;
  end;
  pPPModelData = ^PPModelData;

var
  PPModel: pPPModelData;

procedure PP_error(const Fmt  : string;
                    const Args : array of Const);

procedure PP_initialize;

procedure PP_free;

implementation

uses
  sysutils,
  PP_vapor_pressure;

procedure PP_error(const Fmt  : string;
                   const Args : array of Const);
begin
  writeln(Format(Fmt, Args));
end;

procedure PP_initialize;
begin
  PPModel := callocN(sizeof(PPModelData), 'physical properties model');
  PPModel^.vapor_pressure := callocN(sizeof(ListBase), 'vapor pressure parameters');
end;

procedure PP_free;
begin
  PP_vapor_pressure_free(PPModel^.vapor_pressure);

  //free the whole vapor pressure model items from memory
  freelistN(PPModel^.vapor_pressure);

  //free the vapor pressure list variable
  freeN(PPModel^.vapor_pressure);

  freeN(PPModel);
end;

end.

