{ $Id$ }
{
 /***************************************************************************
 
                  Abstract: The main Integrated Simulation
                            Environment (ISE) module.
                  Initial Revision : 09/03/2006
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
program OpSim;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms
  { add your units here }, MainU, AboutU, PFDDesignerU, UnitopPalletU, DMBaseU, MixerSpecU,
  Utils, PFDGraph, ExtGraphics2, ExtShapes, PFDCtrls, Solver, Entities, Mixer,
  ThermoProvider, zcomponent, PhysicalPropertyExplorerU, TPropertyProviderTestU,
  Thermo, Ports, Eos, Eos_Srk;

begin
  Application.Initialize;
  Application.CreateForm(TMain, Main);
  Application.CreateForm(TUnitopPallet, UnitopPallet);
  Application.CreateForm(TPFDDesigner, PFDDesigner);
  Application.CreateForm(TDMBase, DMBase);
  Application.CreateForm(TMixerSpec, MixerSpec);
  Application.Run;
end.

