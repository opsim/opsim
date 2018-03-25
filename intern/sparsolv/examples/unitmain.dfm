object FormMain: TFormMain
  Left = 192
  Top = 103
  Width = 544
  Height = 375
  Caption = 'SparSolv tester'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  Scaled = False
  ShowHint = True
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 16
  object Splitter1: TSplitter
    Left = 348
    Top = 0
    Width = 3
    Height = 343
    Cursor = crHSplit
    Align = alRight
    Beveled = True
  end
  object PanelMng: TPanel
    Left = 351
    Top = 0
    Width = 185
    Height = 343
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 32
      Top = 64
      Width = 88
      Height = 16
      Caption = '&Equation count'
      FocusControl = SpinEdit1
    end
    object Button1: TButton
      Left = 32
      Top = 16
      Width = 75
      Height = 25
      Caption = '&Solve'
      TabOrder = 0
      OnClick = Button1Click
    end
    object SpinEdit1: TSpinEdit
      Left = 32
      Top = 80
      Width = 121
      Height = 26
      MaxValue = 9999
      MinValue = 1
      TabOrder = 1
      Value = 3
      OnChange = SpinEdit1Change
    end
    object StatusBar1: TStatusBar
      Left = 0
      Top = 324
      Width = 185
      Height = 19
      Panels = <
        item
          Alignment = taRightJustify
          Text = 'Time: '
          Width = 50
        end
        item
          Width = 50
        end>
      SimplePanel = False
    end
  end
  object StringGridMatrix: TStringGrid
    Left = 0
    Top = 0
    Width = 348
    Height = 343
    Align = alClient
    FixedCols = 0
    RowCount = 4
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 1
  end
end
