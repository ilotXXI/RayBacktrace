//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Добавить.".
void __fastcall TForm3::Button1Click(TObject *Sender)
{
 extern int np;
 extern polygon obj[];
 int top_n;
 float x[25], y[25], z[25];
 float r[2], g[2], b[2];
 //Считывание параметров для добавляемого многоугольника.
 top_n = 0;
 while (StringGrid1->Cells[0][top_n].Length())
  {
   x[top_n] = StrToFloat(StringGrid1->Cells[0][top_n]);
   y[top_n] = StrToFloat(StringGrid1->Cells[1][top_n]);
   z[top_n] = StrToFloat(StringGrid1->Cells[2][top_n]);
   ++top_n;
   }
 r[0] = StrToFloat(StringGrid2->Cells[0][0]);  r[1] = StrToFloat(StringGrid2->Cells[1][0]);
 g[0] = StrToFloat(StringGrid2->Cells[0][1]);  g[1] = StrToFloat(StringGrid2->Cells[1][1]);
 b[0] = StrToFloat(StringGrid2->Cells[0][2]);  b[1] = StrToFloat(StringGrid2->Cells[1][2]);
 //Добавление нового многоугольника.
 obj[np].ChangePolygon(x, y, z, top_n, r, g, b, StrToFloat(Edit1->Text), StrToInt(Edit2->Text));
 ++np;
 //Очистка обработанных таблиц, полей Edit и закрытие формы.
 for(--top_n; top_n>=0; --top_n)
  {
   StringGrid1->Cells[0][top_n] = "";
   StringGrid1->Cells[1][top_n] = "";
   StringGrid1->Cells[2][top_n] = "";
   }
 for(top_n=0; top_n<3; ++top_n)
  {
   StringGrid2->Cells[0][top_n] = "";
   StringGrid2->Cells[1][top_n] = "";
   }
 Edit1->Text = "";
 Edit2->Text = "";
 Close();
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Отмена.".
void __fastcall TForm3::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

