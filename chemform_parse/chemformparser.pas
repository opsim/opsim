//inspired on: https://mail.python.org/pipermail/tutor/1999-March/000083.html

unit ChemFormParser;

interface

uses
  Util;

type
  Element = record
    symbol  : string[3];
    name    : string[24];
    anumb   : byte;
    aweight : double;
  end;
  pElement = ^Element;

  Atom = record
    next, prev : pointer;
    mult       : integer;
    elem       : pElement;
  end;
  pAtom = ^Atom;

  ChemForm = record
    name    : string;
    formula : string;
    atoms   : ListBase;
  end;
  pChemForm = ^ChemForm;

(**
 * Parse the chemical formula provided.
 *
 * \param formula: the chemical formula
 * \return the calculated the parsed formula data structure
 *)
function CHE_parse(formula : string): pChemForm;

(**
 * Calculate the molweight from the parsed formula data structure
 *
 * \param cf: the parsed formula data structure
 * \return the calculated molecular weight
 *)
function CHE_calculate_mol_weight(cf : pChemForm): double;

(**
 * Print the atoms from the formula data structure to stdout
 *
 * \param cf: the parsed formula data structure
 *)
procedure CHE_print_atoms(cf : pChemForm);

(**
 * Free the formula data structure from memory
 *
 * \param cf: the parsed formula data structure
 *)
procedure CHE_free(cf : pChemForm);

implementation

