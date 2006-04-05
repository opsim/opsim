{ $Id$ }
{
 /***************************************************************************

                  Abstract: Module for testing TPropertyProvider.
                  Initial Revision : 02/04/2006
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
unit TPropertyProviderTestU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  ComCtrls, StdCtrls, Buttons, Thermo, ThermoProvider;

type

  { TfmTPropertyProviderTest }

  TfmTPropertyProviderTest = class (TForm)
    btnAdd: TButton;
    btnClearCompsList: TButton;
    btnDelete: TButton;
    Button2: TButton;
    edCompID: TEdit;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    lstbxComps: TListBox;
    PageControl1: TPageControl;
    Panel1: TPanel;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    procedure btnAddClick(Sender: TObject);
    procedure btnClearCompsListClick(Sender: TObject);
    procedure btnDeleteClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    PropertyProvider: TPropertyProvider;
    procedure UpdateList;
  end;
  
var
  fmTPropertyProviderTest: TfmTPropertyProviderTest;

implementation

uses
  DMBaseU;

{ TfmTPropertyProviderTest }

{
*************************** TfmTPropertyProviderTest ***************************
}
procedure TfmTPropertyProviderTest.btnAddClick(Sender: TObject);
begin
  //Add the componenent.
  with PropertyProvider do begin
    AddCompound(edCompID.Text);
  end;//with
  UpdateList;
end;

procedure TfmTPropertyProviderTest.btnClearCompsListClick(Sender: TObject);
begin
  with PropertyProvider do begin
    Compounds.Clear;
  end;//with
  UpdateList;
end;

procedure TfmTPropertyProviderTest.btnDeleteClick(Sender: TObject);
begin
  with lstbxComps do
    if ItemIndex >= 0 then
      PropertyProvider.DeleteCompound(TCompound(Items.Objects[ItemIndex]).CompID);
  UpdateList;
end;

procedure TfmTPropertyProviderTest.Button2Click(Sender: TObject);
begin
  Close;
end;

procedure TfmTPropertyProviderTest.FormCreate(Sender: TObject);
begin
  PropertyProvider := TPropertyProvider.Create;
  with PropertyProvider do begin
    DBConnection := DMBase.DbConnectionMaster;
  end//with
end;

procedure TfmTPropertyProviderTest.FormDestroy(Sender: TObject);
begin
  PropertyProvider.Free;
end;

procedure TfmTPropertyProviderTest.UpdateList;
var
  I: Integer;
begin
  with PropertyProvider do begin
    lstbxComps.Items.Clear;
    for I := 0 to Compounds.Count - 1 do
      lstbxComps.Items.AddObject(Compounds[I].CompName, Compounds[I]);
  end;//with
end;

initialization

  {$I TPropertyProviderTestU.lrs}

end.

