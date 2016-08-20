unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, TAGraph, TASeries, DividerBevel, Forms, Controls,
  ComCtrls, ExtCtrls, Grids, StdCtrls, Menus, ImportCSV;

type

  { TMainForm }

  TMainForm = class(TForm)
    Chart1: TChart;
    DataSeries: TLineSeries;
    MainMenu1: TMainMenu;
    MenuItem1: TMenuItem;
    MenuItem10: TMenuItem;
    MenuItem11: TMenuItem;
    MenuItem12: TMenuItem;
    MenuItem2: TMenuItem;
    MenuItem3: TMenuItem;
    MenuItem4: TMenuItem;
    MenuItem5: TMenuItem;
    MenuItem6: TMenuItem;
    MenuItem7: TMenuItem;
    MenuItem8: TMenuItem;
    MenuItem9: TMenuItem;
    ModelSeries: TLineSeries;
    PlotDataCheckBox: TCheckBox;
    PlotModelCheckBox: TCheckBox;
    DividerBevel1: TDividerBevel;
    Image1: TImage;
    ImageList1: TImageList;
    Label1: TLabel;
    Label2: TLabel;
    Memo1: TMemo;
    PageControl1: TPageControl;
    Panel1: TPanel;
    Panel2: TPanel;
    Splitter1: TSplitter;
    StatusBar1: TStatusBar;
    StringGrid1: TStringGrid;
    StringGrid2: TStringGrid;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TabSheet3: TTabSheet;
    TreeView1: TTreeView;
    procedure FormCreate(Sender: TObject);
    procedure PlotDataCheckBoxChange(Sender: TObject);
    procedure PlotModelCheckBoxChange(Sender: TObject);
    procedure SpkLargeButton2Click(Sender: TObject);
    procedure SpkLargeButton7Click(Sender: TObject);
    procedure SpkLargeButton8Click(Sender: TObject);
    procedure SpkLargeButton9Click(Sender: TObject);
    procedure StringGrid1EditingDone(Sender: TObject);
  private
    { private declarations }
    procedure LoadPPModels;
  public
    { public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.lfm}

uses
  LCLIntf,
  About, Settings;

{ TMainForm }

procedure TMainForm.SpkLargeButton8Click(Sender: TObject);
begin
  if AboutForm = nil then
    AboutForm := TAboutForm.Create(nil);

  AboutForm.Show;
end;

procedure TMainForm.SpkLargeButton9Click(Sender: TObject);
begin
  OpenURL('http://www.opsim.cc/user-docs/html/physical_properties.html');
end;

procedure TMainForm.StringGrid1EditingDone(Sender: TObject);
var
  i: integer;
begin
  DataSeries.Clear;

  for i:= 1 to StringGrid1.RowCount - 1 do
  begin
    if (StringGrid1.Cells[1, i] <> '') and (StringGrid1.Cells[2, i] <> '') then
      DataSeries.AddXY(StrToFloat(StringGrid1.Cells[1, i]), StrToFloat(StringGrid1.Cells[2, i]));
  end;

  DataSeries.ShowPoints := PlotDataCheckBox.Checked;
end;

procedure TMainForm.LoadPPModels;
var
  //component : string;
  count     : integer = 0;
  //fs        : TFileStream;
  info      : TSearchRec;
  //jArray    : TJSONArray;
  //jData     : TJSONData;
  //jObject   : TJSONObject;
begin
  TreeView1.Items.Clear;
  TreeView1.SortType := stText;

  if FindFirst('../components/*.prop', faAnyFile and faDirectory, info) = 0 then
  begin
    repeat
      inc(count);
      with info do
      begin
        TreeView1.Items.AddFirst(nil, info.Name);

        //writeln('  ', pp_model^.modelpath + name);
        //fs := TFileStream.Create(pp_model^.modelpath + name, fmOpenRead);
        //
        //jData := GetJSON(fs);
        //jObject := TJSONObject(jData);
        //component := lowercase(jObject.Get('component'));
        //
        //jArray := jObject.Get('vapor pressure', TJSONArray(nil));
        //if jArray <> nil then
        //  PP_vapor_pressure_load_JSON(component, jArray);
        //
        //jArray := jObject.Get('heat capacity gas', TJSONArray(nil));
        //if jArray <> nil then
        //  PP_cp_gas_load_JSON(component, jArray);
        //
        //jData.Free;
        //
        //fs.Free;
      end;
    until FindNext(info) <> 0;
  end;
  FindClose(info);
  TreeView1.AlphaSort;
end;

procedure TMainForm.FormCreate(Sender: TObject);
var
  i: integer;
begin
  LoadPPModels;

  for i := 1 to StringGrid1.RowCount - 1 do
    StringGrid1.Cells[0, i] := IntToStr(i);

  StringGrid2.Cells[0, 1] := 'A';
  StringGrid2.Cells[0, 2] := 'B';
  StringGrid2.Cells[0, 3] := 'C';
end;

procedure TMainForm.PlotDataCheckBoxChange(Sender: TObject);
begin
  DataSeries.ShowPoints := PlotDataCheckBox.Checked;
  StringGrid1EditingDone(nil);
end;

procedure TMainForm.PlotModelCheckBoxChange(Sender: TObject);
begin
  ModelSeries.ShowPoints := PlotModelCheckBox.Checked;
end;

procedure TMainForm.SpkLargeButton2Click(Sender: TObject);
var
  i, j: Integer;
begin
  //load CSV data
  if CSVImportForm = nil then
    CSVImportForm := TCSVImportForm.Create(nil);

  CSVImportForm.ShowModal;

  if CSVImportForm.FDoc <> nil then
  begin
    StringGrid1.BeginUpdate;

    try
      //adjust row count
      i := CSVImportForm.FDoc.RowCount + 1;
      if StringGrid1.RowCount <> i then
        StringGrid1.RowCount := i;

      //adjust column count
      StringGrid1.Columns.Clear;
      for i := 0 to CSVImportForm.FDoc.MaxColCount do
        StringGrid1.Columns.Add;

      for i := 0 to CSVImportForm.FDoc.RowCount - 1 do
      begin
        //row numbering
        StringGrid1.Cells[0, i+1] := IntToStr(i + 1);

        //data from CSV
        for j := 0 to CSVImportForm.FDoc.ColCount[i] - 1 do
          StringGrid1.Cells[j+1, i+1] := CSVImportForm.FDoc.Cells[j, i];
      end;
    finally
      StringGrid1.EndUpdate;
    end;

    StatusBar1.SimpleText := Format('Rows: %d Cols:%d', [StringGrid1.RowCount, StringGrid1.ColCount]);
  end;
end;

procedure TMainForm.SpkLargeButton7Click(Sender: TObject);
begin
  if SettingsForm = nil then
    SettingsForm := TSettingsForm.Create(nil);

  SettingsForm.Show;
end;

end.

