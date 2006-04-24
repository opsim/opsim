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
  Classes, SysUtils, fpcunit, testutils, testregistry, ZConnection,
  ThermoProvider;

type

  TPropertyProviderTestCase = class (TTestCase)
  protected
    DbConnectionMaster: TZConnection;
    PropertyProvider: TPropertyProvider;
    procedure SetUp; override;
    procedure TearDown; override;
  published
    procedure TestAdd;
    procedure TestClear;
    procedure TestDbConnection;
    procedure TestDelete;
    procedure TestFindCompound;
  end;
  
implementation

const
  cCompID1 = 11;
  cCompID2 = 22;
  cCompID3 = 33;

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
  PropertyProvider := TPropertyProvider.Create;
  with PropertyProvider do begin
    DBConnection := DbConnectionMaster;
  end//with
end;

procedure TPropertyProviderTestCase.TearDown;
begin
  DbConnectionMaster.Free;
  PropertyProvider.Free;
end;

procedure TPropertyProviderTestCase.TestAdd;
begin
  with PropertyProvider do begin
    AddCompound(cCompID1);
    AddCompound(cCompID2);
    AddCompound(cCompID3);
    AssertEquals(Compounds.Count, 3);
  end;//with
end;

procedure TPropertyProviderTestCase.TestClear;
begin
  TestAdd;
  with PropertyProvider do begin
    Compounds.Clear;
    AssertEquals(Compounds.Count, 0);
  end;//with
end;

procedure TPropertyProviderTestCase.TestDbConnection;
begin
  DbConnectionMaster.Connected := False;
  DbConnectionMaster.Connected := True;
  AssertEquals(DbConnectionMaster.Connected, True);
end;

procedure TPropertyProviderTestCase.TestDelete;
begin
  TestAdd;
  with PropertyProvider do begin
    DeleteCompound(cCompID2);
    AssertEquals(Compounds.Count, 2);
    AssertEquals(FindCompound(cCompID2) = nil, True);
  end;//with
end;

procedure TPropertyProviderTestCase.TestFindCompound;
begin
  with PropertyProvider do begin
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    TestAdd;
    AssertEquals(FindCompound(cCompID1) <> nil, True);
    AssertEquals(FindCompound(cCompID2) <> nil, True);
    AssertEquals(FindCompound(cCompID3) <> nil, True);
    DeleteCompound(cCompID2);
    AssertEquals(FindCompound(cCompID1) <> nil, True);
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    AssertEquals(FindCompound(cCompID3) <> nil, True);
    Compounds.Clear;
    AssertEquals(FindCompound(cCompID1) <> nil, False);
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    AssertEquals(FindCompound(cCompID3) <> nil, False);
  end;//with
end;

Initialization
  RegisterTest(TPropertyProviderTestCase);
end.

