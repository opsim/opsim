unit ImportCSV;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Dialogs, Grids, StdCtrls, ExtCtrls,
  ButtonPanel, CsvDocument;

type

  { TCSVImportForm }

  TCSVImportForm = class(TForm)
    ButtonPanel1: TButtonPanel;
    cbbDelimiter: TComboBox;
    Delimiter: TLabel;
    sgView: TStringGrid;
    mmSource: TMemo;
    splTop: TSplitter;
    lblSource: TLabel;
    pnButtons: TPanel;
    btnLoad: TButton;
    OpenDialog: TOpenDialog;
    procedure cbbDelimiterChange(Sender: TObject);
    procedure CloseButtonClick(Sender: TObject);
    procedure mmSourceChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure OKButtonClick(Sender: TObject);
    procedure btnLoadClick(Sender: TObject);
  private
    procedure UpdateView;
  public
    { public declarations }
    FDoc: TCSVDocument;
  end;

var
  CSVImportForm: TCSVImportForm;

implementation

{ TCSVImportForm }

procedure TCSVImportForm.mmSourceChange(Sender: TObject);
begin
  FDoc.CSVText := mmSource.Text;
  UpdateView;
end;

procedure TCSVImportForm.cbbDelimiterChange(Sender: TObject);
begin
  FDoc.Delimiter := cbbDelimiter.Text[1];
  FDoc.CSVText := mmSource.Text;
  UpdateView;
end;

procedure TCSVImportForm.CloseButtonClick(Sender: TObject);
begin
  FDoc.Free;
  Close;
end;

procedure TCSVImportForm.FormCreate(Sender: TObject);
begin
  FDoc := TCSVDocument.Create;
  FDoc.Delimiter := ';';
end;

procedure TCSVImportForm.FormDestroy(Sender: TObject);
begin
  FreeAndNil(FDoc);
end;

procedure TCSVImportForm.OKButtonClick(Sender: TObject);
begin
  Close;
end;

procedure TCSVImportForm.btnLoadClick(Sender: TObject);
begin
  if OpenDialog.Execute then
    mmSource.Lines.LoadFromFile(OpenDialog.FileName);
end;

procedure TCSVImportForm.UpdateView;
var
  i, j: Integer;
begin
  sgView.BeginUpdate;
  try
    i := FDoc.RowCount;
    if sgView.RowCount <> i then
      sgView.RowCount := i;

    i := FDoc.MaxColCount;
    if sgView.ColCount <> i then
      sgView.ColCount := i;

    for i := 0 to FDoc.RowCount - 1 do
      for j := 0 to sgView.ColCount - 1 do
        sgView.Cells[j, i] := FDoc.Cells[j, i];
  finally
    sgView.EndUpdate;
  end;
end;

initialization
  {$I importcsv.lrs}

end.
