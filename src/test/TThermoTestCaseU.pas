{ $Id$ }
{
 /***************************************************************************

                  Abstract: Unit tests for thermo classes.
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
unit TThermoTestCaseU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, fpcunit, testutils, testregistry, ThermoProvider, OpSimTestU,
  Entities, Thermo;

type

  TCompoundsTestCase = class (TTestCase)
  private
    CompoundsList: TCompounds;
  protected
    PropertyProvider: TPropertyProvider;
    procedure SetUp; override;
    procedure TearDown; override;
  published
    procedure TestAdd;
    procedure TestAssign;
    procedure TestClear;
    procedure TestFindCompound;
  end;
  
implementation

const
  cCompID1 = 11;
  cCompID2 = 22;
  cCompID3 = 33;

{
****************************** TCompoundsTestCase ******************************
}
procedure TCompoundsTestCase.SetUp;
begin
  PropertyProvider := TPropertyProvider.Create;
  //Sets a provider with a few compounds loaded.
  with PropertyProvider do begin
    DBConnection := GlobalDatabaseConnection;
    AddCompound(cCompID1);
    AddCompound(cCompID2);
    AddCompound(cCompID3);
    //AssertEquals(Compounds.Count, 3);
  end;//with
  CompoundsList := TCompounds.Create;
end;

procedure TCompoundsTestCase.TearDown;
begin
  PropertyProvider.Free;
  CompoundsList.Free;
end;

procedure TCompoundsTestCase.TestAdd;
  
  const
    N = 5;
  var
    I: Integer;
  
begin
  with CompoundsList do begin
    Clear;
    for I := 1 to 5 do Add;
    AssertEquals(Count, N);
  end;//with
end;

procedure TCompoundsTestCase.TestAssign;
var
  I: Integer;
begin
  with CompoundsList do begin
    Clear;
    Assign(PropertyProvider.Compounds);
    //Check total count.
    AssertEquals(Count, PropertyProvider.Compounds.Count);
    //Check a few compound properties.
    for I := 0 to PropertyProvider.Compounds.Count - 1 do begin
      AssertEquals(Items[I].CompID, PropertyProvider.Compounds[I].CompID);
      AssertEquals(Items[I].CompName, PropertyProvider.Compounds[I].CompName);
    end;//for
  end;//with
end;

procedure TCompoundsTestCase.TestClear;
  
  const
    N = 5;
  var
    I: Integer;
  
begin
  TestAdd;
  with CompoundsList do begin
    Clear;
    AssertEquals(Count, 0);
  end;//with
end;

procedure TCompoundsTestCase.TestFindCompound;
begin
  with CompoundsList do begin
    Clear;
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    //Copy compounds from provider.
    TestAssign;
    AssertEquals(FindCompound(cCompID1) <> nil, True);
    AssertEquals(FindCompound(cCompID2) <> nil, True);
    AssertEquals(FindCompound(cCompID3) <> nil, True);
    //Delete one compound and checks deletion.
    DeleteCompound(cCompID2);
    AssertEquals(FindCompound(cCompID1) <> nil, True);
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    AssertEquals(FindCompound(cCompID3) <> nil, True);
    //Delete all and check.
    Clear;
    AssertEquals(FindCompound(cCompID1) <> nil, False);
    AssertEquals(FindCompound(cCompID2) <> nil, False);
    AssertEquals(FindCompound(cCompID3) <> nil, False);
  end;//with
end;

Initialization
  RegisterTest(TCompoundsTestCase);
end.

