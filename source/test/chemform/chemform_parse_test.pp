program ChemForm_parse_test;

{$mode objfpc}

uses
  Classes,
  LinkedList,
  ChemFormParser;

var
  fields : TStringList;
  x      : string;
  ok     : integer;
  cf     : pChemForm = nil;
  loop   : boolean = True;
begin
  fields := TStringList.Create;
  fields.Delimiter := ' ';

  while loop do
  begin
    write('? ');
    readln(x);
    fields.DelimitedText := x;

    if fields.Count <> 2 then
      case fields[0] of
        'help':
        begin
          writeln('use:');
          writeln('  molw <chemical formula>    calculate the molweight from the chemical forumla');
          writeln('  syms <chemical formula>    display the atom data from the chemical forumla');
        end;
        'exit', 'quit', 'q': loop := False;
        else
          writeln('input must have two fields');
      end
    else
    begin
      ok := 0;

      try
        if cf <> nil then
          CHF_free(cf);

        cf := CHF_parse(fields[1]);
        ok  := 1;
      except
        writeln('error parsing formula');
      end;

      if ok = 0 then
        continue;

      if fields[0] = 'molw' then
      begin
        if cf <> nil then
          writeln('molecular weight ', CHF_calculate_mol_weight(cf): 0: 5);
      end
      else
      if fields[0] = 'syms' then
      begin
        if cf <> nil then
          CHF_print_atoms(cf);
      end
      else
        writeln('unknown action: ', fields[0]);
    end;
  end;

  fields.Free;

  CHF_free(cf);
end.
