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

//��� ������� �� ������ "������.".
void __fastcall TForm4::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

//��� ������� �� ������ "��������.".
void __fastcall TForm4::Button1Click(TObject *Sender)
{
 extern spotLight light[];
 extern int nl;
 //���������� ������ ��������� �����.
 light[nl].ChangeSpotLight(StrToFloat(Edit1->Text), StrToFloat(Edit2->Text), StrToFloat(Edit3->Text), StrToFloat(Edit4->Text));
 ++nl;
 //������� ����� Edit � �������� �����.
 Edit1->Text = "";
 Edit2->Text = "";
 Edit3->Text = "";
 Edit4->Text = "";
 Close();
}
//---------------------------------------------------------------------------
