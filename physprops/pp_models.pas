unit PP_models;

{$mode objfpc}{$H-}

interface

uses
  PhysProps;

const
  R = 8.3144598;
  R2 = R * R;

function PP_model_find_callback(method: string): pp_callback;

// Andrade equation for calculating the liquid viscosity
function PP_model_Andrade(vars: SimVars; coeff: PPCoefficients): double;

// Antoine vapor pressure equation
function PP_model_Antoine(vars: SimVars; coeff: PPCoefficients): double;

// Antoine equation for the viscosity
function PP_model_Antoine_viscosity(vars: SimVars; coeff: PPCoefficients): double;

// Barin equations for thermophysical property data
function PP_model_Barin(vars: SimVars; coeff: PPCoefficients): double;

// Bromley-Pitzer activity coefficient model (Aspen[7], page 3-54)
//function PP_model_Bromley_Pitzer(vars  : SimVars; coeff : PPCoefficients) : double;

// BWR-equation of state
function PP_model_BWR(vars: SimVars; coeff: PPCoefficients): double;

// Benedict-Webb-Rubin-Lee-Starling equation of state (Aspen[7], page 3-8)
//function PP_model_BWR_Lee_Starling(vars  : SimVars; coeff : PPCoefficients) : double;

// Chebyshev vapor pressure equation
//function PP_model_Chebyshev(vars  : SimVars; coeff : PPCoefficients) : double;

// Chien-Null model for calculation activity coefficient of highly non-ideal systems (Aspen[7], page 3-55)
//function PP_model_Chien_Null(vars  : SimVars; coeff : PPCoefficients) : double;

// Chung-Lee-Starling correlation of the viscosity and thermal conductivity of liquid or gaseous mixtures  (Aspen[7], page 3-127, 3138))
//function PP_model_Chung_Lee_Starling(vars  : SimVars; coeff : PPCoefficients) : double;

// coefficients of aqueous systems (Aspen [7], page 3-63)
//function PP_model_coefficient_model(vars  : SimVars; coeff : PPCoefficients) : double;

