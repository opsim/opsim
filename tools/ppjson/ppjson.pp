program ppjson;

{$mode objfpc}{$H-}

uses
  SysUtils, Classes,
  fpjson, jsonparser;

var
  fin, fout : TFileStream;
  jParse    : TJSONParser;
  jData     : TJSONData;
  s         : string;
begin
  if argc <> 3  then
  begin
    writeln(stdout, 'Pretty print and validator for JSON files');
    writeln(stdout, 'Usage: ', argv[0], ' infile.json outfile.json');
    halt(1);
  end;

  try
    fin := TFileStream.Create(argv[1], fmOpenRead);
  except
    writeln(stderr, 'cannot open file ', argv[1]);
    halt(1);
  end;

  try
    jParse := TJSONParser.Create(fin);
    jParse.Strict := True;
    jData := jParse.Parse;
  except
    on e: EJSONParser do
    begin
      writeln(stderr, 'JSON file not valid: ', e.Message);
      FreeAndNil(jData);
      FreeAndNil(fin);
      FreeAndNil(jParse);
      FreeAndNil(e);
      halt(1);
    end;
  end;

  try
    s := jData.FormatJSON();
    FreeAndNil(jData);
    FreeAndNil(fin);
    FreeAndNil(jParse);
  except
    writeln(stderr, 'cannot reformat JSON file: ', argv[1]);
    halt(1);
  end;

  try
    fout := TFileStream.Create(argv[2], fmCreate);
    fout.WriteBuffer(s[1], length(s));
    FreeAndNil(fout);
  except
    writeln(stderr, 'cannot write to file ', argv[2]);
    halt(1);
  end;
end.