const
  ATOMCOUNT = 107;

  PeriodicTable: array[1 .. ATOMCOUNT] of Element =
   ((symbol: 'Ac';  name: 'Actinium';     anumb: 89;  aweight: 227),
    (symbol: 'Ag';  name: 'Silver';       anumb: 47;  aweight: 107.868),
    (symbol: 'Al';  name: 'Aluminum';     anumb: 13;  aweight: 26.98154),
    (symbol: 'Am';  name: 'Americium';    anumb: 95;  aweight: 243),
    (symbol: 'Ar';  name: 'Argon';        anumb: 18;  aweight: 39.948),
    (symbol: 'As';  name: 'Arsenic';      anumb: 33;  aweight: 74.9216),
    (symbol: 'At';  name: 'Astatine';     anumb: 85;  aweight: 210),
    (symbol: 'Au';  name: 'Gold';         anumb: 79;  aweight: 196.9665),
    (symbol: 'B';   name: 'Boron';        anumb: 5;   aweight: 10.81),
    (symbol: 'Ba';  name: 'Barium';       anumb: 56;  aweight: 137.33),
    (symbol: 'Be';  name: 'Beryllium';    anumb: 4;   aweight: 9.01218),
    (symbol: 'Bi';  name: 'Bismuth';      anumb: 83;  aweight: 208.9804),
    (symbol: 'Bk';  name: 'Berkelium';    anumb: 97;  aweight: 247),
    (symbol: 'Br';  name: 'Bromine';      anumb: 35;  aweight: 79.904),
    (symbol: 'C';   name: 'Carbon';       anumb: 6;   aweight: 12.011),
    (symbol: 'Ca';  name: 'Calcium';      anumb: 20;  aweight: 40.08),
    (symbol: 'Cd';  name: 'Cadmium';      anumb: 48;  aweight: 112.41),
    (symbol: 'Ce';  name: 'Cerium';       anumb: 58;  aweight: 140.12),
    (symbol: 'Cf';  name: 'Californium';  anumb: 98;  aweight: 251),
    (symbol: 'Cl';  name: 'Chlorine';     anumb: 17;  aweight: 35.453),
    (symbol: 'Cm';  name: 'Curium';       anumb: 96;  aweight: 247),
    (symbol: 'Co';  name: 'Cobalt';       anumb: 27;  aweight: 58.9332),
    (symbol: 'Cr';  name: 'Chromium';     anumb: 24;  aweight: 51.996),
    (symbol: 'Cs';  name: 'Cesium';       anumb: 55;  aweight: 132.9054),
    (symbol: 'Cu';  name: 'Copper';       anumb: 29;  aweight: 63.546),
    (symbol: 'Dy';  name: 'Dysprosium';   anumb: 66;  aweight: 162.50),
    (symbol: 'Er';  name: 'Erbium';       anumb: 68;  aweight: 167.26),
    (symbol: 'Es';  name: 'Einsteinium';  anumb: 99;  aweight: 254),
    (symbol: 'Eu';  name: 'Europium';     anumb: 63;  aweight: 151.96),
    (symbol: 'F';   name: 'Fluorine';     anumb: 9;   aweight: 18.998403),
    (symbol: 'Fe';  name: 'Iron';         anumb: 26;  aweight: 55.847),
    (symbol: 'Fm';  name: 'Fermium';      anumb: 100; aweight: 257),
    (symbol: 'Fr';  name: 'Francium';     anumb: 87;  aweight: 223),
    (symbol: 'Ga';  name: 'Gallium';      anumb: 31;  aweight: 69.735),
    (symbol: 'Gd';  name: 'Gadolinium';   anumb: 64;  aweight: 157.25),
    (symbol: 'Ge';  name: 'Germanium';    anumb: 32;  aweight: 72.59),
    (symbol: 'H';   name: 'Hydrogen';     anumb: 1;   aweight: 1.0079),
    (symbol: 'He';  name: 'Helium';       anumb: 2;   aweight: 4.0026),
    (symbol: 'Hf';  name: 'Hafnium';      anumb: 72;  aweight: 178.49),
    (symbol: 'Hg';  name: 'Mercury';      anumb: 80;  aweight: 200.59),
    (symbol: 'Ho';  name: 'Holmium';      anumb: 67;  aweight: 164.9304),
    (symbol: 'I';   name: 'Iodine';       anumb: 53;  aweight: 126.9045),
    (symbol: 'In';  name: 'Indium';       anumb: 49;  aweight: 114.82),
    (symbol: 'Ir';  name: 'Iridium';      anumb: 77;  aweight: 192.22),
    (symbol: 'K';   name: 'Potassium';    anumb: 19;  aweight: 39.0983),
    (symbol: 'Kr';  name: 'Krypton';      anumb: 36;  aweight: 83.80),
    (symbol: 'La';  name: 'Lanthanum';    anumb: 57;  aweight: 138.9055),
    (symbol: 'Li';  name: 'Lithium';      anumb: 3;   aweight: 6.94),
    (symbol: 'Lr';  name: 'Lawrencium';   anumb: 103; aweight: 260),
    (symbol: 'Lu';  name: 'Lutetium';     anumb: 71;  aweight: 174.96),
    (symbol: 'Md';  name: 'Mendelevium';  anumb: 101; aweight: 258),
    (symbol: 'Mg';  name: 'Magnesium';    anumb: 12;  aweight: 24.305),
    (symbol: 'Mn';  name: 'Manganese';    anumb: 25;  aweight: 54.9380),
    (symbol: 'Mo';  name: 'Molybdenum';   anumb: 42;  aweight: 95.94),
    (symbol: 'N';   name: 'Nitrogen';     anumb: 7;   aweight: 14.0067),
    (symbol: 'Na';  name: 'Sodium';       anumb: 11;  aweight: 22.98977),
    (symbol: 'Nb';  name: 'Niobium';      anumb: 41;  aweight: 92.9064),
    (symbol: 'Nd';  name: 'Neodymium';    anumb: 60;  aweight: 144.24),
    (symbol: 'Ne';  name: 'Neon';         anumb: 10;  aweight: 20.17),
    (symbol: 'Ni';  name: 'Nickel';       anumb: 28;  aweight: 58.71),
    (symbol: 'No';  name: 'Nobelium';     anumb: 102; aweight: 259),
    (symbol: 'Np';  name: 'Neptunium';    anumb: 93;  aweight: 237.0482),
    (symbol: 'O';   name: 'Oxygen';       anumb: 8;   aweight: 15.9994),
    (symbol: 'Os';  name: 'Osmium';       anumb: 76;  aweight: 190.2),
    (symbol: 'P';   name: 'Phosphorous';  anumb: 15;  aweight: 30.97376),
    (symbol: 'Pa';  name: 'Proactinium';  anumb: 91;  aweight: 231.0359),
    (symbol: 'Pb';  name: 'Lead';         anumb: 82;  aweight: 207.2),
    (symbol: 'Pd';  name: 'Palladium';    anumb: 46;  aweight: 106.4),
    (symbol: 'Pm';  name: 'Promethium';   anumb: 61;  aweight: 145),
    (symbol: 'Po';  name: 'Polonium';     anumb: 84;  aweight: 209),
    (symbol: 'Pr';  name: 'Praseodymium'; anumb: 59;  aweight: 140.9077),
    (symbol: 'Pt';  name: 'Platinum';     anumb: 78;  aweight: 195.09),
    (symbol: 'Pu';  name: 'Plutonium';    anumb: 94;  aweight: 244),
    (symbol: 'Ra';  name: 'Radium';       anumb: 88;  aweight: 226.0254),
    (symbol: 'Rb';  name: 'Rubidium';     anumb: 37;  aweight: 85.467),
    (symbol: 'Re';  name: 'Rhenium';      anumb: 75;  aweight: 186.207),
    (symbol: 'Rh';  name: 'Rhodium';      anumb: 45;  aweight: 102.9055),
    (symbol: 'Rn';  name: 'Radon';        anumb: 86;  aweight: 222),
    (symbol: 'Ru';  name: 'Ruthenium';    anumb: 44;  aweight: 101.07),
    (symbol: 'S';   name: 'Sulfur';       anumb: 16;  aweight: 32.06),
    (symbol: 'Sb';  name: 'Antimony';     anumb: 51;  aweight: 121.75),
    (symbol: 'Sc';  name: 'Scandium';     anumb: 21;  aweight: 44.9559),
    (symbol: 'Se';  name: 'Selenium';     anumb: 34;  aweight: 78.96),
    (symbol: 'Si';  name: 'Silicon';      anumb: 14;  aweight: 28.0855),
    (symbol: 'Sm';  name: 'Samarium';     anumb: 62;  aweight: 150.4),
    (symbol: 'Sn';  name: 'Tin';          anumb: 50;  aweight: 118.69),
    (symbol: 'Sr';  name: 'Strontium';    anumb: 38;  aweight: 87.62),
    (symbol: 'Ta';  name: 'Tantalum';     anumb: 73;  aweight: 180.947),
    (symbol: 'Tb';  name: 'Terbium';      anumb: 65;  aweight: 158.9254),
    (symbol: 'Tc';  name: 'Technetium';   anumb: 43;  aweight: 98.9062),
    (symbol: 'Te';  name: 'Tellurium';    anumb: 52;  aweight: 127.60),
    (symbol: 'Th';  name: 'Thorium';      anumb: 90;  aweight: 232.0381),
    (symbol: 'Ti';  name: 'Titanium';     anumb: 22;  aweight: 47.90),
    (symbol: 'Tl';  name: 'Thallium';     anumb: 81;  aweight: 204.37),
    (symbol: 'Tm';  name: 'Thulium';      anumb: 69;  aweight: 168.9342),
    (symbol: 'U';   name: 'Uranium';      anumb: 92;  aweight: 238.029),
    (symbol: 'Unh'; name: 'Unnilhexium';  anumb: 106; aweight: 263),
    (symbol: 'Unp'; name: 'Unnilpentium'; anumb: 105; aweight: 260),
    (symbol: 'Unq'; name: 'Unnilquadium'; anumb: 104; aweight: 260),
    (symbol: 'Uns'; name: 'Unnilseptium'; anumb: 107; aweight: 262),
    (symbol: 'V';   name: 'Vanadium';     anumb: 23;  aweight: 50.9415),
    (symbol: 'W';   name: 'Tungsten';     anumb: 74;  aweight: 183.85),
    (symbol: 'Xe';  name: 'Xenon';        anumb: 54;  aweight: 131.30),
    (symbol: 'Y';   name: 'Yttrium';      anumb: 39;  aweight: 88.9059),
    (symbol: 'Yb';  name: 'Ytterbium';    anumb: 70;  aweight: 173.04),
    (symbol: 'Zn';  name: 'Zinc';         anumb: 30;  aweight: 65.38),
    (symbol: 'Zr';  name: 'Zirconium';    anumb: 40;  aweight: 91.22));

