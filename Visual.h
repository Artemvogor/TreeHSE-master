#pragma once
#include "vebtree.h"

namespace vebclr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Visual
	/// </summary>
	public ref class Visual : public System::Windows::Forms::Form
	{
	public:
		Visual(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Visual()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ VebDisplay;
	private: System::Windows::Forms::TextBox^ t_Input;

	private: System::Windows::Forms::Button^ b_Add;
	private: System::Windows::Forms::Button^ b_Remove;
	private: System::Windows::Forms::Button^ b_Find;

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->VebDisplay = (gcnew System::Windows::Forms::PictureBox());
			this->t_Input = (gcnew System::Windows::Forms::TextBox());
			this->b_Add = (gcnew System::Windows::Forms::Button());
			this->b_Remove = (gcnew System::Windows::Forms::Button());
			this->b_Find = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VebDisplay))->BeginInit();
			this->SuspendLayout();
			// 
			// VebDisplay
			// 
			this->VebDisplay->Location = System::Drawing::Point(34, 22);
			this->VebDisplay->Name = L"VebDisplay";
			this->VebDisplay->Size = System::Drawing::Size(733, 337);
			this->VebDisplay->TabIndex = 0;
			this->VebDisplay->TabStop = false;
			this->VebDisplay->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Visual::VebDisplay_Paint);
			// 
			// t_Input
			// 
			this->t_Input->Location = System::Drawing::Point(34, 383);
			this->t_Input->Name = L"t_Input";
			this->t_Input->Size = System::Drawing::Size(463, 20);
			this->t_Input->TabIndex = 1;
			// 
			// b_Add
			// 
			this->b_Add->Location = System::Drawing::Point(515, 381);
			this->b_Add->Name = L"b_Add";
			this->b_Add->Size = System::Drawing::Size(72, 23);
			this->b_Add->TabIndex = 2;
			this->b_Add->Text = L"Добавить";
			this->b_Add->UseVisualStyleBackColor = true;
			this->b_Add->Click += gcnew System::EventHandler(this, &Visual::b_Add_Click);
			// 
			// b_Remove
			// 
			this->b_Remove->Location = System::Drawing::Point(707, 380);
			this->b_Remove->Name = L"b_Remove";
			this->b_Remove->Size = System::Drawing::Size(60, 23);
			this->b_Remove->TabIndex = 3;
			this->b_Remove->Text = L"Удалить";
			this->b_Remove->UseVisualStyleBackColor = true;
			this->b_Remove->Click += gcnew System::EventHandler(this, &Visual::b_Remove_Click);
			// 
			// b_Find
			// 
			this->b_Find->Location = System::Drawing::Point(613, 380);
			this->b_Find->Name = L"b_Find";
			this->b_Find->Size = System::Drawing::Size(72, 23);
			this->b_Find->TabIndex = 4;
			this->b_Find->Text = L"Найти";
			this->b_Find->UseVisualStyleBackColor = true;
			this->b_Find->Click += gcnew System::EventHandler(this, &Visual::b_Find_Click);
			// 
			// Visual
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(804, 449);
			this->Controls->Add(this->b_Find);
			this->Controls->Add(this->b_Remove);
			this->Controls->Add(this->b_Add);
			this->Controls->Add(this->t_Input);
			this->Controls->Add(this->VebDisplay);
			this->Name = L"Visual";
			this->Text = L"Visual";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VebDisplay))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		Veb* tr;
		int maxt = 10;

		Brush^ SelectBrush(int n) {
			switch (n) {
			case 1:
				return Brushes::Magenta;
			case 2:
				return Brushes::Blue;
			case 3:
				return Brushes::OrangeRed;
			case 4:
				return Brushes::Red;
			case 5:
				return Brushes::IndianRed;
			default:
				return Brushes::White;
			}
		}

		void DrawTree(Graphics^ g, Veb* t, double x0, double y0, double w, double dh)
		{
			Pen^ p = gcnew Pen(Brushes::OrangeRed, 2);
			Pen^ p1 = gcnew Pen(Brushes::DarkOrange, 2);
			Pen^ p2;
			Point^ pxy0 = gcnew Point((int)x0, (int)y0);
			Point^ pxy = gcnew Point((int)x0 - 10, (int)y0 + 10);
			g->DrawEllipse(p, x0 - 2, y0 - 2, 4, 4);
			if (t->SpecFlag != 0)
			{
				p2 = gcnew Pen(SelectBrush(t->SpecFlag), 2);
				g->DrawEllipse(p2, x0 - 4, y0 - 4, 8, 8);
				t->SpecFlag = 0;
			}
			String^ streemax = (t->TreeMax == t->NoneK ? "-" : t->TreeMax.ToString());
			String^ streemin = (t->TreeMin == t->NoneK ? "-" : t->TreeMin.ToString());
			g->DrawString(String::Format("[{0}; {1}]", streemin, streemax), gcnew System::Drawing::Font("Arial", 9), Brushes::Black, x0 - 10, y0 + 10);
			int cnt = 0;
			for (int i = 0; i < t->NoneN; ++i) if (t->Subtrees[i] != nullptr) ++cnt;
			if (cnt == 0) return;
			double dw = w / cnt;
			double x = x0 - w / 2 + (dw / 2);
			double y = y0 + dh;
			for (int i = 0; i < t->NoneN; ++i)
			{
				if (t->Subtrees[i] == nullptr) continue;
				if (t->Subtrees[i]->SpecFlag != 0)
					p2 = gcnew Pen(SelectBrush(t->Subtrees[i]->SpecFlag), 3);
				else
					p2 = p1;
				g->DrawLine(p2, (float)x0, y0, x, y);
				DrawTree(g, t->Subtrees[i].get(), x, y, dw, dh);
				x += dw;
			}

		}
		//protected override void OnRender(DrawingContext dc)
		//{
		//	base.OnRender(dc);
		//	DrawTree(dc, tr, ActualWidth / 2, 16, ActualWidth - 32, ActualHeight / maxt);
		//}

	private:
		System::Void VebDisplay_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;
		DrawTree(g, tr, ((PictureBox^)sender)->Width / 2, 16, ((PictureBox^)sender)->Width - 32, ((PictureBox^)sender)->Height / maxt);

	
	}

	bool IsDigit(wchar_t c) {
		switch (c) {
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			return true;
		}
		return false;
	}

	private: System::Void add_val(String^ s) {
		UInt64 x;
		try {
			x = Convert::ToInt64(s);
			tr->Add(x);
			VebDisplay->Invalidate();
		}
		catch (System::FormatException^ e) {

		}
	}

	private: System::Void b_Add_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ s = t_Input->Text;
		String^ c;
		int k0 = 0;
		bool spcflg = true;
		for (int i = 0; i < s->Length; ++i) {
			if (IsDigit(s[i])) {
				if (spcflg) {
					k0 = i;
					spcflg = false;
				}
			}
			else {
				if (!spcflg) {
					c = s->Substring(k0, i - k0);
					add_val(c);
					spcflg = true;
				}
			}
		}
		if (!spcflg) {
			c = s->Substring(k0);
			add_val(c);
		}
	}
private: System::Void b_Remove_Click(System::Object^ sender, System::EventArgs^ e) {
	UInt64 x;
	String^ s = t_Input->Text;
	try {
		x = Convert::ToInt64(s);
		tr->Remove(x);
		VebDisplay->Invalidate();
	}
	catch (System::FormatException^ e) {

	}
}
private: System::Void b_Find_Click(System::Object^ sender, System::EventArgs^ e) {
	UInt64 x;
	String^ s = t_Input->Text;
	try {
		x = Convert::ToInt64(s);
		bool flag = tr->Lookup(x);
		if (!flag) {
			MessageBox::Show("Число не найдено.");
		}
		VebDisplay->Invalidate();
	}
	catch (System::FormatException^ e) {

	}
}
};
}
