//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Повернуть многоугольники.".
void __fastcall TForm5::Button1Click(TObject *Sender)
{
 extern polygon obj[];
 extern int np;
 int i;
 float alpha;
 try {alpha = StrToFloat(Edit1->Text);}
 catch (...)
  {ShowMessage("Угол введён неправильно.");}
 if (!RadioGroup1->ItemIndex)
  alpha *= 0.017453;
 for(i=0; i<np; ++i)
  if(StringGrid2->Cells[0][i].Length())
   obj[i].Rotate(alpha, RadioGroup2->ItemIndex);
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Повернуть источники света.".
void __fastcall TForm5::Button2Click(TObject *Sender)
{
 extern spotLight light[];
 extern int nl;
 int i;
 float alpha;
 try {alpha = StrToFloat(Edit1->Text);}
 catch (...)
  {ShowMessage("Угол введён неправильно.");}
 if (!RadioGroup1->ItemIndex)
  alpha *= 0.017453;
 for(i=0; i<nl; ++i)
  if(StringGrid1->Cells[0][i].Length())
   light[i].Rotate(alpha, RadioGroup2->ItemIndex);
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Переместить многоугольники.".
void __fastcall TForm5::Button3Click(TObject *Sender)
{
 extern polygon obj[];
 extern int np;
 int i;
 for(i=0; i<np; ++i)
  if(StringGrid2->Cells[0][i].Length())
   obj[i].Replace(StrToFloat(Edit2->Text), StrToFloat(Edit3->Text), StrToFloat(Edit4->Text));
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Переместить источники света.".
void __fastcall TForm5::Button4Click(TObject *Sender)
{
 extern spotLight light[];
 extern int nl;
 int i;
 for(i=0; i<nl; ++i)
  if(StringGrid1->Cells[0][i].Length())
   light[i].Replace(StrToFloat(Edit2->Text), StrToFloat(Edit3->Text), StrToFloat(Edit4->Text));
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Закрыть.".
void __fastcall TForm5::Button5Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Масштабировать многоугольники.".
void __fastcall TForm5::Button6Click(TObject *Sender)
{
 extern polygon obj[];
 extern int np;
 int i;
 for(i=0; i<np; ++i)
  if(StringGrid2->Cells[0][i].Length())
   obj[i].Scale(StrToFloat(Edit5->Text));
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Изменить." (применительно к многоугольникам).
void __fastcall TForm5::Button7Click(TObject *Sender)
{
 extern polygon obj[];
 extern int np;
 int i;
 float Rka, Gka, Bka, Rkd, Gkd, Bkd, ks, n;
 if(StringGrid3->Cells[0][0].Length())
   Rka = StrToFloat( StringGrid3->Cells[0][0] );
  else
   Rka = -1;
 if(StringGrid3->Cells[0][1].Length())
   Gka = StrToFloat( StringGrid3->Cells[0][1] );
  else
   Gka = -1;
 if(StringGrid3->Cells[0][2].Length())
   Bka = StrToFloat( StringGrid3->Cells[0][2] );
  else
   Bka = -1;
 if(StringGrid3->Cells[1][0].Length())
   Rkd = StrToFloat( StringGrid3->Cells[1][0] );
  else
   Rkd = -1;
 if(StringGrid3->Cells[1][1].Length())
   Gkd = StrToFloat( StringGrid3->Cells[1][1] );
  else
   Gkd = -1;
 if(StringGrid3->Cells[1][2].Length())
   Bkd = StrToFloat( StringGrid3->Cells[1][2] );
  else
   Bkd = -1;
 if(Edit6->Text.Length())
   ks = StrToFloat( Edit6->Text );
  else
   ks = -1;
 if(Edit7->Text.Length())
   n = StrToInt( Edit7->Text );
  else
   n = -1;
 for(i=0; i<np; ++i)
  if(StringGrid2->Cells[0][i].Length())
   obj[i].Colors(Rka, Gka, Bka, Rkd, Gkd, Bkd, ks, n);
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Изменить." (применительно к источникам света).
void __fastcall TForm5::Button8Click(TObject *Sender)
{
 extern spotLight light[];
 extern int nl;
 int i;
 for(i=0; i<nl; ++i)
  if(StringGrid1->Cells[0][i].Length())
   light[i].ChangeIntensivity(StrToFloat(Edit8->Text));
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Выделить все многоугольники.".
void __fastcall TForm5::Button9Click(TObject *Sender)
{
 for(int i=0; i<StringGrid2->RowCount; ++i)
  StringGrid2->Cells[0][i] = "+";
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Выделить все источники света.".
void __fastcall TForm5::Button10Click(TObject *Sender)
{
 for(int i=0; i<StringGrid1->RowCount; ++i)
  StringGrid1->Cells[0][i] = "+";
}
//---------------------------------------------------------------------------

