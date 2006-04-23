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
  Classes, SysUtils, fpcunit, testutils, testregistry;

type

  TPropertyProviderTestCase = class (TTestCase)
  protected
    procedure SetUp; override;
    procedure TearDown; override;
  published
    procedure TestHookUp;
  end;
  
implementation

{
************************** TPropertyProviderTestCase ***************************
}
procedure TPropertyProviderTestCase.SetUp;
begin
  
end;

procedure TPropertyProviderTestCase.TearDown;
begin
  
end;

procedure TPropertyProviderTestCase.TestHookUp;
begin
  Fail('Write your own test');
end;

Initialization

  RegisterTest(TPropertyProviderTestCase);
end.

