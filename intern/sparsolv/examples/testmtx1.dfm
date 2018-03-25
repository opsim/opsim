object Form1: TForm1
  Left = 200
  Top = 103
  Width = 380
  Height = 217
  Caption = 'SparSolv Test'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000B000000000000000000B00000000000000000
    0000000000000B00000B0B000B00000000000000000000000000000000000000
    0000000000B000000000000B00B00000000000000000000000000B0000000000
    00000000000000000000000000000000000B0000000000000000B000B0000000
    0000000000000000000B00000000000B00000000000000000000000000000000
    00000000000B000B00000000000000000000000000B00000000000000000B000
    00000000000000000000000000000000000000000000000000B0000000000000
    00000000B0000000B00000000000000000000000000000000B00000000000000
    00B000000B00B0000000000000000000000000B00000000000000000000000B0
    00000B000000000000000B000000000000000000000000000000000000000B00
    0000B000B000000000000000000000000000000000000000000000000B000000
    000000000000000B00000000000000000B00B000000000000000000000000000
    000000000000B00000000000000B0000B000000000000000B000000000000000
    0000000000000000000000B000000B000000000000000000000000000000FFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFDFFFFFFFBEBBFFFFFFFFFFFD
    FFEDFFFFFFBFFFFFFFFFFEFFFF77FFFFFEFFEFFFFFFFFFFEEFFFFFFDFFFF7FFF
    FFFFFFFFFDFFFFF7F7FFFFFFFBFFFDFB7FFFFFDFFFFFDFBFFFBFFFFFFFFFBF77
    FFFFFFFFFFFBFFFFEFFFFB7FFFFFFFFF7FFEF7FFF7FFFFFFFFDFBFFFFFFF}
  OldCreateOrder = True
  PixelsPerInch = 120
  TextHeight = 16
  object FileLabel: TLabel
    Left = 15
    Top = 59
    Width = 56
    Height = 16
    Caption = 'FileLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 12
    Top = 105
    Width = 59
    Height = 16
    Caption = 'Elements:'
  end
  object Label4: TLabel
    Left = 34
    Top = 127
    Width = 37
    Height = 16
    Caption = 'Fillins:'
  end
  object Label2: TLabel
    Left = 41
    Top = 84
    Width = 29
    Height = 16
    Caption = 'Size:'
  end
  object lbSize: TLabel
    Left = 74
    Top = 84
    Width = 3
    Height = 16
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lbElements: TLabel
    Left = 74
    Top = 106
    Width = 3
    Height = 16
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lbFillins: TLabel
    Left = 74
    Top = 128
    Width = 3
    Height = 16
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lbError: TLabel
    Left = 177
    Top = 127
    Width = 3
    Height = 16
    Hint = 
      'Obtained using spCheckSol, and gives'#13#10'the maximun absolute diffe' +
      'rence between'#13#10'the original RHS, and the [Sp]x[SOL] product'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object Label6: TLabel
    Left = 143
    Top = 127
    Width = 32
    Height = 16
    Caption = 'Error:'
  end
  object TimeLabel: TLabel
    Left = 15
    Top = 0
    Width = 3
    Height = 16
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Resolver: TBitBtn
    Left = 254
    Top = 22
    Width = 92
    Height = 31
    Caption = 'Solve !'
    Default = True
    Enabled = False
    TabOrder = 0
    OnClick = ResolverClick
  end
  object ChooseBtn: TBitBtn
    Left = 10
    Top = 22
    Width = 119
    Height = 31
    Caption = 'Choose MTX file'
    Default = True
    TabOrder = 1
    OnClick = ChooseBtnClick
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '*.mtx'
    FileName = '*.mtx'
    Filter = '*.mtx'
    Options = [ofHideReadOnly, ofFileMustExist]
    Left = 184
    Top = 65528
  end
end
