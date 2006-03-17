{
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
program OpSim;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms
  { add your units here }, MainU, AboutU, PfdDesignerU, UnitopPalletU, DMBaseU, MixerSpecU;

begin
  Application.Initialize;
  Application.CreateForm(TMain, Main);
  Application.CreateForm(TUnitopPallet, UnitopPallet);
  Application.CreateForm(TPfdDesigner, PfdDesigner);
  Application.CreateForm(TDMBase, DMBase);
  Application.CreateForm(TMixerSpec, MixerSpec);
  Application.Run;
end.

