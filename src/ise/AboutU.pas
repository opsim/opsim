{ $Id$ }
{
 /***************************************************************************
                               AboutU.pas
                               ----------
                  Abstract: The about box.
                  Initial Revision : 09/03/2006
                  Author: Samuel Jorge Marques Cartaxo


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
unit AboutU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  StdCtrls, ComCtrls, Buttons;

type

  { TAboutForm }

  TAboutForm = class (TForm)
    Bevel1: TBevel;
    btnOk: TButton;
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Memo1: TMemo;
    Memo2: TMemo;
    PageControl1: TPageControl;
    Panel1: TPanel;
    tbshAbout: TTabSheet;
    tbshContributors: TTabSheet;
  end;
  
var
  AboutForm: TAboutForm;

implementation

initialization

  {$I AboutU.lrs}

end.

