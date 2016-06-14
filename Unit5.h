//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TEdit *Edit1;
        TLabel *Label1;
        TRadioGroup *RadioGroup1;
        TButton *Button1;
        TButton *Button2;
        TGroupBox *GroupBox2;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TRadioGroup *RadioGroup2;
        TGroupBox *GroupBox3;
        TEdit *Edit5;
        TLabel *Label5;
        TButton *Button6;
        TStringGrid *StringGrid1;
        TStringGrid *StringGrid2;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TGroupBox *GroupBox4;
        TEdit *Edit6;
        TEdit *Edit7;
        TStringGrid *StringGrid3;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TButton *Button7;
        TGroupBox *GroupBox5;
        TEdit *Edit8;
        TLabel *Label16;
        TButton *Button8;
        TButton *Button9;
        TButton *Button10;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
