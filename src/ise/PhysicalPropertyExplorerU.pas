{ $Id$ }
{
 /***************************************************************************

                  Abstract: Iterface for inspecting the physical properties
                            data on database.
                  Initial Revision : 01/04/2006
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
unit PhysicalPropertyExplorerU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ZDataset,
  ExtCtrls, DBGrids, DBCtrls, DB, Buttons, ZConnection;

type

  { TPhysicalPropertyExplorer }

  TPhysicalPropertyExplorer = class (TForm)
    btnClose: TButton;
    DbConnectionMaster: TZConnection;
    dbGrid1: TdbGrid;
    DBNavigator1: TDBNavigator;
    dsPureComps: TDatasource;
    Panel1: TPanel;
    Panel2: TPanel;
    tblPureComps: TZTable;
    procedure btnCloseClick(Sender: TObject);
    procedure DbConnectionMasterBeforeConnect(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  end;
  
var
  PhysicalPropertyExplorer: TPhysicalPropertyExplorer;

implementation

uses
  DMBaseU;

{ TPhysicalPropertyExplorer }


{
************************** TPhysicalPropertyExplorer ***************************
}
procedure TPhysicalPropertyExplorer.DbConnectionMasterBeforeConnect(Sender: 
        TObject);
begin
  
end;

procedure TPhysicalPropertyExplorer.btnCloseClick(Sender: TObject);
begin
  Close;
end;

procedure TPhysicalPropertyExplorer.FormCreate(Sender: TObject);
begin
  with tblPureComps do begin
    Connection := DMBase.DbConnectionMaster;
    Open;
  end;//with
end;

initialization
  {$I PhysicalPropertyExplorerU.lrs}

end.

