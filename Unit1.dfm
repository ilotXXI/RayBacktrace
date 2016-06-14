object Form1: TForm1
  Left = 201
  Top = 177
  Width = 908
  Height = 504
  Caption = #1051#1072#1073#1086#1088#1072#1090#1086#1088#1085#1072#1103' '#1088#1072#1073#1086#1090#1072'.'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu1: TMainMenu
    Left = 640
    Top = 392
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N5: TMenuItem
        Caption = #1053#1086#1074#1099#1081' '#1092#1072#1081#1083
        OnClick = N5Click
      end
      object N2: TMenuItem
        Caption = #1047#1072#1087#1080#1089#1072#1090#1100' '#1074' '#1092#1072#1081#1083'...'
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1092#1072#1081#1083'...'
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N4Click
      end
    end
    object N6: TMenuItem
      Caption = #1057#1094#1077#1085#1072
      object N7: TMenuItem
        Caption = #1048#1079#1086#1073#1088#1072#1079#1080#1090#1100
        OnClick = N7Click
      end
      object N8: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082'...'
        OnClick = N8Click
      end
      object N9: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082' '#1089#1074#1077#1090#1072'...'
        OnClick = N9Click
      end
      object N10: TMenuItem
        Caption = #1055#1088#1077#1086#1073#1088#1072#1079#1086#1074#1072#1090#1100' '#1089#1094#1077#1085#1091'...'
        OnClick = N10Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Title = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1092#1072#1081#1083'...'
    Left = 688
    Top = 392
  end
  object SaveDialog1: TSaveDialog
    Title = #1047#1072#1087#1080#1089#1072#1090#1100' '#1074' '#1092#1072#1081#1083'...'
    Left = 736
    Top = 392
  end
end
