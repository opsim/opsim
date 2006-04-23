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
unit TCompoundsTestCaseU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, fpcunit, testutils, testregistry;

type

  TCompoundsTestCase = class (TTestCase)
  protected
    procedure SetUp; override;
    procedure TearDown; override;
  published
    procedure TestHookUp;
  end;
  
implementation

{
****************************** TCompoundsTestCase ******************************
}
procedure TCompoundsTestCase.SetUp;
begin
  
end;

procedure TCompoundsTestCase.TearDown;
begin
  
end;

procedure TCompoundsTestCase.TestHookUp;
begin
  Fail('Write your own test');
end;


Initialization
  RegisterTest(TCompoundsTestCase);
end.

