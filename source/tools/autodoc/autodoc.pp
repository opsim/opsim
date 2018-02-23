{
  a very primitive brute force tool to help generate pasdoc placeholder comments
  in the interface section of pascal source code files
  this tool takes a pasdoc settings file as input and processes all source files
  that are referenced there

  if you run it, the tool will add pasdoc comments for:

    - functions
    - procedures
    - record variable

  the tool assumes the code is formatted in a relative nice way, otherwise it fails
  to insert the comments correctly.

  please use it with care and only on code that can be easily reverted or has been
  backed up.
}
program autodoc;

uses
  Classes,
  SysUtils,
  Math,
  INIFiles;

var
  infile: TStrings;
  outfile: TStrings;
  i: integer = 0;

  procedure parse_record;
  var
    pmax: integer = 0;
    itmp: integer;
    stmp: string;
    j: integer;
  begin
    if pos(' = record', infile[i]) <> 0 then
    begin
      Inc(i);
      itmp := i;

      //get the maximum length of the record variables
      while pos('end;', infile[i]) = 0 do
      begin
        if length(trim(infile[i])) > 0 then
          if pos('//', infile[i]) = 0 then
            pmax := max(pmax, length(infile[i]));
        Inc(i);
      end;

      i := itmp;
      while pos('end;', infile[i]) = 0 do
      begin
        if length(trim(infile[i])) > 0 then
          if pos('//', infile[i]) = 0 then
          begin
            stmp := '';

            for j := 0 to pmax - length(infile[i]) do
              stmp += ' ';

            infile[i] := infile[i] + stmp + '  //<';
          end;
        Inc(i);
      end;
    end;
  end;

  procedure parse_function(func: string);
  var
    args: TStrings;
    j1, j2: integer;
    stext: string;
    sline: string;
    funcname: string;
    funcres: string;
    stmp: string;
  begin
    args := TStringList.Create;

    if pos(func, trim(infile[i])) = 1 then
    begin
      //no closing comment found in the line before the function
      if pos('}', trim(infile[i - 1])) = 0 then
      begin
        stext := infile.Text;
        sline := infile[i];

        //find the position of the line in the text
        j1 := pos(trim(sline), stext) + length(func) + 1;
        j2 := j1;

        //now parse the function name
        while (stext[j2] <> '(') and (stext[j2] <> ':') and (stext[j2] <> ';') do
          Inc(j2);
        funcname := copy(stext, j1, j2 - j1);

        //now parse the function arguments
        j1 := j2 + 1;
        while (stext[j2] <> ')') and (stext[j2] <> ':') and (stext[j2] <> ';') do
        begin
          Inc(j2);

          if stext[j2] in [',', ':', ' ', ';'] then
          begin
            stmp := trim(copy(stext, j1, j2 - j1));
            if (stmp <> ';') and (stmp <> ',') and (stmp <> 'const') then
              args.Add(stmp);

            while stext[j2] in [#13, #10, #9, ' '] do
              Inc(j2);

            //skipe type identifier
            if stext[j2] = ':' then
            begin
              while (stext[j2] <> ';') and (stext[j2] <> ')') do
                Inc(j2);

              Inc(j2);
            end;

            while stext[j2] in [#13, #10, #9, ' '] do
              Inc(j2);

            j1 := j2;
          end;
        end;

        //now parse the function result;
        if func = 'function' then
        begin
          while stext[j2] <> ':' do
            Inc(j2);

          j1 := j2 + 1;
          while stext[j2] <> ';' do
            Inc(j2);

          funcres := Trim(copy(stext, j1, j2 - j1));
        end;

        infile.Insert(i, '{');
        Inc(i);
        infile.Insert(i, '   This is the ' + func + ' ' + funcname);
        Inc(i);

        for j1 := 0 to args.Count - 1 do
        begin
          infile.Insert(i, '   @param ' + args[j1] + ':');
          Inc(i);
        end;

        if func = 'function' then
        begin
          infile.Insert(i, '   @return the ' + funcres + ' type');
          Inc(i);
        end;
        infile.Insert(i, '}');
        Inc(i);
      end;
    end;

    args.Free;
  end;

var
  ini: TINIFile;
  Count: integer;
  srcfile: string;
  info: TSearchRec;
  k: integer;
  fname: string;
begin
  if not FileExists(ParamStr(1)) then
  begin
    writeln('error: input file does not exist');
    writeln('please use: autodoc <pasdoc project file>');
    halt(1);
  end;

  ini := TINIFile.Create(ParamStr(1));
  Count := ini.ReadInteger('Files', 'Count', 0);

  infile := TStringList.Create;
  outfile := TStringList.Create;

  for k := 0 to Count - 1 do
  begin
    srcfile := ExpandFileName(ExtractFilePath(ParamStr(1)) + ini.ReadString('Files', Format('Item_%d', [k]), ''));

    if FindFirst(srcfile, faAnyFile, info) = 0 then
    begin
      if (info.Attr and faDirectory) = 0 then
      begin
        fname := ExtractFilePath(srcfile) + info.Name;
        writeln('processing... ', fname);
        infile.LoadFromFile(fname);

        i := 0;

        //skip everything before interface
        while (i < infile.Count) and (trim(infile[i]) <> 'interface') do
          Inc(i);

        //parse interface
        while (i < infile.Count) and (trim(infile[i]) <> 'implementation') do
        begin
          parse_record;
          parse_function('function');
          parse_function('procedure');

          Inc(i);
        end;

        outfile.Text := infile.Text;
        outfile.SaveToFile(fname);
      end;
    end;
  end;

  infile.Free;
  outfile.Free;

  writeln('done.');
end.
