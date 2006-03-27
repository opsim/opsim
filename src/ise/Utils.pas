{ $Id$ }
{
 /***************************************************************************

                  Abstract: Contains general utilities.
                  Initial Revision : 19/03/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Additional contributors...

 ***************************************************************************/

 *****************************************************************************
 *                                                                           *
 *  This file is part of the OpSim - OPEN SOURCE PROCESS SIMULATOR           *
 *                                                                           *
 *  See the file COPYING.GPL, included in this distribution,                 *
 *  for details about the copyright.                                         *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 *****************************************************************************
}
unit Utils;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;

  function IsInvalidPoint(P: TPoint): Boolean;
  procedure SetInvalidPoint(var P: TPoint);
  function ComponentToString(Component: TComponent): string;
  function StringToComponent(Value: string): TComponent;

implementation

function IsInvalidPoint(P: TPoint): Boolean;
begin
  Result := (P.X = -1) and (P.Y = -1);
end;

procedure SetInvalidPoint(var P: TPoint);
begin
  P := Point(-1,-1);
end;

function ComponentToString(Component: TComponent): string;
var
  BinStream:TMemoryStream;
  StrStream: TStringStream;
  s: string;
begin
  BinStream := TMemoryStream.Create;
  try
    StrStream := TStringStream.Create(s);
    try
      BinStream.WriteComponent(Component);
      BinStream.Seek(0, soFromBeginning);
      ObjectBinaryToText(BinStream, StrStream);
      StrStream.Seek(0, soFromBeginning);
      Result:= StrStream.DataString;
    finally
      StrStream.Free;
    end;//try
  finally
    BinStream.Free
  end;//try
end;

function StringToComponent(Value: string): TComponent;
var
  StrStream:TStringStream;
  BinStream: TMemoryStream;
begin
  StrStream := TStringStream.Create(Value);
  try
    BinStream := TMemoryStream.Create;
    try
      ObjectTextToBinary(StrStream, BinStream);
      BinStream.Seek(0, soFromBeginning);
      Result := BinStream.ReadComponent(nil);
    finally
      BinStream.Free;
    end;//try
  finally
    StrStream.Free;
  end;//try
end;

end.

