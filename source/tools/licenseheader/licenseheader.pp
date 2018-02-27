(*
 * ***** BEGIN LICENSE BLOCK *****
 *
 * This file is part of OpSim - THE OPEN SOURCE PROCESS SIMULATOR
 *
 * See the file COPYING.GPL, included in this distribution,
 * for details about the copyright.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * @abstract(insert license header to all sourcefiles recursively)
 * @created(2018-02-27)
 * @author(Darius Blaszyk <darius@opsim.cc>)
 *
 * ***** END LICENSE BLOCK *****
 *)

program licenseheader;

{$mode objfpc}{$H+}

uses
  Classes,
  Process,
  SysUtils,
  StrUtils;

  procedure search_files(var fl: TStrings; const exts, path: string);
  var
    info: TSearchRec;
  begin
    if FindFirst(IncludeTrailingPathDelimiter(path) + '*', faAnyFile, info) = 0 then
    begin
      try
        repeat
          if (info.Attr and faDirectory) = 0 then
          begin
            //add to file list
            if pos(ExtractFileExt(info.Name), exts) <> 0 then
              fl.Add(IncludeTrailingPathDelimiter(path) + info.Name);
          end
          else
          //start the recursive search
          if (info.Name <> '.') and (info.Name <> '..') then
            search_files(fl, exts, IncludeTrailingPathDelimiter(path) + info.Name);

        until FindNext(info) <> 0
      finally
        FindClose(info);
      end;
    end;
  end;

var
  f, flist, license: TStrings;
  i: integer;
begin
  if ParamStr(1) = '' then
  begin
    writeln(format('Usage: %s [base directory]', [ParamStr(0)]));
    halt(1);
  end;

  //create file list
  flist := TStringList.Create;
  search_files(flist, '.pas;.pp', ParamStr(1));

  f := TStringList.Create;

  license := TStringList.Create;
  license.LoadFromFile(ExtractFilePath(ParamStr(0)) + 'opsim_license_header.txt');

  for i := 0 to flist.Count - 1 do
  begin
    f.LoadFromFile(flist[i]);

    while Trim(f[0]) = '' do
      f.Delete(0);

    if pos('(*', Trim(f[0])) = 0 then
    begin
      f.Insert(0, license.Text);
      writeln('Writing file ... ', flist[i]);
      f.SaveToFile(flist[i]);
      break;
    end;
  end;

  flist.Free;
  license.Free;
  f.Free;

  writeln('done.');
end.
