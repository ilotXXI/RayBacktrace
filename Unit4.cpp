//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Отмена.".
void __fastcall TForm4::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Добавить.".
void __fastcall TForm4::Button1Click(TObject *Sender)
{
 extern spotLight light[];
 extern int nl;
 //Добавление нового источника света.
 light[nl].ChangeSpotLight(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text), StrToFloat(Edit4->Text));
 ++nl;
 //Очистка полей Edit и закрытие формы.
 Edit1->Text = "";
 Edit2->Text = "";
 Edit3->Text = "";
 Edit4->Text = "";
 Close();
}
//---------------------------------------------------------------------------
