object Form5: TForm5
  Left = 46
  Top = 142
  Width = 1227
  Height = 524
  Caption = 'Form5'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 16
    Top = 16
    Width = 376
    Height = 16
    Caption = #1052#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1080' ('#1086#1090#1084#1077#1090#1100#1090#1077' '#1090#1077', '#1082#1086#1090#1086#1088#1099#1077' '#1093#1086#1090#1080#1090#1077' '#1080#1079#1084#1077#1085#1080#1090#1100'):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 16
    Top = 288
    Width = 379
    Height = 16
    Caption = #1048#1089#1090#1086#1095#1085#1080#1082#1080' '#1089#1074#1077#1090#1072' ('#1086#1090#1084#1077#1090#1100#1090#1077' '#1090#1077', '#1082#1086#1090#1086#1088#1099#1077' '#1093#1086#1090#1080#1090#1077' '#1080#1079#1084#1077#1085#1080#1090#1100'):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 16
    Top = 40
    Width = 363
    Height = 13
    Caption = 
      '   '#1054#1090#1084#1077#1090#1082#1072':        '#1053#1086#1084#1077#1088':       ka(R, G, B):   kd(R, G, B):     ' +
      '     ks:                 n:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 16
    Top = 312
    Width = 371
    Height = 13
    Caption = 
      '     '#1054#1090#1084#1077#1090#1082#1072':               x:                      y:          ' +
      '            z:         '#1048#1085#1090#1077#1085#1089#1080#1074#1085#1086#1089#1090#1100':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object GroupBox1: TGroupBox
    Left = 456
    Top = 8
    Width = 481
    Height = 193
    Caption = #1055#1086#1074#1086#1088#1086#1090':'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 32
      Width = 34
      Height = 16
      Caption = #1059#1075#1086#1083':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit1: TEdit
      Left = 64
      Top = 32
      Width = 177
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object RadioGroup1: TRadioGroup
      Left = 16
      Top = 80
      Width = 153
      Height = 89
      Caption = #1045#1076#1080#1085#1080#1094#1099' '#1080#1079#1084#1077#1088#1077#1085#1080#1103' '#1091#1075#1083#1072':'
      ItemIndex = 0
      Items.Strings = (
        #1043#1088#1072#1076#1091#1089#1099'.'
        #1056#1072#1076#1080#1072#1085#1099'.')
      TabOrder = 1
    end
    object Button1: TButton
      Left = 288
      Top = 56
      Width = 169
      Height = 41
      Caption = #1055#1086#1074#1077#1088#1085#1091#1090#1100' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1080'.'
      TabOrder = 2
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 288
      Top = 112
      Width = 169
      Height = 41
      Caption = #1055#1086#1074#1077#1088#1085#1091#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082#1080' '#1089#1074#1077#1090#1072'.'
      TabOrder = 3
      OnClick = Button2Click
    end
    object RadioGroup2: TRadioGroup
      Left = 176
      Top = 80
      Width = 105
      Height = 89
      Caption = #1054#1089#1100':'
      ItemIndex = 0
      Items.Strings = (
        'Ox'
        'Oy'
        'Oz')
      TabOrder = 4
    end
  end
  object GroupBox2: TGroupBox
    Left = 456
    Top = 224
    Width = 481
    Height = 145
    Caption = #1055#1077#1088#1077#1084#1077#1097#1077#1085#1080#1077':'
    TabOrder = 1
    object Label2: TLabel
      Left = 8
      Top = 32
      Width = 66
      Height = 16
      Caption = #1055#1086' '#1086#1089#1080' Ox:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 8
      Top = 64
      Width = 67
      Height = 16
      Caption = #1055#1086' '#1086#1089#1080' Oy:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 96
      Width = 66
      Height = 16
      Caption = #1055#1086' '#1086#1089#1080' Oz:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit2: TEdit
      Left = 80
      Top = 32
      Width = 177
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object Edit3: TEdit
      Left = 80
      Top = 64
      Width = 177
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object Edit4: TEdit
      Left = 80
      Top = 96
      Width = 177
      Height = 21
      TabOrder = 2
      Text = '0'
    end
    object Button3: TButton
      Left = 296
      Top = 32
      Width = 169
      Height = 41
      Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1080'.'
      TabOrder = 3
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 296
      Top = 80
      Width = 169
      Height = 41
      Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082#1080' '#1089#1074#1077#1090#1072'.'
      TabOrder = 4
      OnClick = Button4Click
    end
  end
  object Button5: TButton
    Left = 1000
    Top = 440
    Width = 161
    Height = 41
    Caption = #1047#1072#1082#1088#1099#1090#1100'.'
    TabOrder = 2
    OnClick = Button5Click
  end
  object GroupBox3: TGroupBox
    Left = 456
    Top = 392
    Width = 481
    Height = 89
    Caption = #1052#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077':'
    TabOrder = 3
    object Label5: TLabel
      Left = 8
      Top = 32
      Width = 77
      Height = 16
      Caption = #1052#1085#1086#1078#1080#1090#1077#1083#1100':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit5: TEdit
      Left = 88
      Top = 32
      Width = 169
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object Button6: TButton
      Left = 280
      Top = 24
      Width = 193
      Height = 41
      Caption = #1052#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1090#1100' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1080'.'
      TabOrder = 1
      OnClick = Button6Click
    end
  end
  object StringGrid1: TStringGrid
    Left = 16
    Top = 336
    Width = 417
    Height = 113
    DefaultColWidth = 72
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 4
  end
  object StringGrid2: TStringGrid
    Left = 16
    Top = 64
    Width = 417
    Height = 177
    ColCount = 6
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 5
  end
  object GroupBox4: TGroupBox
    Left = 952
    Top = 8
    Width = 249
    Height = 289
    Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1077' '#1082#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1086#1074' '#1086#1090#1088#1072#1078#1077#1085#1080#1103':'
    TabOrder = 6
    object Label10: TLabel
      Left = 80
      Top = 24
      Width = 90
      Height = 13
      Caption = '  ka:                  kd:'
    end
    object Label11: TLabel
      Left = 40
      Top = 48
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
    object Label12: TLabel
      Left = 40
      Top = 80
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
    object Label13: TLabel
      Left = 40
      Top = 104
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
    object Label14: TLabel
      Left = 40
      Top = 160
      Width = 14
      Height = 13
      Caption = 'ks:'
    end
    object Label15: TLabel
      Left = 40
      Top = 192
      Width = 9
      Height = 13
      Caption = 'n:'
    end
    object Edit6: TEdit
      Left = 64
      Top = 160
      Width = 145
      Height = 21
      TabOrder = 0
    end
    object Edit7: TEdit
      Left = 64
      Top = 192
      Width = 145
      Height = 21
      TabOrder = 1
    end
    object StringGrid3: TStringGrid
      Left = 64
      Top = 48
      Width = 137
      Height = 81
      ColCount = 2
      FixedCols = 0
      RowCount = 3
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
      TabOrder = 2
    end
    object Button7: TButton
      Left = 56
      Top = 232
      Width = 145
      Height = 41
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100'.'
      TabOrder = 3
      OnClick = Button7Click
    end
  end
  object GroupBox5: TGroupBox
    Left = 952
    Top = 312
    Width = 249
    Height = 113
    Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1077' '#1080#1085#1090#1077#1085#1089#1080#1074#1085#1086#1089#1090#1080' '#1080#1089#1090#1086#1095#1085#1080#1082#1086#1074' '#1089#1074#1077#1090#1072':'
    TabOrder = 7
    object Label16: TLabel
      Left = 48
      Top = 32
      Width = 9
      Height = 13
      Caption = 'I:'
    end
    object Edit8: TEdit
      Left = 64
      Top = 32
      Width = 145
      Height = 21
      TabOrder = 0
    end
    object Button8: TButton
      Left = 48
      Top = 64
      Width = 161
      Height = 41
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100'.'
      TabOrder = 1
      OnClick = Button8Click
    end
  end
  object Button9: TButton
    Left = 16
    Top = 256
    Width = 185
    Height = 17
    Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1077' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082#1080'.'
    TabOrder = 8
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 16
    Top = 464
    Width = 185
    Height = 17
    Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1077' '#1080#1089#1090#1086#1095#1085#1080#1082#1080' '#1089#1074#1077#1090#1072'.'
    TabOrder = 9
    OnClick = Button10Click
  end
end
