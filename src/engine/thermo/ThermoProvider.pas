{ $Id$ }
{
 /***************************************************************************

                  Abstract: Set of classes for gathering properties
                        from the database and serve the simulator.
                  Initial Revision : 01/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Hazem Haddad
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
unit ThermoProvider;

interface

uses
  SysUtils, Classes, Thermo, Variants, ZConnection, ZDataset;

type

  TPropertyProvider = class (TPersistent)
  private
    DataSet: TZReadOnlyQuery;
    FCompounds: TCompounds;
    FDBConnection: TZConnection;
    procedure Connect;
    procedure Disconnect;
  public
    constructor Create;
    destructor Destroy; override;
    procedure AddCompound(ID: Variant);
    function DeleteCompound(ID: Variant): TCompound;
    function FindCompound(ID: Variant): TCompound;
    property Compounds: TCompounds read FCompounds write FCompounds;
    property DBConnection: TZConnection read FDBConnection write FDBConnection;
  end;
  
implementation

{
****************************** TPropertyProvider *******************************
}
constructor TPropertyProvider.Create;
begin
  inherited Create;
  FCompounds := TCompounds.Create;
  DataSet := TZReadOnlyQuery.Create(nil);
end;

destructor TPropertyProvider.Destroy;
begin
  FCompounds.Free;
  DataSet.Free;
  inherited Destroy;
end;

procedure TPropertyProvider.AddCompound(ID: Variant);
begin
  //Exit if the compound is already in the list.
  if FindCompound(ID) <> nil then
    Exit;
  
  with DataSet do begin
  
    //Sets the current connection.
    Connection := DBConnection;
  
    //For now, the compound ID is then Number field.
    Sql.Text := Format('SELECT * FROM PROPS_PURE_SUBST_RAW WHERE NUMBER = %s', [VarToStr(ID)]);
    Open;
  
    //If the compund is found, then reads its properties.
    if RecordCount > 0 then begin
      with FCompounds.Add do begin
        CompID := FieldValues['NUMBER'];
        CompName := FieldValues['COMPONENT'];
        CPA.Value := FieldValues['CP_A'];
        CPB.Value := FieldValues['CP_B'];
        CPC.Value := FieldValues['CP_C'];
        CPD.Value := FieldValues['CP_D'];
        DELGF.Value := FieldValues['DELGF'];
        DELHG.Value := FieldValues['DELHG'];
        //DIPM.Value := FieldValues['DIPM'];
        //Formula.Value := FieldValues['Formula'];
        LDEN.Value := FieldValues['LIQDEN'];
        MW.Value := FieldValues['MOLE_WT'];
        //NEQ.Value := FieldValues['NEQ'];
        PC.Value := FieldValues['PC'];
        TB.Value := FieldValues['TB'];
        TC.Value := FieldValues['TC'];
        //TCPHIL.Value := FieldValues['TCPHIL'];
        TDEN.Value := FieldValues['TDEN'];
        TFP.Value := FieldValues['TFP'];
        //TMAX.Value := FieldValues['TMAX'];
        //TMIN.Value := FieldValues['TMIN'];
        VC.Value := FieldValues['VC'];
        VPA.Value := FieldValues['HARLACHER_VP_A'];
        VPB.Value := FieldValues['HARLACHER_VP_B'];
        VPC.Value := FieldValues['HARLACHER_VP_C'];
        VPD.Value := FieldValues['HARLACHER_VP_D'];
        //VSTAR.Value := FieldValues['VSTAR'];
        //W.Value := FieldValues['W'];
        //WSRK.Value := FieldValues['WSRK'];
        ZC.Value := FieldValues['ZC'];
        //Zra.Value := FieldValues['Zra'];
      end;//with
    end;//if
  
    Close;
  end;//with
end;

procedure TPropertyProvider.Connect;
begin
  with DBConnection do
    if not Connected then Connected := True;
end;

function TPropertyProvider.DeleteCompound(ID: Variant): TCompound;
var
  C: TCompound;
begin
  C := FindCompound(ID);
  if C <> nil then
    FCompounds.Delete(C.Index);
end;

procedure TPropertyProvider.Disconnect;
begin
  with DBConnection do
    Connected := False;
end;

function TPropertyProvider.FindCompound(ID: Variant): TCompound;
var
  I: Integer;
begin
  Result := nil;
  with FCompounds do
    for I := 0 to Count - 1 do
      if Compounds[I].CompID = ID then begin
        Result := Compounds[I];
        Break;
      end;//if
end;

end.
