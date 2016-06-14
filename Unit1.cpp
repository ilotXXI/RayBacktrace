//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <time.h>

#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//Глобальные переменные.
polygon obj[100];       //Массив многоугольников сцены.
spotLight light[50];    //Массив точечных источников света.
int np=0;               //Кол-во многоугольников.
int nl=0;               //Кол-во источников света.

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//При нажатии на "Файл->Новый файл".
void __fastcall TForm1::N5Click(TObject *Sender)
{
 int i;
 //Удаление многоугольников.
 for (i=0; i<np; ++i)
  obj[i].~polygon();
 np = 0;
 //Удаление источников света.
 nl = 0;
}
//---------------------------------------------------------------------------

//При нажатии на кнопку "Файл->Выход".
void __fastcall TForm1::N4Click(TObject *Sender)
{
 //Освобождение памяти из-под многоугольников.
 for(int i=0; i<np; ++i)
  obj[i].~polygon();
 exit(0);
}
//---------------------------------------------------------------------------

//При нажатии на "Файл->Записать в файл...".
void __fastcall TForm1::N2Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
  {
   FILE *file;
   int i, j;
   //Открытие файла.
   file = fopen((SaveDialog1->FileName + '\0').c_str(), "wb");
   if (!file)
    return;
   //Запись в файл кол-в многоугольников и источников света.
   fwrite(&np, sizeof(int), 1, file);
   fwrite(&nl, sizeof(int), 1, file);
   //Запись в файл многоугольников.
   for(i=0; i<np; ++i)
    {
     //Запись коэффициентов и кол-ва вершин очередного многоугольника.
     fwrite(&(obj[i].Rka), sizeof(float), 1, file);
     fwrite(&(obj[i].Gka), sizeof(float), 1, file);
     fwrite(&(obj[i].Bka), sizeof(float), 1, file);
     fwrite(&(obj[i].Rkd), sizeof(float), 1, file);
     fwrite(&(obj[i].Gkd), sizeof(float), 1, file);
     fwrite(&(obj[i].Bkd), sizeof(float), 1, file);
     fwrite(&(obj[i].ks), sizeof(float), 1, file);
     fwrite(&(obj[i].c_p_k), sizeof(int), 1, file);
     fwrite(&(obj[i].n), sizeof(int), 1, file);
     //Запись координат вершин многоугольника.
     for (j=0; j<obj[i].n; ++j)
      {
       fwrite(&(obj[i].top[j].x), sizeof(float), 1, file);
       fwrite(&(obj[i].top[j].y), sizeof(float), 1, file);
       fwrite(&(obj[i].top[j].z), sizeof(float), 1, file);
       }
     }
   //Запись в файл источников света.
   for(i=0; i<nl; ++i)
    {
     fwrite(&(light[i].place.x), sizeof(float), 1, file);
     fwrite(&(light[i].place.y), sizeof(float), 1, file);
     fwrite(&(light[i].place.z), sizeof(float), 1, file);
     fwrite(&(light[i].intensivity), sizeof(float), 1, file);
     }
   fclose(file);
   }
}
//---------------------------------------------------------------------------