function find_atom_from_list(cf     : pChemForm;
                             symbol : string) : pAtom;
var
  at : pAtom;
begin
  if cf = nil then exit;

  at := cf^.atoms.first;

  while at <> nil do
  begin
    if at^.elem^.symbol = symbol then
      exit(at);

    at := at^.next;
  end;

  exit(nil);
end;

//backwards process the chemical formula
function CHE_parse(formula : string): pChemForm;
var
  mul    : array[0 .. sizeof(byte) - 1] of integer; //multiplication stack
  idx    : integer = -1;                            //index of stack
  lmul   : integer = 1;                             //local multiplier
  temp   : integer;
  i      : integer;
  j      : integer;
  k      : integer;
  s      : integer;
  num    : integer;
  at     : pAtom;
  tvalue : string;
  cf     : pChemForm;
  code   : integer;
  found  : boolean;
begin
  cf := callocN(sizeof(ChemForm), 'chemform');
  cf^.formula := formula;

  i := length(formula);

  //reset the multiplication stack
  for k := 0 to sizeof(byte) do
    mul[k] := 1;

  while i > 0 do
  begin
    s := i;

    //parse  symbols
    if formula[s] in ['a'..'z'] then
      while not (formula[i] in ['A'..'Z']) do
        Dec(i)
    else
    //parse numerals
    if formula[s] in ['0'..'9'] then
    begin
      while formula[i] in ['0'..'9'] do
        Dec(i);

      Inc(i);
    end;

    tvalue := copy(formula, i, s - i + 1);

    if tvalue = ')' then
    begin
      //increase multiplication stack
      inc(idx);
      mul[idx] := lmul;
      lmul := 1;
    end
    else
    if tvalue = '(' then
    begin
      mul[idx] := 1;
      dec(idx)
    end
    else
    begin
      //test if value is numeric (multiplier)
      val(tvalue, num, code);
      if code = 0 then
      begin
        lmul := num;
      end
      else
      begin
        found := False;
        for j := 1 to ATOMCOUNT do
        begin
          if PeriodicTable[j].symbol = tvalue then
          begin
            at := find_atom_from_list(cf, tvalue);

            if at = nil then
            begin
               at := callocN(sizeof(Atom), 'atom');
               addtail(@cf^.atoms, at);

               at^.elem := @PeriodicTable[j];
            end;

            temp := lmul;
            for k := 0 to idx do
              temp := temp * mul[k];

            at^.mult += temp;

            //reset local multiplier
            lmul := 1;

            found := True;
            break;
          end;
        end;
        if not found then
        begin
          writeln('''', tvalue, ''' is not an element symbol');
          writeln(cf^.formula);
          writeln('^': i);
          writeln;

          //remove all added previous atoms, the result is not valid
          CHE_free(cf);
          exit(nil);
        end;
      end;
    end;

    Dec(i);
  end;

  exit(cf);
end;

function CHE_calculate_mol_weight(cf : pChemForm): double;
var
  at : pAtom;
  mw : double = 0;
begin
  if cf = nil then exit;

  at := cf^.atoms.first;

  while at <> nil do
  begin
    mw += at^.mult * at^.elem^.aweight;

    at := at^.next;
  end;

  exit(mw);
end;

procedure CHE_print_atoms(cf : pChemForm);
var
  at : pAtom;
begin
  if cf = nil then exit;

  at := cf^.atoms.first;

  while at <> nil do
  begin
    writeln(at^.elem^.symbol, ' (', at^.elem^.name, '): ', at^.mult);

    at := at^.next;
  end;
end;

procedure CHE_free(cf: pChemForm);
begin
  freelistN(@cf^.atoms);
  freeN(cf);
end;

end.
