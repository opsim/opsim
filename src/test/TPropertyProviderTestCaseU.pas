{ $Id$ }
{
 /***************************************************************************

                  Abstract: Unit tests for classes.
                  Initial Revision : 23/04/2006
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
unit TPropertyProviderTestCaseU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, fpcunit, testutils, testregistry, ZConnection;

type

  TPropertyProviderTestCase = class (TTestCase)
  protected
    DbConnectionMaster: TZConnection;
    procedure SetUp; override;
    procedure TearDown; override;
  published
    procedure TestDbConnection;
  end;
  
implementation

{
************************** TPropertyProviderTestCase ***************************
}
procedure TPropertyProviderTestCase.SetUp;
begin
  DbConnectionMaster := TZConnection.Create(nil);
  with DbConnectionMaster do begin
    Connected := False;
    Protocol := 'firebird-2.0';
    Database := '..\..\app\' + 'SYSDATA.ODB';
    User := 'sysdba';
    Password := 'masterkey';
    Connected := True;
  end;//with
end;

procedure TPropertyProviderTestCase.TearDown;
begin
  DbConnectionMaster.Free;
end;

procedure TPropertyProviderTestCase.TestDbConnection;
begin
  DbConnectionMaster.Connected := False;
  DbConnectionMaster.Connected := True;
  AssertEquals(DbConnectionMaster.Connected, True);
end;

Initialization

  RegisterTest(TPropertyProviderTestCase);
end.

