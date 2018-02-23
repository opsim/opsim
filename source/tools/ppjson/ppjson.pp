program ppjson;

{$mode objfpc}{$H+}

uses
  SysUtils, StrUtils, Classes,
  fpjson, jsonparser;

var
  fin, fout    : TFileStream;
  jParse       : TJSONParser;
  jData        : TJSONData;
  s            : string;
  iname, oname : string;

begin
  if (argc < 2) or (argc > 3)  then
  begin
    writeln(stdout, 'Pretty print and validator for JSON files');
    writeln(stdout, 'Usage: ', argv[0], ' infile.json <outfile.json>');
    halt(1);
  end;

  iname := argv[1];
  if argc = 2 then
    oname := iname
  else
    oname := argv[2];

  try
    fin := TFileStream.Create(iname, fmOpenRead);
  except
    writeln(stderr, 'cannot open file ', iname);
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

    //undo some of the escaping
    //for now we just search replace, perhaps later itterate through all
    //data object strings???
    s := ReplaceStr(s, '\\', '\');
    s := ReplaceStr(s, '\/', '/');
    s := ReplaceStr(s, '\"', '"');

    FreeAndNil(jData);
    FreeAndNil(fin);
    FreeAndNil(jParse);
  except
    writeln(stderr, 'cannot reformat JSON file: ', argv[1]);
    halt(1);
  end;

  try
    fout := TFileStream.Create(oname, fmCreate);
    fout.WriteBuffer(s[1], length(s));
    FreeAndNil(fout);
  except
    writeln(stderr, 'cannot write to file ', oname);
    halt(1);
  end;
  writeln('done.');
end.