// Cragoe vapor pressure equation
function PP_model_Cragoe(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR101 equation
function PP_model_DIPPR101(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR equation for the gas viscosity at 0 atm pressure and the gas thermal conductivity
function PP_model_DIPPR102(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR104 equation
function PP_model_DIPPR104(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR105 equation
function PP_model_DIPPR105(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR106 equation
function PP_model_DIPPR106(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR equation for the ideal heat capacity
function PP_model_DIPPR107(vars: SimVars; coeff: PPCoefficients): double;

// NRTL activity coefficient model for electrolytes(Aspen[7], page 358)
//function PP_model_Electrolyte_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;

// Margules equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-24)
//function PP_model_extended_Margules(vars  : SimVars; coeff : PPCoefficients) : double;

// NRTL activity coefficient model (Aspen[7], page 3-62)
//function PP_model_extended_NRTL_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;

// extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-74)
//function PP_model_extended_UNIQUAC_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;

// extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-26)
//function PP_model_extended_UNIQUAC_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;

// extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-75)
//function PP_model_extended_van_Laar_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;

// extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-28)
//function PP_model_extended_van_Laar_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;

// extended Wilson equation for calculating liquid activity (Aspen[7], page 3-78)
//function PP_model_extended_Wilson_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;

// extended Wilson equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-29)
//function PP_model_extended_Wilson_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;

// general NRTL activity coefficient model (Hysys[9], page A-22)
//function PP_model_general_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;

// Hakim-Steinberg-Stiel equation for the surface tension (Aspen[7], page 3-155)
//function PP_model_Hakim_Steinberg_Stiel(vars  : SimVars; coeff : PPCoefficients) : double;

// Hayden-O''Connel equation of state (Aspen[7], page 3-9)
//function PP_model_Hayden_O_Connel(vars  : SimVars; coeff : PPCoefficients) : double;

// Aspen[7]-equation for the solid heat capacity (page 3-102)
function PP_model_heat_capacity_ASPEN(vars: SimVars; coeff: PPCoefficients): double;

// Jones-Dole equation
function PP_model_Jones_Dole(vars: SimVars; coeff: PPCoefficients): double;

// Lee-Kesler equation of state (Aspen[7], page 3-18)
//function PP_model_Lee_Kesler(vars  : SimVars; coeff : PPCoefficients) : double;

// Lee-Kesler-Plocker equation of state (Aspen[7], page 3-19)
//function PP_model_Lee_Kesler_Plocker(vars  : SimVars; coeff : PPCoefficients) : double;

// DIPPR equation for the liquid viscosity
function PP_model_liquid_viscosity_DIPPR(vars: SimVars; coeff: PPCoefficients): double;

// Margules equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_Margules(vars  : SimVars; coeff : PPCoefficients) : double;

// modified Antoine vapor pressure equation (Aspen[7], page 3-80)
function PP_model_mod_Antoine_Aspen(vars: SimVars; coeff: PPCoefficients): double;

// modified Antoine vapor pressure equation (Hysys[9], page A-36)
function PP_model_mod_Antoine_Hysys(vars: SimVars; coeff: PPCoefficients): double;

// modified polynomial
//function PP_model_modified_polynomial(vars  : SimVars; coeff : PPCoefficients) : double;

// NRTL activity coefficient model (DDB[8], page XVI)
//function PP_model_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;

// standard Peng-Robinson equation of state (Aspen[7], page 3-34)
function PP_model_Peng_Robinson(vars: SimVars; coeff: PPCoefficients): double;

// Peng-Robinson-Boston-Mathias equation of state (Aspen[7], page 325)
function PP_model_Peng_Robinson_Boston_Mathias(vars: SimVars; coeff: PPCoefficients): double;

// Pitzer model for activity
//function PP_model_Pitzer_activity(vars  : SimVars; coeff : PPCoefficients) : double;

// polynomial
function PP_model_polynomial(vars: SimVars; coeff: PPCoefficients): double;

// Redlich-Kister equation for excess properties in binary systems
//function PP_model_Redlich_Kister(vars  : SimVars; coeff : PPCoefficients) : double;

// Redlich-Kister model for calculating activity coefficients (Aspen[7], page 3-66)
//function PP_model_Redlich_Kister_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;

// Redlich-Kwong equation of state (Aspen[7], page 3-27)
function PP_model_Redlich_Kwong(vars: SimVars; coeff: PPCoefficients): double;

// Aspen modification of the Redlich-Kwong equation of state( Aspen[7], page 3-28)
function PP_model_Redlich_Kwong_Aspen(vars: SimVars; coeff: PPCoefficients): double;

// standard Redlich-Kwong-Soave equation of state (Aspen[7], page 335)
function PP_model_Redlich_Kwong_Soave(vars: SimVars; coeff: PPCoefficients): double;

// Redlich-Kwong equation of state with Boston-Mathias alpha function (Aspen[7], page 3-29)
function PP_model_Redlich_Kwong_Soave_Boston_Mathias(vars: SimVars; coeff: PPCoefficients): double;

// Riedel vapor pressure equation
function PP_model_Riedel(vars: SimVars; coeff: PPCoefficients): double;

// Riedel equation for thermal conductivities
function PP_model_Riedel_therm_cond(vars: SimVars; coeff: PPCoefficients): double;

// Scatchard-Hildebrand model (Aspen[7], page 3-67)
//function PP_model_Scatchard_Hildebrand(vars  : SimVars; coeff : PPCoefficients) : double;

// Schwartzentruber-Renon equation of state (Aspen[7], page 3-31)
//function PP_model_Schwartzentruber_Renon(vars  : SimVars; coeff : PPCoefficients) : double;

// Surface Tension after Sprow and Prausnitz
function PP_model_Sprow_Prausnitz(vars: SimVars; coeff: PPCoefficients): double;

// DIPPR correlation for surface tension
function PP_model_suface_tension_DIPPR(vars: SimVars; coeff: PPCoefficients): double;

// NEL equation for thermal conductivity
function PP_model_thermal_conductivity_NEL(vars: SimVars; coeff: PPCoefficients): double;

// extended Margules equation for calculating liquid activity coefficients (Aspen[7], page 3-68)
//function PP_model_three_suffix_Margules(vars  : SimVars; coeff : PPCoefficients) : double;

// UNIQUAC equation for calculating liquid activity coefficients (DDB[8], page XVII)
//function PP_model_UNIQUAC(vars  : SimVars; coeff : PPCoefficients) : double;

// van Laar equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_van_Laar(vars  : SimVars; coeff : PPCoefficients) : double;

// vapor pressure equation
function PP_model_vapor_pressure_1(vars: SimVars; coeff: PPCoefficients): double;

// virial equation
//function PP_model_virial_equation(vars  : SimVars; coeff : PPCoefficients) : double;

// ASPEN[7] mixing rule for the liquid viscosity (listed under the heading Andrade/DIPPR, page 3122)
//function PP_model_viscosity_mixing_rule(vars  : SimVars; coeff : PPCoefficients) : double;

// Wagner vapor pressure equation
function PP_model_Wagner(vars: SimVars; coeff: PPCoefficients): double;

// 2nd Wagner vapor pressure equation
function PP_model_Wagner2(vars: SimVars; coeff: PPCoefficients): double;

// Wagner vapor pressure equation (Aspen)
function PP_model_Wagner3(vars: SimVars; coeff: PPCoefficients): double;

// Wilson equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_Wilson(vars  : SimVars; coeff : PPCoefficients) : double;

// Wrede vapor pressure equation
function PP_model_Wrede(vars: SimVars; coeff: PPCoefficients): double;

// Wrede vapor pressure equation
function PP_model_Wrede_ln(vars: SimVars; coeff: PPCoefficients): double;

// Yen-Woods equation for densities
//function PP_model_Yen_Woods(vars  : SimVars; coeff : PPCoefficients) : double;

// Yuan - Mok equation for the heat capacity
function PP_model_Yuan_Mok(vars: SimVars; coeff: PPCoefficients): double;

const
  PPMODELCOUNT = 35;

  PPModelTable: array[1 .. PPMODELCOUNT] of PPModelCallback =
    (
    (callback: @PP_model_Andrade; method: 'andrade'; description: 'Andrade equation for calculating the liquid viscosity '; equation: 'ln(?) = A + B/T + C*ln(T) '),
    (callback: @PP_model_Antoine; method: 'antoine'; description: 'Antoine vapor pressure equation '; equation: 'log(p) = a - b/(T+c) '),
    (callback: @PP_model_Antoine_viscosity; method: 'antoine_viscosity'; description: 'Antoine equation for the viscosity '; equation: 'ln(?) = a + b/(T+c) '),
    (callback: @PP_model_Barin; method: 'barin'; description: 'Barin equations for thermophysical property data '; equation: 'G = a + b*T + c*(T*ln(T)) + d*T2 + e*T3 + f*T4 + g/T + h/T2 '),
    //(callback: @PP_model_Bromley_Pitzer; method: 'bromley_pitzer'; description: 'Bromley-Pitzer activity coefficient model (Aspen[7], page 3-54) '; equation: ''),
    (callback: @PP_model_BWR; method: 'bwr'; description: 'BWR-equation of state '; equation: 'p = R*T*d + (b0*R*T -a0 c0/T2)*d2 + (b0*R*T - a0)*d3 +a*a*d6 + (c*d3/T2)*(1 + ?*d2)*exp(-?*d2) '),
    //(callback: @PP_model_BWR_Lee_Starling; method: 'bwr_lee_starling'; description: 'Benedict-Webb-Rubin-Lee-Starling equation of state (Aspen[7], page 3-8) '; equation: 'Zm = Zm0 + ?i*Zm1; Zm0, Zm1 =function(T,Tcrit,Vm,vcrit,m) '),
    //(callback: @PP_model_Chebyshev; method: 'chebyshev'; description: 'Chebyshev vapor pressure equation '; equation: 'T*log(p) = c0/2 + ?(s)[cs * Es(x)]; x = 2*T - (Tmax+Tmin)/(Tmax-Tmin); Es(x) = Chebyshev polynomial of order s '),
    //(callback: @PP_model_Chien_Null; method: 'chien_null'; description: 'Chien-Null model for calculation activity coefficient of highly non-ideal systems (Aspen[7], page 3-55) '; equation: ''),
    //(callback: @PP_model_Chung_Lee_Starling; method: 'chung_lee_starling'; description: 'Chung-Lee-Starling correlation of the viscosity and thermal conductivity of liquid or gaseous mixtures  (Aspen[7], page 3-127, 3138)) '; equation: ''),
    //(callback: @PP_model_coefficient_model; method: 'coefficient_model'; description: 'coefficients of aqueous systems (Aspen [7], page 3-63) '; equation: ''),
    (callback: @PP_model_Cragoe; method: 'cragoe'; description: 'Cragoe vapor pressure equation '; equation: 'log(p) = a + b/T + c*T + d*T2 '),
    (callback: @PP_model_DIPPR101; method: 'dippr101'; description: 'DIPPR101 equation'; equation: 'property = exp(A + B/T + C*ln(T) + D*TE) '),
    (callback: @PP_model_DIPPR102; method: 'dippr102'; description: 'DIPPR equation for the gas viscosity at 0 atm pressure and the gas thermal conductivity '; equation: 'property = A*TB/(1 + C/T + D/T2) '),
    (callback: @PP_model_DIPPR104; method: 'dippr104'; description: 'DIPPR104 equation'; equation: 'property = A + B/T + C/T3 + D/T8 + E/T9 '),
    (callback: @PP_model_DIPPR105; method: 'dippr105'; description: 'DIPPR105 equation'; equation: 'property = A/B^[1+(1-T/C)D] '),
    (callback: @PP_model_DIPPR106; method: 'dippr106'; description: 'DIPPR106 equation'; equation: 'property = A*(1-Tr)^(B + C*Tr + D*Tr 2); Tr = T/Tcrrit '),
    (callback: @PP_model_DIPPR107; method: 'dippr107'; description: 'DIPPR equation for the ideal heat capacity '; equation: 'property = A + B[C/T/sinh(C/T)]2 + D[E/T/cosh(E/T)]2 '),
    //(callback: @PP_model_Electrolyte_NRTL; method: 'electrolyte_nrtl'; description: 'NRTL activity coefficient model for electrolytes(Aspen[7], page 358) '; equation: ''),
    //(callback: @PP_model_extended_Margules; method: 'extended_margules'; description: 'Margules equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-24) '; equation: 'ln(?i) = (1-xi)*+2*[Ai + 2+xi*(Bi-Ai)]; Ai = S(j)[xj*(aij+bij*T)/(1-xi)]; Bi = S(j)[xj*(aji+bji*T)/(1-xi)] '),
    //(callback: @PP_model_extended_NRTL_Aspen; method: 'extended_nrtl_aspen'; description: 'NRTL activity coefficient model (Aspen[7], page 3-62) '; equation: ''),
    //(callback: @PP_model_extended_UNIQUAC_Aspen; method: 'extended_uniquac_aspen'; description: 'extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-74) '; equation: ''),
    //(callback: @PP_model_extended_UNIQUAC_Hysys; method: 'extended_uniquac_hysys'; description: 'extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-26) '; equation: ''),
    //(callback: @PP_model_extended_van_Laar_Aspen; method: 'extended_van_laar_aspen'; description: 'extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-75) '; equation: ''),
    //(callback: @PP_model_extended_van_Laar_Hysys; method: 'extended_van_laar_hysys'; description: 'extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-28) '; equation: ''),
    //(callback: @PP_model_extended_Wilson_Aspen; method: 'extended_wilson_aspen'; description: 'extended Wilson equation for calculating liquid activity (Aspen[7], page 3-78) '; equation: ''),
    //(callback: @PP_model_extended_Wilson_Hysys; method: 'extended_wilson_hysys'; description: 'extended Wilson equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-29) '; equation: ''),
    //(callback: @PP_model_general_NRTL; method: 'general_nrtl'; description: 'general NRTL activity coefficient model (Hysys[9], page A-22) '; equation: ''),
    //(callback: @PP_model_Hakim_Steinberg_Stiel; method: 'hakim_steinberg_stiel'; description: 'Hakim-Steinberg-Stiel equation for the surface tension (Aspen[7], page 3-155) '; equation: ''),
    //(callback: @PP_model_Hayden_O_Connel; method: 'hayden_o_connel'; description: 'Hayden-O''Connel equation of state (Aspen[7], page 3-9) '; equation: 'Zm = 1 + B*p/R*T; B = S(i)S(j)Bij(T) '),
    (callback: @PP_model_heat_capacity_ASPEN; method: 'heat_capacity_aspen'; description: 'Aspen[7]-equation for the solid heat capacity (page 3-102) '; equation: 'Cp = c1 + c2*T + c3*T2 + c4/T + c5/T2 + c6/v(T) '),
    (callback: @PP_model_Jones_Dole; method: 'jones_dole'; description: 'Jones-Dole equation '; equation: '?/?0 = 1 + a*vc + b*c '),
    //(callback: @PP_model_Lee_Kesler; method: 'lee_kesler'; description: 'Lee-Kesler equation of state (Aspen[7], page 3-18) '; equation: 'Z = Z0 + (Zr-Z0)*?/?r Z0 = fct0(T/Tcrit, p/prit) Zr  = fctr(T/Tcrit, p/Pcrit) '),
    //(callback: @PP_model_Lee_Kesler_Plocker; method: 'lee_kesler_plocker'; description: 'Lee-Kesler-Plocker equation of state (Aspen[7], page 3-19) '; equation: 'Zm = Zm0 + (?/?r)*(Zm0 - Zmr) ; Zm0 = fct0(T,Tcrit, vm, vcrit,m); Zmr = fctf(T,Tcrit,Vm,vcrit,m) mixing rules for vcrit,m, Tcrit '),
    (callback: @PP_model_liquid_viscosity_DIPPR; method: 'liquid_viscosity_dippr'; description: 'DIPPR equation for the liquid viscosity '; equation: 'ln(?) = c1 + c2/T + c3*ln(T) + c4*Tc5 '),
    //(callback: @PP_model_Margules; method: 'margules'; description: 'Margules equation for calculating liquid activity coefficients (DDB[8], page XVI) '; equation: 'ln(?i) = [Aij + 2*(Aji-Aij)*xi](1-xi)2 '),
    (callback: @PP_model_mod_Antoine_Aspen; method: 'mod_antoine_aspen'; description: 'modified Antoine vapor pressure equation (Aspen[7], page 3-80) '; equation: 'ln(p) = A + B/(T+C) + D*ln(T) + E*TF '),
    (callback: @PP_model_mod_Antoine_Hysys; method: 'mod_antoine_hysys'; description: 'modified Antoine vapor pressure equation (Hysys[9], page A-36) '; equation: 'ln(p) = A + B/(T+C) + D*T + E*ln(T) + F*TG '),
    //(callback: @PP_model_modified_polynomial; method: 'modified_polynomial'; description: 'modified polynomial '; equation: 'property = a + b/T + c/T2 + d*T + e*T2 + f*T3 + ... '),
    //(callback: @PP_model_NRTL; method: 'nrtl'; description: 'NRTL activity coefficient model (DDB[8], page XVI) '; equation: ''),
    (callback: @PP_model_Peng_Robinson; method: 'peng_robinson'; description: 'standard Peng-Robinson equation of state (Aspen[7], page 3-34) '; equation: 'p = R*T/(Vm-b) a/[Vm*(Vm+b)+b*(Vm-b)] '),
    (callback: @PP_model_Peng_Robinson_Boston_Mathias; method: 'peng_robinson_boston_mathias'; description: 'Peng-Robinson-Boston-Mathias equation of state (Aspen[7], page 325) '; equation: 'p = R*T/(Vm-b) -(a/√(T))/[Vm*(Vm+b)]'),
    //(callback: @PP_model_Pitzer_activity; method: 'pitzer_activity'; description: 'Pitzer model for activity '; equation: ''),
    (callback: @PP_model_polynomial; method: 'polynomial'; description: 'polynomial '; equation: 'y = a + b*x + c*x2 + ...+ j*x9 x = any property '),
    //(callback: @PP_model_Redlich_Kister; method: 'redlich_kister'; description: 'Redlich-Kister equation for excess properties in binary systems '; equation: '? property = x1 * x2 * S(i)(ai*(x1x2)i) '),
    //(callback: @PP_model_Redlich_Kister_Aspen; method: 'redlich_kister_aspen'; description: 'Redlich-Kister model for calculating activity coefficients (Aspen[7], page 3-66) '; equation: ''),
    (callback: @PP_model_Redlich_Kwong; method: 'redlich_kwong'; description: 'Redlich-Kwong equation of state (Aspen[7], page 3-27) '; equation: 'p = R*T/(Vm-b) (a/v(T))/[Vm*(Vm+b)] '),
    (callback: @PP_model_Redlich_Kwong_Aspen; method: 'redlich_kwong_aspen'; description: 'Aspen modification of the Redlich-Kwong equation of state( Aspen[7], page 3-28) '; equation: 'p = R*T/(Vm-b) - a/[Vm*(Vm+b)] with mixing rules '),
    (callback: @PP_model_Redlich_Kwong_Soave; method: 'redlich_kwong_soave'; description: 'standard Redlich-Kwong-Soave equation of state (Aspen[7], page 335) '; equation: 'p = R*T/(Vm-b) - a/[Vm*(Vm+b)]'),
    (callback: @PP_model_Redlich_Kwong_Soave_Boston_Mathias; method: 'redlich_kwong_soave_boston_mathias'; description: 'Redlich-Kwong equation of state with Boston-Mathias alpha function (Aspen[7], page 3-29) '; equation: 'p = R*T/(Vm-b) - a/[Vm*(Vm+b)] with mixing rules '),
    (callback: @PP_model_Riedel; method: 'riedel'; description: 'Riedel vapor pressure equation '; equation: 'ln(p) = a - b/T + c*T + d*T2 + e*ln(T) '),
    (callback: @PP_model_Riedel_therm_cond; method: 'riedel_therm_cond'; description: 'Riedel equation for thermal conductivities '; equation: '? = a * (1 + (20/3)*(1 - T/Tcrit)(2/3)) '),
    //(callback: @PP_model_Scatchard_Hildebrand; method: 'scatchard_hildebrand'; description: 'Scatchard-Hildebrand model (Aspen[7], page 3-67) '; equation: ''),
    //(callback: @PP_model_Schwartzentruber_Renon; method: 'schwartzentruber_renon'; description: 'Schwartzentruber-Renon equation of state (Aspen[7], page 3-31) '; equation: 'p = R*T/(Vm+c-b) a/[(Vm+c)*(Vm+c+b)] with mixing rules '),
    //(callback: @PP_model_Sprow_Prausni_tz; method: 'sprow_prausni_tz'; description: 'Surface Tension after Sprow and Prausnitz '; equation: 's = a * (1 - T/Tcrit)b '),
    (callback: @PP_model_suface_tension_DIPPR; method: 'suface_tension_dippr'; description: 'DIPPR correlation for surface tension '; equation: 's = c1*(1-Tr)^(c2 + c3*Tr + c4*Tr 2 + c5*Tr 3); Tr = T/Tcrit '),
    (callback: @PP_model_thermal_conductivity_NEL; method: 'thermal_conductivity_nel'; description: 'NEL equation for thermal conductivity '; equation: '? = a*(1 + b*x(1/3) + c*x(2/3) + d*x); x=1-T/Tcrit '),
    //(callback: @PP_model_three_suffix_Margules; method: 'three_suffix_margules'; description: 'extended Margules equation for calculating liquid activity coefficients (Aspen[7], page 3-68) '; equation: ''),
    //(callback: @PP_model_UNIQUAC; method: 'uniquac'; description: 'UNIQUAC equation for calculating liquid activity coefficients (DDB[8], page XVII) '; equation: ''),
    //(callback: @PP_model_van_Laar; method: 'van_laar'; description: 'van Laar equation for calculating liquid activity coefficients (DDB[8], page XVI) '; equation: ''),
    (callback: @PP_model_vapor_pressure_1; method: 'vapor_pressure_1'; description: 'vapor pressure equation '; equation: 'ln(p) = a + b*T + c/T + d/T2 '),
    //(callback: @PP_model_virial_equation; method: 'virial_equation'; description: 'virial equation '; equation: 'Z = 1 + vc2*p + vc3*p2 + vc4*p4 + ... '),
    //(callback: @PP_model_viscosity_mixing_rule; method: 'viscosity_mixing_rule'; description: 'ASPEN[7] mixing rule for the liquid viscosity (listed under the heading Andrade/DIPPR, page 3122) '; equation: 'ln(?) = S(i)[x_i*ln(?i)] + S(i,j)[(aij + bij/T)*xi*xj + (cij+dij/T)*xi 2*xj 2] '),
    (callback: @PP_model_Wagner; method: 'wagner'; description: 'Wagner vapor pressure equation '; equation: 'ln(p/Pcrit) = (a*x + b*x(3/2) + c*x3 + d*x6)/(T/Tcrit); x = 1 - T/Tcrit '),
    (callback: @PP_model_Wagner2; method: 'wagner2'; description: '2nd Wagner vapor pressure equation '; equation: 'ln(p/Pcrit) = (a*x + b*x(3/2) + c*x3 + d*x7 + e*x9)/(T/Tcrit); x = 1 - T/Tcrit '),
    (callback: @PP_model_Wagner3; method: 'wagner3'; description: 'Wagner vapor pressure equation (Aspen) '; equation: 'ln(p/Pcrit) = (a*x + b*x(3/2) + c*x3 + d*x4)/(T/Tcrit); x = 1 - T/Tcrit '),
    //(callback: @PP_model_Wilson; method: 'wilson'; description: 'Wilson equation for calculating liquid activity coefficients (DDB[8], page XVI) '; equation: ''),
    (callback: @PP_model_Wrede; method: 'wrede'; description: 'Wrede vapor pressure equation '; equation: 'log(p) = a - b/T '),
    (callback: @PP_model_Wrede_ln; method: 'wrede_ln'; description: 'Wrede vapor pressure equation '; equation: 'ln(p) = a - b/T '),
    //(callback: @PP_model_Yen_Woods; method: 'yen_woods'; description: 'Yen-Woods equation for densities '; equation: 'd = dcrit * (a + ?(j)(kj)*(1T/Tcrit)(j/3)) '),
    (callback: @PP_model_Yuan_Mok; method: 'yuan_mok'; description: 'Yuan - Mok equation for the heat capacity '; equation: 'cp = a + b * exp(-c/Tn) ')
    );

implementation

uses
  Math;

function PP_model_find_callback(method: string): pp_callback;
var
  left: word = 1;
  right: word = PPMODELCOUNT;
  mid: word;
begin
  //binary search for the correct position in the periodic table
  while (PPModelTable[right].method <> method) and
    (PPModelTable[left].method <> method) and (right - left > 1) do
  begin
    //calculate midpoint of range
    mid := (left + right) div 2;

    if (PPModelTable[left].method <= method) and (PPModelTable[mid].method >= method) then
      //throw away right half
      right := mid
    else
      //throw away left half
      left := mid;
  end;

  if PPModelTable[right].method = method then
    exit(PPModelTable[right].callback);
  if PPModelTable[left].method = method then
    exit(PPModelTable[left].callback);
  exit(nil);
end;

// Andrade equation for calculating the liquid viscosity
function PP_model_Andrade(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  T := vars.value[0];

  PP_model_Andrade := exp(A + B / T + C * ln(T));
end;

// Antoine vapor pressure equation
function PP_model_Antoine(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  T := vars.value[0];

  PP_model_Antoine := power(10, a - b / (T + c));
end;

// Antoine equation for the viscosity
function PP_model_Antoine_viscosity(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  T := vars.value[0];

  PP_model_Antoine_viscosity := exp(a + b / (T + c));
end;

// Barin equations for thermophysical property data
function PP_model_Barin(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, e, f, g, h: double;
  T, T2, T3, T4: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  e := coeff.data[4];
  f := coeff.data[5];
  g := coeff.data[6];
  h := coeff.data[7];

  T := vars.value[0];
  T2 := T * T;
  T3 := T2 * T;
  T4 := T3 * T;

  PP_model_Barin := a + b * T + c * (T * ln(T)) + d * T2 + e * T3 + f * T4 + g / T + h / T2;
end;

// Bromley-Pitzer activity coefficient model (Aspen[7], page 3-54)
//function PP_model_Bromley_Pitzer(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  beta_ion, delta_ion, beta_0, beta_1, beta_2, beta_3 : double;
//begin

//end;

// BWR-equation of state
function PP_model_BWR(vars: SimVars; coeff: PPCoefficients): double;
var
  a0, b0, c0, a, b, c, alpha, gamma: double;
  d, d2, d3, d6: double;
  T, T2: double;
begin
  a0 := coeff.data[0];
  b0 := coeff.data[1];
  c0 := coeff.data[2];
  a := coeff.data[3];
  b := coeff.data[4];
  c := coeff.data[5];
  alpha := coeff.data[6];
  gamma := coeff.data[7];
  d := coeff.data[6];
  d2 := d * d;
  d3 := d2 * d;
  d6 := d3 * d3;

  T := vars.value[0];
  T2 := T * T;

  PP_model_BWR := R * T * d + (b0 * R * T - a0 - c0 / T2) * d2 + (b0 * R * T - a0) * d3 + alpha * a * d6 + (c * d3 / T2) * (1 + gamma * d2) * exp(-gamma * d2);
end;

// Benedict-Webb-Rubin-Lee-Starling equation of state (Aspen[7], page 3-8)
//function PP_model_BWR_Lee_Starling(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  Tcrit _i, criticalVolume_i, gamma_i, epsilon_i_j, eta_i_j : double;
//begin
//  Zm := Zm0 + ?i*Zm1; Zm0, Zm1 =function(T,Tcrit,Vm,vcrit,m) ;
//end;

// Chebyshev vapor pressure equation
//function PP_model_Chebyshev(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  c_0, c_1, c_2, ....., T_min, T_max : double;
//begin
//  T*log(p) := c0/2 + ?(s)[cs * Es(x)]; x := 2*T - (Tmax+Tmin)/(Tmax-Tmin); Es(x) := Chebyshev polynomial of order s ;
//end;

// Chien-Null model for calculation activity coefficient of highly non-ideal systems (Aspen[7], page 3-55)
//function PP_model_Chien_Null(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, v_i_j : double;
//begin
//  ;
//end;

// Chung-Lee-Starling correlation of the viscosity and thermal conductivity of liquid or gaseous mixtures  (Aspen[7], page 3-127, 3138))
//function PP_model_Chung_Lee_Starling(vars  : SimVars; coeff : PPCoefficients) : double;
//begin
//  ;
//end;

// coefficients of aqueous systems (Aspen [7], page 3-63)
//function PP_model_coefficient_model(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  beta_3, C_p, theta_c_cs, theta_a_as, psi_c_cs_a, psi_c_a_as : double;
//begin
//  ;
//end;

// Cragoe vapor pressure equation
function PP_model_Cragoe(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d: double;
  T, T2: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];

  T := vars.value[0];
  T2 := T * T;

  PP_model_Cragoe := power(10, a + b / T + c * T + d * T2);
end;

// DIPPR101 equation
function PP_model_DIPPR101(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, E: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  E := coeff.data[4];

  T := vars.value[0];

  PP_model_DIPPR101 := exp(A + B / T + C * ln(T) + D * power(T, E));
end;

// DIPPR equation for the gas viscosity at 0 atm pressure and the gas thermal conductivity
function PP_model_DIPPR102(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D: double;
  T, T2: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];

  T := vars.value[0];
  T2 := T * T;

  PP_model_DIPPR102 := A * power(T, B) / (1 + C / T + D / T2);
end;

// DIPPR104 equation
function PP_model_DIPPR104(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, E: double;
  T, T3, T8, T9: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  E := coeff.data[4];

  T := vars.value[0];
  T3 := T * T * T;
  T8 := T3 * T3 * T * T;
  T9 := T8 * T;

  PP_model_DIPPR104 := A + B / T + C / T3 + D / T8 + E / T9;
end;

// DIPPR105 equation
function PP_model_DIPPR105(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];

  T := vars.value[0];

  PP_model_DIPPR105 := A / power(B, power(1 + (1 - T / C), D));
end;

// DIPPR106 equation
function PP_model_DIPPR106(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, Tcrit: double;
  T, Tr, Tr2: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  Tcrit := coeff.data[4];

  T := vars.value[0];
  Tr := T / Tcrit;
  Tr2 := Tr * Tr;

  PP_model_DIPPR106 := power(A * (1 - Tr), (B + C * Tr + D * Tr2));
end;

// DIPPR equation for the ideal heat capacity
function PP_model_DIPPR107(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, E: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  E := coeff.data[4];

  T := vars.value[0];

  PP_model_DIPPR107 := A + B * power((C / T / sinh(C / T)), 2) + D * power(E / T / cosh(E / T), 2);
end;

// NRTL activity coefficient model for electrolytes(Aspen[7], page 358)
//function PP_model_Electrolyte_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  A_B, B_B, C_B, r_i, A_BB, A_BsB, B_BBs, B_BsB, alpha_BB, F_BBs, F_BsB, G_BBs, G_BsB, C_ca_B, C_B_ca, D_ca_B, D_B_ca, E_ca_B, E_B-ca, alpha_ca_B, C_cas_cass, C_cass-cas, c_csa,cssa, C_cssa_csa, D_cas_cass,D_cass_cas, D_csa_cssa, D_cssa_csa, E_cas_cass, E_cass_cas,E_csa_cssa, E_cssa_csa, alpha_cas_cass, alpha_csa_cssa : double;
//begin
//  ;
//end;

// Margules equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-24)
//function PP_model_extended_Margules(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j (i,j=1...2) : double;
//begin
//  ln(?i) := (1-xi)*+2*[Ai + 2+xi*(Bi-Ai)]; Ai := S(j)[xj*(aij+bij*T)/(1-xi)]; Bi := S(j)[xj*(aji+bji*T)/(1-xi)] ;
//end;

// NRTL activity coefficient model (Aspen[7], page 3-62)
//function PP_model_extended_NRTL_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j, e_i_j, f_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-74)
//function PP_model_extended_UNIQUAC_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended UNIQUAC equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-26)
//function PP_model_extended_UNIQUAC_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Aspen[7], page 3-75)
//function PP_model_extended_van_Laar_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended van Laar equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-28)
//function PP_model_extended_van_Laar_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended Wilson equation for calculating liquid activity (Aspen[7], page 3-78)
//function PP_model_extended_Wilson_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// extended Wilson equation for calculating liquid activity coefficients with temperature-independent parameters (Hysys[9], page A-29)
//function PP_model_extended_Wilson_Hysys(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// general NRTL activity coefficient model (Hysys[9], page A-22)
//function PP_model_general_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  form-of_equation, A_j_j, B_i_j, C_i_j, F_i_j, G_i_j, alpha1_i_j, alpha2_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// Hakim-Steinberg-Stiel equation for the surface tension (Aspen[7], page 3-155)
//function PP_model_Hakim_Steinberg_Stiel(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  chi : double;
//begin
//  ;
//end;

// Hayden-O''Connel equation of state (Aspen[7], page 3-9)
//function PP_model_Hayden_O_Connel(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  B_i_j : double;
//begin
//  Zm := 1 + B*p/R*T; B := S(i)S(j)Bij(T) ;
//end;

// Aspen[7]-equation for the solid heat capacity (page 3-102)
function PP_model_heat_capacity_ASPEN(vars: SimVars; coeff: PPCoefficients): double;
var
  c1, c2, c3, c4, c5, c6: double;
  T, T2: double;
begin
  c1 := coeff.data[0];
  c2 := coeff.data[1];
  c3 := coeff.data[2];
  c4 := coeff.data[3];
  c5 := coeff.data[4];
  c6 := coeff.data[5];

  T := vars.value[0];
  T2 := T * T;

  PP_model_heat_capacity_ASPEN := c1 + c2 * T + c3 * T2 + c4 / T + c5 / T2 + c6 / sqrt(T);
end;

// Jones-Dole equation
function PP_model_Jones_Dole(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, n0: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  n0 := coeff.data[3];

  PP_model_Jones_Dole := (1 + a * sqrt(c) + b * c) * n0;
end;

// Lee-Kesler equation of state (Aspen[7], page 3-18)
//function PP_model_Lee_Kesler(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  Tcrit, Pcrit, omega : double;
//begin
//  Z0 := fct0(T/Tcrit, p/prit);
//  Zr  := fctr(T/Tcrit, p/Pcrit);
//  Z := Z0 + (Zr-Z0)*ω/ωr;
//end;

// Lee-Kesler-Plocker equation of state (Aspen[7], page 3-19)
//function PP_model_Lee_Kesler_Plocker(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  Tcrit, Pcrit, vcriticalVolume, omega, Z_c_i, K_i_j : double;
//begin
//  //mixing rules for vcrit,m, Tcrit ;
//  Zm0 := fct0(T,Tcrit, Vm, vcrit,m);
//  Zmr := fctf(T,Tcrit,Vm,vcrit,m)
//  Zm := Zm0 + (?/?r)*(Zm0 - Zmr) ;
//end;

// DIPPR equation for the liquid viscosity
function PP_model_liquid_viscosity_DIPPR(vars: SimVars; coeff: PPCoefficients): double;
var
  c1, c2, c3, c4, c5: double;
  T: double;
begin
  c1 := coeff.data[0];
  c2 := coeff.data[1];
  c3 := coeff.data[2];
  c4 := coeff.data[3];
  c5 := coeff.data[4];

  T := vars.value[0];

  PP_model_liquid_viscosity_DIPPR := exp(c1 + c2 / T + c3 * ln(T) + c4 * power(T, c5));
end;

// Margules equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_Margules(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  A_i_j : double;
//begin
//  ln(?i) := [Aij + 2*(Aji-Aij)*xi](1-xi)2 ;
//end;

// modified Antoine vapor pressure equation (Aspen[7], page 3-80)
function PP_model_mod_Antoine_Aspen(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, E, F: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  E := coeff.data[4];
  F := coeff.data[5];

  T := vars.value[0];

  PP_model_mod_Antoine_Aspen := exp(A + B / (T + C) + D * ln(T) + E * power(T, F));
end;

// modified Antoine vapor pressure equation (Hysys[9], page A-36)
function PP_model_mod_Antoine_Hysys(vars: SimVars; coeff: PPCoefficients): double;
var
  A, B, C, D, E, F, G: double;
  T: double;
begin
  A := coeff.data[0];
  B := coeff.data[1];
  C := coeff.data[2];
  D := coeff.data[3];
  E := coeff.data[4];
  F := coeff.data[5];
  G := coeff.data[6];

  T := vars.value[0];

  PP_model_mod_Antoine_Hysys := exp(A + B / (T + C) + D * T + E * ln(T) + F * power(T, G));
end;

// modified polynomial
//function PP_model_modified_polynomial(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a, b, c, d, e, f, ... : double;
//begin
//  property := a + b/T + c/T2 + d*T + e*T2 + f*T3 + ... ;
//end;

// NRTL activity coefficient model (DDB[8], page XVI)
//function PP_model_NRTL(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  A_i_j, A_j_i, alpha (i,j=1...2) : double;
//begin
//  ;
//end;

{$note check this model for the inputs and outputs}
// standard Peng-Robinson equation of state (Aspen[7], page 3-34)
function PP_model_Peng_Robinson(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b: double;
  alfa, omega, k: double;
  Vm, pc: double;
  T, Tc, Tc2, Tr: double;
begin
  Vm := coeff.data[0];
  omega := coeff.data[1];
  pc := coeff.data[2];

  Tc := coeff.data[3];
  Tc2 := Tc * Tc;

  T := vars.value[0];

  a := 0.457235 * R2 * Tc2 / pc;
  b := 0.077796 * R * Tc / pc;

  Tr := T / Tc;
  k := 0.37464 + 1.54226 * omega - 0.26992 * sqr(omega);
  alfa := (1 - k * sqr(1 - sqrt(T)));

  PP_model_Peng_Robinson := R * T / (Vm - b) - (a * alfa) / (Vm * (Vm + b) + b * (Vm - b));
end;

// Peng-Robinson-Boston-Mathias equation of state (Aspen[7], page 325)
function PP_model_Peng_Robinson_Boston_Mathias(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, Vm: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  Vm := coeff.data[2];

  T := vars.value[0];

  PP_model_Peng_Robinson_Boston_Mathias := R * T / (Vm - b) - (a / sqrt(T)) / (Vm * (Vm + b));
end;

// Pitzer model for activity
//function PP_model_Pitzer_activity(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  beta_0, beta_1, beta_2, : double;
//begin
//  ;
//end;

// polynomial
function PP_model_polynomial(vars: SimVars; coeff: PPCoefficients): double;
var
  temp: double = 0;
  i: integer;
begin
  for i := 0 to coeff.totcoeff - 1 do
  begin
    temp += coeff.data[i] * power(vars.value[i], i);
  end;

  PP_model_polynomial := temp;
end;

// Redlich-Kister equation for excess properties in binary systems
//function PP_model_Redlich_Kister(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_0, a_1, a_2, a_3, ... : double;
//begin
//  ? property := x1 * x2 * S(i)(ai*(x1x2)i) ;
//end;

// Redlich-Kister model for calculating activity coefficients (Aspen[7], page 3-66)
//function PP_model_Redlich_Kister_Aspen(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j, e_i_j, f_i_j, g_i_j, h_i_j, m_i_j, n_i_j, v_i : double;
//begin
//  ;
//end;

// Redlich-Kwong equation of state (Aspen[7], page 3-27)
function PP_model_Redlich_Kwong(vars: SimVars; coeff: PPCoefficients): double;
var
  Pc, Tc, Vm: double;
  a, b: double;
  T: double;
begin
  Pc := coeff.data[0];
  Tc := coeff.data[1];
  Vm := coeff.data[2];

  T := vars.value[0];

  a := 0.42748 * R2 * power(T, 2.5) / Pc;
  b := 0.08664 * R * Tc / Pc;
  PP_model_Redlich_Kwong := R * T / (Vm - b) - a / (sqrt(T) * Vm * (Vm + b));
end;

// Aspen modification of the Redlich-Kwong equation of state( Aspen[7], page 3-28)
function PP_model_Redlich_Kwong_Aspen(vars: SimVars; coeff: PPCoefficients): double;
var
  Pc, Tc, Vm: double;
  a, b: double;
  T: double;
begin
  Pc := coeff.data[0];
  Tc := coeff.data[1];
  Vm := coeff.data[2];

  T := vars.value[0];

  a := 0.42748 * R2 * power(T, 2.5) / Pc;
  b := 0.08664 * R * Tc / Pc;
  PP_model_Redlich_Kwong_Aspen := R * T / (Vm - b) - a / (Vm * (Vm + b));
end;

// standard Redlich-Kwong-Soave equation of state (Aspen[7], page 335)
function PP_model_Redlich_Kwong_Soave(vars: SimVars; coeff: PPCoefficients): double;
var
  Pc, Tc, Vm: double;
  a, b: double;
  T: double;
begin
  Pc := coeff.data[0];
  Tc := coeff.data[1];
  Vm := coeff.data[2];

  T := vars.value[0];

  a := 0.42748 * R2 * power(T, 2.5) / Pc;
  b := 0.08664 * R * Tc / Pc;
  PP_model_Redlich_Kwong_Soave := R * T / (Vm - b) - a / (Vm * (Vm + b));
end;

// Redlich-Kwong equation of state with Boston-Mathias alpha function (Aspen[7], page 3-29)
function PP_model_Redlich_Kwong_Soave_Boston_Mathias(vars: SimVars; coeff: PPCoefficients): double;
var
  Pc, Tc, Vm: double;
  a, b: double;
  T: double;
begin
  Pc := coeff.data[0];
  Tc := coeff.data[1];
  Vm := coeff.data[2];

  T := vars.value[0];

  a := 0.42748 * R2 * power(T, 2.5) / Pc;
  b := 0.08664 * R * Tc / Pc;

  PP_model_Redlich_Kwong_Soave_Boston_Mathias := R * T / (Vm - b) - a / (Vm * (Vm + b));
end;

// Riedel vapor pressure equation
function PP_model_Riedel(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, e: double;
  T, T2: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  e := coeff.data[4];

  T := vars.value[0];
  T2 := T * T;

  PP_model_Riedel := exp(a - b / T + c * T + d * T2 + e * ln(T));
end;

// Riedel equation for thermal conductivities
function PP_model_Riedel_therm_cond(vars: SimVars; coeff: PPCoefficients): double;
var
  a, Tcrit: double;
  T: double;
begin
  a := coeff.data[0];
  Tcrit := coeff.data[1];

  T := vars.value[0];

  PP_model_Riedel_therm_cond := a * (1 + (20 / 3) * power(1 - T / Tcrit, 2 / 3));
end;

// Scatchard-Hildebrand model (Aspen[7], page 3-67)
//function PP_model_Scatchard_Hildebrand(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  Tcrit-i, delta_i, V_i_CVT, V_i_l : double;
//begin
//  ;
//end;

// Schwartzentruber-Renon equation of state (Aspen[7], page 3-31)
//function PP_model_Schwartzentruber_Renon(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  criticalTemperature_i, criticalPressure_i, omega_i, q_0_i, q_1_i, q_2_i, c_0_i, c_1_i, c_2_i, k_0_a_i-j, k_1_a_i_j, k_2_a_i_j, l_0_i_j, l_1_i_j, l_2_i_j, k_0_D_i_j, k_1_D_i_j, k_2_D_i_j : double;
//begin
//  p := R*T/(Vm+c-b)-a/((Vm+c)*(Vm+c+b)) with mixing rules ;
//end;

// Surface Tension after Sprow and Prausnitz
function PP_model_Sprow_Prausnitz(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, Tcrit: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  Tcrit := coeff.data[2];

  T := vars.value[0];

  PP_model_Sprow_Prausnitz := a * power(1 - T / Tcrit, b);
end;

// DIPPR correlation for surface tension
function PP_model_suface_tension_DIPPR(vars: SimVars; coeff: PPCoefficients): double;
var
  c1, c2, c3, c4, c5, Tcrit: double;
  T, Tr, Tr2, Tr3: double;
begin
  c1 := coeff.data[0];
  c2 := coeff.data[1];
  c3 := coeff.data[2];
  c4 := coeff.data[3];
  c5 := coeff.data[4];
  Tcrit := coeff.data[5];

  T := vars.value[0];
  Tr := T / Tcrit;
  Tr2 := Tr * Tr;
  Tr3 := Tr2 * Tr;

  PP_model_suface_tension_DIPPR := c1 * power(1 - Tr, c2 + c3 * Tr + c4 * Tr2 + c5 * Tr3);
end;

// NEL equation for thermal conductivity
function PP_model_thermal_conductivity_NEL(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, Tcrit: double;
  x, T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  Tcrit := coeff.data[5];

  T := vars.value[0];

  x := 1 - T / Tcrit;
  PP_model_thermal_conductivity_NEL := a * (1 + b * power(x, 1 / 3) + c * power(x, 2 / 3) + d * x);
end;

// extended Margules equation for calculating liquid activity coefficients (Aspen[7], page 3-68)
//function PP_model_three_suffix_Margules(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// UNIQUAC equation for calculating liquid activity coefficients (DDB[8], page XVII)
//function PP_model_UNIQUAC(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  u_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// van Laar equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_van_Laar(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  A_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// vapor pressure equation
function PP_model_vapor_pressure_1(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d: double;
  T, T2: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];

  T := vars.value[0];
  T2 := T * T;

  PP_model_vapor_pressure_1 := exp(a + b * T + c / T + d / T2);
end;

// virial equation
//function PP_model_virial_equation(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  vc_2, vc_3, vc_4 : double;
//begin
//  Z := 1 + vc2*p + vc3*p2 + vc4*p4 + ... ;
//end;

// ASPEN[7] mixing rule for the liquid viscosity (listed under the heading Andrade/DIPPR, page 3122)
//function PP_model_viscosity_mixing_rule(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  a_i_j, b_i_j, c_i_j, d_i_j : double;
//begin
//  ln(?) := S(i)[x_i*ln(?i)] + S(i,j)[(aij + bij/T)*xi*xj + (cij+dij/T)*xi 2*xj 2] ;
//end;

// Wagner vapor pressure equation
function PP_model_Wagner(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, Pcrit, Tcrit: double;
  T, x, x3, x6: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  Tcrit := coeff.data[4];
  Pcrit := coeff.data[5];

  T := vars.value[0];
  x := 1 - T / Tcrit;
  x3 := x * x * x;
  x6 := x3 * x3;

  PP_model_Wagner := exp((a * x + b * power(x, 3 / 2) + c * x3 + d * x6) / (T / Tcrit) * Pcrit);
end;

// 2nd Wagner vapor pressure equation
function PP_model_Wagner2(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, e, Pcrit, Tcrit: double;
  T, x, x3, x7, x9: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  e := coeff.data[4];
  Tcrit := coeff.data[5];
  Pcrit := coeff.data[6];

  T := vars.value[0];
  x := 1 - T / Tcrit;
  x3 := x * x * x;
  x7 := x3 * x3 * x;
  x9 := x3 * x3 * x3;

  PP_model_Wagner2 := exp((a * x + b * power(x, 3 / 2) + c * x3 + d * x7 + e * x9) / (T / Tcrit) * Pcrit);
end;

// Wagner vapor pressure equation (Aspen)
function PP_model_Wagner3(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, d, Pcrit, Tcrit: double;
  T, x, x3, x4: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  d := coeff.data[3];
  Tcrit := coeff.data[4];
  Pcrit := coeff.data[5];

  T := vars.value[0];
  x := 1 - T / Tcrit;
  x3 := x * x * x;
  x4 := x3 * x;

  PP_model_Wagner3 := exp((a * x + b * power(x, 3 / 2) + c * x3 + d * x4) / (T / Tcrit) * Pcrit);
end;

// Wilson equation for calculating liquid activity coefficients (DDB[8], page XVI)
//function PP_model_Wilson(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  A_i_j (i,j=1...2) : double;
//begin
//  ;
//end;

// Wrede vapor pressure equation
function PP_model_Wrede(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];

  T := vars.value[0];

  PP_model_Wrede := power(10, a - b / T);
end;

// Wrede vapor pressure equation
function PP_model_Wrede_ln(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];

  T := vars.value[0];

  PP_model_Wrede_ln := exp(a - b / T);
end;

// Yen-Woods equation for densities
//function PP_model_Yen_Woods(vars  : SimVars; coeff : PPCoefficients) : double;
//var
//  criticalDensity, Tcrit, a, k_0, k_1, k_2, ... : double;
//begin
//  d := dcrit * (a + ?(j)(kj)*(1T/Tcrit)(j/3)) ;
//end;

// Yuan - Mok equation for the heat capacity
function PP_model_Yuan_Mok(vars: SimVars; coeff: PPCoefficients): double;
var
  a, b, c, n: double;
  T: double;
begin
  a := coeff.data[0];
  b := coeff.data[1];
  c := coeff.data[2];
  n := coeff.data[3];

  T := vars.value[0];

  PP_model_Yuan_Mok := a + b * exp(-c / power(T, n));
end;

end.