//При нажатии на "Файл->Загрузить файл..."
void __fastcall TForm1::N3Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
  {
   FILE *file;
   float r[2], g[2], b[2];
   float x[25], y[25], z[25];
   float ks;
   int c_p_k;
   int i, j, top_n;
   //Открытие файла для чтения.
   file = fopen((OpenDialog1->FileName + '\0').c_str(), "rb");
   if (!file)
    return;
   //Удаление "старых" многоугольников.
   for (i=0; i<np; ++i)
    obj[i].~polygon();
   //Считывание кол-в многоугольников и источников света.
   fread(&np, sizeof(int), 1, file);
   fread(&nl, sizeof(int), 1, file);
   //Чтение многоугольников.
   for(i=0; i<np; ++i)
    {
     //Считывание коэффициентов и кол-ва вершин очередного многоугольника.
     fread(r, sizeof(float), 1, file);
     fread(g, sizeof(float), 1, file);
     fread(b, sizeof(float), 1, file);
     fread(r+1, sizeof(float), 1, file);
     fread(g+1, sizeof(float), 1, file);
     fread(b+1, sizeof(float), 1, file);
     fread(&ks, sizeof(float), 1, file);
     fread(&c_p_k, sizeof(int), 1, file);
     fread(&top_n, sizeof(int), 1, file);
     //Считывание массива вершин очередного многоуогольника.
     for(j=0; j<top_n; ++j)
      {
       fread(x+j, sizeof(float), 1, file);
       fread(y+j, sizeof(float), 1, file);
       fread(z+j, sizeof(float), 1, file);
       }
     //"Добавление" (т.е. изменение) очередного многоугольника.
     obj[i].ChangePolygon(x, y, z, top_n, r, g, b, ks, c_p_k);
    }
   //Чтение источников света.
   for(i=0; i<nl; ++i)
    {
     fread(x, sizeof(float), 1, file);
     fread(y, sizeof(float), 1, file);
     fread(z, sizeof(float), 1, file);
     fread(r, sizeof(float), 1, file);
     light[i].ChangeSpotLight(x[0], y[0], z[0], r[0]);
     }
   fclose(file);
   }
}
//---------------------------------------------------------------------------

//При нажатии на "Cцена->Изобразить".
void __fastcall TForm1::N7Click(TObject *Sender)
{
 clock_t drawing_time;
 drawing_time = clock();
 Draw(Canvas, ClientWidth, ClientHeight, obj, np, light, nl);
 drawing_time = clock() - drawing_time;
 Canvas->TextOutA(5, ClientHeight - 15, FloatToStr((float)drawing_time / CLOCKS_PER_SEC) + " секунд.");
}
//---------------------------------------------------------------------------

//При нажатии на "Сцена->Добавить многоугольник...".
void __fastcall TForm1::N8Click(TObject *Sender)
{
 Form3->ShowModal();
}
//---------------------------------------------------------------------------

//При нажатии на "Сцена->Добавить источник света...".
void __fastcall TForm1::N9Click(TObject *Sender)
{
 Form4->ShowModal();
}
//---------------------------------------------------------------------------

//При нажатии на "Сцена->Преобразоать сцену...".
void __fastcall TForm1::N10Click(TObject *Sender)
{
 int i;
 //Заполнение таблиц на форме для изменения сцены.
 for(i=0; i<np; ++i)
  {
   Form5->StringGrid2->Cells[0][i] = "";
   Form5->StringGrid2->Cells[1][i] = IntToStr(i);
   Form5->StringGrid2->Cells[2][i] = FloatToStr(obj[i].Rka)+", "+FloatToStr(obj[i].Gka)+", "+FloatToStr(obj[i].Bka);
   Form5->StringGrid2->Cells[3][i] = FloatToStr(obj[i].Rkd)+", "+FloatToStr(obj[i].Gkd)+", "+FloatToStr(obj[i].Bkd);
   Form5->StringGrid2->Cells[4][i] = FloatToStr(obj[i].ks);
   Form5->StringGrid2->Cells[5][i] = FloatToStr(obj[i].c_p_k);
   }
 for(i=0; i<nl; ++i)
  {
   Form5->StringGrid1->Cells[0][i] = "";
   Form5->StringGrid1->Cells[1][i] = FloatToStr(light[i].place.x);
   Form5->StringGrid1->Cells[2][i] = FloatToStr(light[i].place.y);
   Form5->StringGrid1->Cells[3][i] = FloatToStr(light[i].place.z);
   Form5->StringGrid1->Cells[4][i] = FloatToStr(light[i].intensivity);
   }
 Form5->StringGrid2->RowCount = np;
 Form5->StringGrid1->RowCount = nl;
 //Открытие формы для изменения объектов сцены.
 Form5->ShowModal();
}
//---------------------------------------------------------------------------

