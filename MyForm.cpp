#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

void winForm(array<String^>^ args) {
	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	ExpenseTracker::MyForm frm;
	Application::Run(% frm);



}
