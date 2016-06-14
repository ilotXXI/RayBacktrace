object Form3: TForm3
  Left = 288
  Top = 143
  Width = 504
  Height = 389
  Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082'...'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 172
    Height = 16
    Caption = #1042#1077#1088#1096#1080#1085#1099' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1072':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 171
    Height = 13
    Caption = '         x:                    y:                    z:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 264
    Top = 8
    Width = 179
    Height = 16
    Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099' '#1086#1090#1088#1072#1078#1077#1085#1080#1103':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 256
    Top = 88
    Width = 11
    Height = 13
    Caption = 'R:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 256
    Top = 112
    Width = 11
    Height = 13
    Caption = 'G:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 256
    Top = 136
    Width = 10
    Height = 13
    Caption = 'B:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 296
    Top = 64
    Width = 108
    Height = 13
    Caption = '        ka:                  kd:'
  end
  object Bevel1: TBevel
    Left = 248
    Top = 56
    Width = 209
    Height = 177
    Style = bsRaised
  end
  object Label8: TLabel
    Left = 256
    Top = 168
    Width = 17
    Height = 16
    Caption = 'ks:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 256
    Top = 200
    Width = 9
    Height = 13
    Caption = 'n:'
  end
  object StringGrid1: TStringGrid
    Left = 8
    Top = 56
    Width = 217
    Height = 289
    ColCount = 3
    FixedCols = 0
    RowCount = 25
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 0
  end
  object Button1: TButton
    Left = 264
    Top = 248
    Width = 177
    Height = 41
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100'.'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 264
    Top = 304
    Width = 177
    Height = 41
    Caption = #1054#1090#1084#1077#1085#1072'.'
    TabOrder = 2
    OnClick = Button2Click
  end
  object StringGrid2: TStringGrid
    Left = 296
    Top = 80
    Width = 137
    Height = 81
    ColCount = 2
    FixedCols = 0
    RowCount = 3
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 3
  end
  object Edit1: TEdit
    Left = 296
    Top = 168
    Width = 137
    Height = 21
    TabOrder = 4
  end
  object Edit2: TEdit
    Left = 296
    Top = 200
    Width = 137
    Height = 21
    TabOrder = 5
  end
end
