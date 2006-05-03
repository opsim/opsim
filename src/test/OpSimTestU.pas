{ $Id$ }
{
 /***************************************************************************

                  Abstract: General classes and routines for testing.
                  Initial Revision : 03/05/2006
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
unit OpSimTestU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, ZConnection;

procedure CreateDBConnection(var AConnection: TZConnection);

var
  GlobalDatabaseConnection: TZConnection;

implementation

procedure CreateDBConnection(var AConnection: TZConnection);
begin
  AConnection := TZConnection.Create(nil);
  with AConnection do begin
    Connected := False;
    Protocol := 'firebird-2.0';
    {$ifdef unix}
    Database := '../../app/' + 'SYSDATA.ODB';
    {$else}
    Database := '..\..\app\' + 'SYSDATA.ODB';
    {$endif}
    User := 'sysdba';
    Password := 'masterkey';
    Connected := True;
  end;//with
end;

initialization

  CreateDBConnection(GlobalDatabaseConnection);

finalization

  FreeAndNil(GlobalDatabaseConnection);

end.

